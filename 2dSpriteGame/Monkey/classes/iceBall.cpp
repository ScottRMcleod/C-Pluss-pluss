#include "stdwx.h"
#include "iceBall.h"
#include "pickup.h"
Iceball::Iceball(Level *lev, DrawEngine *de, int s_index, float x, float y,
		 float xDir, float yDir, int i_lives): Sprite(lev,de,s_index,x,y,i_lives)
{
	facingDirection.x = xDir;
	facingDirection.y = yDir;

	classID = ICEBALL_CLASSID;
}
void Iceball::idleUpdate(void)
{
	if(Sprite::move(facingDirection.x, facingDirection.y))
	{
		list<Sprite *>::iterator Iter;
		//loop threw enemies
		for(Iter = level->npc.begin(); Iter != level->npc.end(); Iter++)
		{
			if((*Iter)->classID != classID && (int)(*Iter)->getX() == (int)pos.x && (int)(*Iter)->getY() == (int)pos.y)
			{
				(*Iter)->addLives(-1);
				addLives(-1);
			}
		}
		
		for(Iter = level->gameItems.begin(); Iter != level->gameItems.end(); Iter++)
		{
			if((int)(*Iter)->getX() == (int)pos.x && (int)(*Iter)->getY() == (int)pos.y)
			{
				if(((Pickup *)(*Iter))->getVisibleStatus())
				{
					((Pickup *)(*Iter))->setSpawnTime(50);
					addLives(-1); // kill the iceball
				}
			}
		}
	}
	else
		addLives(-1);
}