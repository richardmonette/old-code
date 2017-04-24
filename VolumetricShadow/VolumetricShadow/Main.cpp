
#include <stdio.h>
#include <pez.h>
#include <vectormath_aos.h>
#include <glsw.h>
#include <glew.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <time.h>

#include "RenderTexture.h"
#include "ImprovedNoise.h"
#include "Types.h"
#include "Octree.h"

using namespace Vectormath::Aos;
using namespace std;

enum { SlotPosition, SlotColor };

struct ShaderUniforms {
    GLuint Projection;
	GLuint ProjectionGeom;
    GLuint Modelview;
    GLuint NormalMatrix;
};

struct RenderContext {
    GLuint EffectHandle;
    ShaderUniforms EffectUniforms;
    Matrix4 Projection;
    Matrix4 Modelview;
	Matrix4 LightModelView;
    Matrix3 NormalMatrix;
    float PackedNormalMatrix[9];
    float Theta;
    int IndexCount;
};

RenderTexture *myRenderTexture;

static RenderContext GlobalRenderContext;
static GLuint BuildShader(const char* source, GLenum shaderType);
static GLuint BuildProgram(const char* vsKey, const char* fsKey);
static void LoadEffect();
static void LoadMesh();

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

const char* PezInitialize(int width, int height)
{
	int err = glGetError();
	if (err != GL_NO_ERROR)
	{
		__debugbreak();
	}

	//myRenderTexture = new RenderTexture(512, 512, GL_RGBA);

	err = glGetError();
	if (err != GL_NO_ERROR)
	{
		__debugbreak();
	}

    LoadMesh();

	err = glGetError();
	if (err != GL_NO_ERROR)
	{
		__debugbreak();
	}

	LoadEffect();

	err = glGetError();
	if (err != GL_NO_ERROR)
	{
		__debugbreak();
	}

    //glDisable(GL_DEPTH_TEST);

	//glBlendFunc(GL_ONE, GL_ONE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glPointSize(1.0f);

	//glDisable(GL_CULL_FACE);

    return "Volumetric Shadow";
}

const static int NUM = 4979166;//600000;//62291;//

void PezRender()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RenderContext& rc = GlobalRenderContext;
    
	glUniformMatrix4fv(rc.EffectUniforms.Projection, 1, 0, &rc.Projection[0][0]);
	glUniformMatrix4fv(rc.EffectUniforms.ProjectionGeom, 1, 0, &rc.Projection[0][0]);
	glUniformMatrix4fv(rc.EffectUniforms.Modelview, 1, 0, &rc.Modelview[0][0]);
	glUniformMatrix3fv(rc.EffectUniforms.NormalMatrix, 1, 0, &rc.PackedNormalMatrix[0]);

	glDrawElements(GL_POINTS, rc.IndexCount, GL_UNSIGNED_INT, 0);
}

Point3 cameraPosition(0,0,50);

void PezUpdate(unsigned int elapsedMilliseconds)
{
    RenderContext& rc = GlobalRenderContext;

    rc.Modelview = Matrix4::lookAt(cameraPosition, Point3(0,0,0), Vector3(0,1,0));
	rc.LightModelView = Matrix4::lookAt(Point3(40,30,1), Point3(0,0,0), Vector3(0,1,0));
    rc.NormalMatrix = rc.Modelview.getUpper3x3();

	GLuint LightPosition = glGetUniformLocation(rc.EffectHandle, "LightPosition");
	glUniform3f(LightPosition, sin(rc.Theta) * 5.0, 10.0, cos(rc.Theta) * 5.0);

    for (int i = 0; i < 9; ++i)
        rc.PackedNormalMatrix[i] = rc.NormalMatrix[i / 3][i % 3];

    const float x = 0.6f;
    const float y = x * PEZ_VIEWPORT_HEIGHT / PEZ_VIEWPORT_WIDTH;
    const float left = -x, right = x;
    const float bottom = -y, top = y;
    const float zNear = 4, zFar = 1000;
    rc.Projection = Matrix4::frustum(left, right, bottom, top, zNear, zFar);
}

void PezHandleMouse(int x, int y, int action)
{
}

static GLuint BuildShader(const char* source, GLenum shaderType)
{
    GLint compileSuccess;
    GLchar messages[256];
    GLuint shaderHandle = glCreateShader(shaderType);
    glShaderSource(shaderHandle, 1, &source, 0);
    glCompileShader(shaderHandle);
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);
    glGetShaderInfoLog(shaderHandle, sizeof(messages), 0, &messages[0]);
    PezCheckCondition(compileSuccess, messages);
    return shaderHandle;
}

