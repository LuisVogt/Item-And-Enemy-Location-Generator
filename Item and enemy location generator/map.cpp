#include "map.h"
#include "defines.h"
#include <random>
#include <iostream>
#include <allegro5\allegro_primitives.h>

room* map::spawnRoom = nullptr;

void map::drawRooms(ALLEGRO_BITMAP *drawMap)
{
	for (std::list<room*>::iterator i = rooms.begin(); i != rooms.end(); ++i)
	{
		if ((*i) == spawnRoom)
			spawnRoom->drawMap(drawMap, COLORS::RED());
		else
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

room * map::getRandomRoom()
{
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<> roomRandom(1, numberOfRooms);

	int randomNumber = roomRandom(eng);

	for (std::list<room*>::iterator i = rooms.begin(); i != rooms.end(); ++i)
	{
		randomNumber--;
		if (randomNumber <= 0)
		{
			return (*i);
		}
	}

	return nullptr;
}

void map::setSpawnRoom(room * newSpawnRoom)
{
	spawnRoom = newSpawnRoom;
}

std::list<room*> map::getRoomList()
{
	return rooms;
}

int map::getNumberOfRooms()
{
	return numberOfRooms;
}

int map::getStandardItemChance()
{
	return standardItemChance;
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
							numberOfRooms++;
							tempRoom1 = new room(map, tempTile1, standardItemChance);
							tempRoom1->findFullRoom(map);
							rooms.emplace_back(tempRoom1);
						}
						if (tempRoom2 == nullptr)
						{
							numberOfRooms++;
							tempRoom2 = new room(map, tempTile2, standardItemChance);
							tempRoom2->findFullRoom(map);
							rooms.emplace_back(tempRoom2);
						}
						if (!tempRoom1->isRoomAdjacent(tempRoom2))
							tempRoom1->addAdjRoom(tempRoom2);
						if (!tempRoom2->isRoomAdjacent(tempRoom1))
							tempRoom2->addAdjRoom(tempRoom1);
					}
				}
			}
		}
	}
}

void map::populateRooms(std::list<room*> setRooms)
{
	rooms = setRooms;
	numberOfRooms = rooms.size();
}

void map::populateItems(std::list<item*> items, ALLEGRO_BITMAP *map)
{
	room* randomRoom;
	int tempNumberOfItems = 0;
	for each (item* i in items)
	{
		tempNumberOfItems = i->getNumberOfItems();
		while (tempNumberOfItems>0)
		{
			randomRoom = getRandomRoom();
			spawnItem(i, randomRoom->getRandomTile());
			tempNumberOfItems--;
		}
	}
}

void map::saveLargerMapWithItems(int tileSize, const char* path)
{
	int width = al_get_bitmap_width(thisMap);
	int height = al_get_bitmap_height(thisMap);

	ALLEGRO_BITMAP * tempMap;

	biggerMap=al_create_bitmap(width*tileSize, height*tileSize);
	al_set_target_bitmap(biggerMap);

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (spawnRoom->isTilePartOfRoom(pos2D(i, j)))
				al_draw_filled_rectangle(i*tileSize, j*tileSize, i*tileSize + tileSize, j*tileSize + tileSize, COLORS::RED());
			else
				al_draw_filled_rectangle(i*tileSize, j*tileSize, i*tileSize + tileSize, j*tileSize + tileSize, al_get_pixel(thisMap, i, j));
		}
	}

	for (std::list<itemSpawned*>::iterator i = currentItems.begin(); i != currentItems.end(); i++)
	{
		for each (pos2D pos in (*i)->getPositions())
		{
			tempMap = (*i)->getBitmap();
			al_draw_bitmap((*i)->getBitmap(), pos.x * tileSize, pos.y * tileSize,0);

		}
	}
	if (!al_save_bitmap(path, biggerMap))
	{
		fprintf(stderr, "failed to save image!\n");
		return;
	}
}

void map::spawnItem(item * it, pos2D pos)
{
	for each (itemSpawned* iter in currentItems)
	{
		if (iter->getBaseItem() == it)
		{
			iter->addItem(pos);
			return;
		}
	}
	currentItems.emplace_back(new itemSpawned(it));
	currentItems.back()->addItem(pos);
}

int map::getNumberOfItems()
{
	int n = 0;
	for each (itemSpawned* i in currentItems)
	{
		n += i->getItemCount();
	}
	return n;
}

void map::evaluateItems()
{
	//alterar depois para ser baseado em distância da sala que o item está para outra, ao invés de uma distância em linha reta do item
	//para o primeiro tile da sala inicial
	score = 0;
	for each (itemSpawned* is in currentItems)
	{
		for each (pos2D p in is->getPositions())
		{
			score += spawnRoom->getFirstTile().distanceToPoint(p);
		}
	}
}

float map::getScore()
{
	return score;
}

map::map()
{
}

map::map(ALLEGRO_BITMAP *newMap, int itemChance)
{
	thisMap = newMap;
	standardItemChance = itemChance;
}

map::map(ALLEGRO_BITMAP * newMap, int itemChance, std::list<room*> rooms)
{
	thisMap = newMap;
	standardItemChance = itemChance;
	this->rooms = rooms;
}


map::~map()
{
}
