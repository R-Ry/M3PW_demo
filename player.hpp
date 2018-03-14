#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "constants.hpp"
#include "player_state_info.hpp"
#include "vector.hpp"

class Map;
class ScreenLayer;
class Sprite;

enum PlayerControlAction{
	PLAYER_ACTION_NONE, PLAYER_ACTION_ATTACK_NORMAL_MELEE, PLAYER_ACTION_ATTACK_NORMAL_RANGED,
	PLAYER_ACTION_UP, PLAYER_ACTION_DOWN, PLAYER_ACTION_LEFT, PLAYER_ACTION_RIGHT,
	PLAYER_ACTION_ATTACK_HAIL
};

struct PlayerActionInfo{
	PlayerControlAction action_;
	int start_frame_;
	//stage_ is used to keep track of what part of the action is currently in progress
	int stage_;
	bool is_finished_;
	
	
	//for movement(nothing)
	//all attacks
	
	/*the player's midpoint coordinates*/
	Vector player_position_;
	int direction_;
	/*number of mobs that the player hits*/
	int num_mobs_;
	/*number of hits per mob hit*/
	int num_hits_;
	/*the targets' midpoint coordinates*/
	Vector target_position_[12];
	int damages_[12];
};

static const int kPlayerWidth = 300, kPlayerHeight = 300;

class Player{
public:
	Player();
	
	/*set the player's map....
	 *--->the map must also be given information about the player being in it<----*/
	void set_map(Map *map);
	//------>remove set_position (not part of the final interface)<-------------
	void set_position(Vector p){position_ = p;}
	Map *get_map();
	Vector get_position();
	Vector get_size();
	
	/*returns true if player's health is less than or equal to 0*/
	bool is_dead();
	
	/*takes damage and deducts exp if hp is reduced to less than or equal to 0*/
	void take_damage(int damage);
	
	/*adds exp to current exp and carries it to the next level if a level is gained*/
	void add_exp(int exp);
	
	void update(PlayerControlAction action = PLAYER_ACTION_NONE);
	void draw(ScreenLayer *player_layer);
	
private:
	/*whatever actions made by the user might have been put into the queue,
	 *in order to give the player the capability to do more than one action
	 *at a time.
	 *
	 *update_queue() performs any player action in the queue based on the current
	 *time (frame). update_queue() is called after the player is updated based
	 *on its initial state in the update() function.*/
	void update_queue();

	/*draw_queue() is called after the main switch statement in draw(). Most actions
	 *in the queue end after a certain animation is displayed for an action*/
	void draw_queue(ScreenLayer *player_layer);
	
	Vector position_;
	Vector size_;
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
	std::vector<PlayerActionInfo> queue_;
	Sprite* sprite_;
};

#endif