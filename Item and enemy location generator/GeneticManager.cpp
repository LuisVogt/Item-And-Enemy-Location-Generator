#include "GeneticManager.h"
#include <list>
#include <iostream>

void GeneticManager::makeChildren(map * parentA, map * parentB)
{
	std::cout << "Make Children" << std::endl;
	al_rest(1);

	std::list<itemSpawned*> listA;
	std::list<itemSpawned*> listB;	
	std::cout<< "What" <<std::endl; 
	al_rest(2);
	std::list<itemSpawned*> tempListA = parentA->getItemSpawneds();
	std::list<itemSpawned*> tempListB = parentB->getItemSpawneds();
	std::cout << "Size " << tempListA.size() << std::endl;
	al_rest(2);
	std::cout << "Size " << tempListB.size() << std::endl;
	al_rest(2);
	std::list<itemSpawned*>::iterator itA = tempListA.begin();
	std::list<itemSpawned*>::iterator itB = tempListB.begin();

	std::list<pos2D>::iterator itPos2DA;
	std::list<pos2D>::iterator itPos2DB;

	pos2D tempPosA;
	pos2D tempPosB;

	while (itA != tempListA.end() || itB != tempListB.end())
	{
		std::cout << "While2" << std::endl;
		al_rest(1);
		itPos2DA = (*itA)->getPositions().begin();
		itPos2DB = (*itB)->getPositions().begin();
		listA.emplace_back((*itA));
		listB.emplace_back((*itB));
		while (itPos2DA != (*itA)->getPositions().end() || itPos2DB != (*itB)->getPositions().end())
		{
			std::cout << "While2" << std::endl;
			al_rest(1);
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
}

void GeneticManager::processCurrentGeneration()
{
	std::cout << "ProcessCurrentGen" << std::endl;
	al_rest(1);

	map* tempMap1 = nullptr;
	map* tempMap2 = nullptr;

	map* randomMap;

	std::list<map*>::iterator tempIt;
	std::list<map*>* maps = RandomMan->getMaps();

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
	for each (map* m in (*maps))
	{
		tempIt = std::find(currentMaps.begin(), currentMaps.end(), m);
		if (tempIt != currentMaps.end())
		{
			maps->remove(m);
		}
	}
	RandomMan->addMaps(maps->size() - currentMaps.size());
	currentMaps.clear();
	currentGeneration++;
}

void GeneticManager::processAllGenerations()
{
	std::cout<< "Process All Gens" <<std::endl; 
	al_rest(1);
	while (currentGeneration < maxNumberOfGenerations)
	{
		processCurrentGeneration();
		RandomMan->populateMapsWithItems();
		std::cout << "Processando geracao " << currentGeneration << std::endl;
	}
}

map * GeneticManager::selectRandomMap(std::list<map*> maps)
{
	std::cout<< "SelectRandomMap" <<std::endl; 
	al_rest(1);

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
