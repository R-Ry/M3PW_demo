#include "camera_offset.hpp"
#include "console_graphics.hpp"
#include "constants.hpp"
#include "damage_display.hpp"
#include "framerate.hpp"
#include "map.hpp"
#include "map_mob_selector.hpp"
#include "mob.hpp"
#include "player.hpp"
#include "rand.hpp"
#include "sprite_data_provider.hpp"

#include <stdio.h>

const int kPlayerExp[] = {
	50, 179, 491, 1057, 1897, 2998, 4335, 5886, 7637, 9580,
	11715, 14047, 16586, 19342, 22333, 25576, 29092, 32904, 37037, 41518,
	46377, 51647, 57363, 63563, 70290, 77586, 85502, 94090, 103406, 113512,
	124474, 136365, 149262, 163249, 178419, 194868, 212704, 232043, 253008, 275736,
	300372, 327074, 356013, 387375, 421359, 458181, 498076, 541296, 588116, 638832,
	693764, 753259, 817690, 887464, 963019, 1044828, 1133404, 1229301, 1333118, 1445504
};

Player::Player():
position_(300, 0),
size_(300, 300),
direction_(DIRECTION_DOWN),
map_(0),
level_(1),
hp_(30),
max_hp_(30),
exp_(0),
max_exp_(kPlayerExp[0]),
strength_(20),
speed_(300),
sprite_(SpriteDataProvider::get_instance()->get_sprite_by_name("sprites/player.txt"))
{
	player_state_info_.player_state_ = PLAYER_STATE_STANDING;
	player_state_info_.time_delayed_until_ = 0;
}

void Player::set_map(Map *map){
	map_ = map;
}

Map *Player::get_map(){
	return map_;
}

Vector Player::get_position(){
	return position_;
}

Vector Player::get_size(){
	return size_;
}

bool Player::is_dead(){
	return hp_ < 1;
}
	
void Player::take_damage(int damage){
	hp_ -= damage;
	
	/*the player loses 10% experience if the amount
	 *of damage hit reduces the hp to less than or 
	 *equal to 0*/
	if(hp_ < 1){
		hp_ = 0;
		exp_ -= (max_exp_ / 10);
		
		if(exp_ < 0){
			exp_ = 0;
		}
	}
}

void Player::add_exp(int exp){
	exp_ += exp;
	
	/*if exp_ reaches max_exp_, the player gains a level and
	 *the remaining experience is transferred to the next level*/
	if(exp_ >= max_exp_){
		exp_ -= max_exp_;
		level_++;
		
		/*max_exp_ gets its value from a constant array derived
		 *from a formula*/
		max_exp_ = kPlayerExp[level_ - 1];
	}
}

/*	How update() works, first the player is updated based on its current player_state
	
	THEN the player's state is updated according to the action given to the player so
	that in the next frame, the player will respond to the action.
 */

