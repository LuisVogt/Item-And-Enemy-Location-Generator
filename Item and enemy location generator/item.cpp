#include "item.h"

int item::getSpawnChance()
{
	return spawnChance;
}

ALLEGRO_BITMAP* item::getBitmap()
{
	return sprite;
}

item::item(ALLEGRO_BITMAP * newSprite, int newSpawnChance, pos2D newPos)
{
	sprite = newSprite;
	spawnChance = newSpawnChance;
	pos = newPos;
}

pos2D item::getPos()
{
	return pos;
}

item::~item()
{
}
