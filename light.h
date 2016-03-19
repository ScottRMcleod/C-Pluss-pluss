 // THIS IS FOR CREATING THE LIGHTS FOR THE GL ENGINE
#ifndef LIGHT_H
#define LIGHT_H
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

enum LIGHT_TYPE
{
	 LIGHT_SPOT,
	 LIGHT_POINT,
	 LIGHT_DIRECTIONAL
};
#include <vector>

using std::vector;
class Light
{
public:
	static void Initialise(void);
	Light(LIGHT_TYPE lightType);
	~Light();
	void Visible(bool value = true);

	void setDiffuse(float r, float g, float b, float a);
	void setAmbient(float r, float g, float b, float a);
	void setSpecular(float r, float g, float b, float a);
	
	void setLightType(LIGHT_TYPE type);
	
	void setPosition(float x, float y, float z);
	
	void setSpotDirection(float x, float y, float z);
	void setCutoff(float value);
	void setExponent(float value);
	
	void setAttenuation(float constant, float linear, float quadratic);
	int getLight(void);
	void updateLight(void);

	static int numLights;
	static vector<int> availableLights;
	static vector<Light *> lights;
private:
	GLfloat position[4];
	GLfloat diffuse[4];
	GLfloat ambient[4];
	GLfloat specular[4];
	GLfloat spotDirection[4];

	float cutoff;
	float exponent;

	bool visible;
	int lightType;

	int lightNum;

};

#endif
