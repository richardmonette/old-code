
#include "Skybox.h"
#include "Main.h"

Skybox::Skybox()
{
	CreateTexture(sky_front, "textures/front", true);
	CreateTexture(sky_back,  "textures/back", true);
	CreateTexture(sky_top,   "textures/top", true);
	CreateTexture(sky_left,  "textures/left", true);
	CreateTexture(sky_right, "textures/right", true);
}

Skybox::~Skybox()
{
}

		/*glActiveTexture(GL_TEXTURE0);
		glClientActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,  m_textures[pFace->textureID]);
		glUniform1i(shader->GetDiffuseLocation(), 0);*/

void Skybox::Render()
{
	glPushMatrix();
	glUseProgram(skyboxShader.program);
	glDisable(GL_CULL_FACE);

	glActiveTexture(GL_TEXTURE0);
	glClientActiveTexture(GL_TEXTURE0);
	glUniform1i(skyboxShader.GetDiffuseLocation(), 0);

	glBindTexture(GL_TEXTURE_2D,  sky_front);
	glBegin(GL_QUADS);
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1000.0f, -1000.0f,  1000.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1000.0f, -1000.0f,  1000.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1000.0f,  1000.0f,  1000.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1000.0f,  1000.0f,  1000.0f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,  sky_back);
	glBegin(GL_QUADS);
		// Back Face
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1000.0f, -1000.0f, -1000.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1000.0f,  1000.0f, -1000.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1000.0f,  1000.0f, -1000.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1000.0f, -1000.0f, -1000.0f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, sky_top);
	glBegin(GL_QUADS);
		// Top Face
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1000.0f,  1000.0f, -1000.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1000.0f,  1000.0f,  1000.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1000.0f,  1000.0f,  1000.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1000.0f,  1000.0f, -1000.0f);
	glEnd();

	/*glBegin(GL_QUADS);
		// Bottom Face
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glEnd();*/

	glBindTexture(GL_TEXTURE_2D, sky_right);
	glBegin(GL_QUADS);
		// Right face
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1000.0f, -1000.0f, -1000.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1000.0f,  1000.0f, -1000.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1000.0f,  1000.0f,  1000.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1000.0f, -1000.0f,  1000.0f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, sky_left);
	glBegin(GL_QUADS);
		// Left Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1000.0f, -1000.0f, -1000.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1000.0f, -1000.0f,  1000.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1000.0f,  1000.0f,  1000.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1000.0f,  1000.0f, -1000.0f);
	glEnd();

	glPopMatrix();

	glEnable(GL_CULL_FACE);
}
