#include "item.h"

int item::getSpawnChance()
{
	return spawnChance;
}

ALLEGRO_BITMAP* item::getBitmap()
{
	return sprite;
}

int item::getNumberOfItems()
{
	return numberOfItems;
}

void item::subtractNumberOfItems()
{
	numberOfItems--;
}

void item::addNumberOfItems()
{
	numberOfItems++;
}

item::item(ALLEGRO_BITMAP * newSprite, int newSpawnChance, int numberOfItems)
{
	sprite = newSprite;
	spawnChance = newSpawnChance;
	this->numberOfItems = numberOfItems;
}

item::~item()
{
}
