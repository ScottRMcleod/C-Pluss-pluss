#include "stdwx.h"
#include "level.h"
#include "character.h"
#include "enemy.h"
#include "manaPotion.h"

#include <stdlib.h>
Level::Level(DrawEngine *de, int w, int h)
{
	startX = startY = 1;
	drawArea = de;
	width = w;
	height = h;

	player = 0;

	//create memory for our level
	level = new char *[width];
	
	for(int x = 0; x < width; x++)
	{
		level[x] = new char[height];
	}
	//create the level
	createLevel();
	drawArea->setMap(level);

}
Level::Level(DrawEngine *de, char **grid, int grid_x, int grid_y) : drawArea(de)
{
	level = grid;

	width = grid_x;
	height = grid_y;

	startX = startY = 1;

	for (int x = 0; x < width; x++)
		for(int y = 0; y < height; y++)
		{
			if(level[x][y] == 2)
			{
				level[x][y] = 0;
				startX = x;
				startY = y;
				break;
			}
		}
		drawArea->setMap(level);
}
Level::~Level()
{
	for(int x = 0; x < width; x++)
	{
		delete[] level[x];

	}
	delete[] level;
	
	for(Iter = npc.begin(); Iter != npc.end(); Iter++)
		delete (*Iter);
	
	for(Iter = gameItems.begin(); Iter != gameItems.end(); Iter++)
		delete (*Iter);
}
void Level::createLevel(void)
{
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			int random = rand() % 100;
			if(y == 0 || y == height - 1 || x == 0 || x == width -1)
			{
				level[x][y] = TILE_WALL;
			}
			else
			{
				if(random < 90 ||(x < 3 && y < 3))
					level[x][y] = TILE_EMPTY;
				else
					level[x][y] = TILE_WALL;
			}
		}
	}
}
void Level::draw(void)
{
	// we need to draw the level
	drawArea->drawBackground();
}
void Level::addPlayer(Character *p)
{
	player = p;
}
bool Level::keyPress(char c)
{
	if(player)
	{
		if(player->keyPress(c))
		return true;
	}
	return false;
}
void Level::upDate(long time)
{
	//here we will be able to process fireball ai and stuff
	for(Iter = npc.begin(); Iter != npc.end(); Iter++)
	{
		(*Iter)->setCurrentTime(time);
		
		if((*Iter)->updateSprite())
			(*Iter)->idleUpdate();

		if((*Iter)->isAlive() == false)
		{
			Sprite *temp = *Iter;

			Iter--;
			delete temp;
			npc.remove(temp);
		}
	}
	for(Iter = gameItems.begin(); Iter != gameItems.end(); Iter++)
	{
		(*Iter)->setCurrentTime(time);
		
		if((*Iter)->updateSprite())
			(*Iter)->idleUpdate();
	}
}
void Level::addEnemies(int num, int speed)
{
	int i = num;
	
	if(num == 0)
	{
		for(int x = 0; x < width; x++)
		{
			for(int y = 0; y < height; y++)
			{
				if(level[x][y] == 3)
				{
					Enemy *temp = new Enemy(this, drawArea, SPRITE_ENEMY, (float)x, float(y));
					temp->setSpeed(70);

					temp->addGoal(player);

					addNPC((Sprite *)temp);
				}
			}
		}
	  return;
	}
	while(i > 0)
	{
		int xpos = int((float(rand() % 100) / 100) * (width - 2) + 1);
		int ypos = int((float(rand() % 100) / 100) * (height - 2) + 1);

		if(level[xpos][ypos] != TILE_WALL)
		{
			Enemy *temp = new Enemy(this, drawArea, SPRITE_ENEMY, (float)xpos, float(ypos)); 

			temp->setSpeed(speed);
			temp->addGoal(player);

			addNPC((Sprite *)temp);

			i--;
		}
	}
}
void Level::setPlayerStart(void)
{
	player->setPos(startX, startY);
}
int Level::numEnemies(void)
{
	int num = 0;

	for(Iter = npc.begin(); Iter != npc.end(); Iter++)
	{
		if((*Iter)->getID() == ENEMY_CLASSID)
			num++;
	}
	return num;
}
int Level::getWidth(void)
{
	return width;
}
int Level::getHeight(void)
{
	return height;
}
void Level::addNPC(Sprite *spr)
{
	npc.push_back(spr);
}
void Level::addPickup(int num, int pickupType)
{
	int i = num;
	
	if(num == 0)
	{
		for(int x = 0; x < width; x++)
		{
			for(int y = 0; y < height; y++)
			{
				if(level[x][y] == 4) // check to see if it is a mana potion
				{
					ManaPotion *temp = new ManaPotion(this, drawArea, SPRITE_MANAPOTION, (float)x, (float)y);
					temp->addPlayer(player);
					temp->setPos(x,y);
					addGameItem((Sprite *)temp);
				}
			}
		}
		return;
	}

	while (i > 0)
	{
		int xpos = int((float(rand() % 100) / 100) * (width -2 ) + 1);
		int ypos = int((float(rand() % 100) / 100) * (height -2) + 1);

		if(level[xpos][ypos] != TILE_WALL)
		{
			switch(pickupType)
			{
				case 0: //mana postion
				{
					ManaPotion *temp = new ManaPotion(this, drawArea, SPRITE_MANAPOTION, (float)xpos, (float)ypos);
					temp->addPlayer(player);
					temp->setPos(xpos,ypos);
					addGameItem((Sprite *)temp);
					break;
				}
			}
			i--;
		}
	}

}
void Level::addGameItem(Sprite *spr)
{
	gameItems.push_back(spr);
}