
#include "Scene.h"
#include "Surface.h"
#include "Sphere.h"
#include "BVH.h"
#include "SurfaceList.h"
#include "Triangle.h"

Scene::Scene() 
{
	model = new ObjModel("emptyRoom.obj");

	Surface **temp_surfaces = new Surface*[model->numTriangles];

	for (int i = 0; i < model->numTriangles; i++)
	{
		temp_surfaces[i] = new Triangle(Vector3(model->triangles[i].vertices[0].x, model->triangles[i].vertices[0].y, model->triangles[i].vertices[0].z), 
			                       Vector3(model->triangles[i].vertices[1].x, model->triangles[i].vertices[1].y, model->triangles[i].vertices[1].z),
				            		Vector3(model->triangles[i].vertices[2].x, model->triangles[i].vertices[2].y, model->triangles[i].vertices[2].z),

									Vector2(model->triangles[i].uvs[0].u, model->triangles[i].uvs[0].v),
									Vector2(model->triangles[i].uvs[1].u, model->triangles[i].uvs[1].v),
									Vector2(model->triangles[i].uvs[2].u, model->triangles[i].uvs[2].v),

									Vector3(model->triangles[i].normals[0].x, model->triangles[i].normals[0].y, model->triangles[i].normals[0].z), 
			                       Vector3(model->triangles[i].normals[1].x, model->triangles[i].normals[1].y, model->triangles[i].normals[1].z),
				            		Vector3(model->triangles[i].normals[2].x, model->triangles[i].normals[2].y, model->triangles[i].normals[2].z),

									model->triangles[i].mptr);

		if (model->triangles[i].isLight)
		{
			lights.append(temp_surfaces[i]);
		}
	}

	BVH *bvh = new BVH(temp_surfaces, model->numTriangles);

	surfaces = bvh;
}

