
// Stochastic Progressive Photon Mapping by R. Monette
// originally smallppm, Progressive Photon Mapping by T. Hachisuka
// originally smallpt, a path tracer by Kevin Beason, 2008

#include <SFML/Graphics.hpp>

#include <math.h>   
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <list>

#include "PhotonMap.h"
#include "VolumePhotonMap.h"
#include "ObjModel.h"
#include "Ray.h"
#include "Vector3.h"
#include "BVH.h"
#include "Camera.h"
#include "Scene.h"
#include "ImprovedNoise.h"

#define PI ((double)3.14159265358979)
#define ALPHA ((double)0.7)

#define VOLUME_MODE 1

int primes[61]={2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,
83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,
191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281,283};

inline int rev(const int i,const int p) {if (i==0) return i; else return p-i;}

double hal(const int b, int j) {
  const int p = primes[b]; double h = 0.0, f = 1.0 / (double)p, fct = f;
  while (j > 0) {h += rev(j % p, p) * fct; j /= p; fct *= f;} return h;}

struct HitPoint 
{
	Vector3 f;
	Vector3 pos;
	Vector3 nrm;
	Vector3 flux; 
	Vector3 emission;
	Vector3 weight;
	Vector3 color;
	Ray orig_ray;
	float transmittance;
	float t;
	double r2; 
	unsigned int n; 
	int scrn_x;
	int scrn_y;
	int depth;
	bool isVolumeOnly;
};

struct List 
{
	HitPoint *id;
	List *next;
};

List *ListAdd(HitPoint *i, List *h)
{
	List *p = new List;
	p->id=i;
	p->next=h;
	return p;
}

List *hitpoints = NULL;

PhotonMap *photonmap;
#ifdef VOLUME_MODE
VolumePhotonMap *volume_photonmap;
#endif

int num_photon;

enum Refl_t {DIFF, SPEC, REFR, GLOS, VOLU};  // material types, used in radiance()

int toInt(double x)
{ 
	return int(pow(1-exp(-x),1/2.2)*255+.5); //tone mapping
} 

void generatePhoton(Ray* pr, Vector3* f, int i) 
{
	*f = Vector3(2500, 2500, 2500) * (PI * 4.0);
	double p = 2. * PI * hal(0, i);
	double t = 2. * acos(sqrt(1. - hal(1, i)));
	double st = sin(t);
	pr->setDirection(Vector3(cos(p)*st,cos(t),sin(p)*st));
	pr->setOrigin(Vector3(0,90,50) + Vector3(hal(2,i) * 2., 0, hal(3,i) * 2.));
}

float random()
{
	return rand() / (float)RAND_MAX;
}

Scene *scene;

int numhitpoints = 0;
int numvolumepoints = 0;

inline float density(const Vector3 &pos)
{
	return 0.1f + 
		(1.0f * Noise(pos.x() * 0.1, pos.y() * 0.1, pos.z() * 0.1, 0)) + 
		(0.5f * Noise(pos.x() * 0.5, pos.y() * 0.5, pos.z() * 0.5, 0)) +
		(0.75f * Noise(pos.x() * 1.0, pos.y() * 1.0, pos.z() * 1.0, 0));

	/*if (pos.x() < 50 && pos.x() > -50)
		if (pos.y() < 100 && pos.y() > 0)
			if (pos.z() < 100 && pos.z() > 0)
				return 0.01f;// + (0.1f * Noise(pos.x() * 0.1, pos.y() * 0.1, pos.z() * 0.1, 0));*/

	return 0.0f;
}

