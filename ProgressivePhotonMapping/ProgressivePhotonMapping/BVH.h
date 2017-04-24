
#ifndef _BVH_H_
#define _BVH_H_

#include "Surface.h"
#include "BBox.h"

class SurfaceList;

class BVH : public Surface
{

public:

   BVH();
   BVH(SurfaceList* surfaces);
   BVH(Surface** surfaces, int num_surfaces);
   BVH(Surface* prim1, Surface* prim2);
   BVH(Surface* prim1, Surface* prim2, const BBox& _bbox);
   bool hit(const Ray& r, float tmin, float tmax, SurfaceHitRecord& rec)const;
   bool shadowHit(const Ray& r, float tmin, float tmax) const;
   BBox boundingBox()const;
   Surface* buildBranch (Surface** surfaces, int num_surfaces, int axis = 0);
   bool randomPoint(const Vector3&, const Vector2&, Vector3&, Vector3&, float&, Vector3& )const { return false; }

   BBox bbox;
   Surface* left;
   Surface* right;

};

inline BVH::BVH(Surface* prim1, Surface* prim2, const BBox& _bbox) 
{
   bbox = _bbox;
   left  = prim1;
   right = prim2;
}

inline BVH::BVH(Surface* prim1, Surface* prim2) 
{
   left  = prim1;
   right = prim2;
   bbox = surround(prim1->boundingBox(), 
                   prim2->boundingBox());
}

#endif // _BVH_H_
