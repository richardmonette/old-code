
#ifndef _MIRROR_H_
#define _MIRROR_H_ 1

#include "Material.h"
#include "rgb.h"
#include "RandomMwc.hpp"
#include "Vector3.h"

class Texture;
class RNG;

class MirrorMaterial : public Material
{
public:
   MirrorMaterial() {} 

   virtual rgb ambientResponse(const ONB&,
         const Vector3&,                        // incident direction
         const Vector3&,                        // Texture point
         const Vector2&);                       // Texture coordinate

   virtual bool scatterDirection(const Vector3&,// incident Vector
         const SurfaceHitRecord&,               // hit we are shading
         Vector2&,                              // random seed                    
         rgb&,                                  // color to attenuate by
         bool&,                                 // count emitted light?
         float&,                                // brdf scale
         Vector3&);                             // scattered direction

   virtual bool transmitDirection(const Vector3&,// incident Vector
         const SurfaceHitRecord&,               // hit we are shading
         Vector2&,                              // random seed                    
         rgb&,                                  // color to attenuate by
         bool&,                                 // count emitted light?
         float&,                                // brdf scale 
         Vector3&,
		 const float ior_variance);                           // scattered direction

   virtual bool isSpecular() { return true; }

   RandomMwc rng;
};
#endif // _MIRROR_H_
