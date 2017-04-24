
#include "Scene.h"
#include "Surface.h"
#include "BVH.h"
#include "SurfaceList.h"
#include "Triangle.h"

Scene::Scene() 
{
	model = new ObjModel("cornell.obj");

	Surface **temp_surfaces = new Surface*[model->numTriangles];

	for (int i = 0; i < model->numTriangles; i++)
	{
		temp_surfaces[i] = new Triangle(Vector3(model->triangles[i].p0.x(), model->triangles[i].p0.y(), model->triangles[i].p0.z()), 
			                            Vector3(model->triangles[i].p1.x(), model->triangles[i].p1.y(), model->triangles[i].p1.z()),
				            		    Vector3(model->triangles[i].p2.x(), model->triangles[i].p2.y(), model->triangles[i].p2.z()),

										Vector2(model->triangles[i].u0.x(), model->triangles[i].u0.y()),
										Vector2(model->triangles[i].u1.x(), model->triangles[i].u1.y()),
										Vector2(model->triangles[i].u2.x(), model->triangles[i].u2.y()),

										Vector3(model->triangles[i].n0.x(), model->triangles[i].n0.y(), model->triangles[i].n0.z()), 
										Vector3(model->triangles[i].n1.x(), model->triangles[i].n1.y(), model->triangles[i].n1.z()),
				            			Vector3(model->triangles[i].n2.x(), model->triangles[i].n2.y(), model->triangles[i].n2.z()),

										model->triangles[i].material, model->triangles[i].color);

	}

	BVH *bvh = new BVH(temp_surfaces, model->numTriangles);

	surfaces = bvh;
}

