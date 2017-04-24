
#include <SFML/Graphics.hpp>

#include <math.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include "Math3D.h"
#include "ImprovedNoise.h"

using namespace std;

const int width = 512;
const int height = 512;

const float step = 0.1f;

struct Color
{
	Color(float _red, float _green, float _blue, float _alpha)
	{
		red = _red;
		green = _green;
		blue = _blue;
		alpha = _alpha;
	}

	float red, green, blue, alpha;
};

struct Ray
{
	Vec3 origin;
	Vec3 direction;
};

float dist(const Vec3 &a, const Vec3 &b)
{
	Vec3 temp(b.x - a.x, b.y - a.y, b.z - a.z);

	return fabs(sqrtf(temp.x * temp.x + temp.y * temp.y + temp.z * temp.z));
}

inline float f(float x, float y, float z)
{

#define SPHERE

#ifdef CUBE

	x += 0.1f;
	y += 0.1f;

	//if (x < .25 && x > -.25)
	{
		//if (y < .25 && y > -.25)
		{
			if (z < .25)//if (z < .25 && z > -.25)
			{
				if (ceilf(sin(x * 20.f)) > 0.1f && ceilf(sin(y * 20.f)) > 0.1f && ceilf(sin(z * 20.f)) > 0.1f)
				{
					return 1.f;
				}
			}
		}
	}

	return 0.f;

#endif

#ifdef RING
	const Vec thispos(x,y,z);
	const Vec center(0,0,0);

	float d = dist(thispos, center);

	if (d < 100 && d > 90 && z > 0 && z < 10)
	{
		return 1.f;
	}

	return 0.f;
#endif

#ifdef SPHERE
	/*const float radius = 0.25f;
	const Vec3 thispos(x,y,z);
	const Vec3 spherePos(0,0,0);

	float d = dist(thispos, spherePos);

	if (d < radius)
	{
		return 1.f;
	}
	else
	{
		return 0.f;
	}*/
	
	const float noise = Noise(x*10.0,y*10.0,z*10.0,0) * 0.5f;

	const float outer_radius = 0.3f + noise;
	const float inner_radius = 0.2f + noise;

	const float rr0 = outer_radius * outer_radius;
	const float rr1 = inner_radius * inner_radius;

	const float t = x * x + y * y + z * z;

	if (t > rr0)
		return 0.;
	else if (t < rr1)
		return 1.;
	else
		return (t - rr0) / (rr1 - rr0);

#endif

#ifdef METABALLS

	const float radius = 0.1f;

	const Vec3 thispos(x,y,z);

	Vec3 spheres[3] = {Vec3(0,0,0), Vec3(.2,-.2,0), Vec3(-.2,-.2,.1)};

	float sum = 0.f;

	for (int i = 0; i < 3; i++)
	{
		float d = dist(thispos, spheres[i]);

		sum += radius / d;
	}

	const float sum0 = 1.5f;
	const float sum1 = 1.6f;

	if (sum > sum1)
		return 1.;
	else if (sum < sum0)
		return 0.;
	else
		return (sum - sum0) / (sum1 - sum0);

#endif

}

inline Vec3 getShading(const Vec3 &point, const Vec3 &normal, const Ray &ray)
{
	const Vec3 light(.57f, 0.57f, 0.57f);

	float nl = dot(normal, light);

	nl += 0.1f;

	nl *= 0.75f;

	if (nl < 0) nl = 0;
	if (nl > 1) nl = 1;

	//return Vec3((normal.x + 1.0f) * 0.5f, (normal.y + 1.0f) * 0.5f, (normal.z + 1.0f) * 0.5f);

	Vec3 V = ray.direction;
	Vec3 R = light - 2.0f * dot( light, normal ) * normal;

	float vr = dot( V, R );

	if (vr < 0) vr = 0;

	float spec = powf( vr, 20 );

	spec += nl;

	if (spec < 0) spec = 0;
	if (spec > 1) spec = 1;
	
	return Vec3(spec, spec, spec);
}

inline Vec3 getMaterial(const Vec3 &p, const Vec3 &n)
{
	return Vec3(1,1,1);
}

inline Vec3 applyFog(Vec3 &c, float t)
{
	return c;
}