#ifndef VOLUME_MODE
void traceRay(const Ray &ray, int depth, const Vector3 &fl, const Vector3 &adj, int i, int sx, int sy) 
{
	depth++;
	
	SurfaceHitRecord rec;

	if(!scene->surfaces->hit(ray, 0.01, 99999, rec) || (depth >= 10)) return;
	
	int d3 = depth * 3;
	
	Vector3 x = ray.origin() + ray.direction() * rec.t;
	Vector3 n = rec.uvw.w();
	Vector3 f = rec.color;
	Vector3 nl = dot(n, ray.direction())<0?n:n*-1; 
	double p = f.x()>f.y()&&f.x()>f.z()?f.x():f.y()>f.z()?f.y():f.z();

	if (rec.material == DIFF) 
	{
		HitPoint *hp = new HitPoint; 
		hp->f = f * adj; 
		hp->pos = x;
		hp->nrm = nl; 
		hp->n = 0; 
		hp->scrn_x = sx;
		hp->scrn_y = sy;
		hp->depth = depth;
		hp->weight = adj;
		hp->color = rec.color;
		hp->emission = Vector3(0,0,0);
		hp->flux = Vector3(0,0,0);
		hp->r2 = 0.1;
		hp->source_ray = ray;
		hp->t = rec.t;
		#pragma omp critical
        {
			hitpoints = ListAdd(hp, hitpoints);
		}
		numhitpoints++;
	} 
	else if (rec.material == SPEC) 
	{						
		traceRay(Ray(x, ray.direction()-n*2.0 * dot(n, ray.direction())), depth, f * fl, f * adj, i, sx, sy);
	} 
	else if (rec.material == GLOS) 
	{						
		Vector3 newDirection = ray.direction() - n * 2.0 * dot(n, ray.direction());
		double r1 = 2. * PI * random();//hal(d3-1,i);
		double r2 = random();//hal(d3+0,i);
		double r2s = sqrt(r2);
		Vector3 w = nl;
		Vector3 u = unitVector(cross((fabs(w.x())>.1?Vector3(0,1,0):Vector3(1,0,0)),w));
		Vector3 v = cross(w, u);
		Vector3 d = unitVector(u*cos(r1)*r2s + v*sin(r1)*r2s + w*sqrt(1-r2));
		newDirection = unitVector(d + newDirection);
		traceRay(Ray(x, newDirection), depth, f * fl, f * adj, i, sx, sy);
	} 
	else 
	{
		Ray lr(x,ray.direction() - n*2.0 * dot(n, ray.direction())); 
		bool into = (dot(n, nl)>0.0);
		double nc = 1.0, nt=1.5, nnt = into?nc/nt:nt/nc, ddn = dot(ray.direction(), nl), cos2t;

		if ((cos2t=1-nnt*nnt*(1-ddn*ddn))<0) return traceRay(lr, depth, fl, adj, i, sx, sy);

		Vector3 td = unitVector(ray.direction()*nnt - n*((into?1:-1)*(ddn*nnt+sqrt(cos2t))));
		double a=nt-nc, b=nt+nc, R0=a*a/(b*b), c = 1-(into?-ddn:dot(td, n));
		double Re=R0+(1-R0)*c*c*c*c*c,P=Re;Ray rr(x,td);Vector3 fa=f * adj;

		traceRay(lr, depth, fl, fa * Re, i, sx, sy);
		traceRay(rr, depth, fl, fa * (1.0 - Re), i, sx, sy);
	}
}
#else

#define MAX_MARCH_DEPTH 300

