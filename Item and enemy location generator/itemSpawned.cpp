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
	std::cout << "PING:" << getItemCount() << std::endl;
	itemPos.emplace_back(pos);
	std::cout << "PING2:" << getItemCount() << std::endl;
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
