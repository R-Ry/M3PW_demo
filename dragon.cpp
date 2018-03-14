#include "camera_offset.hpp"
#include "console_graphics.hpp"
#include "constants.hpp"
#include "damage_display.hpp"
#include "dragon.hpp"
#include "framerate.hpp"
#include "map.hpp"
#include "player.hpp"
#include "rand.hpp"
#include "sprite_data_provider.hpp"

#include <stdio.h>

/*	the duration of the dragon's attack is about 2.5 seconds
	first second the dragon is in charged position (doesn't move)
	
	second part is when the dragon hits the player if the player is in range
	(play the bolt sprite on player location
*/

Dragon::Dragon(Map *m, int x_position, int y_position):
Mob(m, x_position, y_position),
is_target_hit_(false),
target_hit_position_(0, 0)
{
	hp_ = 200;
	max_hp_ = 200;
	exp_ = 30;
	strength_ = 5;
	speed_ = 150;
	size_ = Vector(900, 900);
	sprite_ = SpriteDataProvider::get_instance()->get_sprite_by_name("sprites/dragon.txt");
}

void Dragon::control(){
	
	/*control normal movement if there is no player target*/
	if(!target_){
		/*	the mob will more likely tend to be in a standing position_.
			4/5 chance per second to stay standing still,
			1/5 chance per second to change direction_ while standing and moving
			3/5 chance per second to keep moving,
		*/
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
				
				if(position_.y_ + kDragonHeight >= map_->get_height()){
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
				
				if(position_.x_ + kDragonWidth >= map_->get_width()){
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
		return;
	}
	/*else if there is a player target*/
	else{
		/*position vectors of the midpoint of dragon and player */
		Vector player_position = target_->get_position() + target_->get_size() / 2;
		
		/*difference between midpoint of player with respect to dragon*/
		Vector difference = player_position - (position_ + size_ / 2);
		
		switch(mob_state_){
		case MOBSTATE_HIT:
			/*if the mob has been hit, check if enough time has passed to get off the recoil position*/
			if(time_delayed_until_ + (0.5 * FRAMES_PER_SEC) < get_current_frame()){
				mob_state_ = MOBSTATE_MOVING;
			}
			break;
		case MOBSTATE_MOVING:
			{
				/*	first orient the mob towards the player	*/
				/*player is on left/right of dragon (x distance is greater than y distance)*/
				if(difference.x_ * difference.x_ > difference.y_ * difference.y_){
					if(difference.x_ < 0.0){
						direction_ = DIRECTION_LEFT;
					}
					else{
						direction_ = DIRECTION_RIGHT;
					}
				}
				else{
					if(difference.y_ < 0.0){
						direction_ = DIRECTION_UP;
					}
					else{
						direction_ = DIRECTION_DOWN;
					}
				}
				/*	then check if the mob has to get closer to the player */
				/*dot product of difference and difference*/
				int square_distance = difference * difference;
				
				/*if dragon is close enough to player square_distance < (600 * 600) */
				if(square_distance < 360000){
					/*change state to attempt attack player*/
					
					mob_state_ = MOBSTATE_ATTACKING;
					time_delayed_until_ = get_current_frame() + 2.5 * FRAMES_PER_SEC;
				}
				
				/*	else the dragon has to move towards the player	*/
				else{
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
						
						if(position_.y_ + kDragonHeight >= map_->get_width()){
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
						
						if(position_.x_ + kDragonWidth >= map_->get_width()){
							position_.x_ -= speed_ / FRAMES_PER_SEC;
							direction_ = DIRECTION_LEFT;
						}
						break;
					}
				}
			}
			break;
		case MOBSTATE_ATTACKING:
			{
				int elapsed_time = 2.5 * FRAMES_PER_SEC + get_current_frame() - time_delayed_until_;
				
				/*the dragon is in charging position*/
				if(elapsed_time >= 0 && elapsed_time < 1 * FRAMES_PER_SEC){
				}
				/*	the dragon hits the player if in range	*/
				else if(elapsed_time == 1 * FRAMES_PER_SEC){
					Vector difference = (target_->get_position() + Vector(kPlayerWidth/2, kPlayerHeight/2))
										- (position_ + Vector(kDragonWidth/2, kDragonHeight/2));
					
					int square_distance = difference * difference;
					
					if(is_target_hit_ = square_distance < 360000){
						target_hit_position_ = target_->get_position() + Vector(kPlayerWidth/2, kPlayerHeight/2);	//midpoint of player's position
						target_->take_damage(strength_);
					}
				}
				/*	the dragon is stuck at its firing position for another 1.5 seconds	*/
				else if(elapsed_time > 1 * FRAMES_PER_SEC && elapsed_time < 2.5 * FRAMES_PER_SEC){
				}
				else{
					mob_state_ = MOBSTATE_MOVING;
					is_target_hit_ = false;
				}
			}
			break;
		}
	}
}


void Dragon::draw(ScreenLayer* mob_layer){
	Vector draw_position = get_camera_offset(position_);
	
	switch(mob_state_){
	case MOBSTATE_STANDING:
	case MOBSTATE_MOVING:
	case MOBSTATE_HIT:
		
		/*this first part draws the dragon*/
		if(direction_ == DIRECTION_UP){
			mob_layer->draw_image_clip(sprite_, draw_position.x_, draw_position.y_,
										0, 0, 9, 9);
		}
		else if(direction_ == DIRECTION_DOWN){
			mob_layer->draw_image_clip(sprite_, draw_position.x_, draw_position.y_,
										0, 9, 9, 9);
		}
		else if(direction_ == DIRECTION_LEFT){
			mob_layer->draw_image_clip(sprite_, draw_position.x_, draw_position.y_,
										0, 18, 9, 9);
		}
		else{
			mob_layer->draw_image_clip(sprite_, draw_position.x_, draw_position.y_,
										0, 27, 9, 9);
		}
		break;
	case MOBSTATE_ATTACKING:
		
		/*this part draws the dragon in charging position*/
		if(direction_ == DIRECTION_UP){
			mob_layer->draw_image_clip(sprite_, draw_position.x_, draw_position.y_,
										9, 0, 9, 9);
		}
		else if(direction_ == DIRECTION_DOWN){
			mob_layer->draw_image_clip(sprite_, draw_position.x_, draw_position.y_,
										9, 9, 9, 9);
		}
		else if(direction_ == DIRECTION_LEFT){
			mob_layer->draw_image_clip(sprite_, draw_position.x_, draw_position.y_,
										9, 18, 9, 9);
		}
		else{
			mob_layer->draw_image_clip(sprite_, draw_position.x_, draw_position.y_,
										9, 27, 9, 9);
		}
		
		
		
		/*	the dragon hits the player if in range (assigned true or false in update() )
		 *	there are three frames of animation for the fire bolt (3*6 sprite) over a span of 1.5 seconds
		 */
		if(is_target_hit_ == false){
			return;
		}
		else{
			/*number of frames passed since the attack started*/
			int elapsed_time = 2.5 * FRAMES_PER_SEC + get_current_frame() - time_delayed_until_;
			
			/*	3 frames of the fireball within a span of 1.5 * frames_per_sec,
			 *fixed at the recorded position of the player.
			 *
			 *how it is calculated: calculate the number of frame of animation per
			 *second. 3.0 / 1.5 = 2. 
			 *
			 *general formula: (elapsed_time - delay_in_seconds * FRAMES_PER_SEC) * framerate_of_animation / FRAMES_PER_SEC
			 */
			int fireball_frame = (elapsed_time - 1 * FRAMES_PER_SEC) * 2 / FRAMES_PER_SEC;
			
			Vector player_display_position = get_camera_offset(target_hit_position_);
			
			if(elapsed_time == 1.0 * FRAMES_PER_SEC){
				
				DamageDisplay::add_damage(strength_, target_hit_position_);
			}
			if(fireball_frame == 0){
				mob_layer->draw_image_clip(sprite_, player_display_position.x_ - 1, player_display_position.y_ - 4,
											18, 0, 3, 6);
			}
			else if(fireball_frame == 1){
				mob_layer->draw_image_clip(sprite_, player_display_position.x_ - 1, player_display_position.y_ - 4,
											18, 6, 3, 6);
			}
			else{
				mob_layer->draw_image_clip(sprite_, player_display_position.x_ - 1, player_display_position.y_ - 4,
											18, 12, 3, 6);
			}
		}
		break;
	}
}