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
	int numberOfCutOffs;
	RandomManager* RandomMan;
public:
	void makeChildren(map* parentA, map* parentB);
	void makeChildren(map* parentA, map* parentB, std::list<int> cutOffPoints);
	std::list<int> calculateCutOffPoints(itemSpawned* exemple);
	void processCurrentGeneration();
	void processAllGenerations();
	void print4itemPositions(std::list<itemSpawned*> a, std::list<itemSpawned*> b, std::list<itemSpawned*> c, std::list<itemSpawned*> d);
	map* selectRandomMap(std::list<map*> maps);
	GeneticManager(float mutationChance, float parentPercentage, int numberOfGenerations, int numberOfCutOffs, RandomManager* rm);
	~GeneticManager();
};

