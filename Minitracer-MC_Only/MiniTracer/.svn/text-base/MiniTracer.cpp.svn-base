#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <map>

#include <SFML/Graphics.hpp>

#include "SurfacePoint.hpp"
#include "RandomMwc.hpp"
#include "Global.h"
#include "SpatialIndex.hpp"
#include "Triangle.hpp"

const double w = 320;
const double h = 240;
const double samps = 10;

using namespace hxa7241_general;
using namespace hxa7241_minilight;

SpatialIndex *spatialIndex;
RandomMwc random;

vector<Triangle> triangles;
vector<const Triangle*> emitters;
map<string, Material *> materials;

Vec GetReflection
(
   Vec& inDirection,
   Vec& inRadiance,
   Vec& outDirection,
   Vec& normal,
   Vec &reflectivity
) 
{
   const float inDot  = dot( inDirection,  normal );
   const float outDot = dot( outDirection, normal );

   return Vec( (inDot < 0.0f) ^ (outDot < 0.0f) ? Vec(0,0,0) : (inRadiance.mult(reflectivity)) * (fabsf( inDot ) / M_PI) );
}

/*Vec SampleEmitters(Vec& rayDirection, Vec& surfacePoint)
{
	Vec radiance;

	// single emitter sample, ideal diffuse BRDF:
	// reflected = (emitivity * solidangle) * (emitterscount) * (cos(emitdirection) / pi * reflectivity)
	// -- SurfacePoint does the first and last parts (in separate methods)

	// get position on an emitter
	Vec emitterPosition;
	int randomEmitterIndex = static_cast<int>(emitters.size() * random.getFloat());
	float randomU = random.getFloat();
	float randomV = random.getFloat();
	emitters[randomEmitterIndex]->GetPositionBarycentric(&emitterPosition, randomU, randomV);
	int emitterId = emitters[randomEmitterIndex]->id;

	// make direction to emit point
	Vec emitDirection( emitterPosition - surfacePoint );
	float distanceToLight = emitDirection.magnitude();
	emitDirection.norm();

	// send shadow ray
	double intersectDistance = 999999999.0f;
	Vec intersectPosition;
	Vec intersectNormal;
	Triangle tri;
	Triangle *intersectTriangle = 0;

	octree.IntersectTriangles(Ray(surfacePoint, emitDirection), intersectDistance, intersectPosition, intersectNormal, tri);
	intersectTriangle = &tri;

	// if unshadowed, get inward emission value
	Vec emissionIn;
	if( fabs(intersectDistance - distanceToLight)  < 0.01f && tri.id == emitterId )
	{
		emissionIn.x = tri.emission.x;//SurfacePoint( *emitterId, emitterPosition ).getEmission(surfacePoint, -emitDirection, true );
		emissionIn.y = tri.emission.y;
		emissionIn.z = tri.emission.z;
	}

	return emissionIn;

	// get amount reflected by surface
	radiance = GetReflection( emitDirection, emissionIn * numEmitters, rayDirection * -1, tri.getNormal(), tri.reflectivity );

	return radiance;
}

Vec Radiance(Ray &ray, int depth)
{
	Vec radiance(0,0,0);
	
	double intersectDistance = 999999999.0f;
	Vec intersectPosition;
	Vec intersectNormal;
	Triangle tri;
	Triangle *intersectTriangle = NULL;

	octree.IntersectTriangles(ray, intersectDistance, intersectPosition, intersectNormal, tri);
	intersectTriangle = &tri;

	if (intersectDistance >= 1000000.0) 
	{
		return Vec(0,0,0); // If there is no intersection then just return black
	}

	Vec color = intersectTriangle->color;
	
	if (++depth > 5)
	{
		double reflectivityMean = dot( intersectTriangle->reflectivity, Vec(1,1,1) ) / 3.0;
		
		if (random.getFloat() < reflectivityMean)
		{
			color = intersectTriangle->reflectivity * (1.0 / reflectivityMean);
		}
		else
		{
			return intersectTriangle->emission; //R.R.
		}
	}

	Vec nl = dot(intersectNormal, ray.direction) < 0 ? intersectNormal : intersectNormal * -1;

	radiance = radiance + SampleEmitters( ray.direction, intersectPosition);
	
	// Start handling recursion for our three material types: DIFFUSE, SPECULAR and REFRACTION

	{
		// Ideal DIFFUSE reflection
		if (intersectTriangle->reflectionType == DIFFUSE) 
		{                  
			// cosine-weighted importance sample hemisphere

			const float _2pr1 = M_PI * 2.0f * random.getFloat();
			const float sr2   = sqrt( random.getFloat() );

			// make coord frame coefficients (z in normal direction)
			const float x = cos( _2pr1 ) * sr2;
			const float y = sin( _2pr1 ) * sr2;
			const float z = sqrt( 1.0f - (sr2 * sr2) );

			Vec tangent( intersectTriangle->getTangent() );

			// make vector from frame times coefficients
			Vec d = (tangent * x) + (cross( nl, tangent ) * y) + (nl * z);

			radiance = radiance + color.mult( Radiance(Ray(intersectPosition, d), depth) );
		} 
		// Ideal SPECULAR reflection
		else if (intersectTriangle->reflectionType == SPECULAR) 
		{
			radiance = radiance + color.mult(Radiance(Ray(intersectPosition, ray.direction - intersectNormal * 2 * dot(intersectNormal, ray.direction)), depth));
		}
		// Ideal dielectric REFRACTION
		else 
		{
			Ray reflRay(intersectPosition, ray.direction - intersectNormal * 2 * dot(intersectNormal, ray.direction));     
			bool into = dot(intersectNormal, nl) > 0;                // Ray from outside going in?
			double nc = 1;
			double nt = 1.5;
			double nnt = into ? nc / nt : nt / nc; // Pick index of refraction depending on going in or out
			double ddn = dot(ray.direction, nl);
			double cos2t = 1 - nnt * nnt * (1 - ddn * ddn);
			
			if (cos2t < 0) 
			{
				// Total internal reflection
				radiance = radiance + color.mult(Radiance(reflRay,depth));
			}

			Vec tdir = (ray.direction*nnt - intersectNormal*((into?1:-1)*(ddn*nnt+sqrt(cos2t)))).norm();
			
			double a = nt - nc;
			double b = nt + nc;
			double R0 = a * a / (b * b);
			double c = 1 - ( into ? -ddn : dot(tdir, intersectNormal) );
			double Re = R0 + (1 - R0) * c * c * c * c * c;
			double Tr = 1 - Re;
			double P = .25 + .5 * Re;
			double RP = Re/P;
			double TP = Tr/(1-P);
			
			// Russian roulette
			radiance = radiance + color.mult(depth > 2 ? (random.getFloat() < P ? Radiance(reflRay,depth)*RP : Radiance(Ray(intersectPosition,tdir),depth)*TP) : Radiance(reflRay,depth)*Re+Radiance(Ray(intersectPosition,tdir),depth)*Tr);
		}
	}

	return radiance;
}*/

