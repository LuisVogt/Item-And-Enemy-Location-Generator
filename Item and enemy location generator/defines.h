#include <allegro5\allegro.h>

#pragma once
namespace COLORS {
	static const ALLEGRO_COLOR RED()   { return al_map_rgb(255, 0, 0); }
	static const ALLEGRO_COLOR WHITE() { return al_map_rgb(255, 255, 255); }
	static const ALLEGRO_COLOR GREEN() { return al_map_rgb(0, 255, 0); }
	static const ALLEGRO_COLOR BLACK() { return al_map_rgb(0, 0, 0); }
	static const ALLEGRO_COLOR GREY()  { return al_map_rgb(127, 127, 127); }
}