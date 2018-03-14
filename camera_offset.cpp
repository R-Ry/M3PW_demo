#include "camera_offset.hpp"
#include "map.hpp"
#include "player.hpp"

/*	Last updated: January 4, 2018
*/

static const int kMapScreenWidth = 80, kMapScreenHeight = 45;

static Player *player = 0;

/*	ensure that the first time that get_camera_offset is called, the offset is
	updated since the player position is always positive.
*/
static Vector last_player_position(-1,-1);

/*	the offset of the map corner to the upper left corner of the screen in pixels
*/
static int offset_x = 0, offset_y = 0;

void camera_offset_set_player(Player *p){
	player = p;
}

/*get the correct display position of a particular position on the map*/
Vector get_camera_offset(Vector map_position){
	if(player->get_position() == last_player_position){
		return Vector(offset_x, offset_y) + map_position / 100;
	}
	
	Map *player_map = player->get_map();
	
	/*
		check if the screen is static in either the x or y direction
	*/
	bool is_fixed_x, is_fixed_y;
	is_fixed_x = (player_map->get_width() / 100 <= kMapScreenWidth);
	is_fixed_y = (player_map->get_height() / 100 <= kMapScreenHeight);
	
	/*
		calculate the x offset first
	*/
	
	if(is_fixed_x){
		/*
			|------------------------------->|
			|kMapScreenWidth
			|
			|--------------->
			|kMapScreenWidth / 2
			|
			|---->
			|kMapScreenWidth / 2 - player_map->get_width() / 200
		*/
		offset_x = kMapScreenWidth / 2 - player_map->get_width() / 200;
	}
	else{
		/*	calculate the offset so that the player is in the middle of the screen
			unless located near the borders of the map
		*/
		offset_x = player->get_position().x_ / 100 - kMapScreenWidth / 2;
		
		/*	offset_x will be negative if player is far to the left, so limit the offset
			to be minimum of zero
		*/
		if(offset_x < 0){
			offset_x = 0;
		}
		/*
			offset_x will be large enough so that the empty space beyond the map's
			right edge will take up the right of the screen, hence offset_x is limited
		*/
		if(offset_x > (player_map->get_width() / 100 - kMapScreenWidth / 2) ){
			offset_x = player_map->get_width() / 100 - kMapScreenWidth / 2;
		}
	}
	
	/*	calculate y offset
	*/
	if(is_fixed_y){
		offset_y = kMapScreenHeight / 2 - player_map->get_height() / 200;
	}
	else{
		offset_y = player->get_position().y_ / 100 - kMapScreenHeight / 2;
		
		if(offset_y < 0){
			offset_y = 0;
		}
		if(offset_y > (player_map->get_height() / 100 - kMapScreenHeight / 2) ){
			offset_y = player_map->get_height() / 100 - kMapScreenHeight / 2;
		}
	}
	
	return Vector(offset_x, offset_y) + map_position / 100;
}