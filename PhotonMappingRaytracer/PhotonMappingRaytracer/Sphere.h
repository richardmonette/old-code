

#ifndef _SPHERE_H_  
#define _SPHERE_H_ 1 

#include "Surface.h"

class Material;

const float M_PI = 3.14159265359f;

class Sphere : public Surface 
{
public:

   Sphere( Vector3 c, float r, Material* mptr);

   virtual bool hit(const Ray& r,           
         float tmin,              
         float tmax,             
         SurfaceHitRecord& rec)const;

    virtual bool shadowHit(
          const Ray& r,            
          float tmin,             
          float tmax)const; 

    virtual bool randomPoint(const Vector3&,  
          const Vector2&,
          Vector3&,        
          Vector3&,        
          float&,          
          rgb&)const;

    virtual BBox boundingBox() const;
    
    Vector3 center;
    float radius;
    Material *mat_ptr;
};

#endif // _SPHERE_H_
