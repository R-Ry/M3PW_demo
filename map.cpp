#include "dragon.hpp"
#include "map.hpp"
#include "map_data.hpp"

Map::Map(int width, int height):
width_(width),
height_(height)
{
}

Map::Map(MapData *m):
name_(m->name_),
width_(m->width_),
height_(m->height_)
{
	for(int i = 0; i < m->map_mobs_.size(); ++i){
		switch(m->map_mobs_[i].mob_type_){
		case 0:
			mobs_.push_back(new Dragon(this, m->map_mobs_[i].x_, m->map_mobs_[i].y_));
			break;
		}
	}
}

Map::~Map(){
	for(int i = 0; i < mobs_.size(); ++i){
		delete mobs_[i];
	}
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