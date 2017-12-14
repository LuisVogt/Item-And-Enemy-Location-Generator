#include "RandomManager.h"
#include <string>
#include <algorithm>


item * RandomManager::getRandomItem()
{
	std::random_device rd;
	std::mt19937 eng(rd());
	int totalWeight = 0;
	item * temp = nullptr;
	for each (item* itm in items)
	{
		totalWeight += itm->getSpawnChance();
	}
	std::uniform_int_distribution<> itemRandom(1, totalWeight);

	int itemChance = itemRandom(eng);

	for (std::list<item*>::iterator i = items.begin(); i != items.end(); ++i)
	{
		itemChance -= (*i)->getSpawnChance();
		if (itemChance <= 0)
		{
			temp = (*i);
			break;
		}
	}
	return temp;
}

std::list<map*>* RandomManager::getMaps()
{
	return &maps;
}

void RandomManager::defineNumberOfItems()
{
	bool isTotalWeightZero = true;
	for each (item* itm in items)
	{
		if (itm->getSpawnChance() != 0)
			isTotalWeightZero = false;
	}
	if (isTotalWeightZero)
		return;
	int numberOfRooms = maps.front()->getNumberOfRooms();
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<> roomRandom(1, 100);
	int roomChance = 0;
	for (int i = 0; i < numberOfRooms; i++)
	{
		roomChance = roomRandom(eng);

		if (roomChance <= chanceForItemToSpawnInARoom)
		{
			getRandomItem()->addNumberOfItems();
		}
	}
}

void RandomManager::createMaps(int numberOfMaps)
{
	map * firstMap;
	firstMap = new map(mapImage, chanceForItemToSpawnInARoom);
	firstMap->populateRooms();
	maps.emplace_back(firstMap);
	map * m;
	map::setSpawnRoom(firstMap->getRandomRoom());
	firstMap->checkRoomsDistancesToSpawn();
	map::setEndRoom(firstMap->getRandomRoom(firstMap->getMaxDistance() - 1));

	addMaps(numberOfMaps);
}

void RandomManager::addMaps(int numberOfMaps)
{
	map* m;
	for (int i = 1; i < numberOfMaps; i++)
	{
		m = new map(mapImage, chanceForItemToSpawnInARoom);
		m->populateRooms(maps.front()->getRoomList());
		maps.emplace_back(m);
	}
	getMainPaths();
	maps.front()->resetRoomDistances();
	maps.front()->checkRoomsDistancesToMainPath();
}

void RandomManager::addItemFromFileName(const char * filePath, int spawnChance, float distanceMultiplier, float spreadMultiplier)
{
	ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_set_path_filename(path, filePath);
	items.emplace_back(new item(al_load_bitmap(al_path_cstr(path, '/')), spawnChance, 0, 99999, distanceMultiplier, spreadMultiplier));
}

void RandomManager::addItemFromFileName(const char * filePath, int spawnChance, int minNumberOfItems, float distanceMultiplier, float spreadMultiplier)
{
	ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_set_path_filename(path, filePath);
	items.emplace_back(new item(al_load_bitmap(al_path_cstr(path, '/')), spawnChance, minNumberOfItems, 99999, distanceMultiplier, spreadMultiplier));
}

void RandomManager::addItemFromFileName(const char * filePath, int spawnChance, int minNumberOfItems, int maxNumberOfItems, float distanceMultiplier, float spreadMultiplier)
{
	ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_set_path_filename(path, filePath);
	items.emplace_back(new item(al_load_bitmap(al_path_cstr(path, '/')), spawnChance, minNumberOfItems, maxNumberOfItems, distanceMultiplier, spreadMultiplier));
}

void RandomManager::saveMapImages(const char* path, const char* fileName)
{
	std::string command = "del /Q ";
	std::string deletePath = path;
	deletePath += "\\*.png";
	system(command.append(path).c_str());
	int i = 1;
	std::string strPath;
	for each (map* m in maps)
	{
		std::cout << "Saving map " << i << std::endl;
		strPath = path;
		strPath += "/";
		strPath += fileName;
		strPath += std::to_string(i);
		strPath += " - Score ";
		strPath += std::to_string(m->getScore());
		strPath += ".png";
		m->saveLargerMapWithItems(48, strPath.c_str());
		++i;
	}
}

void RandomManager::populateMapsWithItems()
{
	for each (map* m in maps)
	{
		m->populateItems(items, mapImage);
	}
}

void RandomManager::checkIfAllMapsHaveTheSameAmountOfItems()
{
	int n = 1;
	for each (map* m in maps)
	{
		std::cout << "Numero de itens no mapa " << n++ << ":" << m->getNumberOfItems() << std::endl;
	}

}

bool RandomManager::isMapASmallerThanB(map * A, map * B)
{
	float scoreA = A->getScore();
	float scoreB = B->getScore();
	if (scoreA < scoreB)
		return true;
	return false;
}

void RandomManager::sortMaps()
{
	std::list<map*>::iterator begin = maps.begin();
	std::list<map*>::iterator end = maps.end();
	//std::sort(begin, end, isMapASmallerThanB);
}

void RandomManager::calculateScores()
{
	for each (map* m in maps)
	{
		m->evaluateItems();
	}
}

void RandomManager::printScores()
{
	int n = 1;
	for each (map* m in maps)
	{
		std::cout << "Score do mapa " << n++ << ":" << m->getScore() << std::endl;
	}
}

void RandomManager::getMainPaths()
{
	maps.front()->findMainPath();
	for each (map* m in maps)
	{
		m->setMainPath(maps.front()->getMainPath());
	}
}

RandomManager::RandomManager(ALLEGRO_BITMAP * mapImage, int chanceForItemToSpawnInARoom)
{
	this->mapImage = mapImage;
	this->chanceForItemToSpawnInARoom = chanceForItemToSpawnInARoom;
}

RandomManager::~RandomManager()
{
}
