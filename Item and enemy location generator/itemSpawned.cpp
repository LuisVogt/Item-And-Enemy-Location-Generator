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

void itemSpawned::swapItem(pos2D previous, pos2D next)
{
	std::list<pos2D>::iterator it = std::find(itemPos.begin(), itemPos.end(), previous);
	itemPos.emplace(it, next);
	it = std::find(itemPos.begin(), itemPos.end(), previous);
	itemPos.erase(it);
}

void itemSpawned::removeItem(pos2D pos)
{
	itemPos.remove(pos);
}

bool itemSpawned::isThereItemInPos(pos2D pos)
{
	std::list<pos2D>::iterator it = std::find(itemPos.begin(), itemPos.end(), pos);
	if (it != itemPos.end())
		return true;
	return false;
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