void traceRay(Ray &ray, int depth, const Vector3 &fl, const Vector3 &adj, int i, int sx, int sy, float march_depth, float density_total, const Ray &oray) 
{
	depth++;
	
	SurfaceHitRecord rec;
	rec.material = -1;

	if (depth >= 10) return;

	float march_step = 0.5f + random(); // jitter step size

	while (!scene->surfaces->hit(ray, 0.01, march_step, rec) && march_depth < MAX_MARCH_DEPTH )
	{
		march_depth += march_step;

		density_total += density(ray.origin()) * 0.01f;

		ray.setOrigin(ray.origin() + unitVector(ray.direction()) * march_step);
	}

	if (march_depth >= MAX_MARCH_DEPTH)
	{
		if (rec.material == GLOS || rec.material == DIFF || rec.material == SPEC || rec.material == REFR)
		{
		}
		else
		{
			rec.material = VOLU;
		}
	}
	
	int d3 = depth * 3;
	
	Vector3 x = ray.origin() + ray.direction() * rec.t;
	Vector3 n = rec.uvw.w();
	Vector3 f = rec.color;
	Vector3 nl = dot(n, ray.direction())<0?n:n*-1; 
	double p = f.x()>f.y()&&f.x()>f.z()?f.x():f.y()>f.z()?f.y():f.z();

	if (rec.material == DIFF) 
	{
		HitPoint *hp = new HitPoint; 
		hp->f = f * adj; 
		hp->pos = x;
		hp->nrm = nl; 
		hp->n = 0; 
		hp->scrn_x = sx;
		hp->scrn_y = sy;
		hp->depth = depth;
		hp->weight = adj;
		hp->color = rec.color;
		hp->emission = Vector3(0,0,0);
		hp->flux = Vector3(0,0,0);
		hp->r2 = 0.1;

		hp->orig_ray = oray;

		hp->isVolumeOnly = false;

		hp->t = rec.t;
		hp->transmittance = max(min(density_total, 1.f), 0.f);
		#pragma omp critical
        {
			hitpoints = ListAdd(hp, hitpoints);
		}
		numhitpoints++;
	} 
	else if (rec.material == SPEC) 
	{						
		Ray r(x, ray.direction()-n*2.0 * dot(n, ray.direction()));
		traceRay(r, depth, f * fl, f * adj, i, sx, sy, march_depth, density_total, r);
	} 
	else if (rec.material == GLOS) 
	{						
		Vector3 newDirection = ray.direction() - n * 2.0 * dot(n, ray.direction());
		double r1 = 2. * PI * random();//hal(d3-1,i);
		double r2 = random();//hal(d3+0,i);
		double r2s = sqrt(r2);
		Vector3 w = nl;
		Vector3 u = unitVector(cross((fabs(w.x())>.1?Vector3(0,1,0):Vector3(1,0,0)),w));
		Vector3 v = cross(w, u);
		Vector3 d = unitVector(u*cos(r1)*r2s + v*sin(r1)*r2s + w*sqrt(1-r2));
		newDirection = unitVector(d + newDirection);
		Ray r(x, newDirection);
		traceRay(r, depth, f * fl, f * adj, i, sx, sy, march_depth, density_total, r);
	} 
	else if (rec.material == REFR)
	{
		Ray lr(x,ray.direction() - n*2.0 * dot(n, ray.direction())); 
		bool into = (dot(n, nl)>0.0);
		double nc = 1.0, nt=1.5, nnt = into?nc/nt:nt/nc, ddn = dot(ray.direction(), nl), cos2t;

		if ((cos2t=1-nnt*nnt*(1-ddn*ddn))<0) return traceRay(lr, depth, fl, adj, i, sx, sy, march_depth, density_total, lr);

		Vector3 td = unitVector(ray.direction()*nnt - n*((into?1:-1)*(ddn*nnt+sqrt(cos2t))));
		double a=nt-nc, b=nt+nc, R0=a*a/(b*b), c = 1-(into?-ddn:dot(td, n));
		double Re=R0+(1-R0)*c*c*c*c*c,P=Re;Ray rr(x,td);Vector3 fa=f * adj;

		traceRay(lr, depth, fl, fa * Re, i, sx, sy, march_depth, density_total, lr);
		traceRay(rr, depth, fl, fa * (1.0 - Re), i, sx, sy, march_depth, density_total, rr);
	}
	else if (rec.material == VOLU)
	{
		HitPoint *hp = new HitPoint; 
		hp->f = f * adj; 
		hp->pos = x;
		hp->nrm = nl; 
		hp->n = 0; 
		hp->scrn_x = sx;
		hp->scrn_y = sy;
		hp->depth = depth;
		hp->weight = adj;
		hp->color = rec.color;
		hp->emission = Vector3(0,0,0);
		hp->flux = Vector3(0,0,0);
		hp->r2 = 0.1;

		hp->orig_ray = oray;

		hp->isVolumeOnly = true;

		hp->t = rec.t;
		hp->transmittance = max(min(density_total, 1.f), 0.f);
		#pragma omp critical
        {
			hitpoints = ListAdd(hp, hitpoints);
		}
		numhitpoints++;

		numvolumepoints++;
	}

}
#endif

