#ifndef ICEBALL_H
#define ICEBALL_H
#include "sprite.h"
class Iceball : public Sprite
{
public:
	Iceball(Level *lev, DrawEngine *de, int s_index, float x = 1, float y = 1,
		 float xDir = 0, float yDir = 0, int i_lives = 1);
	void idleUpdate(void);
protected:

};
#endif