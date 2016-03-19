#include "Plane.h"
void Plane::createPrimitive(void)
{
	float incX = width / (float)divisions;
	float incY = height / (float)divisions;

	addNormal(0,1,0);
	addTexCoord(0,0);

	for(float x = -width / 2; x < width / 2; x += incX)
	{
		for(float y = -height / 2; y < height / 2; y += incY)
		{
			int v1 = addVertex(x + incX, 0, y + incY);
			int v2 = addVertex(x,		 0, y + incY);
			int v3 = addVertex(x + incX, 0, y);
			int v4 = addVertex(x,		 0, y);

			addFace(v1,v3,v4,0,0,0,0,0,0);
			addFace(v4,v2,v1,0,0,0,0,0,0);
		}
	}
}
void Plane::setSize(float w, float h)
{
	width = w;
	height = h;
}
void Plane::setDivisions(int div)
{
	divisions = div;
}