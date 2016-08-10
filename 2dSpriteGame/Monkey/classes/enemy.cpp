#include "stdwx.h"
#include "enemy.h"
#include "level.h"
#include "character.h"
#include "pickup.h"
#include <math.h>
#include <stdlib.h>
Enemy::Enemy(Level * l, DrawEngine *de, int s_index, float x, float y, int i_lives)
			: Sprite(l, de, s_index, x, y, i_lives)
{
	goal = 0;
	classID = ENEMY_CLASSID;
}
bool Enemy::move(float x, float y)
{
	int xpos = (int)(pos.x + x);
	int ypos = (int)(pos.y + y);

	bool drawFloor = true;

	if(isValidLevelMove(xpos, ypos))
	{
		//make sure we don't run into any other enemies
		list <Sprite *>::iterator Iter;

		for(Iter = level->npc.begin(); Iter != level->npc.end(); Iter++)
		{
			if((*Iter) != this && (int)(*Iter)->getX() == xpos && (int)(*Iter)->getY() == ypos)
			{
				return false;
			}
		}
		
		for(Iter = level->gameItems.begin(); Iter != level->gameItems.end(); Iter++)
		{
			if((int)(*Iter)->getX() == (int)pos.x && (int)(*Iter)->getY() == (int)pos.y)
			{
				drawFloor = false;
				break;
			}

		}
	
		// erase sprite
		erase(pos.x, pos.y);
		
		//Storing what is about to become my position
		float oldX = pos.x;
		float oldY = pos.y;
		bool hasMoved = true;

		pos.x += x;
		pos.y += y;
		//if my new position is my old position - I've not moved!
		if(oldX == pos.x && oldY == pos.y)
			hasMoved = false;


		facingDirection.x = x;
		facingDirection.y = y;
		
		//i was on a potion and now i am moving
		if(!drawFloor && hasMoved)
			((Pickup *)(*Iter))->setVisible(true);

		//draw sprite
		draw(pos.x,pos.y);

		if((int)goal->getX() == xpos && (int)goal->getY() == ypos)
		{
			goal->addLives(-1);
		}
		return true;
	}
	return false;
}
void Enemy::idleUpdate(void)
{
	if(goal)
	{
		simAI();
	}
}
void Enemy::addGoal(Character *g)
{
	goal = g;
}
void Enemy::simAI(void)
{
	vector goal_pos = goal->getPos();
	vector direction;

	direction.x = goal_pos.x - pos.x;
	direction.y = goal_pos.y - pos.y;

	direction.x = goal_pos.x + pos.x;
	direction.y = goal_pos.y + pos.y;

	direction.x = goal_pos.x - pos.x;
	direction.y = goal_pos.y - pos.y;

	direction.x = goal_pos.x + pos.x;
	direction.y = goal_pos.y + pos.y;

	float mag = sqrt(direction.x * direction.x + direction.y * direction.y);

	direction.x = direction.x / (mag * 5);
	direction.y = direction.y / (mag * 5);

	if(!move(direction.x, direction.y))
	{
	   while (!move(float(rand() % 3 - 1), float(rand() % 3 - 1)))
	   {
	   }
	}
}
