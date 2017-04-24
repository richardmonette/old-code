
#include "Main.h"
#include "Camera.h"
#include "Quake3Bsp.h"
#include "Frustum.h"
#include "Shader.h"
#include "DefaultShader.h"
#include "WaterShader.h"
#include "TextShader.h"
#include "Image.h"
#include "Skybox.h"
#include "Water.h"
#include "Flythrough.h"
#include "Sound.h"

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

#include "freeglut.h"

Camera camera;
Q3BSP level;
Skybox *skybox;
Water *water;
DefaultShader *defaultShader;
Frustum frustum;
TextShader *textShader;
Sound sound;

ofstream flypath;

const char* fullscreenString = "1920x1200:32@60";
const int width = 1024;
const int height = 768;
const bool fullscreen = false;

int lasttime = 0;
float milliseconds = 0;

Vector2 currentMousePosition(0,0);
Vector2 previousMousePosition(0,0);
Vector2 deltaMousePosition(0,0);

bool forwardDown = false;
bool backwardDown = false;
bool rightDown = false;
bool leftDown = false;

void CheckGLError(const char* file, int line)
{
#ifdef _DEBUG	
	GLenum errCode;
	const GLubyte *errString;

	if ((errCode = glGetError()) != GL_NO_ERROR) 
	{
		errString = gluErrorString(errCode);
		fprintf (stderr, "GL Err: %s %d %s\n", file, line, errString);
		int i = 0;
	}
#endif
}

void RenderText(float x, float y, const char *string)
{
	int i, len;

	glDisable(GL_TEXTURE_2D);

	glUseProgram(textShader->program);

	glLoadIdentity();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glTranslatef(0.0f, 0.0f, -5.0f);
	glRasterPos2f(x, y);

	for (i = 0, len = strlen(string); i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (int)string[i]);
	}
	glEnable(GL_TEXTURE_2D);
}

bool InitGame()
{
	ifstream config;
	config.open("Config.ini");
	
	string strLevel = "";
	string strTemp = "";

	if(config.fail())
	{
		return false;
	}

	config >> strLevel >> strLevel;

	config.close();

	if(!level.LoadBSP(strLevel.c_str()))
	{
		return false;
	}

	camera.SetPhysicsController(level.physicsManager.CreateCharacterController(&camera,80,220,55));
	camera.PositionCamera( 80, 220, 55,	0, 0, 1,	0, 1, 0);

	glEnable(GL_DEPTH_TEST);	
	glEnable(GL_TEXTURE_2D);

	glCullFace(GL_FRONT);
 	glEnable(GL_CULL_FACE);

	defaultShader = new DefaultShader();
	textShader = new TextShader();

	skybox = new Skybox();

	CheckGLError(__FILE__,__LINE__);

	water = new Water();

	CheckGLError(__FILE__,__LINE__);

	return true;
}

float theta = 0.0f;

bool CreateTexture(GLuint &texture, const char* strFileName, bool clampToEdge)
{
	if(!strFileName) 
		return false;

	tImage *pImage = NULL;

	string withextension = strFileName;
	withextension.append(".tga");

	pImage = LoadTGA(withextension.c_str());

	if(pImage == NULL)	
	{
		texture = -666;
		return false;
	}

	glGenTextures(1, &texture);

	if (clampToEdge)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	glBindTexture(GL_TEXTURE_2D, texture);

	int textureType = GL_RGB;

	if(pImage->channels == 4)
		textureType = GL_RGBA;
		
	gluBuild2DMipmaps(GL_TEXTURE_2D, pImage->channels, pImage->sizeX, pImage->sizeY, textureType, GL_UNSIGNED_BYTE, pImage->data);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	CheckGLError(__FILE__,__LINE__);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	if (pImage)
	{
		if (pImage->data)
		{
			free(pImage->data);
		}

		free(pImage);
	}

	return true;
}

