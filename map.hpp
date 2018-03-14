#ifndef MAP_HPP
#define MAP_HPP

#include <string>
#include <vector>

class MapData;
class Mob;

class Map{
public:
	Map(int width, int height);
	Map(MapData *m);
	~Map();
	
	int get_width()		const;
	int get_height()	const;
	
	std::vector<Mob*> *get_mobs();
	
private:
	std::string name_;
	int width_;
	int height_;
	
	std::vector<Mob*> mobs_;
};

#endif