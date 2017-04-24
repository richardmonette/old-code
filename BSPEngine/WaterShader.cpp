
#include "WaterShader.h"
#include "Main.h"
#include <iostream>

WaterShader::WaterShader() : Shader("water")
{
	reflectionTextureLocation = glGetUniformLocationARB(program, "reflection");
	refractionTextureLocation = glGetUniformLocationARB(program, "refraction");
	normalTextureLocation = glGetUniformLocationARB(program, "normal");
	dudvTextureLocation = glGetUniformLocationARB(program, "dudv");

	time = glGetUniformLocationARB(program, "time");

	CreateTexture(normalTextureID, "textures/normalmap");
	CreateTexture(dudvTextureID, "textures/dudvmap");

	CreateRefractionFBO();
	CreateReflectionFBO();
}

WaterShader::~WaterShader()
{
}

// FBO routines based off the following:
// <cite>Quick example, render_to_texture (2D), mipmaps http://www.opengl.org/wiki/GL_EXT_framebuffer_object#Quick_example.2C_render_to_texture_.282D.29.2C_mipmaps </cite>

void WaterShader::CreateReflectionFBO()
{
	glGenTextures(1, &reflectionTextureID);
	glBindTexture(GL_TEXTURE_2D, reflectionTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//NULL means reserve texture memory, but texels are undefined
	//**** Tell OpenGL to reserve level 0
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	//You must reserve memory for other mipmaps levels as well either by making a series of calls to
	//glTexImage2D or use glGenerateMipmapEXT(GL_TEXTURE_2D).
	//Here, we'll use :
	glGenerateMipmapEXT(GL_TEXTURE_2D);
	//-------------------------
	glGenFramebuffersEXT(1, &reflectionFBO);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, reflectionFBO);
	//Attach 2D texture to this FBO
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, reflectionTextureID, 0);
	//-------------------------
	glGenRenderbuffersEXT(1, &reflectionFBODepth);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, reflectionFBODepth);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, 1024, 1024);
	//-------------------------
	//Attach depth buffer to FBO
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, reflectionFBODepth);
	//-------------------------
	//Does the GPU support current FBO configuration?
	GLenum status;
	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	switch(status)
	{
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			std::cout << "reflection FBO OK\n";
			break;
		default:
			std::cout << "reflection FBO ERROR\n";
	}
}

void WaterShader::CreateRefractionFBO()
{
	glGenTextures(1, &refractionTextureID);
	glBindTexture(GL_TEXTURE_2D, refractionTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//NULL means reserve texture memory, but texels are undefined
	//**** Tell OpenGL to reserve level 0
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	//You must reserve memory for other mipmaps levels as well either by making a series of calls to
	//glTexImage2D or use glGenerateMipmapEXT(GL_TEXTURE_2D).
	//Here, we'll use :
	glGenerateMipmapEXT(GL_TEXTURE_2D);
	//-------------------------
	glGenFramebuffersEXT(1, &refractionFBO);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, refractionFBO);
	//Attach 2D texture to this FBO
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, refractionTextureID, 0);
	//-------------------------
	glGenRenderbuffersEXT(1, &refractionFBODepth);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, refractionFBODepth);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, 1024, 1024);
	//-------------------------
	//Attach depth buffer to FBO
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, refractionFBODepth);
	//-------------------------
	//Does the GPU support current FBO configuration?
	GLenum status;
	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	switch(status)
	{
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			std::cout << "Refraction FBO OK\n";
			break;
		default:
			std::cout << "Refraction FBO ERROR\n";
	}
}