static GLuint BuildProgram(const char* vsKey, const char *gsKey, const char* fsKey)
{
    PezDebugString("Compiling '%s'...\n", vsKey);
    const char* vsString = glswGetShader(vsKey);
    PezCheckCondition(vsString, glswGetError());
    GLuint vsHandle = BuildShader(vsString, GL_VERTEX_SHADER);

	PezDebugString("Compiling '%s'...\n", gsKey);
    const char* gsString = glswGetShader(gsKey);
    PezCheckCondition(gsString, glswGetError());
    GLuint gsHandle = BuildShader(gsString, GL_GEOMETRY_SHADER_EXT);

    PezDebugString("Compiling '%s'...\n", fsKey);
    const char* fsString = glswGetShader(fsKey);
    PezCheckCondition(fsString, glswGetError());
    GLuint fsHandle = BuildShader(fsString, GL_FRAGMENT_SHADER);

    PezDebugString("Linking...\n");
    GLint linkSuccess;
    GLchar messages[256];
    GLuint programHandle = glCreateProgram();
    glAttachShader(programHandle, vsHandle);
	glAttachShader(programHandle, gsHandle);
    glAttachShader(programHandle, fsHandle);

	glProgramParameteriEXT(programHandle, GL_GEOMETRY_INPUT_TYPE_EXT, GL_POINTS);
	glProgramParameteriEXT(programHandle, GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_TRIANGLE_STRIP/*GL_POINTS*/);
	glProgramParameteriEXT(programHandle, GL_GEOMETRY_VERTICES_OUT_EXT, 4);

	//int temp;
	//glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &temp);
	//glProgramParameteriEXT(programHandle, GL_GEOMETRY_VERTICES_OUT_EXT, temp);

    glBindAttribLocation(programHandle, SlotPosition, "Position");
	glBindAttribLocation(programHandle, SlotColor, "Color");
    glLinkProgram(programHandle);
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
    glGetProgramInfoLog(programHandle, sizeof(messages), 0, &messages[0]);
    PezCheckCondition(linkSuccess, messages);
	PezDebugString("Program done...\n");
    return programHandle;
}

float min_dist = 999999;
float max_dist = -999999;

int compare (const void * a, const void * b)
{
	Vector3 *aa = (Vector3*)a;
	Vector3 *bb = (Vector3*)b;

	Vector3 va = cameraPosition - Point3(aa->getX(), aa->getY(), aa->getZ());
	float da = va.getX()*va.getX() + va.getY()*va.getY() + va.getZ()*va.getZ();

	Vector3 vb = cameraPosition - Point3(bb->getX(), bb->getY(), bb->getZ());
	float db = vb.getX()*vb.getX() + vb.getY()*vb.getY() + vb.getZ()*vb.getZ();

	if (da < min_dist) min_dist = da;
	if (db < min_dist) min_dist = db;

	if (da > max_dist) max_dist = da;
	if (db > max_dist) max_dist = db;

	return ( db - da );
}