double toRadians(double degrees)
{
	return degrees * (3.14/180.0);
}

void StringExplode(string str, string separator, vector<string>* results)
{
	int found;
	found = str.find_first_of(separator);
	while(found != string::npos){
		if(found > 0){
			results->push_back(str.substr(0,found));
		}
		str = str.substr(found+1);
		found = str.find_first_of(separator);
	}
	if(str.length() > 0){
		results->push_back(str);
	}
}

void ParseMtlFile(const char *filename)
{
	vector<string> shrapnel;
	string currentMtl;

	FILE *file = fopen ( filename, "r" );

	if ( file != NULL )
	{
		char line[128];

		while ( fgets( line, sizeof line, file ) != NULL )
		{
			shrapnel.clear();
			string lineStr = line;
			StringExplode(lineStr, " ", &shrapnel);

			if (strcmp(shrapnel[0].c_str(), "newmtl") == 0)
			{
				materials[shrapnel[1]] = new Material;
				currentMtl = shrapnel[1];
			}
			else if (strcmp(shrapnel[0].c_str(), "illum") == 0)
			{
				
			}
			else if (strcmp(shrapnel[0].c_str(), "Kd") == 0)
			{
				vector<string> diffuseComponents;
				StringExplode(lineStr, " ", &diffuseComponents);
				materials[currentMtl]->diffuse.x = atof(diffuseComponents[1].c_str());
				materials[currentMtl]->diffuse.y = atof(diffuseComponents[2].c_str());
				materials[currentMtl]->diffuse.z = atof(diffuseComponents[3].c_str());
			}
			else if (strcmp(shrapnel[0].c_str(), "Ka") == 0)
			{
				vector<string> ambientComponents;
				StringExplode(lineStr, " ", &ambientComponents);
				materials[currentMtl]->ambient = Vec( atof(ambientComponents[1].c_str()), atof(ambientComponents[2].c_str()), atof(ambientComponents[3].c_str()) );
			}
			else if (strcmp(shrapnel[0].c_str(), "Tf") == 0)
			{
			}
			else if (strcmp(shrapnel[0].c_str(), "Ni") == 0)
			{
				materials[currentMtl]->refractiveIndex = atof(shrapnel[1].c_str());
			}
			else if (strcmp(shrapnel[0].c_str(), "Ks") == 0)
			{
				vector<string> specularComponents;
				StringExplode(lineStr, " ", &specularComponents);
				materials[currentMtl]->specular = Vec( atof(specularComponents[1].c_str()), atof(specularComponents[2].c_str()), atof(specularComponents[3].c_str()) );
			}
		}

		fclose ( file );
	}
}

