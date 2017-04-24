
#include "MirrorMaterial.h"
//#include <math.h>

bool MirrorMaterial::scatterDirection (const Vector3& in_dir,
      const SurfaceHitRecord& rec, 
      Vector2& seed, 
      rgb& color, 
      bool& CEL,
      float& brdf,
      Vector3& v_out)
{
	/*CEL = true;

	Vector3 normal = rec.uvw.w();

	v_out = reflect(in_dir, normal); 
	brdf  = 1.0f;

	color = rgb(1.0f, 1.0f, 1.0f); 

   return true;*/

	CEL = true;

	const float R0 = 0.8;
   
	float R;
	Vector3 normal = rec.uvw.w();
	float cosine = dot(in_dir, normal);

	// determine reflectance
	cosine = -cosine;
	float temp1 = 1.0f - cosine;
	R = R0 + (1.0f - R0) * temp1*temp1*temp1*temp1*temp1;

	v_out = reflect(in_dir, normal); 
	brdf  = R;

	color = rgb(1.0f, 1.0f, 1.0f); 

	return true;
}

rgb MirrorMaterial::ambientResponse(const ONB&, const Vector3&, const Vector3& p, const Vector2& uv) 
{ 
	return rgb(1.0f, 1.0f, 1.0f); 
}

bool MirrorMaterial::transmitDirection(const Vector3&,// incident Vector
         const SurfaceHitRecord&,               // hit we are shading
         Vector2&,                              // random seed                    
         rgb&,                                  // color to attenuate by
         bool&,                                 // count emitted light?
         float&,                                // brdf scale 
         Vector3&,
		 const float ior_variance)                           // scattered direction
{
	return false;
}
   