static void LoadMesh()
{
	time_t start, end;
	time_t start_readingData, end_readingData;
	time_t start_sorting, end_sorting;
	time_t start_makeTree, end_makeTree;
	time_t start_lighting, end_lighting;

	time(&start);

    RenderContext& rc = GlobalRenderContext;

	const int vertexCount = NUM;

	Vector3 *positions = new Vector3[vertexCount];
	Vector3 *colors = new Vector3[vertexCount];

	time(&start_readingData);

	FILE *file = fopen ( "C:\\particles.bin", "rb" );

	fread(positions, sizeof(Vector3), vertexCount, file);

	fclose(file);

	time(&end_readingData);

	time(&start_sorting);

	qsort(positions, vertexCount, sizeof(Vector3), compare);

	time(&end_sorting);

	PezDebugString("Sorting complete...\n");

	time(&start_makeTree);

	Octree *octree = new Octree(Vector3(-50,-50,-50), Vector3(50,50,50), NULL);

	for (int i = 0; i < vertexCount; i += 2)
	{
		octree->Add(&positions[i]);
	}

	octree->Adfs();

	time(&end_makeTree);

	PezDebugString("Tree build complete...\n");

	time(&start_lighting);

	Vector3 lightPosition(0,10,0);

	#pragma omp parallel for
	for (int i = 0; i < vertexCount; i++)
	{
		Vector3 d = lightPosition - positions[i];
		Ray r(positions[i], Vectormath::Aos::normalize(d));

		int pcount = 0;

		octree->Intersect(r, &pcount);

		float power = powf(2.718f, -pcount * 0.01f);

		colors[i].setX(power);
		colors[i].setY(power);
		colors[i].setZ(power);
	}

	time(&end_lighting);

	PezDebugString("Min distance is: %f...\n", sqrtf(min_dist));
	PezDebugString("Max distance is: %f...\n", sqrtf(max_dist));

	const int indexCount = NUM;

	rc.IndexCount = indexCount;

	unsigned int *indices = new unsigned int[indexCount];

	for (int i = 0; i < NUM; i++)
	{
		indices[i] = i;
	}

    // Create the VBO for positions:

	{
		GLuint handle;
		GLsizeiptr size = vertexCount * sizeof(Vector3);
		glGenBuffers(1, &handle);
		glBindBuffer(GL_ARRAY_BUFFER, handle);
		glBufferData(GL_ARRAY_BUFFER, size, positions, GL_STATIC_DRAW);
		glEnableVertexAttribArray(SlotPosition);
		glVertexAttribPointer(SlotPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);
	}

	// Create the VBO for colors:

	{
		GLuint handle;
		GLsizeiptr size = vertexCount * sizeof(Vector3);
		glGenBuffers(1, &handle);
		glBindBuffer(GL_ARRAY_BUFFER, handle);
		glBufferData(GL_ARRAY_BUFFER, size, colors, GL_STATIC_DRAW);
		glEnableVertexAttribArray(SlotColor);
		glVertexAttribPointer(SlotColor, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);
	}

    // Create the VBO for indices:

	{
		GLuint handle;
		GLsizeiptr size = indexCount * sizeof(unsigned int);
		glGenBuffers(1, &handle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	}

	time (&end);

	float dif_total = difftime(end, start);
	float dif_readData = difftime(end_readingData, start_readingData);
	float dif_sorting = difftime(end_sorting, start_sorting);
	float dif_tree = difftime(end_makeTree, start_makeTree);
	float dif_lighting = difftime(end_lighting, start_lighting);

	PezDebugString("Reading data took: %fs...\n", dif_readData);
	PezDebugString("Sorting took: %fs...\n", dif_sorting);
	PezDebugString("Tree building took: %fs...\n", dif_tree);
	PezDebugString("Lighting took: %fs...\n", dif_lighting);
	PezDebugString("Total setup took: %fs...\n", dif_total);
}

static void LoadEffect()
{
    RenderContext& rc = GlobalRenderContext;

    glswInit();
    glswSetPath("", ".glsl");
    glswAddDirectiveToken("GL3", "#version 130");

    const char* vsKey = "VolumetricShadow.Vertex." PEZ_GL_VERSION_TOKEN;
	const char* gsKey = "VolumetricShadow.Geometry." PEZ_GL_VERSION_TOKEN;
    const char* fsKey = "VolumetricShadow.Fragment." PEZ_GL_VERSION_TOKEN;

    rc.EffectHandle = BuildProgram(vsKey, gsKey, fsKey);
    rc.EffectUniforms.Projection = glGetUniformLocation(rc.EffectHandle, "Projection");
	rc.EffectUniforms.ProjectionGeom = glGetUniformLocation(rc.EffectHandle, "ProjectionGeom");
    rc.EffectUniforms.Modelview = glGetUniformLocation(rc.EffectHandle, "Modelview");
    rc.EffectUniforms.NormalMatrix = glGetUniformLocation(rc.EffectHandle, "NormalMatrix");
    rc.Theta = 0;

    glUseProgram(rc.EffectHandle);

    GLuint LightPosition = glGetUniformLocation(rc.EffectHandle, "LightPosition");
    GLuint AmbientMaterial = glGetUniformLocation(rc.EffectHandle, "AmbientMaterial");
    GLuint DiffuseMaterial = glGetUniformLocation(rc.EffectHandle, "DiffuseMaterial");
    GLuint SpecularMaterial = glGetUniformLocation(rc.EffectHandle, "SpecularMaterial");
    GLuint Shininess = glGetUniformLocation(rc.EffectHandle, "Shininess");

    glUniform3f(DiffuseMaterial, 0.75, 0.75, 0.5);
    glUniform3f(AmbientMaterial, 0.04f, 0.04f, 0.04f);
    glUniform3f(SpecularMaterial, 0.5, 0.5, 0.5);
    glUniform1f(Shininess, 50);
    glUniform3f(LightPosition, 0, 1.0, 0);
}
