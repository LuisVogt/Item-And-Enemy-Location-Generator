#include "map.h"
#include "defines.h"
#include <random>
#include <iostream>
#include <allegro5\allegro_primitives.h>

void map::drawRooms(ALLEGRO_BITMAP *drawMap)
{
	for (std::list<room*>::iterator i = rooms.begin(); i != rooms.end(); ++i)
	{
		(*i)->drawMap(drawMap, COLORS::WHITE());
	}
}

void map::drawRoomsWithRandomColors(ALLEGRO_BITMAP *drawMap)
{
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<> dist(63, 255);

	for (std::list<room*>::iterator i = rooms.begin(); i != rooms.end(); ++i)
	{
		(*i)->drawMap(drawMap, al_map_rgb(dist(eng), dist(eng), dist(eng)));
	}
}

room * map::findRoomFromTile(pos2D tile)
{
	for(std::list<room*>::iterator i = rooms.begin(); i!= rooms.end(); ++i)
	{
		if ((*i)->isTilePartOfRoom(tile))
		{
			return (*i);
		}
	}
	return nullptr;
}

void map::populateRooms()
{
	ALLEGRO_BITMAP *map = thisMap;
	int width = al_get_bitmap_width(map);
	int height = al_get_bitmap_height(map);
	std::list<pos2D> tempTile;
	pos2D tempTile1;
	pos2D tempTile2;
	room* tempRoom1;
	room* tempRoom2;

	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			std::cout << i << "," << j << std::endl;
			if (areTwoColorsTheSame(al_get_pixel(map, i, j), COLORS::GREEN()))
			{
				tempTile = pos2D(i, j).get4WayAdjacentPositions();
				al_draw_pixel(i, j, COLORS::GREEN());
				for (std::list<pos2D>::iterator aroundDoor = tempTile.begin(); aroundDoor != tempTile.end(); ++aroundDoor)
				{
					if (areTwoColorsTheSame(al_get_pixel(map, aroundDoor->x, aroundDoor->y), COLORS::WHITE()))
					{
						tempTile1 = (*aroundDoor);
						tempTile2 = ((*aroundDoor) + (pos2D(i, j) - (*aroundDoor)) * 3);

						tempRoom1 = findRoomFromTile(tempTile1);
						tempRoom2 = findRoomFromTile(tempTile2);
						if (tempRoom1 == nullptr)
						{
							tempRoom1 = new room(map, tempTile1, standardItemChance);
							tempRoom1->findFullRoom(map);
							rooms.emplace_back(tempRoom1);
						}
						if (tempRoom2 == nullptr)
						{
							tempRoom2 = new room(map, tempTile2, standardItemChance);
							tempRoom2->findFullRoom(map);
							rooms.emplace_back(tempRoom2);
						}
						if (tempRoom1->isRoomAdjacent(tempRoom2))
							tempRoom1->addAdjRoom(tempRoom2);
						if (tempRoom2->isRoomAdjacent(tempRoom1))
							tempRoom2->addAdjRoom(tempRoom1);
					}
				}
			}
		}
	}
}

void map::populateItems(std::list<item> items,ALLEGRO_BITMAP *map)
{
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<> roomRandom(1, 100);

	int fullSpawnChance = 0;
	int tempRand;
	for (std::list<item>::iterator i = items.begin(); i != items.end(); ++i)
	{
		fullSpawnChance += i->getSpawnChance();
	}
	std::uniform_int_distribution<> itemRandom(1, fullSpawnChance);
	for (std::list<room*>::iterator j = rooms.begin(); j != rooms.end(); ++j)
	{
		if (roomRandom(eng) < (*j)->getItemChance())
		{
			tempRand = itemRandom(eng);
			for (std::list<item>::iterator i = items.begin(); i != items.end(); ++i)
			{
				if (tempRand > 0)
					tempRand -= i->getSpawnChance();
				else
				{
					items.emplace_back((*new item(i->getBitmap(), 0, (*j)->getRandomTile())));
					break;
				}
			}
			for (std::list<room*>::iterator k = (*j)->getAdjRooms().begin(); k != (*j)->getAdjRooms().end(); ++k)
			{
				(*k)->changeItemChance(-20);
			}
			
		}
	}

}

void map::saveLargerMapWithItems(int tileSize)
{
	int width = al_get_bitmap_width(thisMap);
	int height = al_get_bitmap_height(thisMap);

	const char *save3 = "teste3.png";


	biggerMap=al_create_bitmap(width*tileSize, height*tileSize);
	al_set_target_bitmap(biggerMap);

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			al_draw_filled_rectangle(i*tileSize, j*tileSize, i*tileSize + tileSize, j*tileSize + tileSize, al_get_pixel(thisMap, i, j));
		}
	}
	for (std::list<item>::iterator k = itemsOnMap.begin(); k != itemsOnMap.end(); ++k)
	{
		al_draw_bitmap(k->getBitmap(), k->getPos().x, k->getPos().y, 0);
	}
	if (!al_save_bitmap(save3, biggerMap))
	{
		fprintf(stderr, "failed to save image!\n");
		return;
	}
}

map::map()
{
}

map::map(ALLEGRO_BITMAP *newMap, int itemChance)
{
	thisMap = newMap;
	standardItemChance = itemChance;
}


map::~map()
{
}
