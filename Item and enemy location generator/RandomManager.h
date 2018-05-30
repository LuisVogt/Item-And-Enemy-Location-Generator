#include "map.h"
#include "structs.h"
#include <list>
#include "item.h"
#include <random>
#include <string>
#include <iostream>

#pragma once

class RandomManager
{
private:
	ALLEGRO_BITMAP * floorMap;
	std::list<map*> maps;
	std::list<item*> items;
	ALLEGRO_BITMAP * mapImage;
	int chanceForItemToSpawnInARoom;
public:
	item* getRandomItem();
	std::list<map*>* getMaps();
	ALLEGRO_BITMAP* getMapBitMap();
	void defineNumberOfItems();
	void createMaps(int numberOfMaps);
	void addMaps(int numberOfMaps);
	void addMap(map* m);
	void addItemFromFileName(const char* filePath, int spawnChance, float distanceMultiplier, float spreadMultiplier, float roomSpreadMultiplier);
	void addItemFromFileName(const char* filePath, int spawnChance, int minNumberOfItems, float distanceMultiplier, float spreadMultiplier, float roomSpreadMultiplier);
	void addItemFromFileName(const char* filePath, int spawnChance, int minNumberOfItems, int maxNumberOfItems, float distanceMultiplier, float spreadMultiplier, float roomSpreadMultiplier);
	void saveMapImages(const char* path, const char* fileName);
	void saveMapImages(const char* path, const char* fileName, int n);
	void saveItemImages(const char* path, const char* fileName, int n);
	void populateMapsWithItems();
	void checkIfAllMapsHaveTheSameAmountOfItems();
	void sortMaps();
	void calculateScores();
	void printScores();
	RandomManager(ALLEGRO_BITMAP * mapImage, int chanceForItemToSpawnInARoom);
	~RandomManager();
};