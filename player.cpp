#include "camera_offset.hpp"
#include "console_graphics.hpp"
#include "constants.hpp"
#include "damage_display.hpp"
#include "framerate.hpp"
#include "map.hpp"
#include "mob.hpp"
#include "player.hpp"
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
	draw_image(sprite_, 0, 0);
	render();
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

bool Player::is_dead(){
	return hp_ < 1;
}
	
void Player::take_damage(int damage){
	hp_ -= damage;
	
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
	
	if(exp_ >= max_exp_){
		exp_ -= max_exp_;
		level_++;
		max_exp_ = kPlayerExp[level_ - 1];
	}
}

/*	the player attack is currently implemented as follows:
	there is a 1/2 second delay after pressing the attack button,
	attack starts, physical attacks hit immediately.
	
	then there is a 1/2 second delay
	
	the hit positions and hit damages are delimited by -1 at the end of
		their arrays, if smaller than 6
	
 */

void Player::update(PlayerControlAction action){
	
	/* update player based on its state from the last frame*/
	switch(player_state_info_.player_state_){
	case PLAYER_STATE_STANDING:
		break;
	case PLAYER_STATE_MOVING:
		switch(direction_){
		case DIRECTION_UP:
			position_.y_ -= speed_ / FRAMES_PER_SEC;
			if(position_.y_ < 0){
				position_.y_ = 0;
			}
			break;
		case DIRECTION_DOWN:
			position_.y_ += speed_ / FRAMES_PER_SEC;
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
		
		//set the player state to not moving, since the state should not stay moving automatically.
		player_state_info_.player_state_ = PLAYER_STATE_STANDING;
		break;
	case PLAYER_STATE_ATTACKING:
		/* code that executes a regular close combat attack */
		
		//NOTE: THIS SHOULD BE COMPLETELY REPLACED BY A SWITCH STATEMENT FOR THE TYPE OF ATTACK AND
		//SHOULD LOOK LIKE
		//
		//player_attack_info.update_(this, &player_state_info_);
		{
			int elapsed_time = get_current_frame() - player_state_info_.time_of_attack_;
			
			if(elapsed_time == 0 || elapsed_time < FRAMES_PER_SEC / 2){
				/*nothing happens for first half after starting attack*/
			}
			if(elapsed_time == FRAMES_PER_SEC / 2){
				/*hit the mob*/
				/*find the closest mob in front of the player*/
				std::vector<Mob*> *map_mobs = map_->get_mobs();
				
				std::vector<Mob*> mobs_in_line;
				Mob* closest_mob = 0;
				
				/*first list all mobs in line of player's direction*/
				for(int i = 0; i < map_mobs->size(); ++i){
					
					Vector position_difference = (*map_mobs)[i]->get_position() + (*map_mobs)[i]->get_size() / 2 - position_ - Vector(150, 150);
					
					switch(direction_){
					case DIRECTION_UP:
						if(position_difference.y_ < 0 &&
							(float(position_difference.x_) / float(position_difference.y_)) < 0.5 &&
							(float(position_difference.x_) / float(position_difference.y_)) > -0.5){
								
							mobs_in_line.push_back((*map_mobs)[i]);
						}
						break;
					case DIRECTION_DOWN:
						if(position_difference.y_ > 0 &&
							(float(position_difference.x_) / float(position_difference.y_)) < 0.5 &&
							(float(position_difference.x_) / float(position_difference.y_)) > -0.5){
								
							mobs_in_line.push_back((*map_mobs)[i]);
						}
						break;
					case DIRECTION_LEFT:
						if(position_difference.x_ < 0 &&
							(float(position_difference.y_) / float(position_difference.x_)) < 0.5 &&
							(float(position_difference.y_) / float(position_difference.x_)) > -0.5){
								
							mobs_in_line.push_back((*map_mobs)[i]);
						}
						break;
					case DIRECTION_RIGHT:
						if(position_difference.x_ > 0 &&
							(float(position_difference.y_) / float(position_difference.x_)) < 0.5 &&
							(float(position_difference.y_) / float(position_difference.x_)) > -0.5){
							
							mobs_in_line.push_back((*map_mobs)[i]);
						}
						break;
					}
				}
				
				/*select the closest mob to player*/
				int closest_square_distance = 62500;
				for(int i = 0; i < mobs_in_line.size(); ++i){
					
					Vector distance = mobs_in_line[i]->get_position() + mobs_in_line[i]->get_size() / 2 - position_ - Vector(150, 150);
					
					int square_distance = distance * distance - Vector(150,150)*Vector(150,150) - ((*map_mobs)[i]->get_size()/2)*((*map_mobs)[i]->get_size()/2);
					
					if(square_distance < closest_square_distance){
						closest_mob = mobs_in_line[i];
						closest_square_distance = square_distance;
					}
				}															printf("a2.7\n");
				
				/*if a mob is in range of the player's attack, hit mob and record damages and their positions*/
				player_state_info_.mob_hit_positions_[0] = -1;
				player_state_info_.mob_hit_damages_[0] = -1;
				if(closest_mob != 0){
					closest_mob->take_damage(strength_, this);
					
					player_state_info_.mob_hit_positions_[0] = closest_mob->get_position().x_ + closest_mob->get_size().x_ / 2;
					player_state_info_.mob_hit_positions_[1] = closest_mob->get_position().y_ + closest_mob->get_size().y_ / 2;
					player_state_info_.mob_hit_positions_[2] = -1;
					player_state_info_.mob_hit_damages_[0] = strength_;
					player_state_info_.mob_hit_damages_[1] = -1;
				}
			}
			if(elapsed_time > FRAMES_PER_SEC / 2 && elapsed_time <= FRAMES_PER_SEC * 1){
			}
			if(elapsed_time > FRAMES_PER_SEC * 1){
				player_state_info_.player_state_ = PLAYER_STATE_STANDING;
			}
		}		
		break;
	}
	
	/* process keyboard input */
	if(player_state_info_.player_state_ == PLAYER_STATE_ATTACKING){
		return;
	}
	
	switch(action){
	case PLAYER_ACTION_NONE:
		break;
	case PLAYER_ACTION_UP:
		player_state_info_.player_state_ = PLAYER_STATE_MOVING;
		direction_ = DIRECTION_UP;
		break;
	case PLAYER_ACTION_DOWN:
		player_state_info_.player_state_ = PLAYER_STATE_MOVING;
		direction_ = DIRECTION_DOWN;
		break;
	case PLAYER_ACTION_LEFT:
		player_state_info_.player_state_ = PLAYER_STATE_MOVING;
		direction_ = DIRECTION_LEFT;
		break;
	case PLAYER_ACTION_RIGHT:
		player_state_info_.player_state_ = PLAYER_STATE_MOVING;
		direction_ = DIRECTION_RIGHT;
		break;
	case PLAYER_ACTION_ATTACK:
		player_state_info_.time_of_attack_ = get_current_frame();
		player_state_info_.player_state_ = PLAYER_STATE_ATTACKING;
		break;
	}
}

void Player::draw(ScreenLayer *player_layer){
	Vector display_position = get_camera_offset(position_); draw_int_left_end(display_position.x_, 60, 0); draw_int_left_end(display_position.y_, 65, 0);
															draw_int_left_end(position_.x_, 60, 1); draw_int_left_end(position_.y_, 65, 1);
	
	switch(player_state_info_.player_state_){
	case PLAYER_STATE_ATTACKING:
		{
			int elapsed_time = get_current_frame() - player_state_info_.time_of_attack_;
			
			if(elapsed_time == FRAMES_PER_SEC / 2){
				for(int i = 0; i < 6 && player_state_info_.mob_hit_positions_[i * 2] != -1; ++i){
					
					Vector damage_display_position = get_camera_offset(Vector(player_state_info_.mob_hit_positions_[i * 2], player_state_info_.mob_hit_positions_[i * 2 + 1]) );
					DamageDisplay::add_damage(player_state_info_.mob_hit_damages_[i * 6], damage_display_position);
				}
			}
		}
	case PLAYER_STATE_STANDING:
	case PLAYER_STATE_MOVING:
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
	
	player_layer->draw_str("HP: ", 0, 49);
	player_layer->draw_int_right_end(hp_, 6, 49);
	player_layer->draw_str("/", 7, 49);
	player_layer->draw_int_left_end(max_hp_, 8, 49);
	
	player_layer->draw_str("EXP: ", 20, 49);
	player_layer->draw_int_right_end(exp_, 30, 49);
	player_layer->draw_str("/", 31, 49);
	player_layer->draw_int_left_end(max_exp_, 32, 49);
}
