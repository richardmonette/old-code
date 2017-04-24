
#define WIN32_LEAN_AND_MEAN

#include <fstream>
#include <stdio.h>
#include <time.h>

#include <SFML/Graphics.hpp>

#include "Camera.h"
#include "Vector3.h"
#include "Scene.h"
#include "Renderer.h"
#include "Image.h"
#include "RandomMwc.hpp"
#include "Triangle.h"
#include "RandomMwc.hpp"
#include "IrradianceCache.h"
#include "Sample2.h"
#include "Util.h"
#include "IrradianceMap.h"
#include "WorkGenerator.h"
#include "SimpleTexture.h"

#include <windows.h>

const int image_width = 512;
const int image_height = 512;

const float half_image_width = image_width / 2.0f;
const float half_image_height = image_height / 2.0f;

sf::Image sfmlImage;
sf::Sprite sprite;

sf::RenderWindow *App;

Scene *scene;
Camera *camera;
Renderer *renderer;
Image *image;

WorkGenerator *workGenerator;

PhotonMap *causticPhotons;
PhotonMap *directPhotons;
PhotonMap *indirectPhotons;

PhotonMap *directVolumePhotons;
PhotonMap *indirectVolumePhotons;

IrradianceCache *irradianceCache;

IrradianceMap *irradianceMap;

const int numThreads = 8;

DWORD ThreadID[numThreads];
HANDLE TName[numThreads];
float rectpos[numThreads][numThreads];

void PrecomputeIrradiances()
{
	Report("Starting precomputation of irradiances");

	irradianceMap = new IrradianceMap((indirectPhotons->get_stored_photons() * 0.25f) + 1);

	Photon *photons = indirectPhotons->get_photons();

	for (int i = 0; i < indirectPhotons->get_stored_photons(); i+=4)
	{
		float n[3];

		indirectPhotons->photon_dir(n, &photons[i]);

		float irradiance[3];

		indirectPhotons->irradiance_estimate(irradiance, photons[i].pos, n, 10.0f, 50);

		irradianceMap->store(irradiance, photons[i].pos, n);
	}

	irradianceMap->balance();

	Report("Precomputation of irradiances complete");
}

static float sigma_a = 0.1; // absorption cross section
static float sigma_s = 0.1; // scattering coefficient
static float sigma_t = 0.1; // attenuation coefficient

