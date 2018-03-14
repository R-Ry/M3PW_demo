#ifndef MAP_MOB_SELECTOR
#define MAP_MOB_SELECTOR

#include "constants.hpp"
#include <vector>

class Mob;
class Player;

//this is an actual 2 dimensional vector, not to be confused with std::vector
class Vector;

/*returns a list of mobs that are in proximity of the player
 *in a given radius, number of mobs, and direction*/
 
std::vector<Mob*> get_near_mobs(Player* player, int max_radius, int max_num_mobs, Direction direction = DIRECTION_NONE, float max_tangent_value = 0.5);

#endif