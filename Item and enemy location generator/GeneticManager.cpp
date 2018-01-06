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

void GeneticManager::makeChildren(map * parentA, map * parentB, std::list<int> cutOffPoints)
{
	if (parentA == parentB)
	{
		std::cout << "Why are you the same though" << std::endl;
		std::cout << '\a' << std::endl;
		al_rest(2);
	}
	std::list<itemSpawned*> listA;
	std::list<itemSpawned*> listB;

	std::list<itemSpawned*> tempListA = parentA->getItemSpawneds();
	std::list<itemSpawned*> tempListB = parentB->getItemSpawneds();

	std::list<itemSpawned*>::iterator itA = tempListA.begin();
	std::list<itemSpawned*>::iterator itB = tempListB.begin();

	std::list<pos2D> tempListPos2DA;
	std::list<pos2D> tempListPos2DB;

	std::list<pos2D>::iterator itPos2DA;
	std::list<pos2D>::iterator itPos2DB;

	std::list<int> cutOffB = cutOffPoints;

	pos2D tempPosA;
	pos2D tempPosB;

	int n = 0;
	bool alternate = true;
	int size = tempListA.front()->getItemCount();
	size++;
	int tempInt = cutOffPoints.front();
	while (itA != tempListA.end() || itB != tempListB.end())
	{
		tempListPos2DA = (*itA)->getPositions();
		tempListPos2DB = (*itB)->getPositions();

		itPos2DA = tempListPos2DA.begin();
		itPos2DB = tempListPos2DB.begin();

		listA.emplace_back(new itemSpawned((*itA)->getBaseItem()));
		listB.emplace_back(new itemSpawned((*itA)->getBaseItem()));

		while (itPos2DA != tempListPos2DA.end() || itPos2DB != tempListPos2DB.end())
		{
			if (alternate)
			{
				tempPosA = (*itPos2DA);
				tempPosB = (*itPos2DB);
			}
			else
			{
				tempPosA = (*itPos2DB);
				tempPosB = (*itPos2DA);
			}
			if (n >= tempInt)
			{
				if (!cutOffPoints.empty())
					cutOffPoints.pop_front();
				if (cutOffPoints.empty())
					tempInt = size;
				else
					tempInt = cutOffPoints.front();
				alternate = !alternate;
			}
			listA.back()->addItem(tempPosA);
			listB.back()->addItem(tempPosB);
			++n;
			++itPos2DA;
			++itPos2DB;

		}
		++itA;
		++itB;
	}
	processMutation(listA, cutOffB, parentA);
	processMutation(listB, cutOffB, parentA);
	currentMaps.emplace_back(new map(RandomMan->getMapBitMap(), mapRooms, mainPath, listA));
	currentMaps.emplace_back(new map(RandomMan->getMapBitMap(), mapRooms, mainPath, listB));
	//print4itemPositions(parentA->getItemSpawneds(), parentB->getItemSpawneds(), listA, listB);
}

std::list<int> GeneticManager::calculateCutOffPoints(itemSpawned * exemple)
{
	std::list<int> result;
	std::list<int>::iterator it = result.begin();
	int n = exemple->getItemCount();
	int tempRandom = getRandomNumberInRange(1, n);
	bool test;
	for (int i = 0; i < numberOfCutOffs; ++i)
	{
		tempRandom = getRandomNumberInRange(1, n);
		result.emplace_back(tempRandom);
	}
	result.sort();
	for each (int i in result)
	{
		std::cout << "Cut Off:" << i << std::endl;
	}
	return result;
}

void GeneticManager::processCurrentGeneration()
{
	map* tempMap1 = nullptr;
	map* tempMap2 = nullptr;

	map* randomMap;

	std::list<map*>::iterator tempIt;
	std::list<map*>::iterator findIt;
	std::list<map*>* maps = RandomMan->getMaps();
	std::list<map*> deleteMaps;
	int newMapSize = maps->size();
	int currentMapSize = 0;
	int size = newMapSize - (newMapSize * parentPercentage);


	std::list<int> cutOffs = calculateCutOffPoints(maps->front()->getItemSpawneds().front());
	if ((newMapSize - size) % 2 != 0)
	{
		size++;
	}
	tempIt = maps->begin();

	std::cout << "Sizes:" << newMapSize << " / " << size << std::endl;
	for (int i = 0; i < size; i++)
	{
		currentMaps.emplace_back(new map((*tempIt)));
		currentMaps.back()->populateRooms(mapRooms);
		++tempIt;
	}
	while (currentMaps.size() < newMapSize)
	{
		randomMap = selectRandomMap((*maps));
		maps->remove(randomMap);
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
			makeChildren(tempMap1, tempMap2, cutOffs);
			delete(tempMap1);
			delete(tempMap2);
			tempMap1 = nullptr;
			tempMap2 = nullptr;
		}
	}
	maps->clear();
	for each (map* m in currentMaps)
	{
		maps->emplace_back(m);
		maps->back()->setMainPath(mainPath);
	}
	RandomMan->populateMapsWithItems();
	RandomMan->calculateScores();
	RandomMan->sortMaps();
	calculateStandardDeviation((*RandomMan->getMaps()));
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
		RandomMan->printScores();
	}
}

void GeneticManager::calculateStandardDeviation(std::list<map*> maps)
{
	float med = 0;
	float sqrtCalc = 0;
	int n = 0;
	for each (map* m in maps)
	{
		med += m->getScore();
		++n;
	}
	med = med / n;
	for each (map* m in maps)
	{
		sqrtCalc += (m->getScore() - med) * (m->getScore() - med);
	}
	standardDeviationOfPreviousGen = sqrt(sqrtCalc);
	biggestScoreFromPreviousGen = maps.front()->getScore();

}