void LoadOBJFile()
{
	vector<Vector3f> vertices;
	vector<UV> uvs;

	vector<string> shrapnel;
	string currentMtl;

	FILE *file = fopen ( "scene.obj", "r" );

	if ( file != NULL )
	{
		char line[128];

		while ( fgets( line, sizeof line, file ) != NULL )
		{
			shrapnel.clear();
			string lineStr = line;
			StringExplode(lineStr, " ", &shrapnel);

			if (strcmp(shrapnel[0].c_str(), "v") == 0)
			{
				vertices.push_back(Vector3f(atof(shrapnel[1].c_str()), atof(shrapnel[2].c_str()), atof(shrapnel[3].c_str())));
			}
			else if (strcmp(shrapnel[0].c_str(), "vt") == 0)
			{
				uvs.push_back(UV(atof(shrapnel[1].c_str()), atof(shrapnel[2].c_str())));
			}
			else if (strcmp(shrapnel[0].c_str(), "f") == 0)
			{
				vector<string> vert0, vert1, vert2;
				StringExplode(shrapnel[1], "/", &vert0);
				StringExplode(shrapnel[2], "/", &vert1);
				StringExplode(shrapnel[3], "/", &vert2);

				Triangle tri(vertices[atoi(vert0[0].c_str())-1],
							 vertices[atoi(vert1[0].c_str())-1],
							 vertices[atoi(vert2[0].c_str())-1],
							 uvs[atoi(vert0[1].c_str())-1],
							 uvs[atoi(vert1[1].c_str())-1],
							 uvs[atoi(vert2[1].c_str())-1]
							);

				triangles[numTriangles].emission = materials[currentMtl]->ambient;

				if ( triangles[numTriangles].emission.x > 0 )
				{
					emitters[numEmitters] = &triangles[numTriangles];
					numEmitters++;
				}

				triangles[numTriangles].color = materials[currentMtl]->diffuse;
				triangles[numTriangles].reflectivity = materials[currentMtl]->diffuse;
				triangles[numTriangles].reflectionType = DIFFUSE;

				if ( materials[currentMtl]->refractiveIndex > 1.0f ) 
				{
					triangles[numTriangles].reflectionType = REFRACTIVE;
				}

				if ( materials[currentMtl]->specular.x > 0 )
				{
					triangles[numTriangles].reflectionType = SPECULAR;
				}

				for (int i = 0; i < 3; i++)
				{
					triangles[numTriangles].boundingBox.Expand(&triangles[numTriangles].vertices[i]);
				}

				numTriangles++;
			}
			else if (strcmp(shrapnel[0].c_str(), "usemtl") == 0)
			{
				currentMtl = shrapnel[1];
			}
			else if (strcmp(shrapnel[0].c_str(), "mtllib") == 0)
			{
				ParseMtlFile(shrapnel[1].substr(0,shrapnel[1].length()-1).c_str());
			}
		}

		fclose ( file );
	}
}

int main()
{
	LoadOBJFile();

	spatialIndex = new SpatialIndex();

	sf::RenderWindow App(sf::VideoMode(w, h, 32), "MiniTracer");
	sf::Image image(w, h, sf::Color(0, 0, 0));
	image.LoadFromFile("background.png");
	sf::Sprite sprite;
	sprite.SetImage(image);

	Vec r;

	double sampleWeight = 1.0f / samps;
	Vec cameraUp(0,-1,0); // Just hacking to flip the image "right side up"
	Vec cameraRight(1,0,0);
	Vec viewDirection(0,0,-1);
	Vec viewPosition(0,0,50);
	double viewAngle = toRadians(60.0f);
	Vec offset;
	Vec d;

	double tanViewAngleHalf = tan(viewAngle * 0.5);

	App.Clear();

	for (int y = 0; y < h; y++)
	{                       
		for (unsigned short x = 0; x < w; x++)
		{	
			for (int s = 0; s < samps; s++)
			{
				double xF = ((x + random.getFloat()) * 2.0 / w ) - 1.0;
				double yF = ((y + random.getFloat()) * 2.0 / h ) - 1.0;

				offset = ( (cameraRight * xF) + (cameraUp * yF * (h / w)) );

				d = (viewDirection + (offset * tanViewAngleHalf));
				d.norm();

				r = r + Radiance(Ray( viewPosition, d ), 0) * sampleWeight;
			} 

			int rr = r.x * 255;
			int gg = r.y * 255;
			int bb = r.z * 255;

			if (rr > 255) rr = 255;
			if (gg > 255) gg = 255;
			if (bb > 255) bb = 255;

			image.SetPixel(x, y, sf::Color(rr, gg, bb));
			r.zero();
		}
		
		App.Draw(sprite);
		App.Display();
	}

	sf::Image screen = App.Capture();
	screen.SaveToFile("render.png");
}
