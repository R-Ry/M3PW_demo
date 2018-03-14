#ifndef DAMAGE_DISPLAY_HPP
#define DAMAGE_DISPLAY_HPP

#include "vector.hpp"

class ScreenLayer;

/*used to draw damage values of attacks onto the screen.
 *add_damage stores the damage information
 *
 *draw draws the damages onto a screen layer.
 */

namespace DamageDisplay{
	void add_damage(int damage, Vector map_position);
	void draw(ScreenLayer *damage_layer);
}

#endif