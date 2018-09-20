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
	static std::list<room*> rooms; 
	static std::list<room*> mainPath; 

	static room* spawnRoom;
	static room* endRoom;
	static int numberOfRooms; 
	static ALLEGRO_BITMAP *thisMap;
	ALLEGRO_BITMAP *biggerMap;
	std::list<itemSpawned*> currentItems;
	static int standardItemChance;
	static float maxDistanceScore;
	static float maxSpreadScore;
	static float maxRoomSpreadScore;
	float score;
public:
	static void setBitmap(ALLEGRO_BITMAP* m);
	void drawRooms(ALLEGRO_BITMAP *drawMap);
	void drawRoomsWithRandomColors(ALLEGRO_BITMAP *drawMap);
	static room* findRoomFromTile(pos2D tile);
	static room* getRandomRoom();
	static room* getRandomRoom(int minDistance); 
	static std::list<room*> getRoomsInRangeFromSpawn(int minDistance); 
	static int getMaxDistance(); 

	pos2D getRandomTileWithoutItems();
	bool isThereItemInPos(pos2D pos);

	static void setSpawnRoom(room* newSpawnRoom);
	static void setEndRoom(room* newEndRoom);

	std::list<itemSpawned*> getItemSpawneds();
	void setItemSpawned(std::list<itemSpawned*> its);

	bool hasItems();

	static std::list<room*> getRoomList();
	static int getNumberOfRooms();
	static int getStandardItemChance();

	static void populateRooms();

	void populateItems(std::list<item*> items, ALLEGRO_BITMAP *map);
	void saveLargerMapWithItems(int tileSize, const char* path);
	void saveItemMap(const char* path);
	void spawnItem(item * it, pos2D pos);
	int getNumberOfItems();
	void evaluateItems();

	float evaluateDistance();
	float evaluateSpread();
	float evaluateRoomSpread();
	float evaluateSpreadOfItem(itemSpawned* is);




	static void resetRoomDistances(); 
	static void resetRoomParents();
	static void checkRoomsDistancesToSpawn();
	static void checkRoomsDistancesToMainPath(); 
	static int checkRoomDistanceToMainPath(room* r); 
	static void findMainPath();

	static void setMaxDistanceScore(float s);
	static void setMaxSpreadScore(std::list<float> multipliers);
	static void setMaxRoomSpreadScore(std::list<float> multipliers, std::list<int> amount);

	static std::list<room*> getMainPath();
	float getScore();
	map();
	map(map* m);
	map(std::list<itemSpawned*> currentItems);
	map(ALLEGRO_BITMAP *newMap, int itemChance);
	map(ALLEGRO_BITMAP *newMap, int itemChance, std::list<room*> rooms);
	map(ALLEGRO_BITMAP *newMap, std::list<room*> rooms, std::list<room*> mainPath, std::list<itemSpawned*> currentItems);
	map(ALLEGRO_BITMAP *newMap, std::list<room*> rooms, std::list<room*> mainPath);
	~map();
};