void Player::update(PlayerControlAction action){
	
	/* update player based on its state from the last frame*/
	switch(player_state_info_.player_state_){
	/*do nothing if the player is not moving*/
	case PLAYER_STATE_STANDING:
		break;
	/*update the position according to the last input*/
	case PLAYER_STATE_MOVING:
		switch(direction_){
		case DIRECTION_UP:
			position_.y_ -= speed_ / FRAMES_PER_SEC;
			/*if the player's top left point position is beyond the top left corner
			 *of the map, the player can't go up any further*/
			if(position_.y_ < 0){
				position_.y_ = 0;
			}
			break;
		case DIRECTION_DOWN:
			position_.y_ += speed_ / FRAMES_PER_SEC;
			/*if the player's top left point position is beyond the map height minus
			 *the player height, don't let the player move further in that direction*/
			if(position_.y_ > map_->get_height() - kPlayerHeight){
				position_.y_ = map_->get_height() - kPlayerHeight;
			}
			break;
		case DIRECTION_LEFT:
			position_.x_ -= speed_ / FRAMES_PER_SEC;
			if(position_.x_ < 0){
				position_.x_ = 0;
			}
			break;
		case DIRECTION_RIGHT:
			position_.x_ += speed_ / FRAMES_PER_SEC;
			if(position_.x_ > map_->get_width() - kPlayerWidth){
				position_.x_ = map_->get_width() - kPlayerWidth;
			}
			break;
		}
		
		/*set the player state to not moving, since the state should not stay moving automatically.
		 *HOWEVER, if the player just started moving, to compensate for the brief lack of input of
		 *console windows, the player state will remain moving for at least 24/30 seconds*/
		
		if( (player_state_info_.time_of_start_movement_ + FRAMES_PER_SEC * 24 / 30) < get_current_frame()){
			player_state_info_.player_state_ = PLAYER_STATE_STANDING;
		}
		break;
	}
	
	/*update the player according to what is currently in the queue.*/
	update_queue();

	/*Check the player action queue and remove any concurrent action that is finished*/
	if(queue_.size() > 0){
		std::vector<PlayerActionInfo> new_action_queue;
		for(int i = 0; i < queue_.size(); ++i){
			if(!queue_[i].is_finished_){
				new_action_queue.push_back(queue_[i]);
			}
		}
		queue_ = new_action_queue;
	}

	//------------------->Input section of update()<-----------------------------------
	//=================================================================================


	/* process keyboard input
	 *if some player action is in progress, input is not accepted*/
	if(player_state_info_.time_delayed_until_ > get_current_frame()){
		return;
	}
	
	switch(action){
	case PLAYER_ACTION_NONE:
		break;
	case PLAYER_ACTION_UP:
		/*if the player just started to move up, then record the time when the player started to move up
		 *so that the player will continue to move at least 24/30 frames after starting*/
		if(player_state_info_.player_state_ != PLAYER_STATE_MOVING){
			player_state_info_.time_of_start_movement_ = get_current_frame();
		}
		
		player_state_info_.player_state_ = PLAYER_STATE_MOVING;
		direction_ = DIRECTION_UP;
		break;
	case PLAYER_ACTION_DOWN:
		if(player_state_info_.player_state_ != PLAYER_STATE_MOVING){
			player_state_info_.time_of_start_movement_ = get_current_frame();
		}
		
		player_state_info_.player_state_ = PLAYER_STATE_MOVING;
		direction_ = DIRECTION_DOWN;
		break;
	case PLAYER_ACTION_LEFT:
		if(player_state_info_.player_state_ != PLAYER_STATE_MOVING){
			player_state_info_.time_of_start_movement_ = get_current_frame();
		}
		
		player_state_info_.player_state_ = PLAYER_STATE_MOVING;
		direction_ = DIRECTION_LEFT;
		break;
	case PLAYER_ACTION_RIGHT:
		if(player_state_info_.player_state_ != PLAYER_STATE_MOVING){
			player_state_info_.time_of_start_movement_ = get_current_frame();
		}
		
		player_state_info_.player_state_ = PLAYER_STATE_MOVING;
		direction_ = DIRECTION_RIGHT;
		break;
	case PLAYER_ACTION_ATTACK_NORMAL_MELEE:
		{
			/*add all the needed information to the player action queue which will then use
			 *the information*/
			PlayerActionInfo info;
			info.action_ = PLAYER_ACTION_ATTACK_NORMAL_MELEE;
			info.start_frame_ = get_current_frame();
			info.stage_ = 0;
			info.is_finished_ = false;
			info.player_position_ = position_ + size_ / 2;
			info.direction_ = direction_;
			info.num_hits_ = 1;
			queue_.push_back(info);

			player_state_info_.player_state_ = PLAYER_STATE_ATTACKING;
			player_state_info_.time_delayed_until_ = get_current_frame() + FRAMES_PER_SEC * 0.7;
		}
		break;
	case PLAYER_ACTION_ATTACK_HAIL:
		{
			/*add all the needed information to the player action queue which will then use
			 *the information*/
			PlayerActionInfo info;
			info.action_ = PLAYER_ACTION_ATTACK_HAIL;
			info.start_frame_ = get_current_frame();
			info.stage_ = 0;
			info.is_finished_ = false;
			info.player_position_ = position_ + size_ / 2;
			info.direction_ = direction_;
			info.num_hits_ = 1;
			queue_.push_back(info);

			player_state_info_.player_state_ = PLAYER_STATE_ATTACKING;
			player_state_info_.time_delayed_until_ = get_current_frame() + FRAMES_PER_SEC * 3.0;
		}
		break;
	}
}

void Player::draw(ScreenLayer *player_layer){
	/*the draw function draws according to the player's state (standing/moving/attacking)*/
	
	Vector display_position = get_camera_offset(position_);
	
	switch(player_state_info_.player_state_){
	case PLAYER_STATE_ATTACKING:
	case PLAYER_STATE_STANDING:
	case PLAYER_STATE_MOVING:
		/*in all cases the player sprite is drawn on the screen
		 *
		 *parameters of draw_image_clip is the sprite, draw position,
		 *position from sprite to draw, and the dimensions of the sprite to draw
		 *
		 *refer to console_graphics.hpp for more info*/
		if(direction_ == DIRECTION_UP){
			player_layer->draw_image_clip(sprite_, display_position.x_, display_position.y_, 0, 0, 3, 3);
		}
		if(direction_ == DIRECTION_DOWN){
			player_layer->draw_image_clip(sprite_, display_position.x_, display_position.y_, 0, 3, 3, 3);
		}
		if(direction_ == DIRECTION_LEFT){
			player_layer->draw_image_clip(sprite_, display_position.x_, display_position.y_, 0, 6, 3, 3);
		}
		if(direction_ == DIRECTION_RIGHT){
			player_layer->draw_image_clip(sprite_, display_position.x_, display_position.y_, 0, 9, 3, 3);
		}
		break;
	}

	draw_queue(player_layer);
	
	/*draw the amount of player hp left of max hp and current exp of max exp on
	 *the bottom of the screen*/
	player_layer->draw_str("HP: ", 0, 49);
	player_layer->draw_int_right_end(hp_, 6, 49);
	player_layer->draw_str("/", 7, 49);
	player_layer->draw_int_left_end(max_hp_, 8, 49);
	
	player_layer->draw_str("EXP: ", 20, 49);
	player_layer->draw_int_right_end(exp_, 30, 49);
	player_layer->draw_str("/", 31, 49);
	player_layer->draw_int_left_end(max_exp_, 32, 49);
}

