#ifndef DAMAGE_DISPLAY_HPP
#define DAMAGE_DISPLAY_HPP

#include "vector.hpp"

class ScreenLayer;

namespace DamageDisplay{
	void add_damage(int damage, Vector position);
	void draw(ScreenLayer *damage_layer);
}

#endif