void GeneratePhotonMaps()
{
	Report("Generating photon maps");

	const int nCausticPhotons = 0;//100000;
	causticPhotons = new PhotonMap(nCausticPhotons);

	const int nDirectPhotons = 100000;
	directPhotons = new PhotonMap(nDirectPhotons);

	const int nIndirectPhotons = 0;
	indirectPhotons = new PhotonMap(nIndirectPhotons);

	const int nDirectVolumePhotons = 10000000;
	directVolumePhotons = new PhotonMap(nDirectVolumePhotons);

	const int nIndirectVolumePhotons = 1000000;
	indirectVolumePhotons = new PhotonMap(nIndirectVolumePhotons);

	int nEmittedPhotons = 0;

	int v1_i = 0;
	int v2_i = 0;
	int dir1_i = 0;
	int dir2_i = 0;
	int light_i = 0;
	int continue_i = 0;

	Ray photonRay;
	SurfaceHitRecord rec;

	RandomMwc random;

	while ( causticPhotons->get_stored_photons() < nCausticPhotons || 
		    directPhotons->get_stored_photons() < nDirectPhotons || 
			indirectPhotons->get_stored_photons() < nIndirectPhotons)// ||
			//directVolumePhotons->get_stored_photons() < nDirectVolumePhotons ||
			//indirectVolumePhotons->get_stored_photons() < nIndirectVolumePhotons)
	{
		Vector2 v1(random.getFloat(), random.getFloat());//Vector2 v1(Halton(v1_i, 2), Halton(v1_i, 3));
		Vector2 v2(random.getFloat(), random.getFloat());//Vector2 v2(Halton(v2_i, 2), Halton(v2_i, 3));

		v1_i++;
		v2_i++;

		Triangle *light = static_cast<Triangle*>(scene->lights[int(Halton(light_i, 2) * 2.0f)]);

		light_i++;

		light->generatePhoton(photonRay, v1, v2);

		rgb photonColor = light->photonColor();
		
		nEmittedPhotons++;

		if ( photonRay.direction().y() > 0.1f ) // HACK
		{
			photonRay.data[2] *= -1;
		}

		bool specularPath = false;
		int nIntersections = 0;

		float ior_variance = 0.0f;

#ifdef DISPERSION
		float spectrum = photonRNG.getFloat();

		if (spectrum < .33f)
		{
			ior_variance = -.05;
			photonColor = rgb(1,0,0);
		}
		else if (spectrum < .66f)
		{
			ior_variance = 0;
			photonColor = rgb(0,1,0);
		}
		else
		{
			ior_variance = .05;
			photonColor = rgb(0,0,1);
		}
#endif

		int steps = 0;
		int maxSteps = 300;

		while (steps < maxSteps)
		{
			sigma_t = sigma_s + sigma_a;
			
			float alpha = sigma_s / sigma_t;

			if (random.getFloat() <= alpha)
			{
				Vector3 d(random.getFloat() - 0.5f, random.getFloat() - 0.5f, random.getFloat() - 0.5f);
				d.makeUnitVector();
				photonRay.setDirection(d * 1.0f + photonRay.direction() * 0.0f);
			}
			else
			{
				//break;
			}

			float power[3] = {photonColor.r(), photonColor.g(), photonColor.b()};
			float pos[3] = {photonRay.origin().x(), photonRay.origin().y(), photonRay.origin().z()};
			float dir[3] = {photonRay.direction().x(), photonRay.direction().y(), photonRay.direction().z()};
			float s_dir[3] = {6,6,6};
				
			directVolumePhotons->store(power, pos, dir, s_dir);

			float d = 1.0f / sigma_t;

			photonRay.setOrigin(photonRay.origin() + photonRay.direction() * d);

			steps++;

			bool isHit = scene->surfaces->hit(photonRay, 0.01f, 1.0f, rec);
			float dist = (photonRay.origin() - rec.p).squaredLength();
			
			if ( isHit && dist < 1.0f ) 
			{
				bool cel;
				float brdf;
				Vector3 v_out;

				nIntersections++;

				if ( !rec.mat_ptr->isSpecular() )
				{	
					Vector3 wo = photonRay.direction();

					float power[3] = {photonColor.r(), photonColor.g(), photonColor.b()};
					float pos[3] = {rec.p.x(), rec.p.y(), rec.p.z()};
					float dir[3] = {wo.x(), wo.y(), wo.z()};
					float s_dir[3] = {rec.uvw.w().x(), rec.uvw.w().y(), rec.uvw.w().z()};

					if (nIntersections == 1) // Process direct lighting photon intersection
					{
						directPhotons->store(power, pos, dir, s_dir);
					}
					else if (specularPath) //Process caustic photon intersection
					{
						causticPhotons->store(power, pos, dir, s_dir);

						break;
					}
					else // Process indirect lighting photon intersection
					{
						indirectPhotons->store(power, pos, dir, s_dir);
					}
				}
				else
				{
					if (nIntersections == 1)
					{
						specularPath = true;
					}
				}

				photonColor = photonColor * rec.mat_ptr->ambientResponse(rec.uvw, photonRay.direction(), rec.p, rec.uv); 

				if (nIntersections > 3) 
				{
					float continueProbability = .5f;

					if (Halton(continue_i, 2) > continueProbability)
					{
						break;
					}

					continue_i++;

					photonColor /= continueProbability;
				}

				Vector2 v1(Halton(dir1_i, 2), Halton(dir1_i, 3));
				Vector2 v2(Halton(dir2_i, 2), Halton(dir2_i, 3));

				dir1_i++;
				dir2_i++;

				rgb color;

				if ( rec.mat_ptr->transmitDirection(photonRay.direction(), rec, v1, color, cel, brdf, v_out, ior_variance) )
				{
					photonColor *= color;

					photonRay.setDirection(v_out);
					photonRay.setOrigin(rec.p);
				}
				else if ( rec.mat_ptr->scatterDirection(photonRay.direction(), rec, v2, color, cel, brdf, v_out) )
				{
					photonRay.setDirection(v_out);
					photonRay.setOrigin(rec.p);
				}

				break;
			}
		}
	}

	causticPhotons->scale_photon_power(1.0f / nEmittedPhotons);
	directPhotons->scale_photon_power(1.0f / nEmittedPhotons);
	indirectPhotons->scale_photon_power(1.0f / nEmittedPhotons);
	directVolumePhotons->scale_photon_power(1.0f / nEmittedPhotons);
	indirectVolumePhotons->scale_photon_power(1.0f / nEmittedPhotons);

	causticPhotons->balance();
	directPhotons->balance();
	indirectPhotons->balance();
	directVolumePhotons->balance();
	indirectVolumePhotons->balance();

	Report("Photon pre-process complete");

	cout << "Direct volume photons: " << directVolumePhotons->get_stored_photons() << "\n";
	cout << "Direct photons: " << directPhotons->get_stored_photons() << "\n";
	cout << "Emitted " << nEmittedPhotons << " photons.\n";
}

void UI_Update()
{
	sf::Context context;

	App = new sf::RenderWindow(sf::VideoMode(image_width, image_height, 32), "Render");
	sfmlImage.Create(image_width, image_height, sf::Color(0,0,0));
	sprite.SetImage(sfmlImage);

	while (true)
	{
		App->Draw(sprite);

		for (int i = 0; i < numThreads; i++)
		{
			sf::Shape Rect1 = sf::Shape::Rectangle(rectpos[i][0], image_height - rectpos[i][1] - 32, 32, 32, sf::Color(255,255,255), 1.0f, sf::Color(255,255,255));
			Rect1.EnableFill(false);
			Rect1.EnableOutline(true);

			App->Draw(Rect1);
		}

		App->Display();

		Sleep(1000);
	}
}

