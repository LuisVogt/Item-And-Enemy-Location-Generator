#include<allegro5\allegro.h>
#include <list>
#include "structs.h"

#pragma once

bool areTwoColorsTheSame(ALLEGRO_COLOR a, ALLEGRO_COLOR b);
bool isPos2DonList(std::list<pos2D> &list, pos2D pos);