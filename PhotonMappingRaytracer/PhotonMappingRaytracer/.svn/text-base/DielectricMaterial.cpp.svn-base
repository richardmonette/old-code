
#include "DielectricMaterial.h"

DielectricMaterial::DielectricMaterial(float _nt, const rgb& _ex) 
   : nt(_nt), extinction(_ex) 
{
   R0 = (nt - 1.0f) / (nt + 1.0f);
   R0 *= R0;

   float r = log(extinction.r());
   float g = log(extinction.g());
   float b = log(extinction.b());

   extinction = rgb(r, g, b);
}

rgb DielectricMaterial::ambientResponse(const ONB&, const Vector3&, const Vector3& p, const Vector2& uv) 
{ 
	return rgb(1.0f, 1.0f, 1.0f); 
}

bool DielectricMaterial::transmitDirection(const Vector3& in_dir,
      const SurfaceHitRecord& rec, 
      Vector2& seed, 
      rgb& color, 
      bool& CEL,
      float& brdf,
      Vector3& v_out,
	  const float ior_variance)
{
	float old_nt = nt;

	nt += ior_variance;

	R0 = (nt - 1.0f) / (nt + 1.0f);
    R0 *= R0;

	CEL = true;
   
	float R;
	Vector3 normal = rec.uvw.w();
	float cosine = dot(in_dir, normal);

	if (cosine < 0.0f) // ray is incoming
	{
		// determine reflectance
		cosine = -cosine;
		float temp1 = 1.0f - cosine;
		R = R0 + (1.0f - R0) * temp1*temp1*temp1*temp1*temp1;
			
		float temp2 = 1.0f / nt;
		float root  = 1.0f - (temp2 * temp2) * (1.0f - cosine * cosine);
		v_out = in_dir * temp2 + normal * (temp2 * cosine - sqrt(root));

		brdf  = (1.0f - R);

		color = rgb(1.0f, 1.0f, 1.0f); 
	}
	else // ray is outgoing
	{
		// determine color attenuation
		color.data[0] = exp(extinction.data[0] * rec.t);
		color.data[1] = exp(extinction.data[1] * rec.t);
		color.data[2] = exp(extinction.data[2] * rec.t);

		float temp2 = -(dot(in_dir, -normal));
		float root = 1.0f - (nt * nt) * (1.0f - temp2 * temp2);
		if (root < 0.0f) // total internal reflection
		{ 
			v_out = reflect(in_dir, -normal); 
			brdf = 1.0f; 
		}
		else
		{
			// determine reflectance
			float temp1 = 1.0f - cosine;
			R = R0 + (1.0f - R0) * temp1*temp1*temp1*temp1*temp1;
         
			v_out = in_dir*nt + -normal*(nt*temp2 - sqrt(root));
			brdf = (1.0f - R);
		}
	}

	nt = old_nt;

	return true;
}

bool DielectricMaterial::scatterDirection (const Vector3& in_dir,
      const SurfaceHitRecord& rec, 
      Vector2& seed, 
      rgb& color, 
      bool& CEL,
      float& brdf,
      Vector3& v_out)
{
	CEL = true;
   
	float R;
	Vector3 normal = rec.uvw.w();
	float cosine = dot(in_dir, normal);

	if (cosine < 0.0f) // ray is incoming
	{
		// determine reflectance
		cosine = -cosine;
		float temp1 = 1.0f - cosine;
		R = R0 + (1.0f - R0) * temp1*temp1*temp1*temp1*temp1;

		v_out = reflect(in_dir, normal); 
		brdf  = R;

		color = rgb(1.0f, 1.0f, 1.0f); 
	}
	else // ray is outgoing
	{
		// determine color attenuation
		color.data[0] = exp(extinction.data[0] * rec.t);
		color.data[1] = exp(extinction.data[1] * rec.t);
		color.data[2] = exp(extinction.data[2] * rec.t);

		float temp2 = -(dot(in_dir, -normal));
		float root = 1.0f - (nt * nt) * (1.0f - temp2 * temp2);
		v_out = reflect(in_dir, -normal); 

		if (root < 0.0f) // total internal reflection
		{ 
			brdf = 1.0f;
		}
		else
		{
			// determine reflectance
			float temp1 = 1.0f - cosine;
			R = R0 + (1.0f - R0) * temp1*temp1*temp1*temp1*temp1;

			brdf = R;
		}
	}
	return true;
}
   
