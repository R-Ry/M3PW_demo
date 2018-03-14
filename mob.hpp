#ifndef MOB_HPP
#define MOB_HPP

#include "constants.hpp"
#include "vector.hpp"

#include <vector>

class Map;
class Player;
class ScreenLayer;
class Sprite;

enum MobType{
	MOB_DRAGON
};

enum MobState{
	MOBSTATE_STANDING, MOBSTATE_MOVING, MOBSTATE_ATTACKING, MOBSTATE_HIT, MOBSTATE_DYING
};

/*for the action queue*/
enum MobAction{
	MOB_ACTION_NONE, MOB_ACTION_MOVE, MOB_ACTION_ATTACK, MOB_ACTION_HIT /*the mob is being hit, not hitting a target*/,
	MOB_ACTION_DIE
};

/*for the action queue*/
struct MobActionInfo{
	MobAction action_;
	int time_start_;
	int stage_;
	bool is_finished_;
	
	/*for action_ == MOB_ACTION_HIT*/
	int distance_push_back_;
};

class Mob{
public:
	Mob(Map *m, float x_position, float y_position);
	
	/*sets Player target_ to attacker if not null. Attacker gains
	 *experience if damage is greater than hp_*/
	void take_damage(int damage, Player *attacker);
	
	/*control() updates the mob and controls it depending on its current state.
	 *
	 *to be called once each game frame*/
	virtual void control();

	/*restores the mob back to original state*/
	//virtual void reset() = 0;

	/*pushes the mob back and starts the dying animation if hp < 0
	 *should work as part of an action queue, with a value for
	 *the distance to move back (the induced speed is uniform and 
	 *the same for all cases)
	 */
	//void hit(int distance, Direction direction);
	
	/*call each game frame to update the mob layer of the display*/
	virtual void draw(ScreenLayer* mob_layer) = 0;
	
	/*returns true if hp == 0 or hp < 0*/
	bool is_dead();
	
	/*returns true if Player target_ is assigned a player address*/
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
	MobState mob_state_;
	int time_delayed_until_;
	Map *map_;
	Sprite *sprite_;
};

#endif