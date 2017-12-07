#include "RandomManager.h"
#include <string>


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

void RandomManager::defineNumberOfItems()
{
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
	map::setEndRoom(firstMap->getRandomRoom(firstMap->getMaxDistance()-1));
	//getMainPaths();
	////firstMap->findMainPath();
	//firstMap->resetRoomDistances();
	//firstMap->checkRoomsDistancesToMainPath();
	
	for (int i = 1; i < numberOfMaps; i++)
	{
		m = new map(mapImage, chanceForItemToSpawnInARoom);
		m->populateRooms(firstMap->getRoomList());
		maps.emplace_back(m);
	}
	getMainPaths();
	//firstMap->findMainPath();
	firstMap->resetRoomDistances();
	firstMap->checkRoomsDistancesToMainPath();
}

void RandomManager::addItemFromFileName(const char * filePath, int spawnChance, float distanceMultiplier)
{
	ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_set_path_filename(path, filePath);
	items.emplace_back(new item(al_load_bitmap(al_path_cstr(path, '/')), spawnChance, 0, distanceMultiplier));
}

void RandomManager::addItemFromFileName(const char * filePath, int spawnChance, int predefinedNumber, float distanceMultiplier)
{
	ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_set_path_filename(path, filePath);
	items.emplace_back(new item(al_load_bitmap(al_path_cstr(path, '/')), spawnChance, predefinedNumber,distanceMultiplier));
}

void RandomManager::saveMapImages(const char* path)
{
	int i = 1;
	std::string strPath;
	for each (map* m in maps)
	{
		strPath = path;
		strPath+=std::to_string(i);
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
