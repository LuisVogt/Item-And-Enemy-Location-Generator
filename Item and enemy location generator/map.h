#include <list>
#include "room.h"
#include "extraFunctions.h"
#include "structs.h"
#include <allegro5\allegro.h>
#include "item.h"
#include "itemSpawned.h"

#pragma once
class map
{
private:
	std::list<room*> rooms;
	std::list<room*> mainPath;
	static room* spawnRoom;
	static room* endRoom;
	int numberOfRooms;
	ALLEGRO_BITMAP *thisMap;
	ALLEGRO_BITMAP *biggerMap;
	std::list<itemSpawned*> currentItems;
	int standardItemChance;
	float score;
public:
	void drawRooms(ALLEGRO_BITMAP *drawMap);
	void drawRoomsWithRandomColors(ALLEGRO_BITMAP *drawMap);
	room* findRoomFromTile(pos2D tile);
	room* getRandomRoom();
	room* getRandomRoom(int minDistance);
	std::list<room*> getRoomsInRangeFromSpawn(int minDistance);
	int getMaxDistance();

	static void setSpawnRoom(room* newSpawnRoom);
	static void setEndRoom(room* newEndRoom);
	
	std::list<room*> getRoomList();
	int getNumberOfRooms();
	int getStandardItemChance();
	void populateRooms();
	void populateRooms(std::list<room*> setRooms);
	void populateItems(std::list<item*> items, ALLEGRO_BITMAP *map);
	void saveLargerMapWithItems(int tileSize, const char* path);
	void spawnItem(item * it, pos2D pos);
	int getNumberOfItems();
	void evaluateItems();
	void resetRoomDistances();
	void resetRoomParents();
	void checkRoomsDistancesToSpawn();
	void checkRoomsDistancesToMainPath();
	void findMainPath();
	void setMainPath(std::list<room*> p);
	std::list<room*> getMainPath();
	float getScore();
	map();
	map(ALLEGRO_BITMAP *newMap, int itemChance);
	map(ALLEGRO_BITMAP *newMap, int itemChance, std::list<room*> rooms);
	~map();
};