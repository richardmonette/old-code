
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "glut.h"

#include "Octree.h"
#include "ImprovedNoise.h"
#include "camera.h"
#include "ObjModel.h"

Octree *octree;
CCamera *camera;

float randfloat() 
{ 
    return rand()/(float(RAND_MAX)+1); 
} 

float randfloat(float max) 
{ 
    return randfloat()*max; 
} 

float randfloat(float min, float max) 
{ 
    if (min>max) 
    { 
        return randfloat()*(min-max)+max;     
    } 
    else 
    { 
        return randfloat()*(max-min)+min; 
    }     
} 

void changeSize(int w, int h) 
{
	if(h == 0)
		h = 1;

	float ratio = 1.0 * w / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
    glViewport(0, 0, w, h);

	gluPerspective(45,ratio,1,5000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0,0,0,  0,0,0,  0,0,0);
}

void renderScene(void) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();  

	camera->Render();

	glBegin(GL_POINTS);

	octree->Draw(0, camera->GetPosition());

	glEnd();

	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y) 
{
	if (key == 27) 
	{
		exit(0);
	}

	if (key == 'w')
    {
		camera->MoveForward(-1.0);
    }
    else if (key == 's')
    {
		camera->MoveForward(1.0);
    }
    else if (key == 'd')
    {
		camera->StrafeRight(-1.0);
    }
    else if (key == 'a')
    {
		camera->StrafeRight(1.0);
    }
	else if (key == 'b')
	{
		/*Voxel *voxel = new Voxel();
		voxel->position = camera->GetPosition();
		voxel->color = Vector3(randfloat(), randfloat(), randfloat());
		octree->add(voxel);*/
		cout << "New Block: " << camera->GetPosition().x() << " " << camera->GetPosition().y() << " " << camera->GetPosition().z() << "\n";
	}
}

float lastx, lasty;

void processMouse(int button, int state, int x, int y) 
{
	lastx = x;
	lasty = y;
}

void mouseMovement(int x, int y) 
{
    int diffx = x - lastx;
    int diffy = y - lasty;
    lastx = x;
    lasty = y;
	camera->SetViewByMouse((float)diffx * 0.1, (float)diffy * 0.1);
}

int main(int argc, char* argv[])
{
	const int DIM = 2048;

	camera = new CCamera();
	camera->Move(Vector3(0,0,1));

	octree = new Octree(Vector3(-DIM,-DIM,-DIM), Vector3(DIM,DIM,DIM), 0);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE); 
	glutInitWindowPosition(500,300);
	glutInitWindowSize(512,512);
	glutCreateWindow("Octree Test");
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

	glutMotionFunc(mouseMovement);
	glutMouseFunc(processMouse);
	glutKeyboardFunc(processNormalKeys);

	glEnable(GL_DEPTH_TEST);
	glEnable(GLUT_MULTISAMPLE);

	InitNoise();

	ObjModel *model = new ObjModel("bunny.obj");

	for (int i = 0; i < model->numTriangles; i++)
	{
		Voxel *voxel = new Voxel();
		voxel->position = (model->triangles[i].p0 + model->triangles[i].p1 + model->triangles[i].p2) * 0.333f;
		float ndotl = max(dot(model->triangles[i].n0, Vector3(0,1,0)),0.f) + 0.1f;
		voxel->color = Vector3(ndotl,ndotl,ndotl);
		octree->add(voxel);
	}

	delete model;

	octree->ComputeColor();

	glPointSize(3.0);

	glutMainLoop();

	delete octree;
	delete camera;

	return 0;
}

