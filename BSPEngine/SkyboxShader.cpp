
#include "SkyboxShader.h"

SkyboxShader::SkyboxShader() : Shader("skybox")
{
	diffuseLocation = glGetUniformLocation(program, "diffuseSampler");
}

SkyboxShader::~SkyboxShader()
{
}

GLuint SkyboxShader::GetDiffuseLocation()
{
	return diffuseLocation;
}