#ifndef VOLUME_MODE
void tracePhoton(const Ray &ray, int depth, const Vector3 &fl, int i) 
{
	depth++;

	SurfaceHitRecord rec;
	
	if(!scene->surfaces->hit(ray, 0.01, 99999, rec) || (depth >= 10)) return;
	
	int d3 = depth * 3;
	
	Vector3 x = ray.origin() + ray.direction() * rec.t;
	Vector3 n = rec.uvw.w();
	Vector3 f = rec.color;
	Vector3 nl = dot(n, ray.direction())<0?n:n*-1; 
	double p = f.x()>f.y()&&f.x()>f.z()?f.x():f.y()>f.z()?f.y():f.z();

	if (rec.material == DIFF) 
	{
		double r1 = 2.*PI*hal(d3-1,i),r2=hal(d3+0,i);
		double r2s = sqrt(r2);
		Vector3 w = nl, u = unitVector(cross((fabs(w.x())>.1?Vector3(0,1,0):Vector3(1,0,0)),w));
		Vector3 v = cross(w, u);
		Vector3 d = unitVector(u*cos(r1)*r2s + v*sin(r1)*r2s + w*sqrt(1-r2));

		// PHOTON ON DIFFUSE SURFACE -> STORE IN KD-TREE

		Vector3 ffl = f * fl;
		float power[3] = {ffl.x(), ffl.y(), ffl.z()};
		float pos[3] = {x.x(), x.y(), x.z()};
		float dir[3] = {ray.direction().x(), ray.direction().y(), ray.direction().z()};
		float s_dir[3] = {nl.x(), nl.y(), nl.z()};

		#pragma omp critical
        {
			photonmap->store(power, pos, dir, s_dir);
		}

		if (hal(d3+1,i)<p) tracePhoton(Ray(x,d), depth, f * fl * (1./p), i);
	} 
	else if (rec.material == SPEC) 
	{						
		tracePhoton(Ray(x, ray.direction()-n*2.0*dot(n, ray.direction())), depth, f * fl, i);
	} 
	else if (rec.material == GLOS) 
	{						
		Vector3 newDirection = ray.direction() - n * 2.0 * dot(n, ray.direction());
		double r1 = 2. * PI * random();
		double r2 = random();
		double r2s = sqrt(r2);
		Vector3 w = nl;
		Vector3 u = unitVector(cross((fabs(w.x())>.1?Vector3(0,1,0):Vector3(1,0,0)), w));
		Vector3 v = cross(w, u);
		Vector3 d = unitVector(u*cos(r1)*r2s + v*sin(r1)*r2s + w*sqrt(1-r2));
		newDirection = unitVector(d + newDirection);
		tracePhoton(Ray(x, newDirection), depth, f * fl, i);
	} 
	else 
	{
		Ray lr(x,ray.direction()-n*2.0*dot(n, ray.direction())); 
		bool into = (dot(n, nl)>0.0);
		double nc = 1.0, nt=1.5, nnt = into?nc/nt:nt/nc, ddn = dot(ray.direction(), nl), cos2t;

		if ((cos2t=1-nnt*nnt*(1-ddn*ddn))<0) return tracePhoton(lr, depth, fl, i);

		Vector3 td = unitVector(ray.direction()*nnt - n*((into?1:-1)*(ddn*nnt+sqrt(cos2t))));
		double a=nt-nc, b=nt+nc, R0=a*a/(b*b), c = 1-(into?-ddn:dot(td, n));
		double Re=R0+(1-R0)*c*c*c*c*c,P=Re;Ray rr(x,td);

		(hal(d3 - 1, i) < P) ? tracePhoton(lr, depth, fl, i) : tracePhoton(rr, depth, fl, i);
	}
}
#endif

