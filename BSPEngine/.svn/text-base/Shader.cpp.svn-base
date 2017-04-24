#include "Shader.h"

#include "stdio.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

// Shader loading routines based off the following:
// <cite>Joe Groff http://duriansoftware.com/joe/An-intro-to-modern-OpenGL.-Chapter-2.2:-Shaders.html </cite>

Shader::Shader(const char *shaderName)
{
	ostringstream vertexShaderPath;
	vertexShaderPath << "shaders/" << shaderName << ".vert";

	vertexShader = MakeShader(GL_VERTEX_SHADER, vertexShaderPath.str().c_str());

	ostringstream fragmentShaderPath;
	fragmentShaderPath << "shaders/" << shaderName << ".frag";

    fragmentShader = MakeShader(GL_FRAGMENT_SHADER, fragmentShaderPath.str().c_str());
    
	program = MakeProgram(vertexShader, fragmentShader); 
}

Shader::~Shader()
{
}

void Shader::ShowInfoLog(GLuint object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog)
{
    GLint log_length;
    char *log;

    glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
    log = (char*)malloc(log_length);
    glGet__InfoLog(object, log_length, NULL, log);
    fprintf(stderr, "%s", log);
    free(log);
}

GLuint Shader::MakeShader(GLenum type, const char *filename)
{
	string line;
	string source;
	ifstream file(filename);

	if (file.is_open())
	{
		while (getline(file, line))
		{
			source.append(line);
			source.append("\n");
		}

		file.close();
	}

    GLint length = source.length();
    GLuint shader;
    GLint shader_ok;

	if (source.length() < 1)
        return 0;

	const GLchar *p = source.c_str();

	shader = glCreateShader(type);
	glShaderSource(shader, 1, &p, 0);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
    if (!shader_ok) {
		cout << source;
        fprintf(stderr, "Failed to compile %s:\n", filename);
        ShowInfoLog(shader, glGetShaderiv, glGetShaderInfoLog);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint Shader::MakeProgram(GLuint vertex_shader, GLuint fragment_shader)
{
    GLint program_ok;

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
    if (!program_ok) {
        fprintf(stderr, "Failed to link shader program:\n");
        ShowInfoLog(program, glGetProgramiv, glGetProgramInfoLog);
        glDeleteProgram(program);
        return 0;
    }
    return program;
}

