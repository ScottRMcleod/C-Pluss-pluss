#include "Transform.h"

Transform::Transform() : Node()
{
	scale = Vector3(1,1,1);
}
Transform::~Transform()
{

}
void Transform::setTranslate(float x, float y, float z)
{
	translate.x = x;
	translate.y = y;
	translate.z = z;

}
void Transform::setRotation(float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;

}
void Transform::setScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
}