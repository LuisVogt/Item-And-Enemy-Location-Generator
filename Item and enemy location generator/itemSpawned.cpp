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

item * itemSpawned::getBaseItem()
{
	return baseItem;
}


itemSpawned::~itemSpawned()
{
}
