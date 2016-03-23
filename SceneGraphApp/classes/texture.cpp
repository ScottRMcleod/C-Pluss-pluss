/*
	<---------------------------------------------------------
	this class could be extended to enable the 
	user to load other external file formats and support other
	Opertating systems file formats
	---------------------------------------------------------->
*/
#include "texture.h"

vector<Texture *> Texture::textures;
Texture::Texture(string in_filename, string in_name)
{
	imageData = NULL;
	loadTGA(in_filename);
	name= in_name;
	textures.push_back(this);

}
Texture::~Texture()
{
	for(vector<Texture *>::iterator it = textures.begin(); it != textures.end(); it ++)
	{
		if((*it)==this)
		{

			textures.erase(it);
		}
		if(imageData)
		{
			delete imageData;
		}
	}

}
// this function loads in a file of type tga
bool Texture::loadTGA(string filename)
{
	TGA_Header TGAheader;

	ifstream file(filename.data(), std::ios_base::binary);

	if(!file.is_open())
	{
		return false;
	}
	
	//type casting TGAheader to a char pointer e.g.((char *)&TGAheader)
	
	if(!file.read((char *)&TGAheader, sizeof(TGAheader)))
	{
		fprintf(stderr, "evil comes\n");
		return false;
	}

	//test where or not the file is a supported format
	if(TGAheader.ImageType !=2)
	{
		//error message e.g.(this file format is not supported)
		return false;
	}

	width = TGAheader.ImageWidth;
	height = TGAheader.ImageHeight;
	bpp = TGAheader.PixelDepth;
	

	//Test to see if the file is not corrupted
	if(width <= 0 || height <= 0 || (bpp != 24 && bpp != 32))
	{
		//error message e.g.(this file has been corrupted please try a diffrect file)
		fprintf(stderr, "evil comes\n");
		return false;
	}

	// GL_RGBA stands for red green blue and alpha colors
	GLuint type = GL_RGBA;
	if (bpp == 24)
		type = GL_RGB;
	GLuint bytesPerPixel = bpp / 8;
	GLuint imageSize = width * height * bytesPerPixel;

	imageData = new GLubyte[imageSize];

	if (imageData == NULL)
		return false;

	if (!file.read((char *)imageData, imageSize))
	{
		delete imageData;

		//out of memory for the creation of the image file
		//image file is to large to be loaded in
		return false;
	}
	
	/*
	Note: 
		Due to the convention of TGA files read color properties are read
		in backwards to solve this problem 
		we need to loop threw each pixel, converting the BGR to RGB thus 
		displaying the TGA file properly on our model. 
		to do this there has been a temporay vairble created and assigned 
	`	the value of imageData to perform the swap of b and r 
		so that the produced
	*/

	for (GLuint i = 0; i < (int)imageSize; i+= bytesPerPixel)
	{
		GLuint temp = imageData[1];
		imageData[i] = imageData[i + 2];
		imageData[i + 2] = temp;
	}
	bool flipH = ((TGAheader.ImageDescription & 0x10) == 0x10);
	bool flipV = ((TGAheader.ImageDescription & 0x20) == 0x20);
	flipImage(imageData, flipH, flipV, width, height, bpp);
	createTexture(imageData, width, height, type);

	// Everythig worked!!!
	return true;
}
void Texture::flipImage(unsigned char *image, bool flipH, bool flipV, GLushort width, GLushort Height, GLbyte bpp)
{
	GLbyte Bpp = bpp / 8;
    if (flipH){
        for (int h = 0; h < height; h++) {
            for (int w = 0; w < width / 2; w++){                
                swap(image + (h * width + w) * Bpp, image + (h * width + width - w - 1)* Bpp, Bpp);
            }
        }
    }

    if (flipV){
        for (int w = 0; w < width; w++){
            for (int h = 0; h < height / 2; h++) {
                swap(image + (h * width + w) * Bpp, image + ((height - h - 1) * width + w)* Bpp, Bpp);
            }
        }
    }
}
void Texture::swap(unsigned char *ori, unsigned char *dest, GLint size)
{
	 GLubyte * temp = new unsigned char[size];

	memcpy(temp, ori, size);
    memcpy(ori, dest, size);
    memcpy(dest, temp, size);

    delete [] temp;
}
bool Texture::createTexture(unsigned char *imageData, int width, int height, int type)
{
	glGenTextures(1, &textID);

	glBindTexture(GL_TEXTURE_2D, textID);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, imageData);

	return true;
}


