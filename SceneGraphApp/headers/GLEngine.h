#include <stdlib.h>
#include "SDL.h"

#ifndef GL_ENGINE_H
#define GL_ENGINE_H

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

#if defined(__APPLE__) && defined(__MACH__)
	#include <opengl/gl.h>
	#include <opengl/glu.h>
#else
	#include <gl/gl.h>
	#include <gl/glu.h>
#endif

#include "light.h"
#include "texture.h"

#define iGLEngine GLEngine::getEngine()
class GLEngine
{
public:
	GLEngine();
	~GLEngine();

	static GLvoid Uninitialise(GLvoid);
	static GLEngine *getEngine(GLvoid);

	GLvoid Intialise(GLint width, GLint height);

	GLvoid buildTextureFont(GLvoid);
	GLvoid drawText(GLint x, GLint y, const char *text, ...);
	
	GLuint getTextWidth(const char *text);
	GLuint getTextHeight(const char *text);
private:
	GLuint fontBase;
	Texture *fontTexture;
};

#endif