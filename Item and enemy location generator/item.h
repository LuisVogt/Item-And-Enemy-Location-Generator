#include <allegro5\allegro.h>
#include "structs.h"
#pragma once
class item
{
private:
	int spawnChance;
	int numberOfItems;
	ALLEGRO_BITMAP *sprite;
public:
	int getSpawnChance();
	ALLEGRO_BITMAP* getBitmap();
	int getNumberOfItems();
	void subtractNumberOfItems();
	void addNumberOfItems();
	item(ALLEGRO_BITMAP *newSprite,int newSpawnChance,int numberOfItems);
	~item();
};