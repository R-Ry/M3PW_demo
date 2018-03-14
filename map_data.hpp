#ifndef MAP_DATA_HPP
#define MAP_DATA_HPP

#include <string>
#include <vector>
#include <iosfwd>

struct MapMobObject{
	int mob_type_;
	
	int x_, y_;
};

struct MapObject{
	int x_, y_;
	int width_, height_;
	//any other flag info
	int object_type_;
	
	//the sprite that the object uses,
	//starting with the index number of
	//the sprite in the mapdata file,
	//then the position and dimensions of
	//the sprite clip.
	
	int sprite_number_;
	int sprite_x_, sprite_y_;
	int sprite_width_, sprite_height_;
};

class MapData{
public:
	MapData();
	~MapData();
	
	//empty the arrays, set int values to 0, set name to "Untitled Map"
	void reset();
	
	//file i/o with standard file streams.
	std::ofstream& write_to_file(std::ofstream& out);
	std::ifstream& read_from_file(std::ifstream& in);
	
	std::string name_;
	int width_, height_;
	
	//spawn rate in terms of how many seconds the number of mobs
	//in the map would respawn by 50%
	//A higher number gives a lower spawn rate
	//15 would mean that half the dead mobs would respawn every 15 seconds
	int max_num_mobs_;
	int spawn_rate_;
	
	std::vector<MapMobObject> map_mobs_;
	std::vector<MapObject> map_objects_;
	std::vector<std::string> sprite_filenames_;
};

#endif