/*
 * mac_font.h
 *
 *  Created on: Jul 26, 2016
 *      Author: aliaksei
 */

#ifndef MAC_FONT_H_
#define MAC_FONT_H_

#include <CoreFoundation/CoreFoundation.h>
#include <ApplicationServices/ApplicationServices.h>

#include "nsUnicharUtils.h"

#ifdef __cplusplus
extern "C" {
#endif

CGFontRef moz2d_mac_font_get_cf_font_ref(const char* fontName, double size);
CGFontRef moz2d_mac_font_get_cf_font_ref_data(const char* data, size_t size, double fontSize);
const char* moz2d_mac_font_cg_font_full_name (CGFontRef fontRef);

#ifdef __cplusplus
}
#endif

#endif /* MAC_FONT_H_ */
