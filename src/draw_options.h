/*
 * draw_options.h
 *
 *  Created on: Jun 4, 2016
 *      Author: aliaksei
 */

#ifndef DRAW_OPTIONS_H_
#define DRAW_OPTIONS_H_

#include "2d/2D.h"

using namespace mozilla::gfx;

#ifdef __cplusplus
extern "C" {
#endif

DrawOptions* moz2d_draw_options_create(Float alpha, CompositionOp aCompositionOp, AntialiasMode aAntialiasMode);

void moz2d_draw_options_delete(DrawOptions *drawOptions);

Float moz2d_draw_options_get_alpha(DrawOptions *drawOptions);

CompositionOp moz2d_draw_options_get_compositionOp(DrawOptions *drawOptions);

AntialiasMode moz2d_draw_options_get_antialias_mode(DrawOptions *drawOptions);

#ifdef __cplusplus
}
#endif

#endif /* DRAW_OPTIONS_H_ */
