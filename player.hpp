#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "constants.hpp"
#include "vector.hpp"
#include <vector>

class Map;
class ScreenLayer;
class Sprite;

enum PlayerControlAction{
	PLAYER_ACTION_NONE, PLAYER_ACTION_ATTACK, PLAYER_ACTION_UP, PLAYER_ACTION_DOWN,
	PLAYER_ACTION_LEFT, PLAYER_ACTION_RIGHT
};

enum PlayerState{
	PLAYER_STATE_STANDING, PLAYER_STATE_MOVING, PLAYER_STATE_ATTACKING
};

typedef struct PlayerStateInfo{
	PlayerState player_state_;
	union{
		struct{
			int time_of_attack_;
			int mob_hit_positions_[6 * 2];
			int mob_hit_damages_[6 * 6];
		};
	};
} PlayerStateInfo;

static const int kPlayerWidth = 300, kPlayerHeight = 300;

class Player{
public:
	Player();
	
	void set_map(Map *map);
	Map *get_map();
	Vector get_position();
	bool is_dead();
	
	void take_damage(int damage);
	void add_exp(int exp);
	
	void update(PlayerControlAction action = PLAYER_ACTION_NONE);
	void draw(ScreenLayer *player_layer);
private:
	Vector position_;
	Direction direction_;
	Map *map_;
	int level_;
	int hp_;
	int max_hp_;
	int exp_;
	int max_exp_;
	int strength_;
	int speed_;
	
	PlayerStateInfo player_state_info_;
	Sprite* sprite_;
};

#endif