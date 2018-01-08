#ifndef CAMERA_OFFSET_HPP
#define CAMERA_OFFSET_HPP

#include "vector.hpp"

class Player;

void camera_offset_set_player(Player *p);
Vector get_camera_offset(Vector map_position);

#endif