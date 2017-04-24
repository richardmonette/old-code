#pragma once

#define NOMINMAX // for PhysX

#include "GL/glew.h"
#include "freeglut.h"

#include <stdio.h>

#define MAX_TEXTURES 1000

bool CreateTexture(GLuint &texture, const char* strFileName, bool clampToEdge = false);
void CheckGLError(const char* file, int line);

void ReleaseKey(unsigned char key, int x, int y);