const float scattering_coefficient = 10.f;
const float absorption_coefficient = 1.f;

#ifdef VOLUME_MODE
void tracePhotonParticipatingMedia(Ray &ray, int depth, const Vector3 &fl, int i, bool interior, float march_depth) 
{
	depth++;

	SurfaceHitRecord rec;

	if (depth >= 10) return;

	float march_step = 0.5f;

	float scattering_albedo = scattering_coefficient / (absorption_coefficient + scattering_coefficient);
	
	while (!scene->surfaces->hit(ray, 0.01, march_step, rec) && march_depth < 500 ) 
	{
		march_depth += march_step;

		ray.setOrigin(ray.origin() + unitVector(ray.direction()) * march_step);

		float d = density(ray.origin());

		if (d > random()) 
		{
			if (scattering_albedo > random())
			{
				// Scattering event

				Vector3 x = ray.origin();

				ray.setDirection(unitVector(Vector3(random() * 2.0 - 1.0, random() * 2.0 - 1.0, random() * 2.0 - 1.0) * 0.1 + ray.direction() * 0.9));

				Vector3 ffl = fl;
				float power[3] = {ffl.x(), ffl.y(), ffl.z()};
				float pos[3] = {x.x(), x.y(), x.z()};

				#pragma omp critical
				{
					volume_photonmap->store(power, pos);
				}
			}
			else
			{
				// Absorption event

				Vector3 x = ray.origin();

				Vector3 ffl = fl;
				float power[3] = {ffl.x(), ffl.y(), ffl.z()};
				float pos[3] = {x.x(), x.y(), x.z()};

				#pragma omp critical
				{
					volume_photonmap->store(power, pos);
				}

				return;
			}
		}
	}
	
	int d3 = depth * 3;
	
	Vector3 x = ray.origin() + ray.direction() * rec.t;
	Vector3 n = rec.uvw.w();
	Vector3 f = rec.color;
	Vector3 nl = dot(n, ray.direction())<0?n:n*-1; 
	double p = f.x()>f.y()&&f.x()>f.z()?f.x():f.y()>f.z()?f.y():f.z();

	if (rec.material == DIFF) 
	{
		double r1 = 2.*PI*hal(d3-1,i),r2=hal(d3+0,i);
		double r2s = sqrt(r2);
		Vector3 w = nl, u = unitVector(cross((fabs(w.x())>.1?Vector3(0,1,0):Vector3(1,0,0)),w));
		Vector3 v = cross(w, u);
		Vector3 d = unitVector(u*cos(r1)*r2s + v*sin(r1)*r2s + w*sqrt(1-r2));

		// PHOTON ON DIFFUSE SURFACE -> STORE IN KD-TREE

		Vector3 ffl = f * fl;
		float power[3] = {ffl.x(), ffl.y(), ffl.z()};
		float pos[3] = {x.x(), x.y(), x.z()};
		float dir[3] = {ray.direction().x(), ray.direction().y(), ray.direction().z()};
		float s_dir[3] = {nl.x(), nl.y(), nl.z()};

		#pragma omp critical
        {
			photonmap->store(power, pos, dir, s_dir);
		}

		if (hal(d3+1,i)<p) tracePhotonParticipatingMedia(Ray(x,d), depth, f * fl * (1./p), i, interior, march_depth);
	} 
	else if (rec.material == SPEC) 
	{						
		tracePhotonParticipatingMedia(Ray(x, ray.direction()-n*2.0*dot(n, ray.direction())), depth, f * fl, i, interior, march_depth);
	} 
	else if (rec.material == GLOS) 
	{						
		Vector3 newDirection = ray.direction() - n * 2.0 * dot(n, ray.direction());
		double r1 = 2. * PI * random();
		double r2 = random();
		double r2s = sqrt(r2);
		Vector3 w = nl;
		Vector3 u = unitVector(cross((fabs(w.x())>.1?Vector3(0,1,0):Vector3(1,0,0)), w));
		Vector3 v = cross(w, u);
		Vector3 d = unitVector(u*cos(r1)*r2s + v*sin(r1)*r2s + w*sqrt(1-r2));
		newDirection = unitVector(d + newDirection);
		tracePhotonParticipatingMedia(Ray(x, newDirection), depth, f * fl, i, interior, march_depth);
	} 
	else 
	{
		Ray lr(x,ray.direction()-n*2.0*dot(n, ray.direction())); 
		bool into = (dot(n, nl)>0.0);
		double nc = 1.0, nt=1.5, nnt = into?nc/nt:nt/nc, ddn = dot(ray.direction(), nl), cos2t;

		interior = into;

		if ((cos2t=1-nnt*nnt*(1-ddn*ddn))<0) return tracePhotonParticipatingMedia(lr, depth, fl, i, interior, march_depth);

		Vector3 td = unitVector(ray.direction()*nnt - n*((into?1:-1)*(ddn*nnt+sqrt(cos2t))));
		double a=nt-nc, b=nt+nc, R0=a*a/(b*b), c = 1-(into?-ddn:dot(td, n));
		double Re=R0+(1-R0)*c*c*c*c*c,P=Re;Ray rr(x,td);

		(hal(d3 - 1, i) < P) ? tracePhotonParticipatingMedia(lr, depth, fl, i, interior, march_depth) : tracePhotonParticipatingMedia(rr, depth, fl, i, interior, march_depth);
	}
}
#endif
	
