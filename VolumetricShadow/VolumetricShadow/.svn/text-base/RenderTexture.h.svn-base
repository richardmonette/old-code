
#pragma once

#include <glsw.h>
#include <glew.h>

class RenderTexture 
{
	private:
		//  we don't want to allow copies to be made
		RenderTexture&  operator = (const RenderTexture& other) {}
		RenderTexture(const RenderTexture& other) {}

	protected:
		GLuint texID;
		GLuint fbo;
		GLuint width;
		GLuint height;

	public:
		RenderTexture(GLuint p_width, GLuint p_height, GLuint format);
		~RenderTexture();
		void startRender();
		void finishRender();
		void bind(GLuint unit=0);
};