void GeneticManager::processMutation(std::list<itemSpawned*> &items, std::list<int> cutOff, map* m)
{
	int numberOfZones = cutOff.size() + 1;
	int numberOfItems = 0;

	int multiplier = 1;

	int numberA;
	int numberB;
	int i = 0;

	pos2D tempPos;

	//std::cout << "Standard:" << standardDeviationOfPreviousGen << "// Biggest: " << biggestScoreFromPreviousGen*0.05 << std::endl;
	if (standardDeviationOfPreviousGen < biggestScoreFromPreviousGen * criticalThreshold)
	{
		++generationsBellowThreshold;
		multiplier = 1 + generationsBellowThreshold / numberOfGenerationsToIncreaseMutation;
	}
	else
		generationsBellowThreshold = 0;

	std::list<int> randomStuff;
	for each (itemSpawned* is in items)
	{
		numberOfItems += is->getItemCount();
	}
	cutOff.emplace_front(0);
	cutOff.emplace_back(numberOfItems - 1);

	std::list<int>::iterator it = cutOff.begin();

	for (int i = 0; i < numberOfZones; ++i)
	{
		if (getRandomPercentage() < mutationChance * multiplier)
		{
			numberA = (int)(*it++);
			numberB = (int)(*it);
			//while (i++ < generationsBellowThreshold / 5 + 1)
			randomStuff.emplace_back(getRandomNumberInRange(numberA, numberB));
			randomStuff.unique();
		}
	}

	bool end = false;

	for each (int i in randomStuff)
	{
		for each(itemSpawned* is in items)
		{
			for each (pos2D p in is->getPositions())
			{
				if (i <= 0 && end == false)
				{
					tempPos = m->getRandomTileWithoutItems();
					is->swapItem(p, tempPos);
					end = true;
				}
				i--;
			}
		}
	}
}

void GeneticManager::print4itemPositions(std::list<itemSpawned*> a, std::list<itemSpawned*> b, std::list<itemSpawned*> c, std::list<itemSpawned*> d)
{
	std::list<itemSpawned*> aL = a;
	std::list<itemSpawned*> bL = b;
	std::list<itemSpawned*> cL = c;
	std::list<itemSpawned*> dL = d;


	std::list<itemSpawned*>::iterator ita = aL.begin();
	std::list<itemSpawned*>::iterator itb = bL.begin();
	std::list<itemSpawned*>::iterator itc = cL.begin();
	std::list<itemSpawned*>::iterator itd = dL.begin();

	std::list<pos2D> listPosA;
	std::list<pos2D> listPosB;
	std::list<pos2D> listPosC;
	std::list<pos2D> listPosD;

	std::list<pos2D>::iterator posItA;
	std::list<pos2D>::iterator posItB;
	std::list<pos2D>::iterator posItC;
	std::list<pos2D>::iterator posItD;

	int i = 1;

	std::cout << "--------Map1 --- Map2 --- MapRef1 --- MapRef2" << std::endl;

	while (ita != aL.end() || itb != bL.end() || itc != cL.end() || itd != dL.end())
	{
		listPosA = (*ita)->getPositions();
		listPosB = (*itb)->getPositions();
		listPosC = (*itc)->getPositions();
		listPosD = (*itd)->getPositions();

		posItA = listPosA.begin();
		posItB = listPosB.begin();
		posItC = listPosC.begin();
		posItD = listPosD.begin();

		while (posItA != listPosA.end() || posItB != listPosB.end() || posItC != listPosC.end() || posItD != listPosD.end())
		{
			std::cout << i;
			if (i++ < 10)
				std::cout << " ";
			std::cout << "----- ";
			std::cout << posItA->x;
			if (posItA->x < 10)
			{
				std::cout << " ";
			}
			std::cout << "," << posItA->y;
			if (posItA->y < 10)
			{
				std::cout << " ";
			}
			std::cout << "    ";
			std::cout << posItB->x;
			if (posItB->x < 10)
			{
				std::cout << " ";
			}
			std::cout << "," << posItB->y;
			if (posItB->y < 10)
			{
				std::cout << " ";
			}
			std::cout << "    ";
			std::cout << posItC->x;
			if (posItC->x < 10)
			{
				std::cout << " ";
			}
			std::cout << "," << posItC->y;
			if (posItC->y < 10)
			{
				std::cout << " ";
			}
			std::cout << "    ";
			std::cout << posItD->x;
			if (posItD->x < 10)
			{
				std::cout << " ";
			}
			std::cout << "," << posItD->y << std::endl;

			++posItA;
			++posItB;
			++posItC;
			++posItD;
		}
		++ita;
		++itb;
		++itc;
		++itd;
	}
	std::cin.get();
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

GeneticManager::GeneticManager(float mutationChance, float parentPercentage, int numberOfGenerations, int numberOfCutOffs, RandomManager* rm, int criticalThrehold, int numberOfGenerationsToIncreaseMutation)
{
	this->mutationChance = mutationChance;
	this->parentPercentage = parentPercentage;
	this->maxNumberOfGenerations = numberOfGenerations;
	this->numberOfCutOffs = numberOfCutOffs;
	this->RandomMan = rm;
	this->currentGeneration = 0;
	this->standardDeviationOfPreviousGen = 10;
	this->biggestScoreFromPreviousGen = 0;
	this->criticalThreshold = criticalThrehold;
	this->generationsBellowThreshold = 0;
	this->numberOfGenerationsToIncreaseMutation = numberOfGenerationsToIncreaseMutation;
}



GeneticManager::~GeneticManager()
{
}
