#ifndef MOB_DRAGON_HPP
#define MOB_DRAGON_HPP

#include "mob.hpp"

class Map;
class ScreenLayer;

static const int kDragonWidth = 900, kDragonHeight = 900;

class Dragon: public Mob{
public:
	Dragon(Map *m, int x_position, int y_position);
	virtual void control();
	virtual void draw(ScreenLayer *mob_layer);
private:
	bool is_target_hit_;
	Vector target_hit_position_;
};

#endif