#include "Cube.h"	
void Cube::createPrimitive(void)
{
	addVertex(-1,-1,-1);
	addVertex( 1,-1,-1);
	addVertex( 1,-1, 1);
	addVertex(-1,-1, 1);
	addVertex(-1, 1,-1);
	addVertex( 1, 1,-1);
	addVertex( 1, 1, 1);
	addVertex(-1, 1, 1);

	addNormal( 1, 0, 0);
	addNormal(-1, 0, 0);
	addNormal( 0, 1, 0);
	addNormal( 0,-1, 0);
	addNormal( 0, 0, 1);
	addNormal( 0, 0,-1);

	addTexCoord(0, 0);
	addTexCoord(0, 1);
	addTexCoord(1, 0);
	addTexCoord(1, 1);

	// -Z
	addFace(0,4,5,5,5,5,0,1,2);
	addFace(5,1,0,5,5,5,3,2,0);
	
	// +Z
	addFace(2,6,7,4,4,4,0,1,2);
	addFace(7,3,2,4,4,4,3,2,0);

	// -Y
	addFace(0,1,2,3,3,3,0,1,2);
	addFace(2,3,0,3,3,3,3,2,0);

	// +Y
	addFace(5,4,7,2,2,2,0,1,2);
	addFace(7,6,5,2,2,2,3,2,0);

	// -X
	addFace(3,7,4,1,1,1,0,1,2);
	addFace(4,0,3,1,1,1,3,2,0);

	// +X
	addFace(1,5,6,0,0,0,0,1,2);
	addFace(6,2,1,0,0,0,3,2,0);

}
