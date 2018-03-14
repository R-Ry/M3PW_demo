#include <fstream>
#include <string.h>

#include "map_data.hpp"

MapData::MapData():
name_("Untitled Map"),
width_(0),
height_(0),
max_num_mobs_(0),
spawn_rate_(0),
map_mobs_(),
map_objects_()
{
}

MapData::~MapData(){
}

void MapData::reset(){
	name_ = "Untitled Map";
	width_ = 0;
	height_ = 0;
	max_num_mobs_ = 0;
	spawn_rate_ = 0;
	map_mobs_.clear();
	map_objects_.clear();
	sprite_filenames_.clear();
}

std::ofstream& MapData::write_to_file(std::ofstream& out){
	int name_strlen = strlen(name_.c_str()) + 1;
	out.write((char*)(&name_strlen), sizeof(int));
	out.write(name_.c_str(), name_strlen);
	out.write((char*)(&width_), sizeof(int));
	out.write((char*)(&height_), sizeof(int));
	out.write((char*)(&max_num_mobs_), sizeof(int));
	out.write((char*)(&spawn_rate_), sizeof(int));
	
	int num_mobs = map_mobs_.size();
	out.write((char*)(&num_mobs), sizeof(int));
	for(int i = 0; i < num_mobs; ++i){
		out.write((char*)(&map_mobs_[i]), sizeof(MapMobObject));
	}
	
	int num_objects = map_objects_.size();
	out.write((char*)(&num_objects), sizeof(int));
	for(int i = 0; i < num_objects; ++i){
		out.write((char*)(&map_objects_[i]), sizeof(MapObject));
	}
	
	int num_spritefiles = sprite_filenames_.size();
	out.write((char*)(&num_spritefiles), sizeof(int));
	for(int i = 0; i < num_spritefiles; ++i){
		int filename_strlen = strlen(sprite_filenames_[i].c_str()) + 1;
		out.write((char*)(&filename_strlen), sizeof(int));
		out.write(sprite_filenames_[i].c_str(), filename_strlen);
	}
	
	return out;
}

std::ifstream& MapData::read_from_file(std::ifstream& in){
	char strbuf[80];
	
	int name_strlen;
	in.read((char*)(&name_strlen), sizeof(int));
	in.read(strbuf, name_strlen);
	name_ = strbuf;
	in.read((char*)(&width_), sizeof(int));
	in.read((char*)(&height_), sizeof(int));
	in.read((char*)(&max_num_mobs_), sizeof(int));
	in.read((char*)(&spawn_rate_), sizeof(int));
	
	int num_mobs;
	in.read((char*)(&num_mobs), sizeof(int));
	for(int i = 0; i < num_mobs; ++i){
		MapMobObject buf;
		in.read((char*)(&buf), sizeof(MapMobObject));
		map_mobs_.push_back(buf);
	}
	
	int num_objects;
	in.read((char*)(&num_objects), sizeof(int));
	for(int i = 0; i < num_objects; ++i){
		MapObject buf;
		in.read((char*)(&buf), sizeof(MapObject));
		map_objects_.push_back(buf);
	}
	
	int num_spritefiles;
	in.read((char*)(&num_spritefiles), sizeof(int));
	for(int i = 0; i < num_spritefiles; ++i){
		int filename_strlen;
		in.read((char*)(&filename_strlen), sizeof(int));
		in.read(strbuf, filename_strlen);
		sprite_filenames_.push_back(strbuf);
	}
	
	return in;
}