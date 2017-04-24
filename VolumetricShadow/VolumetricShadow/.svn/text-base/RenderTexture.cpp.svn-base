
#include "RenderTexture.h"

RenderTexture::RenderTexture(GLuint p_width, GLuint p_height, GLuint format) : width(p_width), height(p_height) 
{
	//  allocate the texture that we will render into
	glGenTextures( 1, &texID );
	glBindTexture(GL_TEXTURE_2D, texID);
	if(format==GL_DEPTH_COMPONENT)
		glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, 0);

	//  allocate a framebuffer object
	glGenFramebuffersEXT(1, &fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

	//  attach the framebuffer to our texture, which may be a depth texture
	if(format==GL_DEPTH_COMPONENT) {
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, texID, 0);
		//  disable drawing to any buffers, we only want the depth
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	} else
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texID, 0);

	//  you can check to see if the framebuffer is 'complete' with no errors
	if(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)!=GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		#ifdef _DEBUG
			__debugbreak();
		#endif
		//  error!  might want to handle this somehow
	}

	//  unbind our framebuffer, return to default state
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

RenderTexture::~RenderTexture() 
{
	//  free our texture
	glDeleteTextures( 1, &texID );
	//  free our framebuffer
	glDeleteFramebuffersEXT( 1, &fbo );
}

void RenderTexture::startRender() 
{
	//  bind the framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	//  set the viewport to our texture dimensions
	glViewport(0,0,width,height);
}

void RenderTexture::finishRender() 
{
	//  unbind our framebuffer, return to default state
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	//  remember to restore the viewport when you are ready to render to the screen!
}

void RenderTexture::bind(GLuint unit) 
{
	glActiveTextureARB(GL_TEXTURE0_ARB + unit);
	glBindTexture( GL_TEXTURE_2D, texID );
}
