#include "framerate.hpp"
#include "map.hpp"
#include "mob.hpp"
#include "player.hpp"
#include "rand.hpp"

#include <stdio.h>


Mob::Mob(Map* m, float x_position, float y_position):
hp_(0),
max_hp_(0),
exp_(0),
strength_(0),
speed_(0),
target_(0),
position_(x_position, y_position),
size_(0, 0),
direction_(DIRECTION_RIGHT),
mob_state_(MOBSTATE_STANDING),
time_delayed_until_(0),
map_(m),
sprite_(0)
{
}

void Mob::take_damage(int damage, Player *attacker){
	target_ = attacker;
	
	mob_state_ = MOBSTATE_MOVING;
	
	hp_ -= damage;
	
	if(hp_ < 1){
		hp_ = 0;
		
		if(target_){
			target_->add_exp(exp_);
		}
	}
}

/*when the mob is not aggro'd by player, doing its normal business*/
void Mob::control(){
	/*	the mob will more likely tend to be in a standing position_.
		4/5 chance per second to stay standing still,
		1/5 chance per second to change direction_ while standing and moving
		3/5 chance per second to keep moving,
	*/
	if(time_delayed_until_ > get_current_frame()){
		return;
	}

	switch(mob_state_){
	case MOBSTATE_STANDING:
		{
			int chance = random_number() % (5 * FRAMES_PER_SEC);
			if(chance == 0){
				/*start moving*/
				mob_state_ = MOBSTATE_MOVING;
			}
			if(chance == 1){
				/*change direction_*/
				int new_direction = random_number() % (DIRECTION_COUNT);
				
				direction_ = (Direction)(new_direction);
			}
		}
		break;
	case MOBSTATE_MOVING:
		/*first begin by changing the position_*/
		switch(direction_){
		case DIRECTION_UP:
			position_.y_ -= speed_ / FRAMES_PER_SEC;
			
			/* switch direction if hit a wall*/
			if(position_.y_ < 0){
				position_.y_ = -position_.y_;
				direction_ = DIRECTION_DOWN;
			}
			break;
		case DIRECTION_DOWN:
			position_.y_ += speed_ / FRAMES_PER_SEC;
			
			if(position_.y_ >= map_->get_width()){
				position_.y_ -= speed_ / FRAMES_PER_SEC;
				direction_ = DIRECTION_UP;
			}
			break;
		case DIRECTION_LEFT:
			position_.x_ -= speed_ / FRAMES_PER_SEC;
			
			if(position_.x_ < 0){
				position_.x_ = -position_.x_ / FRAMES_PER_SEC;
				direction_ = DIRECTION_RIGHT;
			}
			break;
		case DIRECTION_RIGHT:
			position_.x_ += speed_ / FRAMES_PER_SEC;
			
			if(position_.x_ >= map_->get_width()){
				position_.x_ -= speed_ / FRAMES_PER_SEC;
				direction_ = DIRECTION_LEFT;
			}
			break;
		}
		/*then there's the chance the mob will stop moving or change direction_.*/
		{
			int chance = random_number() % (5 * FRAMES_PER_SEC);
			if(chance > -1 && chance < 2){
				/*stop moving*/
				mob_state_ = MOBSTATE_STANDING;
			}
			if(chance == 2){
				/*change direction_*/
				int new_direction = random_number() % (DIRECTION_COUNT);
				
				direction_ = (Direction)(new_direction);
			}
		}
		break;
	}
}		

bool Mob::is_dead(){
	return hp_ < 1;
}

bool Mob::has_target(){
	return target_;
}

Vector Mob::get_position(){
	return position_;
}

Vector Mob::get_size(){
	return size_;
}