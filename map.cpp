#include "map.hpp"

Map::Map(int width, int height):
width_(width),
height_(height)
{
}

int Map::get_width() const{
	return width_;
}

int Map::get_height() const{
	return height_;
}

std::vector<Mob*> *Map::get_mobs(){
	return &mobs_;
}