/*	demo1.cpp
	
	December 6, 2017
	
	this will involve a character on a map screen with other monsters on the screen.
	
	the map screen is an overhead view
	map can be less than, equal to, or greater than the screen width/height
	the only collisions for now will be the map borders
	
	The character has hp, exp, and strength.
	the character can move up, down, left and right; can attack mobs directly.
	Mobs attack the player when they are near enough, like in RO.
	
	there will be 2 types of monsters
	mushroom with hp 70, exp 12, sprite 3*3
	dragon with hp 200, exp 30, sprite 9*9
	
	mushroom and dragon have same movement patterns, but dragon is faster
	when attacked by player, mushroom follows up to player and attacks strength 5,
		dragon can attack player from a range (fire), strength 5
		
	Jan 3, 2018
	
	there is only dragon mob right now, no mushroom... 
	things to add:
		interface to control character from key events
		correct the position of the sprite of the attack made on dragon from the player.
		
	things to consider later:
		as the player has the possiblility of using sever different kinds of attacks, should
		there be an abstract "functor" object that executes the attack and draws accordingly?
		Or perhaps simply a function pointer implemented as:
			void (*attack_update)(Player*, PlayerStateInfo*);
			void (*attack_draw)(Player*, PlayerStateInfo*);
	
	Jan 4, 2018
	
	Consider changing the player (and maybe even the mob) attack algorithm so that the attack
	range is based on a square area rather than circular. (less processing and more 
*/

#include "camera_offset.hpp"
#include "console_graphics.hpp"
#include "constants.hpp"
#include "damage_display.hpp"
#include "dragon.hpp"
#include "framerate.hpp"
#include "key_event.hpp"
#include "map.hpp"
#include "player.hpp"
#include "sprite_data_provider.hpp"
#include "vector.hpp"

#include <stdio.h>

const int kMobCount = 1;

int main(){
	
	/*set the console window graphics*/
	set_framerate(FRAMES_PER_SEC);
	set_screen_dimensions(80, 50);
	set_background(' ');
	
	/*define the map and the player and mobs that will be in the map*/
	Map map(7800, 4500);
	Player player;
	Mob *mobs[kMobCount];
	player.set_map(&map);
	
	/*make sure that the player will be displayed within the boundaries of the screen*/ 
	camera_offset_set_player(&player);
	
	/*the screen layers onto which the player, mob, and damage hit values will be drawn*/
	ScreenLayer player_layer(80, 50);
	ScreenLayer mob_layer(80, 50);
	ScreenLayer damage_layer(80, 50);
	
	/*make the mob(s) in the array dragons and initialize them to be in the map*/
	for(int i = 0; i < kMobCount; ++i){
		mobs[i] = new Dragon(&map, i * 900, i * 900);
	}
	/*the map also has to have access to the mobs in it*/
	std::vector<Mob*> *map_mobs = map.get_mobs();
	for(int i = 0; i < kMobCount; ++i){
		map_mobs->push_back(mobs[i]);
	}
	
	/*this will be used to make the game run at a certain framerate
	 *get_current_frame() returns an value depending on the amount of time passed after
	 *the application is run (the function uses clock() )
	 */
	int last_frame = get_current_frame();
	
	
	
	do{
		//game logic step, mobs move normally if player has not attacked them
		for(int i = 0; i < kMobCount; ++i){
			if(mobs[i]->has_target() && (!(mobs[i]->is_dead())) ){
				mobs[i]->control_aggro_movement();
			}
			else if(!(mobs[i]->is_dead())){
				mobs[i]->control_normal_movement();
			}
		}
		
		//check what buttons were pressed (up/down/left/right/z/ESC)
		KeyEvents *key_event = get_key_events();
		do{
			if(key_event->keys_pressed_[KeyEvents::KU]){
				player.update(PLAYER_ACTION_UP);
				break;
			}
			if(key_event->keys_pressed_[KeyEvents::KD]){
				player.update(PLAYER_ACTION_DOWN);
				break;
			}
			if(key_event->keys_pressed_[KeyEvents::KL]){
				player.update(PLAYER_ACTION_LEFT);
				break;
			}
			if(key_event->keys_pressed_[KeyEvents::KR]){
				player.update(PLAYER_ACTION_RIGHT);
				break;
			}
			if(key_event->keys_pressed_['z']){
				player.update(PLAYER_ACTION_ATTACK);
				break;
			}
			
			player.update(PLAYER_ACTION_NONE);
		} while(0);
		
		/*pressing escape breaks from the game loop, ending the game*/
		if(key_event->keys_pressed_[KeyEvents::ESC]){
			break;
		}
		
		//drawing functions
		player.draw(&player_layer);
		for(int i = 0; i < kMobCount; ++i){
			mobs[i]->draw(&mob_layer);
		}
		DamageDisplay::draw(&damage_layer);
		
		
		draw_layer(&player_layer, 0, 0);
		draw_layer(&mob_layer, 0, 0);
		draw_layer(&damage_layer, 0, 0);
		
		render();
		
		/*clear the layers so that they can be drawn on in the next frame*/
		player_layer.clear();
		mob_layer.clear();
		damage_layer.clear();
		
		last_frame = get_current_frame();
		
		while(last_frame == current_frame()){
			//pause for a frame
		}
	} while(last_frame < 100000);
	
	for(int i = 0; i < kMobCount; ++i){
		delete mobs[i];
	}
	
	return 0;
}