inline Vec3 getNormal(const Vec3 &point) 
{
	const float normal_step = 0.005f;

	Vec3 d(f(point.x - normal_step, point.y, point.z) - f(point.x + normal_step, point.y, point.z),
		   f(point.x, point.y - normal_step, point.z) - f(point.x, point.y + normal_step, point.z),
		   f(point.x, point.y, point.z - normal_step) - f(point.x, point.y, point.z + normal_step) );

	normalizeSelf(d);

	return d;
}

inline Vec3 surfaceColor( const Ray & ray, float t )
{
	const Vec3 p = ray.origin + ray.direction * t;
	const Vec3 n = getNormal( p );
	const Vec3 s = getShading( p, n, ray );
	const Vec3 m = getMaterial( p, n );

	return applyFog( m * s, t );
}

bool castRay( const Vec3 &ro, const Vec3 &rd, float &resT )
{
	const float delt = 0.001f;
	const float mint = 0.001f;
	const float maxt = 2.0f;

	for( float t = mint; t < maxt; t += delt )
	{
		const Vec3 p = ro + rd * t;

		if( f( p.x, p.y, p.z ) > 0.999f )
		{
			resT = t - 0.5f * delt;
			return true;
		}
	}

	return false;
}


inline Vec3 skyColor(const float x, const float y)
{
	const Vec3 color1X(.078f, .078f, .074f);
	const Vec3 color2X(.12f, .12f, .10f);

	const Vec3 color1Y(.27f, .24f, .20f);
	const Vec3 color2Y(.85f, .76f, .58f);

	const float tx = x / (float)width;
	const Vec3 colorX = tx * color1X + (1 - tx) * color2X;

	const float ty = y / (float)height;
	const Vec3 colorY = ty * color1Y + (1 - ty) * color2Y;

	return (colorX + colorY) * 0.5f;
}

inline Ray generateRay(float x, float y)
{
	Ray r;

	Vec3 cameraUp(0,-1,0); // Just hacking to flip the image "right side up"
	Vec3 cameraRight(1,0,0);
	Vec3 viewDirection(0,0,-1);
	Vec3 viewPosition(0,0,1);
	double viewAngle = toRadians(60.0f);

	double tanViewAngleHalf = tan(viewAngle * 0.5f);

	double xF = ((x) * 2.0f / width ) - 1.0f;//(x / width) - 0.5f;//
	double yF = ((y) * 2.0f / height ) - 1.0f;//(y / height) - 0.5f;//

	Vec3 offset = ( (cameraRight * xF) + (cameraUp * yF * (height / width)) );

	r.direction = (viewDirection + (offset * tanViewAngleHalf));
	normalizeSelf(r.direction);

	r.origin = viewPosition;

	return r;
}

void main()
{
	sf::Image image(width, height, sf::Color(0,0,0));
	sf::Sprite sprite;
	sprite.SetImage(image);

	InitNoise();

	for( int j = 0; j < height; j++ )
	{
		for( int i = 0; i < width; i++ )
		{
			Ray ray = generateRay( i, j );

			float t;

			if( castRay( ray.origin, ray.direction, t ) )
			{
				Vec3 c = surfaceColor( ray, t );

				image.SetPixel(i, j, sf::Color(c.x * 255, c.y * 255, c.z * 255));
			}
			else
			{
				Vec3 c = skyColor(i, j);

				image.SetPixel(i, j, sf::Color(c.x * 255, c.y * 255, c.z * 255));
			}
		}

		system("cls");
		cout << (int)(j / (float)height * 100.0f) << "%";
	}

	ostringstream strstream;
	string currentContents;
	int index = -1;

	fstream in;
	in.open ("index.txt");
	in >> currentContents;
	index = atoi(currentContents.c_str());	
	in.close();

	ofstream out;
	out.open("index.txt");
	out << (index+1);
	out.close();

	strstream << "isosurface" << index << ".png";

	image.SaveToFile(strstream.str().c_str());

	sf::RenderWindow App(sf::VideoMode(width, height, 32), "Ray Marching");

	while (App.IsOpened())
	{
		sf::Event Event;
		while (App.GetEvent(Event))
		{
			if (Event.Type == sf::Event::Closed)
				App.Close();
		}

		App.Clear();

		App.Draw(sprite);

		App.Display();
	}

}