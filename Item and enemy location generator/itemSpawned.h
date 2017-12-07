#include "structs.h"
#include "item.h"
#include <list>

#pragma once
class itemSpawned
{
private:
	item* baseItem;
	std::list<pos2D> itemPos;
public:
	int getItemCount();
	itemSpawned(item* item);
	void addItem(pos2D pos);
	ALLEGRO_BITMAP * getBitmap();
	std::list<pos2D> getPositions();
	item* getBaseItem();
	float getMultiplier();
	~itemSpawned();
};

