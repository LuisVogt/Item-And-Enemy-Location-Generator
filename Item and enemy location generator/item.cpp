#include "item.h"

int item::getSpawnChance()
{
	return spawnChance;
}

ALLEGRO_BITMAP* item::getBitmap()
{
	return sprite;
}

item::item(ALLEGRO_BITMAP * newSprite, int newSpawnChance)
{
	sprite = newSprite;
	spawnChance = newSpawnChance;
}

item::~item()
{
}
