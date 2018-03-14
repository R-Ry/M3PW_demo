/*DEMO 2!
 *
 *January 15, 2018
 *
 *Demo 1 is finally finished! It took a while for me to realize but the initial goal has
 *been complete (as diversifying the mobs was decided to be implemented later.)
 *
 *In this demo, the following will be implemented in the order given:
 *	1. Separate the player attack function and use enumerations and switch statements
	   to call for specific attacks.
	
	2. Define another function attack_normal_range() and its draw function.
	
	3. Define a new mob: the mushroom, who is weaker.
 */
 /*
  */

#include "camera_offset.hpp"
#include "console_graphics.hpp"
#include "damage_display.hpp"
#include "dragon.hpp"
#include "framerate.hpp"
#include "key_event.hpp"
#include "map.hpp"
#include "map_data.hpp"
#include "mob.hpp"
#include "player.hpp"

#include <fstream>

int main(){
	
	/*set the console window graphics*/
	set_framerate(FRAMES_PER_SEC);
	set_screen_dimensions(80, 50);
	set_background(' ');
	
	/*define the map and the player and mobs that will be in the map*/
	Player player;
	
	/*make sure that the player will be displayed within the boundaries of the screen*/ 
	camera_offset_set_player(&player);
	
	/*the screen layers onto which the player, mob, and damage hit values will be drawn*/
	ScreenLayer player_layer(80, 50);
	ScreenLayer mob_layer(80, 50);
	ScreenLayer damage_layer(80, 50);
	
	/*make the mob(s) in the array dragons and initialize them to be in the map*/
	std::ifstream map_file;
	map_file.open("map.txt", std::ios::binary);
	MapData m;
	m.read_from_file(map_file);
	map_file.close();
	
	Map map(&m);
	player.set_map(&map);
	/*mobs[0] = new Dragon(&map, 500, 1500);
	mobs[1] = new Dragon(&map, 5200, 2100);
	mobs[2] = new Dragon(&map, 3400, 600);
	mobs[3] = new Dragon(&map, 6600, 3000);
	mobs[4] = new Dragon(&map, 4250, 1800);
	mobs[5] = new Dragon(&map, 3000, 1500);
	mobs[6] = new Dragon(&map, 1800, 2300);
	mobs[7] = new Dragon(&map, 2900, 3400);
	mobs[8] = new Dragon(&map, 3200, 2550);
	mobs[9] = new Dragon(&map, 1000, 2700);*/
	
	
	/*this will be used to make the game run at a certain framerate
	 *get_current_frame() returns an value depending on the amount of time passed after
	 *the application is run (the function uses clock() )
	 */
	int last_frame = get_current_frame();
	
	do{
		//game logic step, mobs move if not dead
		std::vector<Mob*> *mobs = map.get_mobs();
		for(int i = 0; i < mobs->size(); ++i){
			if( !(*mobs)[i]->is_dead() ){
				(*mobs)[i]->control();
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
				player.update(PLAYER_ACTION_ATTACK_NORMAL_MELEE);
				break;
			}
			if(key_event->keys_pressed_['x']){
				player.update(PLAYER_ACTION_ATTACK_HAIL);
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
		for(int i = 0; i < mobs->size(); ++i){
			(*mobs)[i]->draw(&mob_layer);
		}
		DamageDisplay::draw(&damage_layer);
		
		
		draw_layer(&mob_layer, 0, 0);
		draw_layer(&player_layer, 0, 0);
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
	
	return 0;
}