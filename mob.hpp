#ifndef MOB_HPP
#define MOB_HPP

#include "constants.hpp"
#include "vector.hpp"

class Map;
class Player;
class ScreenLayer;
class Sprite;

enum MobState{
	MOBSTATE_STANDING, MOBSTATE_MOVING, MOBSTATE_ATTACKING, MOBSTATE_RECOIL
};

struct MobStateInfo{
	MobState mob_state;
	union{
		int last_attack;
		int recoil_start;
	};
};

class Mob{
public:
	Mob(Map *m, float x_position, float y_position);
	
	void take_damage(int damage, Player *attacker);
	virtual void control_normal_movement();
	virtual void control_aggro_movement() = 0;
	virtual void draw(ScreenLayer* mob_layer) = 0;
	
	bool is_dead();
	bool has_target();
	Vector get_position();
	Vector get_size();
	
protected:
	int hp_;
	int max_hp_;
	int exp_;
	int strength_;
	int speed_;
	Player *target_;
	Vector position_;
	Vector size_;
	Direction direction_; /*direction the mob is facing*/
	MobStateInfo mob_state_info_; /*whether the mob is moving or not*/
	Map *map_;
	Sprite *sprite_;
};

#endif