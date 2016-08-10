#include "stdwx.h"

#include "manaPotion.h"
#include "mage.h"
ManaPotion::ManaPotion(Level *l, DrawEngine *de, int s_index, float x, float y, int value)
			: Pickup(l,de,s_index,x,y)
{
	classID = MANAPOTION_CLASSID;
	manaValue = value;

}
ManaPotion::~ManaPotion(void)
{
}
void ManaPotion::idleUpdate(void)
{
	Pickup::idleUpdate();
	if(visible)
	{
		vector player_pos = player->getPos();
		if(pos.x == player_pos.x && pos.y == player_pos.y)
		{
			spawnTime = 50;
			visible = false;
			((Mage *)player)->setMana(manaValue);
		}
	}
}