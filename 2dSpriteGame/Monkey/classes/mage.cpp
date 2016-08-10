#include "stdwx.h"
#include "mage.h"
#include "fireball.h"
#include "iceBall.h"
#include "level.h"
Mage::Mage(Level *l,DrawEngine *de, int s_index, float x, float y,
			int lives, int mana, char spell_key, char up_key, char down_key, 
			char left_key, char right_key)
			: Character(l,de,s_index,x,y,lives,up_key,down_key,left_key,right_key)
{
	spellKey = spell_key;
	classID = MAGE_CLASSID;
	manaLevel = mana;
}		
bool Mage::keyPress(char c)
{
	bool val = Character::keyPress(c);

	if(val == false)
	{
		if(c==spellKey)
		{
			castSpell();
			return true;
		}
	}
	return val;
}
void Mage::castSpell(void)
{
	if(facingDirection.x == 0 && facingDirection.y == 0)
		return;

	if(facingDirection.y < -1 && facingDirection.y < -1)
		return;

	if(manaLevel > 0)
	{
		manaLevel -= 2;
		Fireball *temp = new Fireball(level, drawArea,SPRITE_FIREBALL, 
			(int)pos.x+facingDirection.x, (int)pos.y+facingDirection.y, 
			facingDirection.x, facingDirection.y);

		if(temp->move(facingDirection.x, facingDirection.y))
		{
			temp->idleUpdate();
			level->addNPC((Sprite *)temp);
		}
		else
			delete temp;
	}
	update();
}

int Mage::getMana(void)
{
	return manaLevel;
}
void Mage::setMana(int amount)
{
	manaLevel += amount;
}
void Mage::addLives(int num)
{
	Character::addLives(num); 
	manaLevel = 50;
}