
#ifndef _DIELECTRIC_H_
#define _DIELECTRIC_H_ 1

#include "Material.h"
#include "rgb.h"
#include "RandomMwc.hpp"
#include "Vector3.h"

class Texture;
class RNG;

class DielectricMaterial : public Material
{
public:
   DielectricMaterial() {} 
   DielectricMaterial(float nt, const rgb& _ex); 

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

   float R0;
   float nt;
   rgb   extinction;
   RandomMwc rng;
};
#endif // _DIELECTRIC_H_