void Render(LPVOID lpParam)
{
	int min_x, min_y, max_x, max_y;

	const int id = *((int*)lpParam);

	RandomMwc random;

	while (workGenerator->GetNextBucket(min_x, min_y, max_x, max_y))
	{
		rectpos[id][0] = min_x;
		rectpos[id][1] = min_y;

		for (float x = min_x; x < max_x; x++)
		{
			for (float y = min_y; y < max_y; y++)
			{
				rgb color(0,0,0);

				for (int j = 0; j < 4; j++)
				{
					Ray r = camera->getRay(x - half_image_width + (Halton(j, 2) - 0.5f), y - half_image_height + (Halton(j, 3) - 0.5f), 0, 0);

					float intersect_distance;

					rgb c = renderer->color(r, scene, 0.01, 1000, Vector2(random.getFloat(), random.getFloat()), Vector2(random.getFloat(), random.getFloat()), true, 0, true, true, intersect_distance, x, y, 128.0f) * 0.25f;
				
					color += max(powf(2.718, -sigma_t * intersect_distance), 0.0f) * c;

					float distStepped = 0.0f;
					Vector3 stepPos = r.origin();

					/*rgb volumeRadiance(0,0,0);

					while (distStepped < intersect_distance)
					{
						stepPos += r.direction() * 2.0f;
						distStepped += 2.0f;

						float irradiance[3];
						float pos[3] = {stepPos.x(), stepPos.y(), stepPos.z()};
						float n[3] = {6,6,6};
					
						float max_dist = 2.0f;
						int num_photons = 100;

						directVolumePhotons->irradiance_estimate(irradiance, pos, n, max_dist, num_photons);
						volumeRadiance += powf(2.71, -sigma_t * distStepped) * rgb(irradiance[0], irradiance[1], irradiance[2]);
					}

					color += volumeRadiance;*/
				}

				image->setPixel(x, y, color);

				const float Ywa = 10.0f;
				const float pre_scale = 1.0f;
				const float alpha = 0.1f;
				const float burn = 5.0f;
				const float post_scale = 1.0f;

				const float Yw = pre_scale * alpha * burn;
				const float invY2 = 1.f / (Yw * Yw);
				const float pScale = post_scale * pre_scale * alpha / Ywa;
				const float ys = color[1];

				color *= (pScale * (1.f + ys * invY2) / (1.f + ys));

				const float gamma = 1.0 / 2.2;

				color[0] = pow(color[0], gamma);
				color[1] = pow(color[1], gamma);
				color[2] = pow(color[2], gamma);

				sfmlImage.SetPixel(x, image_height - y, sf::Color(min(color[0] * 255.f, 255.f), min(color[1] * 255.f, 255.f), min(color[2] * 255.f, 255.f)));
			}
		}
	}
}

int main(int argc, char* argv[])
{
	Report("Intaglio Renderer");
	Report("Created by Richard Monette");
	Report("Starting up");

	time_t then = time(NULL);

	const Vector3 origin(0,50,250);
	const Vector3 gaze(0,0,-1);
	const Vector3 up(0,1,0);

	camera = new Camera(origin, gaze, up, 1, -1, 1, -1, 1, 1500);

	scene = new Scene();
	scene->background = new SimpleTexture(rgb(0,0,0));

	const int maxSpecularSteps = 10;

	renderer = new Renderer(maxSpecularSteps);

	GeneratePhotonMaps();

	//PrecomputeIrradiances();

	Vector3 modelMin(scene->GetMin()); //-100,-50,-50
	Vector3 modelMax(scene->GetMax()); // 100,150,150

	irradianceCache = new IrradianceCache(100000, modelMin, modelMax);

	renderer->irradianceCache = irradianceCache;
	renderer->irradianceMap = irradianceMap;
	renderer->causticPhotons = causticPhotons;
	renderer->indirectPhotons = indirectPhotons;
	renderer->directPhotons = directPhotons;

	image = new Image(image_width, image_height);

	DWORD uiThreadID;
	HANDLE UI_Thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)UI_Update, NULL, 0, &uiThreadID);

	Sleep(1000);

	workGenerator = new WorkGenerator(image_width, image_height, 32, 32);

	int id[8] = {0,1,2,3,4,5,6,7};

	for (int i = 0; i < 1; i++) // Two passes (first primes the Irradiance Cache)
	{
		for (int j = 0; j < numThreads; j++)
		{
			TName[j] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Render, &id[j], 0, &ThreadID[j]);
		}

		WaitForMultipleObjects(numThreads, TName, true, INFINITE);

		//CloseHandle(UI_Thread);
		//CloseHandle(TName);

		workGenerator->Reset();
	}

	//irradianceCache->DrawPoints(irradianceCache->octree, image);

	ofstream imageFile("image_out.ppm", std::ios::binary);
	image->writePPM(imageFile);
	imageFile.close();

	int dif = difftime (time(NULL), then);

	int seconds = dif % 60;
	int minutes = (dif - seconds) / 60;

	cout << "\nEverything complete! Took " << minutes << "m " << seconds << "s \a\n";

	delete camera;
	delete renderer;
	delete causticPhotons;
	delete directPhotons;
	delete indirectPhotons;
	delete irradianceCache;

	cin.get();

	return 0;
}