void Reshape(int width, int height)
{
	glViewport(0, 0, (GLsizei)(width), (GLsizei)(height));
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)width/(float)height, 1, 3000);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Display()
{
	CheckGLError(__FILE__,__LINE__);

	glLoadIdentity();
	camera.Look();

	frustum.CalculateFrustum();

	{
		CheckGLError(__FILE__,__LINE__);

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, water->waterShader->refractionFBO);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, water->waterShader->refractionFBODepth);

		CheckGLError(__FILE__,__LINE__);

		glViewport(0, 0, 1024, 1024);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(camera.GetPosition().y > water->height)
		{
			double plane[4] = {0.0, 1.0, 0.0, water->height}; 
			glEnable(GL_CLIP_PLANE0);
			glClipPlane(GL_CLIP_PLANE0, plane);
			level.RenderLevel(camera.GetPosition(), defaultShader, &frustum);
			CheckGLError(__FILE__,__LINE__);
			glDisable(GL_CLIP_PLANE0);
		}
		else
		{
			double plane[4] = {0.0, -1.0, 0.0, water->height}; 
			glEnable(GL_CLIP_PLANE0);
			glClipPlane(GL_CLIP_PLANE0, plane);
			level.RenderLevel(camera.GetPosition(), defaultShader, &frustum);
			skybox->Render();
			CheckGLError(__FILE__,__LINE__);
			glDisable(GL_CLIP_PLANE0);
		}
	}

	glLoadIdentity();
	camera.Look(); // Note to self: May need to recalc the frustum for the reflection view

	{
		CheckGLError(__FILE__,__LINE__);

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, water->waterShader->reflectionFBO);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, water->waterShader->reflectionFBODepth);

		CheckGLError(__FILE__,__LINE__);

		glViewport(0, 0, 1024, 1024);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		CheckGLError(__FILE__,__LINE__);

		glPushMatrix();

			if(camera.GetPosition().y > water->height)
			{
				glTranslatef(0.0f, water->height * 2.0f, 0.0f);
				glScalef(1.0, -1.0, 1.0);

				glCullFace(GL_BACK); // Q3 uses front by default so we flip here since we are upside down

				CheckGLError(__FILE__,__LINE__);

				double plane[4] = {0.0, 1.0, 0.0, water->height};
				glEnable(GL_CLIP_PLANE0);
				glClipPlane(GL_CLIP_PLANE0, plane);

				level.RenderLevel(camera.GetPosition(), defaultShader, &frustum);
				skybox->Render();
				CheckGLError(__FILE__,__LINE__);

				glDisable(GL_CLIP_PLANE0);

				glCullFace(GL_FRONT);

				CheckGLError(__FILE__,__LINE__);
			}
			else
			{
				double plane[4] = {0.0, -1.0, 0.0, water->height};
				glEnable(GL_CLIP_PLANE0);
				glClipPlane(GL_CLIP_PLANE0, plane);
				level.RenderLevel(camera.GetPosition(), defaultShader, &frustum);
				skybox->Render();
				CheckGLError(__FILE__,__LINE__);
				glDisable(GL_CLIP_PLANE0);
			}

		glPopMatrix();
	}

	CheckGLError(__FILE__,__LINE__);

	glViewport(0, 0, width, height);

	CheckGLError(__FILE__,__LINE__);

	glLoadIdentity();
	camera.Look();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

	CheckGLError(__FILE__,__LINE__);

	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	CheckGLError(__FILE__,__LINE__);

	level.RenderLevel(camera.GetPosition(), defaultShader, &frustum);
	skybox->Render();

	glDisable(GL_CULL_FACE);
	water->Render(&camera.GetPosition());
	glEnable(GL_CULL_FACE);

	CheckGLError(__FILE__,__LINE__);

	ostringstream s;
	s << milliseconds << "ms";

	RenderText(-3, -2, s.str().c_str());

	RenderText(-3, 2, "Esc - Exit    WASD - Move    Space - Jump    Mouse - Look     C - Toggle Wapointed Camera");

	RenderText(-3, 1.8, "     L - Toggle Lightmaps     T - Toggle Textures");

	glutSwapBuffers();

	CheckGLError(__FILE__,__LINE__);
}

int resetCursor = 0;
int updateNumber = 0;

