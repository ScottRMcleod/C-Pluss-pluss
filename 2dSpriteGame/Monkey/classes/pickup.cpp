#include "stdwx.h"
#include "pickup.h"

Pickup::Pickup(Level *l, DrawEngine *de, int s_index, float x, float y, bool reSpawn)
	     : Sprite(l,de,s_index, x, y)
{
	itemCanRespawn = reSpawn;
	spawnTime = 0;
	visible = true;
}
Pickup::~Pickup(void)
{

}
void Pickup::setSpawnTime(int time)
{
	spawnTime = time;
	visible = false;
}
int Pickup::getSpawnTime(void)
{
	return spawnTime;
}
void Pickup::setVisible(bool vis)
{
	visible = vis;
	if(vis)
		this->draw(pos.x, pos.y);
	else
		this->erase(pos.x, pos.y);
}
bool Pickup::getVisibleStatus(void)
{
	return visible;
}
void Pickup::addPlayer(Character *p)
{
	player = p;
}
void Pickup::idleUpdate(void)
{
	if(itemCanRespawn && spawnTime > 0)
	{
		spawnReaction();
	}
}
void Pickup::spawnReaction(void)
{
	spawnTime --;
	if(spawnTime == 0)
	{
		draw(pos.x, pos.y);
		visible = true;
	}
}