#ifndef MANAPOTION_H
#define MANAPOTION_H

#include "pickup.h"

class Mage;

class ManaPotion : public Pickup
{
public:
	ManaPotion(Level *l, DrawEngine *de, int s_index, float x = 1, float y = 1, int value = 6);
	~ManaPotion(void);

	void idleUpdate(void);
private:
	int manaValue;
protected:

};
#endif