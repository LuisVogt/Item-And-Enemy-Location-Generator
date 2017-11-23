#include <list>

#pragma once

struct pos2D
{
	int x;
	int y;

	pos2D() {}
	pos2D(float x, float y) : x(x), y(y) {}
	pos2D operator * (int c) const {
		return pos2D(x*c, y*c);
	}
	pos2D operator + (pos2D vectr) const {
		return pos2D(x + vectr.x, y + vectr.y);
	}
	pos2D operator - (pos2D vectr) const {
		return pos2D(x - vectr.x, y - vectr.y);
	}
	bool operator == (pos2D pos) const {
		return (x == pos.x&&y == pos.y);
	}

	std::list<pos2D> getAdjacentPositions()
	{
		std::list<pos2D> listaAdj;
		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				if (!(0 < x + i || 0 < y + i || (x == 0 && y == 0)))
				{
					listaAdj.emplace_back(pos2D(x + i, y + j));
				}
			}
		}
		return listaAdj;
	}

	std::list<pos2D> get4WayAdjacentPositions()
	{
		std::list<pos2D> listaAdj;
		listaAdj.emplace_back(pos2D(x, y + 1));
		listaAdj.emplace_back(pos2D(x, y - 1));
		listaAdj.emplace_back(pos2D(x + 1, y));
		listaAdj.emplace_back(pos2D(x - 1, y));
		return listaAdj;
	}

	float distanceToPoint(pos2D p)
	{
		float tempX = abs(p.x - x);
		float tempY = abs(p.y - y);
		float tempSquare = tempX * tempX + tempY * tempY;
		return sqrt(tempSquare);
	}

	static pos2D UP()    { return pos2D(0, -1);}
	static pos2D DOWN()  { return pos2D(0, 1); }
	static pos2D LEFT()  { return pos2D(-1, 0);}
	static pos2D RIGHT() { return pos2D(1, 0); }

};