#ifndef	PLANE_H
#define PLANE_H

#include "Primitive.h"

class Plane : public Primitive
{
public:
	void			setSize(float width, float height);
	void			setDivisions(int divisions);

	virtual void	createPrimitive(void);
protected:
	float width;
	float height;

	int divisions;

};
#endif