#ifndef	MESH_H
#define MESH_H

#include "GLEngine.h"
#include "Node.h"
#include "Transform.h"

#include <vector>
using std::vector;

struct Face
{
	Vector3 *vertices[3];
	Vector3 *normals[3];
	Vector3 *UVs[3];

	Face(	Vector3 *v1,Vector3 *v2, Vector3 *v3,
			Vector3 *n1, Vector3 *n2, Vector3 *n3,
			Vector3 *uv1,Vector3 *uv2,Vector3 *uv3)
	{
		vertices[0] = v1;
		vertices[1] = v2;
		vertices[2] = v3;

		normals[0] = n1;
		normals[1] = n2;
		normals[2] = n3;

		UVs[0] = uv1;
		UVs[1] = uv2;
		UVs[2] = uv3;

	}
};
class Mesh : public Node
{
public:
	Mesh();
	virtual ~Mesh();
	bool drawMesh(bool drawChildren = true, Mesh *drawSelected = NULL);

	Transform &getTransform(void);
protected:
	void drawFace(Face *face);

	int addVertex(float x, float y, float z);
	int addNormal(float x, float y, float z);
	int addTexCoord(float s, float t);

	void	addFace(int v1, int v2, int v3, 
					int n1, int n2, int n3,
					int uv1, int uv2, int uv3);

protected:
	Transform transform;
	vector<Vector3 *> vertices;
	vector<Vector3 *> normals;
	vector<Vector3 *> UVs;

	vector<Face *> faces;


};
#endif