void Player::update_queue(){
	/*execute anything that the may be in progress in the action queue*/

	for(int i = 0; i < queue_.size(); ++i){

		int elapsed_frames = get_current_frame() - queue_[i].start_frame_;

		switch(queue_[i].action_){
		case PLAYER_ACTION_ATTACK_NORMAL_MELEE:
			/*all that must be done is pick which mob to hit*/
			if(queue_[i].stage_ == 0){
				std::vector<Mob*> mob_to_hit = get_near_mobs(this, 100, 1, direction_, 0.5);

				queue_[i].num_mobs_ = mob_to_hit.size();

				if(mob_to_hit.size() > 0){
					/*the damage hit is 50 ~ 100% of the strength of the player*/
					queue_[i].damages_[0] = strength_ / 2 + random_number() % (strength_ / 2);

					/*record where the mob is during the attack*/
					Vector mob_midpoint = mob_to_hit[0]->get_position() + mob_to_hit[0]->get_size() / 2;
					queue_[i].target_position_[0] = mob_midpoint;
					mob_to_hit[0]->take_damage(queue_[i].damages_[0], this);
				}

				queue_[i].stage_++;
			}
			break;
		case PLAYER_ACTION_ATTACK_HAIL:
			/*ALL THAT MUST BE DONE IS PICK WHICH MOBS (MAX 4, RANGE 500) TO HIT*/
			if(queue_[i].stage_ == 0){
				std::vector<Mob*> mobs_to_hit = get_near_mobs(this, 500, 4, DIRECTION_NONE);

				queue_[i].num_mobs_ = mobs_to_hit.size();

				for(int mob_num = 0; mob_num < mobs_to_hit.size(); ++mob_num){
					/*the damage hit is 250 ~ 500% of the strength of the player*/
					queue_[i].damages_[mob_num] = strength_ * 5 / 2 + random_number() % (strength_  * 5 / 2);

					/*record where the mob is during the attack*/
					Vector mob_midpoint = mobs_to_hit[mob_num]->get_position() + mobs_to_hit[mob_num]->get_size() / 2;
					queue_[i].target_position_[mob_num] = mob_midpoint;

					/*hit the mobs*/
					mobs_to_hit[mob_num]->take_damage(queue_[i].damages_[mob_num], this);
				}

				queue_[i].stage_++;
			}
			break;
		}
	}
}

void Player::draw_queue(ScreenLayer *player_layer){
	for(int i = 0; i < queue_.size(); ++i){
		/*the amount of time since the start frame*/
		int elapsed_frames = get_current_frame() - queue_[i].start_frame_;

		switch(queue_[i].action_){
		case PLAYER_ACTION_ATTACK_NORMAL_MELEE:
			if(queue_[i].stage_ == 1 && elapsed_frames > 0.5 * FRAMES_PER_SEC){
				/*draw the damage hit on the monster*/
				if(queue_[i].num_mobs_ > 0){
					DamageDisplay::add_damage(queue_[i].damages_[0], queue_[i].target_position_[0]);
				}

				queue_[i].stage_++;
				queue_[i].is_finished_ = true;
			}
			break;
		case PLAYER_ACTION_ATTACK_HAIL:
			if(queue_[i].stage_ == 1 || queue_[i].stage_ == 2 && elapsed_frames > 0.5 * FRAMES_PER_SEC){

				/*start the animation of the ice shard hitting the monsters*/
				for(int num_mobs = 0; num_mobs < queue_[i].num_mobs_; ++num_mobs){

					Sprite* hail_animation = SpriteDataProvider::get_instance()->get_sprite_by_name("sprites/player_attack_hail.txt");

					Vector draw_position = get_camera_offset(queue_[i].target_position_[num_mobs] - Vector(450, 450));
					/*there are 8 frames total in the animation. they should play
					 *between 0.5 seconds and 3.5 seconds after attacking

					 *span of 3.0 * FRAMES_PER_SEC frames*/
					int animation_frame = (int)(elapsed_frames - 0.5 * FRAMES_PER_SEC) * 8 / 3 / FRAMES_PER_SEC % 8;
					player_layer->draw_image_clip(hail_animation, draw_position.x_, draw_position.y_, 0, animation_frame * 9, 9, 9);
				}
				
				/*show the damages on the mobs at 3.0 seconds*/
				if(elapsed_frames > 3.0 * FRAMES_PER_SEC && queue_[i].stage_ == 1){
					for(int num_mobs = 0; num_mobs < queue_[i].num_mobs_; ++num_mobs){
						DamageDisplay::add_damage(queue_[i].damages_[num_mobs], queue_[i].target_position_[num_mobs]);
					}
					queue_[i].stage_++;
				}
			}
			if(queue_[i].stage_ == 2 && elapsed_frames > 3.5 * FRAMES_PER_SEC){
				queue_[i].is_finished_ = true;
			}
			break;
		}
	}
}