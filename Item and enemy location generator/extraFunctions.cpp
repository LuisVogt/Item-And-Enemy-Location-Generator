#include "extraFunctions.h"

bool areTwoColorsTheSame(ALLEGRO_COLOR first, ALLEGRO_COLOR second)
{
	unsigned char firstR;
	unsigned char firstG;
	unsigned char firstB;
	unsigned char secondR;
	unsigned char secondG;
	unsigned char secondB;
	al_unmap_rgb(first,  &firstR,  &firstG,  &firstB);
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
