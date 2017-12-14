#include "map.h"
#include "defines.h"
#include <random>
#include <vector>
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
	for each (room* r in rooms)
	{
		if (r->isTilePartOfRoom(tile))
		{
			return r;
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

std::list<itemSpawned*> map::getItemSpawneds()
{
	std::cout<< "CurrentSize: "<< currentItems.size() <<std::endl; 
	al_rest(3);
	return currentItems;
}

void map::setItemSpawned(std::list<itemSpawned*> its)
{
	currentItems = its;
	evaluateItems();
}

bool map::hasItems()
{
	if (!currentItems.empty())
		return true;
	return false;
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
	if (!currentItems.empty())
	{
		return;
	}
	room* randomRoom;
	int tempNumberOfItems = 0;
	for each (item* i in items)
	{
		tempNumberOfItems = i->getNumberOfItems();
		while (tempNumberOfItems > 0)
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

	biggerMap = al_create_bitmap(width*tileSize, height*tileSize);
	al_set_target_bitmap(biggerMap);

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			//al_draw_filled_rectangle(i*tileSize, j*tileSize, i*tileSize + tileSize, j*tileSize + tileSize, al_get_pixel(thisMap, i, j));
			it = std::find(mainPath.begin(), mainPath.end(), findRoomFromTile(pos2D(i, j)));
			if (spawnRoom->isTilePartOfRoom(pos2D(i, j)))
				al_draw_filled_rectangle(i*tileSize, j*tileSize, i*tileSize + tileSize, j*tileSize + tileSize, COLORS::RED());
			else if (endRoom->isTilePartOfRoom(pos2D(i, j)))
				al_draw_filled_rectangle(i*tileSize, j*tileSize, i*tileSize + tileSize, j*tileSize + tileSize, COLORS::PURPLE());
			else if (it != mainPath.end())
				al_draw_filled_rectangle(i*tileSize, j*tileSize, i*tileSize + tileSize, j*tileSize + tileSize, COLORS::BLUE());
			else
				al_draw_filled_rectangle(i*tileSize, j*tileSize, i*tileSize + tileSize, j*tileSize + tileSize, al_get_pixel(thisMap, i, j));
			it = std::find(mainPath.begin(), mainPath.end(), findRoomFromTile(pos2D(i, j)));
		}
	}

	for (std::list<itemSpawned*>::iterator i = currentItems.begin(); i != currentItems.end(); i++)
	{
		for each (pos2D pos in (*i)->getPositions())
		{
			tempMap = (*i)->getBitmap();
			al_draw_bitmap((*i)->getBitmap(), pos.x * tileSize, pos.y * tileSize, 0);

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
	score = evaluateDistance() * evaluateSpread();
}

float map::evaluateDistance()
{
	float tempScore = 0;
	for each (itemSpawned* is in currentItems)
	{
		for each (pos2D p in is->getPositions())
		{
			tempScore += findRoomFromTile(p)->getDistance() * is->getDistanceMultiplier();
		}
	}
	return tempScore;
}

float map::evaluateSpread()
{
	float tempScore = 0;
	for each (itemSpawned* is in currentItems)
	{
		tempScore += evaluateSpreadOfItem(is) * is->getDistanceMultiplier();
	}
	return tempScore;
}

float map::evaluateSpreadOfItem(itemSpawned* is)
{
	float xMed = 0;
	float yMed = 0;

	int n = 0;

	std::vector<float> x;
	std::vector<float> y;

	float xSumSqrt = 0;
	float ySumSqrt = 0;

	for each (pos2D p in is->getPositions())
	{
		n++;
		xMed += p.x;
		yMed += p.y;
		x.emplace_back(p.x);
		y.emplace_back(p.y);
	}
	xMed = xMed / n;
	yMed = yMed / n;
	for (int i = 0; i < n; ++i)
	{
		xSumSqrt += (x[i] - xMed)*(x[i] - xMed);
		ySumSqrt += (y[i] - yMed)*(y[i] - yMed);
	}
	return sqrt(xSumSqrt / n + ySumSqrt / n);
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
	//std::cout << "SPAWN ROOM:" << spawnRoom << std::endl;
	//std::cout << "Rooms:" << std::endl;
	//printRoomAdresses(rooms);
	resetRoomParents();
	for each (room* r in rooms)
	{
		if (r != spawnRoom)
		{
			resetRoomParents();
			r->setDistance(Djikstra(spawnRoom, r).size());
		}
	}
}

void map::checkRoomsDistancesToMainPath()
{
	for each (room* r in rooms)
	{
		r->setDistance(checkRoomDistanceToMainPath(r));
	}
	for each (room* m in mainPath)
	{
		m->setDistance(0);
	}
}

int map::checkRoomDistanceToMainPath(room * r)
{
	int distance = rooms.size() * 99;
	std::list<room*> path;
	for each (room* p in mainPath)
	{
		resetRoomParents();
		path = Djikstra(r, p);
		if (distance > path.size())
		{
			distance = path.size();
		}
		if (r == p)
		{
			return 0;
		}
	}
	return distance;
}

void map::findMainPath()
{
	resetRoomParents();
	mainPath = Djikstra(spawnRoom, endRoom);
	mainPath.emplace_back(spawnRoom);
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
