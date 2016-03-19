#include "Mesh.h"

Mesh::Mesh() : Node()
{
	
}
Mesh::~Mesh()
{

}
bool Mesh::drawMesh(bool drawChildren, Mesh *drawSelected)
{
	bool drawn = false;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(transform.translate.x,transform.translate.y,transform.translate.z);
	glScalef(transform.scale.x,transform.scale.y,transform.scale.z);
	glRotatef(transform.rotation.x, 1, 0, 0);
	glRotatef(transform.rotation.y, 0, 1, 0);
	glRotatef(transform.rotation.z, 0, 0, 1);

	if(drawSelected ==NULL || drawSelected == this)
	{

		glPushName((int )this);

		for(int i = 0; i < (int)faces.size(); i++)
		{
			drawFace(faces[i]);
		}
		glPopName();
		drawn = true;
	}
	if(drawChildren == true)
	{
		for(list<Node *>::iterator it = children.begin(); it != children.end(); it++)
		{
			Mesh *mesh = dynamic_cast<Mesh *>(*it);

			if(mesh !=NULL)
			{
				drawn = mesh->drawMesh(true, drawSelected) | drawn;
			}
		}
	}
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	return drawn;
}
Transform &Mesh::getTransform(void)
{
	return transform;
}
void Mesh::drawFace(Face *face)
{
	glBegin(GL_TRIANGLES);
	for(int i = 0; i < 3; i++)
	{
		glTexCoord2f(face->UVs[i]->x, face->UVs[i]->y);
		glNormal3d(face->normals[i]->x,face->normals[i]->y,
				   face->normals[i]->z);
		glVertex3d(face->vertices[i]->x,face->vertices[i]->y,
				   face->vertices[i]->z);

	}
	glEnd();
}
int Mesh::addVertex(float x, float y, float z)
{
	vertices.push_back(new Vector3(x,y,z));

	return ((int)vertices.size() - 1);
}
int Mesh::addNormal(float x, float y, float z)
{
	normals.push_back(new Vector3(x,y,z));

	return ((int)normals.size() - 1);
}
int Mesh::addTexCoord(float s, float t)
{

	UVs.push_back(new Vector3(s,t,0));

	return ((int)UVs.size() - 1);
}
void Mesh::addFace(int v1, int v2, int v3, int n1, 
				   int n2, int n3, int uv1, int uv2, int uv3)
{
	faces.push_back(new Face(	vertices[v1],vertices[v2],
								vertices[v3],normals[n1],
								normals[n2],normals[n3],
								UVs[uv1],UVs[uv2],
								UVs[uv3]));	
}
