#include "camera_offset.hpp"
#include "console_graphics.hpp"
#include "constants.hpp"
#include "damage_display.hpp"
#include "framerate.hpp"

#include <deque>

/*
	Damage dealt by a mob or player should last about three thirds a second.
	In effect, the damage infos stored in the deque experience "first in first out"
	However, all data members must be accessed so deque is used instead of queue.
*/
static const int kDamageDuration = 1 * FRAMES_PER_SEC;

namespace DamageDisplay{
	typedef struct DamageInfo{
		int damage_, x_position_, y_position_, start_frame_;
	}DamageInfo;
	
	static std::deque<DamageInfo> damage_infos;
	
	void add_damage(int damage, Vector position){
		DamageInfo damage_info;
		damage_info.damage_ = damage;
		damage_info.x_position_ = position.x_;
		damage_info.y_position_ = position.y_;
		damage_info.start_frame_ = get_current_frame();
		
		damage_infos.push_back(damage_info);
	}
		
	void draw(ScreenLayer *damage_layer){
		std::deque<DamageInfo>::iterator it = damage_infos.begin();
		
		while(it != damage_infos.end()){
			Vector display_position = Vector(it->x_position_, it->y_position_);
			damage_layer->draw_int_left_end(it->damage_, display_position.x_, display_position.y_);
			it++;
			
			if(it->start_frame_ + kDamageDuration <= get_current_frame()){
				damage_infos.pop_front();
			}
		}
	}
}