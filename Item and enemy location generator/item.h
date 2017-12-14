#include <allegro5\allegro.h>
#include "structs.h"
#pragma once
class item
{
private:
	int spawnChance;
	int numberOfItems;
	int maxNumberOfItems;
	float distanceMultiplier;
	float spreadMultiplier;
	ALLEGRO_BITMAP *sprite;
public:
	int getSpawnChance();
	ALLEGRO_BITMAP* getBitmap();
	int getNumberOfItems();
	void subtractNumberOfItems();
	void addNumberOfItems();
	float getDistanceMultiplier();
	float getSpreadMultiplier();
	item(ALLEGRO_BITMAP *newSprite,int newSpawnChance,int minNumberOfItems, int maxNumberOfItems,float newDistanceMultiplier, float newSpreadMultiplier);
	~item();
};