#ifndef	LEVEL_H
#define LEVEL_H
#include <list>

using std::list;
enum
{
	SPRITE_PLAYER,
	SPRITE_ENEMY,
	//SPRITE_ICEBALL,
	SPRITE_FIREBALL,
	SPRITE_MANAPOTION
};
enum{
	TILE_EMPTY,
	TILE_WALL
};
#include "drawEngine.h"

class Sprite;
class Character;
class Level 
{
public:
	Level(DrawEngine *de, int width = 30, int height = 20);
	Level(DrawEngine *de, char **grid, int width, int height);
	~Level();

	void addPlayer(Character *p);
	void upDate(long time);
	void draw(void);
	bool keyPress(char c);

	void addEnemies(int num, int speed = 5);
	void addNPC(Sprite *spr);
	
	void addPickup(int num, int pickupType);
	void addGameItem(Sprite *spr);

	friend class Sprite;
	void setPlayerStart(void);
	int numEnemies(void);
	int getWidth(void);
	int getHeight(void);
private:
	int width;
	int height;

	int startX, startY;
	char **level;
	Character *player;
	DrawEngine *drawArea;

protected:
	void createLevel(void);
public:
	list <Sprite *> npc;
	list <Sprite *> gameItems;
	list <Sprite *>::iterator Iter;
};
#endif