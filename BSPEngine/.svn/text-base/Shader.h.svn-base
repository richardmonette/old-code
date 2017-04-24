
#pragma once

#include "GL/glew.h"
#include "freeglut.h"

class Shader
{

public:
	
	Shader(const char *shaderName);
	~Shader();

	void ShowInfoLog(GLuint object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog);
	GLuint MakeShader(GLenum type, const char *filename);
	GLuint MakeProgram(GLuint vertex_shader, GLuint fragment_shader);

	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint program;
	
};
