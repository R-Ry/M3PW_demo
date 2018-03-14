#ifndef CAMERA_OFFSET_HPP
#define CAMERA_OFFSET_HPP

/*this header is used to convert any physical position of an entity
 *to the display position needed to draw the entity in the correct position.
 *
 *get_camera_offset() will NOT work without initializing which player's
 *view to refer to
 */

#include "vector.hpp"

class Player;

void camera_offset_set_player(Player *p);

/*get the display position of a map position
 *
 *----->Will crash the application if player has not been set<-----
 */
Vector get_camera_offset(Vector map_position);

#endif