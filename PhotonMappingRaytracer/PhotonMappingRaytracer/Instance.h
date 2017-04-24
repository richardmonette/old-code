
#ifndef _INSTANCE_H_
#define _INSTANCE_H_ 1

#include "Surface.h"
#include "Matrix.h"

class Matrix;

class Instance : public Surface
{
public:
   Instance () {}
   ~Instance() {}
   Instance(Matrix trans, Matrix trans_inverse, Surface* _prim);
   Instance(Matrix trans, Surface* _prim);
   bool hit(const Ray& r, float tmin, float tmax,
         SurfaceHitRecord& rec)const; 
   bool shadowHit(const Ray& r, float tmin, float tmax)const;
   BBox boundingBox()const;
   bool randomPoint(const Vector3&, const Vector2&, float time, Vector3&, 
         Vector3&, float&, rgb&)const;
  
   Matrix M;
   Matrix N;
   Surface* prim;
};

#endif // _INSTANCE_H_
