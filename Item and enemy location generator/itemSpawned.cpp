#include "itemSpawned.h"
#include <iostream>


int itemSpawned::getItemCount()
{
	return itemPos.size();
}

itemSpawned::itemSpawned(item * item)
{
	baseItem = item;
}

itemSpawned::itemSpawned(itemSpawned * iS)
{
	baseItem = iS->getBaseItem();
	itemPos = iS->getPositions();
}

void itemSpawned::addItem(pos2D pos)
{
	itemPos.emplace_back(pos);
}

ALLEGRO_BITMAP * itemSpawned::getBitmap()
{
	return baseItem->getBitmap();
}

std::list<pos2D> itemSpawned::getPositions()
{
	return itemPos;
}

void itemSpawned::setPositions(std::list<pos2D> pos)
{
	itemPos = pos;
}

item * itemSpawned::getBaseItem()
{
	return baseItem;
}

float itemSpawned::getDistanceMultiplier()
{
	return baseItem->getDistanceMultiplier();
}

float itemSpawned::getSpreadMultiplier()
{
	return baseItem->getSpreadMultiplier();
}


itemSpawned::~itemSpawned()
{
}
