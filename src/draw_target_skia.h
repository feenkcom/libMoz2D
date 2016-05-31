/*
 * draw_target_skia.h
 *
 *  Created on: May 29, 2016
 *      Author: aliaksei
 */

#ifndef DRAW_TARGET_SKIA_H_
#define DRAW_TARGET_SKIA_H_

typedef void* CDrawTargeSkia;

#ifdef __cplusplus
extern "C" {
#endif

CDrawTargeSkia draw_target_skia_create();
void draw_target_skia_flush(CDrawTargeSkia);
bool draw_target_skia_is_valid(CDrawTargeSkia syrel);

#ifdef __cplusplus
}
#endif

#endif /* DRAW_TARGET_SKIA_H_ */
