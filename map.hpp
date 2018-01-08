#ifndef MAP_HPP
#define MAP_HPP

#include <vector>

class Mob;

class Map{
public:
	Map(int width, int height);
	
	int get_width()		const;
	int get_height()	const;
	
	std::vector<Mob*> *get_mobs();
	
private:
	int width_;
	int height_;
	
	std::vector<Mob*> mobs_;
};

#endif