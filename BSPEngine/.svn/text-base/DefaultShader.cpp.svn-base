
#include "DefaultShader.h"

DefaultShader::DefaultShader() : Shader("default")
{
	diffuseLocation = glGetUniformLocation(program, "diffuseSampler");
	lightmapLocation = glGetUniformLocation(program, "lightmapSampler");

	noLightmaps = glGetUniformLocation(program, "noLightmaps");
	noTextures = glGetUniformLocation(program, "noTextures");

	tmaps = 0;
	lmaps = 0;

	glUseProgram(program);

	glUniform1i(noLightmaps, lmaps);
	glUniform1i(noTextures, tmaps);
}

DefaultShader::~DefaultShader()
{
}

GLuint DefaultShader::GetDiffuseLocation()
{
	return diffuseLocation;
}

GLuint DefaultShader::GetLightmapLocation()
{
	return lightmapLocation;
}
