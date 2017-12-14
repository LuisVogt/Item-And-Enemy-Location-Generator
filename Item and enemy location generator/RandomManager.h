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
	void defineNumberOfItems();
	void createMaps(int numberOfMaps);
	void addMaps(int numberOfMaps);
	void addItemFromFileName(const char* filePath, int spawnChance, float distanceMultiplier, float spreadMultiplier);
	void addItemFromFileName(const char* filePath, int spawnChance, int minNumberOfItems, float distanceMultiplier, float spreadMultiplier);
	void addItemFromFileName(const char* filePath, int spawnChance, int minNumberOfItems, int maxNumberOfItems, float distanceMultiplier, float spreadMultiplier);
	void saveMapImages(const char* path, const char* fileName);
	void populateMapsWithItems();
	void checkIfAllMapsHaveTheSameAmountOfItems();
	bool isMapASmallerThanB(map* A, map* B);
	void sortMaps();
	void calculateScores();
	void printScores();
	void getMainPaths();
	RandomManager(ALLEGRO_BITMAP * mapImage, int chanceForItemToSpawnInARoom);
	~RandomManager();
};