#include "constants.hpp"
#include "map.hpp"
#include "map_mob_selector.hpp"
#include "mob.hpp"
#include "player.hpp"

/*returns a list of mobs that are in proximity of the player
 *in a given radius, number of mobs, and direction
 *
 *EDIT: DON'T USE CALCULATIONS BASED ON RADIUS BUT USE SQUARE AREA INSTEAD
 *... EXAMPLE ON PAPER
 *
 *JANUARY 17, 2018 - THE CODE IS ALL TYPED, BUT NOT TESTED
 */
 
std::vector<Mob*> get_near_mobs(Player* player, int max_radius, int max_num_mobs, Direction direction, float max_tangent_value){
	std::vector<Mob*> *map_mobs = player->get_map()->get_mobs();
	
	/*mobs that are near the player with the given radius are selected, then the mobs that are
	 *in line are selected*/
	std::vector<Mob*> nearby_mobs, mobs_in_line, mobs_to_hit;
	
	/*first select the mobs that are in proximity of the player within the max distance*/
	for(int i = 0; i < map_mobs->size(); ++i){
		
		Mob* p_mob = (*map_mobs)[i];
		
		Vector player_midpoint = player->get_position() + player->get_size() / 2;
		Vector mob_midpoint = p_mob->get_position() + p_mob->get_size() / 2;
		
		Vector distance = mob_midpoint - player_midpoint;
		Vector max_distance = player->get_size() / 2 + p_mob->get_size() / 2 + Vector(max_radius, max_radius);
		
		if( (distance.x_ * distance.x_ < max_distance.x_ * max_distance.x_)
			&& (distance.y_ * distance.y_  < max_distance.y_ * max_distance.y_) ){
			
			nearby_mobs.push_back(p_mob);
		}
	}
	
	/*copy the elements from nearby_mobs that are in line with the player's direction
	 *to mobs_in_line.*/
	
	for(int i = 0; i < nearby_mobs.size(); ++i){
		float tan_value;
		
		Vector distance = (nearby_mobs[i]->get_position() + nearby_mobs[i]->get_size() / 2)
							- (player->get_position() + player->get_size() / 2);
		
		switch(direction){
		/* in this case, the direction is not put into account so any entity in proximity of
		 * the player will be selected*/
		case DIRECTION_NONE:
			mobs_in_line.push_back(nearby_mobs[i]);
			break;
		
		case DIRECTION_UP:
			if(distance.y_ < 0){
				tan_value = (float)(distance.x_) / (float)(distance.y_);
			}
			else{
				break;
			}
			
			if(tan_value * tan_value < max_tangent_value * max_tangent_value){
				mobs_in_line.push_back(nearby_mobs[i]);
			}
			break;
		case DIRECTION_DOWN:
			if(distance.y_ > 0){
				tan_value = (float)(distance.x_) / (float)(distance.y_);
			}
			else{
				break;
			}
			
			if(tan_value * tan_value < max_tangent_value * max_tangent_value){
				mobs_in_line.push_back(nearby_mobs[i]);
			}
			break;
		case DIRECTION_LEFT:
			if(distance.x_ < 0){
				tan_value = (float)(distance.y_) / (float)(distance.x_);
			}
			else{
				break;
			}
			
			if(tan_value * tan_value < max_tangent_value * max_tangent_value){
				mobs_in_line.push_back(nearby_mobs[i]);
			}
			break;
		case DIRECTION_RIGHT:
			if(distance.x_ > 0){
				tan_value = (float)(distance.y_) / (float)(distance.x_);
			}
			else{
				break;
			}
			
			if(tan_value * tan_value < max_tangent_value * max_tangent_value){
				mobs_in_line.push_back(nearby_mobs[i]);
			}
			break;
		}
	}		
	
	/*sort the mobs from closest to farthest from player*/
	
	/*first copy the sqaure distance between mobs_in_line and player to an array*/
	std::vector<int> square_distances;
	
	for(int i = 0; i < mobs_in_line.size(); ++i){
		Vector distance = (mobs_in_line[i]->get_position() + mobs_in_line[i]->get_size() / 2)
							- ( player->get_position() + player->get_size() / 2);
		
		square_distances.push_back(distance * distance);
	}
	
	/*sort a number of elements from smallest distance in front to largest in the back*
	 *
	 *at the same time, sort the mobs from the mobs_in_line array.
	 *
	 *at the end of each loop of sorting, copy the element sorted to the front of the
	 *mobs_in_line array to the mobs_to_hit array until either the max number of mobs
	 *has been put into the mobs_to_hit array or the entire mobs_in_line array has been
	 *sorted.*/
	
	for(int num_mobs = 0; num_mobs < mobs_in_line.size() && num_mobs < max_num_mobs; ++num_mobs){
		for(int i = mobs_in_line.size() - 1; i > 0; --i){
			
			if(square_distances[i] < square_distances[i - 1]){
				int int_temp = square_distances[i - 1];
				Mob *mob_temp = mobs_in_line[i - 1];
				
				square_distances[i - 1] = square_distances[i];
				square_distances[i] = int_temp;
				
				mobs_in_line[i - 1] = mobs_in_line[i];
				mobs_in_line[i] = mob_temp;
			}
		}
		mobs_to_hit.push_back(mobs_in_line[num_mobs]);
	}
	
	return mobs_to_hit;
}
