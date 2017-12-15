#pragma once
#include <list>
#include "map.h"
#include "extraFunctions.h"
#include "RandomManager.h"
class GeneticManager
{
private:
	float mutationChance;
	float parentPercentage;
	int currentGeneration;
	int maxNumberOfGenerations;
	std::list<map*> currentMaps;
	std::list<room*> mapRooms;
	ALLEGRO_BITMAP* mapBitmap;
	std::list<room*> mainPath;
	int numberOfMaps;
	RandomManager* RandomMan;
public:
	void makeChildren(map* parentA, map* parentB);
	void processCurrentGeneration();
	void processAllGenerations();
	map* selectRandomMap(std::list<map*> maps);
	GeneticManager(float mutationChance, float parentPercentage, int numberOfGenerations, RandomManager* rm);
	~GeneticManager();
};
