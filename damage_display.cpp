#include "camera_offset.hpp"
#include "console_graphics.hpp"
#include "constants.hpp"
#include "damage_display.hpp"
#include "framerate.hpp"

#include <vector>

/*for debug*/
#include <stdio.h>

/*
	Damage dealt by a mob or player should last about three thirds a second.
	In effect, the damage infos stored in the deque experience "first in first out"
	However, all data members must be accessed so deque is used instead of queue.

	UPDATE (JAN. 23, 2018
*/

/* the number of frames that attack damage will be shown on screen*/
static const int kDamageDuration = 1 * FRAMES_PER_SEC;

namespace DamageDisplay{
	
	typedef struct DamageInfo{
		int damage_, x_position_, y_position_, start_frame_;
	}DamageInfo;
	
	/*all the damages from player and mob attacks will be stored to damage_infos via add_damage()*/
	static std::vector<DamageInfo> damage_infos;
	
	/* stores damage value and position to damage_infos. the time of the damage is taken from get_current_frame()*/
	void add_damage(int damage, Vector map_position){
		
		/*convert the map position to the display position in player's perspective*/
		Vector display_position = get_camera_offset(map_position);
		
		/*store a data structure to be used later by the draw function*/
		DamageInfo damage_info;
		damage_info.damage_ = damage;
		damage_info.x_position_ = display_position.x_;
		damage_info.y_position_ = display_position.y_;
		damage_info.start_frame_ = get_current_frame();
		
		damage_infos.push_back(damage_info);
	}
	
	/* draws all the damages made within their time duration
	 * all damages made longer than kDamageDuration ago will be removed
	 */
	void draw(ScreenLayer *damage_layer){
		/*after displaying the damages, the damages which no longer have to be displayed will
		 *be removed and the rest will be transferred to this list.*/
		std::vector<DamageInfo> updated_damage_list;
		
		for(int i = 0; i < damage_infos.size(); ++i){
			Vector display_position = Vector(damage_infos[i].x_position_, damage_infos[i].y_position_);
			damage_layer->draw_int_left_end(damage_infos[i].damage_, display_position.x_, display_position.y_);
			
			/*remove damage values that were made kDamageDuration ago or longer
			 *the old damage value will not be copied to the new array*/
			if(damage_infos[i].start_frame_ + kDamageDuration <= get_current_frame()){
				continue;
			}
			else{
				updated_damage_list.push_back(damage_infos[i]);
			}
		}
		damage_infos = updated_damage_list;
	}
}