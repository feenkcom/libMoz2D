/*
 * font.h
 *
 *  Created on: Jul 26, 2016
 *      Author: aliaksei
 */

#ifndef FONT_H_
#define FONT_H_

#include "2d/2D.h"
#include "2d/ScaledFontMac.h"
#include "mac_font.h"
#include "cairo-quartz.h"

using namespace mozilla::gfx;

class MozFont {

};


#ifdef __cplusplus
extern "C" {
#endif

ScaledFont* moz2d_mac_font_create_scaled_font(const char* fontName, double size);
ScaledFont* moz2d_mac_font_create_scaled_font_data(const char* data, size_t size, double fontSize);
const char* moz2d_mac_font_create_scaled_font_data_name(const char* data, size_t size, double fontSize);

#ifdef __cplusplus
}
#endif

#endif /* FONT_H_ */
