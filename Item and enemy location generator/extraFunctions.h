#include<allegro5\allegro.h>
#include <list>
#include "structs.h"
#include "room.h"

#pragma once

bool areTwoColorsTheSame(ALLEGRO_COLOR a, ALLEGRO_COLOR b);
bool isPos2DonList(std::list<pos2D> &list, pos2D pos);
int getDistanceToFirstRoom(room* r);
std::list<room*> Djikstra(room * init, room * end);
std::list<room*> getRoomPath(room* r);
int getRandomNumberInRange(int begining, int end);
float getRandomPercentage();
float getRandomNumberInRange(float begining, float end);
bool isRoomOnList(std::list<room*> l, room* r);
void insertInOrder(std::list<room*>& rList, room* rm);
void printRoomAdresses(std::list<room*>& rList);
void printAdjRooms(room* rm);