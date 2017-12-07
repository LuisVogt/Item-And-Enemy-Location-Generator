#include "map.h"
#include "defines.h"
#include <random>
#include <iostream>
#include <allegro5\allegro_primitives.h>

room* map::spawnRoom = nullptr;
room* map::endRoom = nullptr;

void map::drawRooms(ALLEGRO_BITMAP *drawMap)
{
	for (std::list<room*>::iterator i = rooms.begin(); i != rooms.end(); ++i)
	{
		if ((*i) == spawnRoom)
			spawnRoom->drawMap(drawMap, COLORS::RED());
		else if ((*i) == endRoom)
			endRoom->drawMap(drawMap, COLORS::BLACK());
		else
			(*i)->drawMap(drawMap, COLORS::WHITE());
	}
	int n = 0;
	/*std::list<room*> path = Djikstra(rooms.front(), rooms.back(), n);
	for each (room* r in path)
	{
		r->drawMap(drawMap, COLORS::BLUE());
	}*/
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
	int randomNumber = getRandomNumberInRange(1, numberOfRooms);

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

std::list<room*> map::getRoomsInRangeFromSpawn(int minDistance)
{
	std::list<room*> minDistanceRooms;
	for each (room* r in rooms)
	{
		if (r->getDistance() >= minDistance)
		{
			minDistanceRooms.emplace_back(r);
		}
	}
	return minDistanceRooms;
}

room * map::getRandomRoom(int minDistance)
{
	std::list<room*> minDistanceRooms;
	minDistanceRooms = getRoomsInRangeFromSpawn(minDistance);
	int randomNumber;
	if (minDistance <= 1)
		randomNumber = 1;
	else
		randomNumber = getRandomNumberInRange(1, minDistance);

	for each (room* r in minDistanceRooms)
	{
		if (--randomNumber <= 0)
		{
			return r;
		}
	}
	return minDistanceRooms.front();

}

int map::getMaxDistance()
{
	int distance = 0;
	for each (room* r in rooms)
	{
		if (r->getDistance() > distance)
		{
			distance = r->getDistance();
		}
	}
	return distance;
}

void map::setSpawnRoom(room * newSpawnRoom)
{
	spawnRoom = newSpawnRoom;
}

void map::setEndRoom(room * newEndRoom)
{
	endRoom = newEndRoom;
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
			if (areTwoColorsTheSame(al_get_pixel(map, i, j), COLORS::GREEN()))
			{
				tempTile = pos2D(i, j).get4WayAdjacentPositions();
				al_draw_pixel(i, j, COLORS::GREEN());
				for each (pos2D posAround in tempTile)
				{
					if (areTwoColorsTheSame(al_get_pixel(map, posAround.x, posAround.y), COLORS::WHITE()))
					{
						tempTile1 = posAround;
						tempTile2 = (posAround + (pos2D(i, j) - posAround) * 3);

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
				/*for (std::list<pos2D>::iterator aroundDoor = tempTile.begin(); aroundDoor != tempTile.end(); ++aroundDoor)
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
				}*/
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
	
	std::list<room*>::iterator it;

	biggerMap=al_create_bitmap(width*tileSize, height*tileSize);
	al_set_target_bitmap(biggerMap);

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			al_draw_filled_rectangle(i*tileSize, j*tileSize, i*tileSize + tileSize, j*tileSize + tileSize, al_get_pixel(thisMap, i, j));
			it = std::find(mainPath.begin(), mainPath.end(), findRoomFromTile(pos2D(i, j)));
			if (it != mainPath.end())
			{
				al_draw_filled_rectangle(i*tileSize, j*tileSize, i*tileSize + tileSize, j*tileSize + tileSize, COLORS::BLUE());
			}
			if (spawnRoom->isTilePartOfRoom(pos2D(i, j)))
				al_draw_filled_rectangle(i*tileSize, j*tileSize, i*tileSize + tileSize, j*tileSize + tileSize, COLORS::RED());
			else if (endRoom->isTilePartOfRoom(pos2D(i, j)))
				al_draw_filled_rectangle(i*tileSize, j*tileSize, i*tileSize + tileSize, j*tileSize + tileSize, COLORS::PURPLE());
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
	al_destroy_bitmap(biggerMap);
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
			score += findRoomFromTile(p)->getDistance() * is->getMultiplier();

		}
	}
	
}

void map::resetRoomDistances()
{
	for each (room* r in rooms)
	{
		r->resetDistance();
	}
}

void map::resetRoomParents()
{
	for each (room* r in rooms)
	{
		r->resetParent();
	}
}

void map::checkRoomsDistancesToSpawn()
{
	int distance = 0;
	std::cout << "SPAWN ROOM:" << spawnRoom << std::endl;
	std::cout << "Rooms:" << std::endl;
	printRoomAdresses(rooms);
	resetRoomParents();
	for each (room* r in rooms)
	{
		if (r != spawnRoom)
		{
			resetRoomParents();
			Djikstra(spawnRoom, r, distance);
			r->setDistance(distance);
		}
	}
}

void map::checkRoomsDistancesToMainPath()
{
	int dist;
	for each (room* r in rooms)
	{
		for each (room* path in mainPath)
		{
			resetRoomParents();
			Djikstra(r, path, dist);
			if (r->getDistance() == -1 || r->getDistance() > dist)
			{
				r->setDistance(dist);
			}
		}
	}
}

void map::findMainPath()
{
	int a;
	resetRoomParents();
	mainPath = Djikstra(spawnRoom, endRoom,a);
}

void map::setMainPath(std::list<room*> p)
{
	mainPath = p;
}

std::list<room*> map::getMainPath()
{
	return mainPath;
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
