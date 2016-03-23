#ifndef	PRIMITIVE_H
#define PRIMITIVE_H

#include "Mesh.h"
class Primitive : public Mesh
{
public:
	virtual void createPrimitive(void) = 0;
private:
protected:
};
#endif