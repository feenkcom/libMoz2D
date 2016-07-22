/*
 * color.cpp
 *
 *  Created on: Jun 4, 2016
 *      Author: aliaksei
 */

#include "color.h"

Color* moz2d_color_create(Float red, Float green, Float blue, Float alpha) {
	return new Color(red, green, blue, alpha);
}

void moz2d_color_delete(Color *color) {
	delete color;
}

Float moz2d_color_get_red(Color *color) {
	return color->r;
}

Float moz2d_color_get_green(Color *color) {
	return color->g;
}

Float moz2d_color_get_blue(Color *color) {
	return color->b;
}

Float moz2d_color_get_alpha(Color *color) {
	return color->a;
}
