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
	itemSpawned(itemSpawned* iS);
	void addItem(pos2D pos);
	ALLEGRO_BITMAP * getBitmap();
	std::list<pos2D> getPositions();
	void setPositions(std::list<pos2D> pos);
	item* getBaseItem();
	float getDistanceMultiplier();
	float getSpreadMultiplier();
	~itemSpawned();
};

