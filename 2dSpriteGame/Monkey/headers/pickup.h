#ifndef	PICKUP_H
#define PICKUP_H

#include "sprite.h"

class Character;

class Pickup : public Sprite
{
public:
	Pickup(Level *l, DrawEngine *de, int s_index, float x = 1, float y = 1, bool reSpawn = true);
	~Pickup(void);

	void setSpawnTime(int time);
	int getSpawnTime(void);

	void setVisible(bool vis);
	bool getVisibleStatus(void);

	void addPlayer(Character *p);
	virtual void idleUpdate(void);

private:

protected:
	bool itemCanRespawn;
	int spawnTime;
	bool visible;

	void spawnReaction(void);

	//character pointer
	Character *player;

};
#endif