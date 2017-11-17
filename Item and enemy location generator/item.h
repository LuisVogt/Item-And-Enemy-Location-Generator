#include <allegro5\allegro.h>
#include "structs.h"
#pragma once
class item
{
private:
	int spawnChance;
	ALLEGRO_BITMAP *sprite;
	pos2D pos;
public:
	int getSpawnChance();
	ALLEGRO_BITMAP* getBitmap();
	item(ALLEGRO_BITMAP *newSprite,int newSpawnChance,pos2D newPos);
	pos2D getPos();
	~item();
};