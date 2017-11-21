#include <allegro5\allegro.h>
#include "structs.h"
#pragma once
class item
{
private:
	int spawnChance;
	ALLEGRO_BITMAP *sprite;
public:
	int getSpawnChance();
	ALLEGRO_BITMAP* getBitmap();
	item(ALLEGRO_BITMAP *newSprite,int newSpawnChance);
	~item();
};