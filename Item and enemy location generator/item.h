#include <allegro5\allegro.h>
#include "structs.h"
#pragma once
class item
{
private:
	int spawnChance;
	int numberOfItems;
	float distanceMultiplier;
	ALLEGRO_BITMAP *sprite;
public:
	int getSpawnChance();
	ALLEGRO_BITMAP* getBitmap();
	int getNumberOfItems();
	void subtractNumberOfItems();
	void addNumberOfItems();
	float getDistanceMultiplier();
	item(ALLEGRO_BITMAP *newSprite,int newSpawnChance,int numberOfItems, float newDistanceMultiplier);
	~item();
};