#pragma once

#include "Shader.h"

class WaterShader :
	public Shader
{
public:
	WaterShader();
	~WaterShader();

	void CreateReflectionFBO();
	void CreateRefractionFBO();

	GLuint reflectionTextureID;
	GLuint normalTextureID;
	GLuint dudvTextureID;
	GLuint refractionTextureID;

	GLuint reflectionTextureLocation;
	GLuint refractionTextureLocation;
	GLuint normalTextureLocation;
	GLuint dudvTextureLocation;

	GLuint time;

	GLuint refractionFBO, reflectionFBO;
	GLuint refractionFBODepth, reflectionFBODepth;
};
