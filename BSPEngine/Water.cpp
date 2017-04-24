
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "stdio.h"

using namespace std;

#include "Water.h"

Water::Water()
{
	waterDisplacement = 0.0;
	height = 50.f;

	waterShader = new WaterShader();
}

Water::~Water()
{
}

float t = 0.0f;

void Water::Render(Vector3 *cameraPosition)
{
	// Setup the water quad and use the correct shader

	glUseProgram(waterShader->program);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, waterShader->reflectionTextureID);
	glGenerateMipmapEXT(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, waterShader->normalTextureID);
	glGenerateMipmapEXT(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, waterShader->dudvTextureID);
	glGenerateMipmapEXT(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0 + 3);
	glBindTexture(GL_TEXTURE_2D, waterShader->refractionTextureID);
	glGenerateMipmapEXT(GL_TEXTURE_2D);

	glUniform1i(waterShader->reflectionTextureLocation, 0);
	glUniform1i(waterShader->normalTextureLocation, 1);
	glUniform1i(waterShader->dudvTextureLocation, 2);
	glUniform1i(waterShader->refractionTextureLocation, 3);

	glUniform1f(waterShader->time, t);

	t += 0.05;

	GLfloat vertices[12] = { 500,50,500,  500,50,-500,  -500,50,-500,  -500,50,500 };
	GLbyte indices[4] = { 3, 2, 1, 0 };
	GLfloat uvs[8] = { 5.0+waterDisplacement,5.0, 5.0+waterDisplacement,0.0, 0.0+waterDisplacement,0.0, 0.0+waterDisplacement,5.0 };
	GLfloat uvs2[8] = { 5.0,5.0, 5.0,0.0, 0.0,0.0, 0.0,5.0 };

	waterDisplacement += 0.001;

	glEnableClientState(GL_VERTEX_ARRAY);

		glActiveTexture(GL_TEXTURE0 + 1);
		glClientActiveTexture(GL_TEXTURE0 + 1);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 2 * sizeof(GL_FLOAT), uvs );

		glActiveTexture(GL_TEXTURE0 + 2);
		glClientActiveTexture(GL_TEXTURE0 + 2);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 2 * sizeof(GL_FLOAT), uvs2 );

		glVertexPointer(3, GL_FLOAT, 0, vertices);

		glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, indices);

	glDisableClientState(GL_VERTEX_ARRAY);
}
