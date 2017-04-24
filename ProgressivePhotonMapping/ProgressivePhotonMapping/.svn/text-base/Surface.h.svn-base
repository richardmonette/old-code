#ifndef SURFACE_H
#define SURFACE_H

//  Abstract class Surface may be subclassed to a particular type of 
//  Surface such as a Sphere, or may be subclassed to an object set such
//  as a space subdivision grid.
//

#include "BBox.h"
#include "ONB.h"
#include "Vector2.h"

class SurfaceHitRecord 
{
 public:
   float t;            // Ray hits at p = Ray.origin() + r*Ray.direction()
   Vector3 p;          // point of intersection
   Vector3 texp;       // point of intersection for Texture mapping
   ONB uvw;           // w is the outward normal
   Vector2 uv;
   Vector3 color;
   int material;
};

class Surface 
{
 public:

    // does a Ray hit the Surface?
    virtual bool hit(        
          const Ray& r,             // Ray being sent
          float tmin,               // minimum hit parameter to be searched for
          float tmax,               // maximum hit parameter to be searched for
          SurfaceHitRecord& rec 
          ) const = 0;

    // does a Ray hit any Surface?  Allows early termination 
    virtual bool shadowHit(        
          const Ray& r,             // Ray being sent
          float tmin,               // minimum hit parameter to be searched for
          float tmax                // maximum hit parameter to be searched for
          ) const { 
       SurfaceHitRecord rec;
       return hit(r, tmin, tmax, rec);
    }

    virtual BBox boundingBox() const = 0;

    virtual bool randomPoint(const Vector3&,  // lighted point
          const Vector2&,          // random seed
          Vector3&,                // random point
          Vector3&,                // normal vector
          float&,                  // pdf
          Vector3&                     // emitted radiance
          ) const = 0;

    virtual int causticPhotons() { return 0; }
    virtual int globalPhotons() { return 0; }
    virtual Vector3 photonColor() { return Vector3(0, 0, 0); }
    virtual bool generatePhoton(Ray & ray, const Vector2&, const Vector2&)
    { return false; }
};


#endif
