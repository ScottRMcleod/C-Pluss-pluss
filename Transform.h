#ifndef	TRANSFORM_H
#define TRANSFORM_H
#include "Node.h"
#include "Vector3.h"
class Transform : public Node
{
public:
	Transform();
	virtual ~Transform();

	void setTranslate(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void setScale(float x, float y, float z);
public:
	Vector3 translate;
	Vector3 rotation;
	Vector3 scale;
private:
protected:
};
#endif