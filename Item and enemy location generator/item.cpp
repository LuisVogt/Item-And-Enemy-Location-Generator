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

float item::getDistanceMultiplier()
{
	return distanceMultiplier;
}

item::item(ALLEGRO_BITMAP * newSprite, int newSpawnChance, int numberOfItems, float newDistanceMultiplier)
{
	sprite = newSprite;
	spawnChance = newSpawnChance;
	this->numberOfItems = numberOfItems;
	distanceMultiplier = newDistanceMultiplier;
}

item::~item()
{
}