void Idle()
{
	if (updateNumber < 10)
	{
		camera.SetPosition(Vector3(80, 220, 55));
	}

	sound.SetListenerPosition(camera.GetPosition(), camera.GetUpVector());

	updateNumber++;
	
	milliseconds = glutGet(GLUT_ELAPSED_TIME) - lasttime;
	resetCursor += milliseconds;
	lasttime = glutGet(GLUT_ELAPSED_TIME);

	level.Update(milliseconds);

	camera.CheckForMovement(forwardDown, backwardDown, leftDown, rightDown);

	deltaMousePosition.x = currentMousePosition.x - width/2;
	deltaMousePosition.y = currentMousePosition.y - height/2;

	camera.SetViewByMouse(-deltaMousePosition.x / 5.0, -deltaMousePosition.y / 5.0);
	
	if (resetCursor > 20)
	{
		resetCursor = 0;
		glutWarpPointer(width /2, height / 2);
	}

	glutPostRedisplay();
}

void PressKey(unsigned char key, int x, int y) 
{
	if (key == 27) 
	{
		exit(0);
	}

	if (key == 119)
	{
		forwardDown = true;
	}

	if (key == 100)
	{
		rightDown = true;
	}

	if (key == 97)
	{
		leftDown = true;
	}

	if (key == 115)
	{
		backwardDown = true;
	}

	if (key == 99)
	{
		camera.freelook = !camera.freelook;
	}

	/*if (key == 116) // Used for creating the path file(s)
	{
		Vector3 p = camera.GetPosition();
		cout << p.x << " " << p.y << " " << p.z << "\n";
		Vector3 look = camera.GetLook();
		if (flypath.is_open())
		{
			flypath << p.x << " " << p.y << " " << p.z << " " << look.x << " " << look.y << " " << look.z << "\n";
		}
	}*/

	if (key == 108)
	{
		defaultShader->ToggleLightmaps();
	}

	if (key == 116)
	{
		defaultShader->ToggleTextures();
	}
}

void ProcessMouse(int button, int state, int x, int y) 
{
}

void ProcessMouseActiveMotion(int x, int y) 
{
	currentMousePosition.x = x;
	currentMousePosition.y = y;
}

void ProcessMousePassiveMotion(int x, int y) 
{
	currentMousePosition.x = x;
	currentMousePosition.y = y;
}

void InitCallbacks()
{
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(PressKey);
	glutKeyboardUpFunc(ReleaseKey);
	
	glutMouseFunc(ProcessMouse);
	glutMotionFunc(ProcessMouseActiveMotion);
	glutPassiveMotionFunc(ProcessMousePassiveMotion);

	glutDisplayFunc(&Display);
    glutIdleFunc(&Idle);
	glutReshapeFunc(&Reshape);
}

void ReleaseKey(unsigned char key, int x, int y) 
{
	if (key == 27) 
	{
		exit(0);
	}

	if (key == 119)
	{
		forwardDown = false;
	}

	if (key == 100)
	{
		rightDown = false;
	}

	if (key == 97)
	{
		leftDown = false;
	}

	if (key == 115)
	{
		backwardDown = false;
	}
}

void main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitDisplayString("rgba double samples>=4"); 
    glutInitWindowSize(width, height);
	glutInitWindowPosition(glutGet(GLUT_SCREEN_WIDTH)/2 - width/2, glutGet(GLUT_SCREEN_HEIGHT)/2 - height/2);
    if (fullscreen)
	{	
		glutGameModeString(fullscreenString);
		glutEnterGameMode();
	}
	else
	{
		glutCreateWindow("BSP Engine");
	}
	glutSetCursor(GLUT_CURSOR_NONE);

	InitCallbacks();

	glewInit();

	CheckGLError(__FILE__,__LINE__);
    
	if (!GLEW_VERSION_2_0) {
        fprintf(stderr, "OpenGL 2.0 not available\n");
        return;
    }

	Reshape(width, height);

	CheckGLError(__FILE__,__LINE__);
    
	if (!InitGame()) {
        fprintf(stderr, "Failed to load resources\n");
        return;
    }

	CheckGLError(__FILE__,__LINE__);

	//flypath.open("flypathout.path");

    glutMainLoop();

	//flypath.close();

    return;
}