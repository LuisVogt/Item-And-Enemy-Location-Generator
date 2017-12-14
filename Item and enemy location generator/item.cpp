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
	if (numberOfItems < maxNumberOfItems)
		numberOfItems++;
}

float item::getDistanceMultiplier()
{
	return distanceMultiplier;
}

float item::getSpreadMultiplier()
{
	return spreadMultiplier;
}

item::item(ALLEGRO_BITMAP * newSprite, int newSpawnChance, int minNumberOfItems, int maxNumberOfItems,float newDistanceMultiplier, float newSpreadMultiplier)
{
	sprite = newSprite;
	spawnChance = newSpawnChance;
	this->numberOfItems = minNumberOfItems;
	this->maxNumberOfItems = maxNumberOfItems;
	distanceMultiplier = newDistanceMultiplier;
	spreadMultiplier = newSpreadMultiplier;
}

item::~item()
{
}
