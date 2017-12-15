#include "GeneticManager.h"
#include <list>
#include <iostream>

void GeneticManager::makeChildren(map * parentA, map * parentB)
{
	std::list<itemSpawned*> listA;
	std::list<itemSpawned*> listB;

	std::list<itemSpawned*> tempListA = parentA->getItemSpawneds();
	std::list<itemSpawned*> tempListB = parentB->getItemSpawneds();

	std::list<itemSpawned*>::iterator itA = tempListA.begin();
	std::list<itemSpawned*>::iterator itB = tempListB.begin();

	std::list<pos2D> tempListPos2DA;
	std::list<pos2D> tempListPos2DB;

	std::list<pos2D> nextListPos2DA;
	std::list<pos2D> nextListPos2DB;

	std::list<pos2D>::iterator itPos2DA;
	std::list<pos2D>::iterator itPos2DB;

	pos2D tempPosA;
	pos2D tempPosB;

	while (itA != tempListA.end() || itB != tempListB.end())
	{
		tempListPos2DA = (*itA)->getPositions();
		tempListPos2DB = (*itB)->getPositions();

		itPos2DA = tempListPos2DA.begin();
		itPos2DB = tempListPos2DB.begin();

		listA.emplace_back(new itemSpawned((*itA)->getBaseItem()));
		listB.emplace_back(new itemSpawned((*itA)->getBaseItem()));

		nextListPos2DA.clear();
		nextListPos2DB.clear();

		while (itPos2DA != tempListPos2DA.end() || itPos2DB != tempListPos2DB.end())
		{
			if (getRandomPercentage() < mutationChance)
				tempPosA = parentA->getRandomRoom()->getRandomTile();
			else
				tempPosA = (*itPos2DA);
			if (getRandomPercentage() < mutationChance)
				tempPosB = parentB->getRandomRoom()->getRandomTile();
			else
				tempPosB = (*itPos2DB);
			if (getRandomNumberInRange(1, 100) <= 50)
			{
				listA.back()->addItem(tempPosA);
				listB.back()->addItem(tempPosB);

			}
			else
			{
				listA.back()->addItem(tempPosB);
				listB.back()->addItem(tempPosA);
			}
			++itPos2DA;
			++itPos2DB;
		}
		++itA;
		++itB;
	}
	parentA->setItemSpawned(listA);
	parentB->setItemSpawned(listB);
	currentMaps.emplace_back(parentA);
	currentMaps.emplace_back(parentB);
}

void GeneticManager::processCurrentGeneration()
{
	map* tempMap1 = nullptr;
	map* tempMap2 = nullptr;

	map* randomMap;

	std::list<map*>::iterator tempIt;
	std::list<map*>* maps = RandomMan->getMaps();
	std::list<map*> deleteMaps;
	int newMapSize = maps->size();
	int currentMapSize = 0;

	while (currentMaps.size() < parentPercentage * maps->size())
	{
		randomMap = selectRandomMap((*maps));
		tempIt = std::find(currentMaps.begin(), currentMaps.end(), randomMap);
		if (tempIt == currentMaps.end())
		{
			if (tempMap1 == nullptr)
				tempMap1 = randomMap;
			else if (tempMap2 == nullptr)
				tempMap2 = randomMap;
		}
		if (tempMap1 != nullptr && tempMap2 != nullptr)
		{
			makeChildren(tempMap1, tempMap2);
			currentMaps.emplace_back(tempMap1);
			currentMaps.emplace_back(tempMap2);
			tempMap1 = nullptr;
			tempMap2 = nullptr;
		}
	}
	maps->clear();
	for each (map* m in currentMaps)
	{
		maps->emplace_back(m);
	}
	RandomMan->addMaps(newMapSize - currentMaps.size());
	RandomMan->populateMapsWithItems();
	RandomMan->calculateScores();
	RandomMan->sortMaps();
	currentMaps.clear();
	currentGeneration++;
}

void GeneticManager::processAllGenerations()
{
	mapRooms = RandomMan->getMaps()->front()->getRoomList();
	mainPath = RandomMan->getMaps()->front()->getMainPath();

	while (currentGeneration < maxNumberOfGenerations)
	{
		std::cout << "Processando geracao " << currentGeneration << std::endl;
		processCurrentGeneration();
	}
}

map * GeneticManager::selectRandomMap(std::list<map*> maps)
{
	float combinedWeights = 0;
	float tempWeight;

	for each (map* m in maps)
	{
		combinedWeights += m->getScore();
	}

	tempWeight = getRandomNumberInRange(0.0f, combinedWeights);
	for each (map* m in maps)
	{
		tempWeight -= m->getScore();
		if (tempWeight <= 0.0f)
		{
			return m;
		}
	}
}

GeneticManager::GeneticManager(float mutationChance, float parentPercentage, int numberOfGenerations, RandomManager* rm)
{
	this->mutationChance = mutationChance;
	this->parentPercentage = parentPercentage;
	this->maxNumberOfGenerations = numberOfGenerations;
	this->RandomMan = rm;
	this->currentGeneration = 0;
}



GeneticManager::~GeneticManager()
{
}