int main(int argc, char *argv[]) 
{
	InitNoise();

	scene = new Scene();

	int w=512, h=512, samps = 1000;

	Vector3 *c = new Vector3[w*h];
	
	int photons_per_pass = 100000;

	num_photon = samps; 

	const Vector3 origin(0,52,245);
	const Vector3 gaze(0,0,-1);
	const Vector3 up(0,1,0);

	Camera *camera = new Camera(origin, gaze, up, 1, -1, 1, -1, 1, 1500);
	
	for(int i = 0; i < num_photon; i++) 
	{
		numhitpoints = 0;
		numvolumepoints = 0;

		std::cout << "PASS # " << i << "\n";

		std::cout << "Tracing hit points\n";

		#pragma omp parallel for schedule(dynamic, 1)
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++) 
			{
				Ray r = camera->getRay(x - 256, y - 256, 0, 0);

				traceRay(r, 0, Vector3(), Vector3(1, 1, 1), 0, x, 511 - y, 0, 0, r);
			}
		}

		std::cout << "# Hit Points: " << numhitpoints << "\n";
		std::cout << "# Volume Points: " << numvolumepoints << "\n";

		std::cout << "Tracing photons\n";

		photonmap = new PhotonMap(300000);
#ifdef VOLUME_MODE
		volume_photonmap = new VolumePhotonMap(500000);
#endif

		double p=100.*(i+1)/num_photon;
		int m = photons_per_pass * i;
		Ray r;
		Vector3 f;
		#pragma omp parallel for schedule(dynamic, 1)
		for(int j = 0; j < photons_per_pass; j++)
		{
			generatePhoton(&r, &f, m+j); 
#ifdef VOLUME_MODE
			tracePhotonParticipatingMedia(r, 0, f, m+j, false, 0);
#else
			tracePhoton(r, 0, f, m+j);
#endif
			
		}

		photonmap->balance();
