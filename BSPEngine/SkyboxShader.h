#pragma once

#include "Shader.h"

class SkyboxShader : public Shader
{
public:
	SkyboxShader();
	~SkyboxShader();
	
	GLuint GetDiffuseLocation();

	GLuint diffuseLocation;
};
