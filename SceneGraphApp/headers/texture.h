/*
	<---------------------------------------------------------
		This class has been developed to enable users to load 
		textures for 3d models created in opengl and has been 
		designed to support not only muiltiple file types 
		but also muitliple Operating systems.
	---------------------------------------------------------->
*/
/*
	<---------------------------------------------------------
				Preprocessor Definitions
	---------------------------------------------------------->
*/
#ifndef TEXTURE_H
#define TEXTURE_H
/*
	<---------------------------------------------------------
		Opengl Platform inderpendent Included header files
	---------------------------------------------------------->
*/
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
/*
	<---------------------------------------------------------
			Required Header files for this class Definition
	---------------------------------------------------------->
*/
#include <string>
#include <vector>
#include <fstream>

using std::vector;
using std::string;
using std::ifstream;

struct TGA_Header
{
	GLubyte ID_Length;
	GLubyte ColorMapType;
	GLubyte ImageType;
	GLubyte colorMapSpecs[5];
	GLshort xOrigin;
	GLshort yOrigin;
	GLshort ImageWidth;
	GLshort ImageHeight;
	GLubyte PixelDepth;
	GLubyte ImageDescription;
};
class Texture
{
//Member function
public:
	Texture(string filename, string name = "");
	~Texture();
//Member Variables
public:
	unsigned char *imageData;
	unsigned int  bpp;
	unsigned int  width;
	unsigned int  height;
	unsigned int  textID;
	
	string name;
	static vector<Texture *> textures;
private:
	bool loadTGA(string filename);
	bool createTexture(unsigned char *imageData, int width, int height, int type);
	void flipImage(unsigned char *imageData, bool flipH, bool flipV, GLushort width, GLushort Height, GLbyte bpp);
	void swap(unsigned char *ori, unsigned char *dest, GLint size);
protected:

};

#endif