#pragma once

#include "Shader.h"

class DefaultShader : public Shader
{
public:
	DefaultShader();
	~DefaultShader();

	void ToggleLightmaps()
	{
		if (lmaps == 1) lmaps = 0;
		else lmaps = 1;

		glUseProgram(program);
		glUniform1i(noLightmaps, lmaps);
	}

	void ToggleTextures()
	{
		if (tmaps == 1) tmaps = 0;
		else tmaps = 1;

		glUseProgram(program);
		glUniform1i(noTextures, tmaps);
	}

	GLuint GetDiffuseLocation();
	GLuint GetLightmapLocation();

	GLuint diffuseLocation;
	GLuint lightmapLocation;

	GLuint noLightmaps;
	GLuint noTextures;

	int lmaps;
	int tmaps;
};