#ifdef VOLUME_MODE
		volume_photonmap->balance();
#endif

		std::cout << "Used " << volume_photonmap->get_stored_photons() << " volume photons...\n";
		std::cout << "Used " << photonmap->get_stored_photons() << " surface photons...\n";

		std::cout << "Updating image\n";

		List *lst = hitpoints; 
		List *prev;
		
		while (lst != NULL) 
		{
			HitPoint *hp = lst->id;
			prev = lst;
			lst = lst->next;

			float M = 0;
			float N = hp->n;
			Vector3 flux(0,0,0);
			Vector3 volume_flux(0,0,0);
			Vector3 contrib(0,0,0);

			if (hp->depth != -1) 
			{
				float irrad[3] = {0,0,0};
				float pos[3] = {hp->pos.x(), hp->pos.y(), hp->pos.z()};
				float normal[3];
				float max_dist = sqrtf(hp->r2);
				int nphotons = 50;

				M = photonmap->irradiance_estimate(irrad, pos, normal, max_dist, nphotons);
				flux = Vector3(irrad[0], irrad[1], irrad[2]);
			} 
			else 
			{
				M = 0;
				flux = Vector3(0, 0, 0);
				std::cout << "Setting flux to zero!\n";
			}

			if (N+M == 0) 
			{
				hp->flux = contrib = Vector3(0, 0, 0);
			} 
			else 
			{
				float ratio = (N + ALPHA * M) / (N + M);
				hp->flux = (hp->flux + hp->weight * flux + hp->emission * photonmap->get_stored_photons() * PI * hp->r2) * ratio;
				hp->r2 = hp->r2 * std::sqrt(ratio);
				hp->n = N + ALPHA * M;
			}

			contrib = hp->flux * ( 1.0 / ((float) num_photon * photonmap->get_stored_photons() * hp->r2 * PI)) * (1.0f - hp->transmittance);

#ifdef VOLUME_MODE
			Ray r = camera->getRay(hp->scrn_x - 256, (511-hp->scrn_y) - 256, 0, 0);
			float dist = random() * (float)MAX_MARCH_DEPTH;
			Vector3 vp = r.origin() + (r.direction() * dist);
			float rpos[3] = {vp.x(), vp.y(), vp.z()};

			float irrad2[3] = {0,0,0};
			volume_photonmap->irradiance_estimate(irrad2, rpos, 1.f/*max_dist*/, 50);
			volume_flux = Vector3(irrad2[0], irrad2[1], irrad2[2]);

			contrib += /*flux * (1.0f - hp->transmittance);*/ (volume_flux * (1.f - (dist / (float)MAX_MARCH_DEPTH))) * 0.000001f;
#endif

			int i = hp->scrn_x + hp->scrn_y * w;
			c[i] = c[i] + contrib;

			delete hp;
			delete prev;
		}

		hitpoints = NULL;

		delete photonmap;
#ifdef VOLUME_MODE
		delete volume_photonmap;
#endif

		std::cout << "Image updated\n";

		std::cout << "Cycle complete\n";

		if (i % 10 == 0)
		{
			sf::Image image;
			image.Create(w, h, sf::Color(0,0,0));

			sf::Sprite Sprite(image);
	
			for (int i = 0; i < w * h; i++)
			{
				image.SetPixel(i % w, floorf(i / w), sf::Color(toInt(c[i].x()), toInt(c[i].y()), toInt(c[i].z())));
			}

			image.SaveToFile("image.png");
		}

	}

	sf::Image image;
	image.Create(w, h, sf::Color(0,0,0));

	sf::Sprite Sprite(image);
	
	for (int i = 0; i < w * h; i++)
	{
		image.SetPixel(i % w, floorf(i / w), sf::Color(toInt(c[i].x()), toInt(c[i].y()), toInt(c[i].z())));
	}

	image.SaveToFile("image.png");

	fprintf(stderr, "Done\n");
}