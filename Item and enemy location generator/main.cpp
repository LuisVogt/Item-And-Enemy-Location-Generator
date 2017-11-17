#include <iostream>
#include <fstream>
#include <string>
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_image.h>
#include "defines.h"
#include "extraFunctions.h"
#include "room.h"
#include "map.h"

const int TILE_SIZE = 1;
const int MAP_SIZE = 50; //Define tamanho do mapa, mudar depois para achar dinamicamente.

using namespace std;

void drawBitmapMap(int i, int j, char color)
{
	//Desenha o mapa no bitmap ativo.
	ALLEGRO_COLOR c;
	switch (color)
	{
	case '0':
		c = COLORS::BLACK();
		break;
	case '1':
		c = COLORS::WHITE();
		break;
	case '2':
		c = COLORS::GREY();
		break;
	case '3':
		c = COLORS::GREEN();
		break;
	default:
		c = COLORS::RED();
		break;
	}
	float x1 = i*(float)TILE_SIZE;
float y1 = j*(float)TILE_SIZE;
float x2 = x1 + TILE_SIZE;
float y2 = y1 + TILE_SIZE;
al_draw_filled_rectangle(x1, y1, x2, y2, c);
}

void readMap()
{
	std::ifstream terrainFile;
	terrainFile.open("map.txt");
	std::string output;
	int line = 0;
	if (terrainFile.is_open())
	{
		while (!terrainFile.eof())
		{
			terrainFile >> output;
			for (signed int i = 0; i < output.length(); i++)
			{
				drawBitmapMap(i, line, output[i]);
			}
			line++;
		}
	}
	terrainFile.close();
}

int main(int argc, char **argv)
{
	const char *save = "teste.png";
	const char *save2 = "teste2.png";
	const char *item1 = "beringela.png";
	const char *item2 = "faca.png";
	const char *item3 = "escudo.png";

	int size = MAP_SIZE * TILE_SIZE;
	ALLEGRO_BITMAP *bitmap;
	ALLEGRO_BITMAP *secondMap;

	list<item> items;
	items.emplace_back(item(al_load_bitmap(item1), 3, pos2D(0, 0)));
	items.emplace_back(item(al_load_bitmap(item2), 2, pos2D(0, 0)));
	items.emplace_back(item(al_load_bitmap(item3), 1, pos2D(0, 0)));

	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	al_init_image_addon();
	al_init_primitives_addon();
	bitmap = al_create_bitmap(size, size);
	secondMap = al_create_bitmap(size, size);
	al_set_target_bitmap(bitmap);
	al_clear_to_color(al_map_rgb(0, 0, 255));
	readMap();
	if (!al_save_bitmap(save, bitmap))
	{
		fprintf(stderr, "failed to save image!\n");
		return -1;
	}
	al_set_target_bitmap(secondMap);
	al_clear_to_color(al_map_rgb(0, 0, 0));


	map* Map = new map(bitmap,70);


	Map->populateRooms();
	Map->drawRoomsWithRandomColors(secondMap);
	if (!al_save_bitmap(save2, secondMap))
	{
		fprintf(stderr, "failed to save image!\n");
		return -1;
	}
	//Map->populateItems(items, bitmap);
	Map->saveLargerMapWithItems(48);
	al_rest(3);
	
}