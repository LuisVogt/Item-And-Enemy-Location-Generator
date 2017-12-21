#include "extraFunctions.h"
#include <random>
#include <iostream>

#pragma once

bool areTwoColorsTheSame(ALLEGRO_COLOR first, ALLEGRO_COLOR second)
{
	unsigned char firstR;
	unsigned char firstG;
	unsigned char firstB;
	unsigned char secondR;
	unsigned char secondG;
	unsigned char secondB;
	al_unmap_rgb(first, &firstR, &firstG, &firstB);
	al_unmap_rgb(second, &secondR, &secondG, &secondB);
	bool r = firstR == secondR;
	bool g = firstG == secondG;
	bool b = firstB == secondB;
	return r && b && g;
}

bool isPos2DonList(std::list<pos2D>& list, pos2D pos)
{
	if (list.empty())
	{
		return false;
	}
	for (std::list<pos2D>::iterator i = list.begin(); i != list.end(); ++i)
	{
		if ((*i) == pos)
			return true;
	}
	return false;
}

int getDistanceToFirstRoom(room* r)
{
	if (r->getDistance() > 0)
	{
		return r->getDistance();
	}
	else
	{
		room* temp = r;
		int distancia = 0;
		while (temp->getParent() != nullptr)
		{
			temp = temp->getParent();
			++distancia;
			if (distancia > 100)
			{
				std::cout << "Agarrou em:" << temp << " e " << temp->getParent() << std::endl;
				printAdjRooms(temp);
				printAdjRooms(temp->getParent());
			}
		}
		r->setDistance(distancia);
		return distancia;
	}
}

std::list<room*> Djikstra(room * init, room * end)
{

	std::list<room*> open;
	std::list<room*> closed;
	std::list<room*> path;
	std::list<room*> tempAdjRooms;
	open.emplace_back(init);

	room* tempRoom = open.front();

	int tempDistance = 0;
	while (!open.empty())
	{
		tempRoom = open.front();
		open.pop_front();
		closed.emplace_back(tempRoom);

		if (tempRoom == end)
		{
			path = getRoomPath(end);
			//path.emplace_front(init);
			for each (room* r in closed)
			{
				r->resetParent();
			}
			return path;
		}

		tempAdjRooms = tempRoom->getAdjRooms();
		tempDistance = getDistanceToFirstRoom(tempRoom);
		for each (room* r in tempAdjRooms)
		{
			if (isRoomOnList(closed, r))
				continue;
			if (tempDistance + 1 < getDistanceToFirstRoom(r)
				|| !isRoomOnList(open, r))
			{
				r->setParent(tempRoom);
				if (!isRoomOnList(open, r))
					insertInOrder(open, r);
			}
		}
	}
	return path;
}

std::list<room*> getRoomPath(room * r)
{
	std::list<room*> roomPath;
	room* room = r;
	while (room->getParent() != nullptr)
	{
		roomPath.emplace_front(room);
		room = room->getParent();
	}
	return roomPath;
}

int getRandomNumberInRange(int begining,int end)
{
	std::random_device rd;
	//std::mt19937 eng(rd());
	std::uniform_int_distribution<> roomRandom(begining, end);
	return roomRandom(rd);
}

float getRandomPercentage()
{
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_real_distribution<> randomNumber(0, 1);

	return randomNumber(eng);
}

float getRandomNumberInRange(float begining, float end)
{
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_real_distribution<> randomNumber(begining, end);
	return randomNumber(eng);
}

void insertInOrder(std::list<room*>& rList, room* rm)
{
	int distance = rm->getDistance();

	for (std::list<room*>::iterator i = rList.begin(); i != rList.end(); ++i)
	{
		if ((*i)->getDistance() < distance)
		{
			rList.emplace(i, rm);
			return;
		}
	}
	rList.emplace_back(rm);
}

void printRoomAdresses(std::list<room*>& rList)
{
	int i = 1;
	for each (room* r in rList)
	{
		std::cout << "Sala " << i++ << ": " << r << std::endl;
	}
}

void printAdjRooms(room * rm)
{
	std::cout << "Vizinhos de " << rm << ":" << std::endl;
	for each (room* r in rm->getAdjRooms())
	{
		std::cout << r << std::endl;
	}
	std::cout << "--------" << std::endl;
}

bool isRoomOnList(std::list<room*> l, room * r)
{
	std::list<room*>::iterator i;
	i = std::find(l.begin(), l.end(), r);
	if (i != l.end())
		return true;
	return false;
}
