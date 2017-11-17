#include "structs.h"
#include <list>
#include <allegro5\allegro.h>

#pragma once

class room
{
private:
	std::list<pos2D> tilePos;
	std::list<room*> adjRooms;
	std::list<pos2D> doors;
	std::list<pos2D> previousTileToDoor;
	ALLEGRO_BITMAP *bitmap;
	int itemChance;
	int roomSize;
public:
	void addTile(pos2D newTilePos);
	void addDoor(pos2D door, pos2D previousTile);
	void addAdjRoom(room *newRoom);
	bool isRoomAdjacent(room *newRoom);
	bool isTilePartOfRoom(pos2D tile);
	void findFullRoom(ALLEGRO_BITMAP *bm);
	void drawMap(ALLEGRO_BITMAP *bm, ALLEGRO_COLOR color);
	int getItemChance();
	void changeItemChance(int changeValue);
	pos2D getRandomTile();
	std::list<room*> getAdjRooms();

	room(ALLEGRO_BITMAP* btmp, pos2D firstRoom, int newItemChance);
	~room();
};
