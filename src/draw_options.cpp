/*
 * draw_options.cpp
 *
 *  Created on: Jun 4, 2016
 *      Author: aliaksei
 */

#include "draw_options.h"

DrawOptions* moz2d_draw_options_create(Float alpha, CompositionOp aCompositionOp, AntialiasMode aAntialiasMode) {
	return new DrawOptions(alpha, aCompositionOp, aAntialiasMode);
}

void moz2d_draw_options_delete(DrawOptions *drawOptions) {
	delete drawOptions;
}

Float moz2d_draw_options_get_alpha(DrawOptions *drawOptions) {
	return drawOptions->mAlpha;
}

CompositionOp moz2d_draw_options_get_compositionOp(DrawOptions *drawOptions) {
	return drawOptions->mCompositionOp;
}

AntialiasMode moz2d_draw_options_get_antialias_mode(DrawOptions *drawOptions) {
	return drawOptions->mAntialiasMode;
}
