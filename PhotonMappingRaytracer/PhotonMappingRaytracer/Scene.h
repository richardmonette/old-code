#ifndef SCENE_H
#define SCENE_H

#include "SurfaceList.h"
#include "Texture.h"
#include "DynArray.h"
#include "ObjModel.h"

class Scene 
{
 public:
   
   Scene();

   Vector3 GetMin()
   {
	   return Vector3(model->minX - 50.0f, model->minY - 50.0f, model->minZ - 50.0f);
   }

   Vector3 GetMax()
   {
	   return Vector3(model->maxX + 50.0f, model->maxY + 50.0f, model->maxZ + 50.0f);
   }
   
   void setView(const Vector3& e, 
	        const Vector3& g, 
		const Vector3& u,
	        float aper, 
		float l, 
		float r, 
		float t, 
		float b, 
		float d) 
   { 
      eye = e; gaze = g; up = u; aperture = aper; 
      left = l; right = r; top = t; bottom = b; distance = d; 
   }
   
   // viewing parameters
   Vector3 eye;
   Vector3 gaze;
   Vector3 up;
   float aperture;
   float left;
   float right;
   float top;
   float bottom;
   float distance;

   ObjModel *model;
   
   // scene geometry
   Surface* surfaces;
   DynArray<Surface*> lights;
   Texture *background;
};


#endif
