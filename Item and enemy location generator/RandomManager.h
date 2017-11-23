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
	void defineNumberOfItems(); //Define o n�mero de itens que o mapa ter�.
	void createMaps(int numberOfMaps);
	void addItemFromFileName(const char* filePath, int spawnChance);
	void saveMapImages(const char* path);
	void populateMapsWithItems();
	void checkIfAllMapsHaveTheSameAmountOfItems();
	RandomManager(ALLEGRO_BITMAP * mapImage, int chanceForItemToSpawnInARoom);
	~RandomManager();
};