#include "room.h"
#include <list>
#include <allegro5\allegro.h>
#include "extraFunctions.h"
#include "defines.h"
#include <iostream>
#include <random>

void room::addTile(pos2D newTilePos)
{
	tilePos.emplace_back(newTilePos);
}

void room::addDoor(pos2D door, pos2D previousTile)
{
	doors.emplace_back(door);
	previousTileToDoor.emplace_back(previousTile);
}

void room::addAdjRoom(room * newRoom)
{
	adjRooms.emplace_back(newRoom);
}

bool room::isRoomAdjacent(room * newRoom)
{
	for (std::list<room*>::iterator i = adjRooms.begin(); i != adjRooms.end(); ++i)
	{
		if (newRoom == (*i))
		{
			return true;
		}
	}
	return false;
}

bool room::isTilePartOfRoom(pos2D tile)
{
	for (std::list<pos2D>::iterator i = tilePos.begin(); i != tilePos.end(); ++i)
	{
		if ((*i) == tile)
			return true;
	}
	return false;
}

void room::findFullRoom(ALLEGRO_BITMAP *bm)
{
	//findFullRoom encontra os tiles da sala, a partir do tile inicial.
	int width = al_get_bitmap_width(bm);
	int height = al_get_bitmap_height(bm);

	std::list<pos2D> open;//Lista para guardar lugares não visitados
	std::list<pos2D> temp;

	open.emplace_back((*tilePos.begin()));
	while (!open.empty())
	{
		temp = open.front().get4WayAdjacentPositions();
		while(!temp.empty())
		{
			if (areTwoColorsTheSame(al_get_pixel(bitmap, temp.front().x, temp.front().y), COLORS::WHITE()))
			{
				if (!isTilePartOfRoom(temp.front()))
				{
					open.emplace_back(temp.front());
					tilePos.emplace_back(temp.front());
					roomSize++;
				}
			}
			temp.pop_front();
		}
		open.pop_front();
	}
}

void room::drawMap(ALLEGRO_BITMAP * bm, ALLEGRO_COLOR color)
{
	for (std::list<pos2D>::iterator i = tilePos.begin(); i != tilePos.end(); ++i)
	{
		al_draw_pixel((*i).x, (*i).y, color);
	}
}

int room::getItemChance()
{
	return itemChance;
}

void room::changeItemChance(int changeValue)
{
	itemChance += changeValue;
	if (itemChance < 0)
		itemChance = 0;
	else if (itemChance > 100)
		itemChance = 100;
}

pos2D room::getRandomTile()
{
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<> dist(0, tilePos.size()-1);
	std::list<pos2D>::iterator it;
	it = tilePos.begin();
	for (int i = 0; i < dist(eng); i++)
	{
		++it;
	}
	return (*it);
}

pos2D room::getFirstTile()
{
	return tilePos.front();
}

std::list<room*> room::getAdjRooms()
{
	return adjRooms;
}

void room::resetDistance()
{
	roomDistance = -1;
}

void room::setDistance(int d)
{
	if (d >= 0)
	{
		roomDistance = d;
	}
}

int room::getDistance()
{
	return roomDistance;
}

void room::setParent(room * r)
{
	parent = r;
}

void room::resetParent()
{

	parent = nullptr;
}

room * room::getParent()
{
	return parent;
}


room::room(ALLEGRO_BITMAP * btmp, pos2D firstRoom, int newItemChance)
{
	bitmap = btmp;
	tilePos.emplace_back(firstRoom);
	roomSize = 1;
	itemChance = newItemChance;
	findFullRoom(bitmap);
	parent = nullptr;
	roomDistance = -1;
}

room::~room()
{
}
