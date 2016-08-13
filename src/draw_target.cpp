/*
 * draw_target.cpp
 *
 *  Created on: May 29, 2016
 *      Author: aliaksei
 */

#include "draw_target.h"

#include "2d/Filters.h"
#include "SkiaGLGlue.h"
#include "gfxPlatform.h"

#include <stack> // std::stack

using namespace mozilla::gfx;

/* --------------------------------------------------- */
/* ----------------- C R E A T I O N ----------------- */
/* --------------------------------------------------- */

DrawTarget* moz2d_draw_target_create (int32_t width, int32_t height, SurfaceFormat aFormat) {
	return gfxPlatform::GetPlatform()->CreateOffscreenCanvasDrawTarget(IntSize(width, height), aFormat).take();
}

DrawTarget* moz2d_draw_target_create_type (BackendType aBackend, int32_t width, int32_t height, SurfaceFormat aFormat) {
	if (aBackend == BackendType::SKIA) {
		mozilla::gl::SkiaGLGlue* skiaGlue = gfxPlatform::GetPlatform()->GetSkiaGLGlue();
		if (skiaGlue)
			return Factory::CreateDrawTargetSkiaWithGrContext(skiaGlue->GetGrContext(), IntSize(width, height), aFormat).take();
	}
	return Factory::CreateDrawTarget(aBackend, IntSize(width, height), aFormat).take();
}

DrawTarget* moz2d_draw_target_create_for_data_type (BackendType aBackend, unsigned char* aData, int32_t width, int32_t height, int32_t aStride, SurfaceFormat aFormat, bool aUninitialized) {
	return Factory::CreateDrawTargetForData(aBackend, aData, IntSize(width, height), aStride, aFormat, aUninitialized).take();
}

DrawTarget* moz2d_draw_target_create_for_data (unsigned char* aData, int32_t width, int32_t height, int32_t aStride, SurfaceFormat aFormat) {
	return gfxPlatform::GetPlatform()->CreateDrawTargetForData(aData, IntSize(width, height), aStride, aFormat).take();
}

void moz2d_draw_target_delete(DrawTarget *drawTarget) {
	delete drawTarget;
}

/* --------------------------------------------------- */
/* ------------------- T E S T I N G ----------------- */
/* --------------------------------------------------- */

bool moz2d_draw_target_is_valid(DrawTarget *drawTarget) {
	return drawTarget->IsValid();
}

bool moz2d_draw_target_is_recording(DrawTarget *drawTarget) {
	return drawTarget->IsRecording();
}

bool moz2d_draw_target_get_permit_subpixel_aa(DrawTarget *drawTarget) {
	return drawTarget->GetPermitSubpixelAA();
}

/* --------------------------------------------------- */
/* ----------------- A C C E S S I N G --------------- */
/* --------------------------------------------------- */

DrawTargetType moz2d_draw_target_get_type (DrawTarget *drawTarget) {
	return drawTarget->GetType();
}

BackendType moz2d_draw_target_get_backend_type (DrawTarget *drawTarget) {
	return drawTarget->GetBackendType();
}

SurfaceFormat moz2d_draw_target_get_surface_format (DrawTarget *drawTarget) {
	return drawTarget->GetFormat();
}

void moz2d_draw_target_get_size(DrawTarget *drawTarget, IntSize* aSize) {
	IntSize size = drawTarget->GetSize();
	aSize->width = size.width;
	aSize->height = size.height;
}




SourceSurface* moz2d_draw_target_snapshot(DrawTarget *drawTarget) {
	return drawTarget->Snapshot().take();
}

void moz2d_draw_target_flush(DrawTarget *drawTarget) {
	drawTarget->Flush();
}

void moz2d_draw_target_set_permit_subpixel_aa(DrawTarget *drawTarget, bool aPermitSubpixelAA) {
	drawTarget->SetPermitSubpixelAA(aPermitSubpixelAA);
}

uint32_t * moz2d_draw_target_get_data(DrawTarget* drawTarget) {
	RefPtr<SourceSurface> snapshot = drawTarget->Snapshot();
	RefPtr<DataSourceSurface> mDataSnapshot = snapshot->GetDataSurface();
	return (uint32_t *) mDataSnapshot->GetData();
}

/* --------------------------------------------------- */
/* ------------------- C L I P P I N G --------------- */
/* --------------------------------------------------- */
static UserDataKey sClippingBounds;

void moz2d_draw_target_pop_clip(DrawTarget* drawTarget) {
	drawTarget->PopClip();

	std::stack<Rect> *stack = static_cast<std::stack<Rect>*>(drawTarget->GetUserData(&sClippingBounds));
	if (!stack) {
		return;
	}
	if (stack->empty()) {
		return;
	}

	stack->pop();
}

/**
 * Clip is in user space
 */
void push_clip(DrawTarget* drawTarget, Rect clip) {
	Rect deviceClip = drawTarget->GetTransform().Inverse().TransformBounds(clip);

	std::stack<Rect> *stack = static_cast<std::stack<Rect>*>(drawTarget->GetUserData(&sClippingBounds));
	if (!stack) {
		stack = new std::stack<Rect>();
		drawTarget->AddUserData(&sClippingBounds, stack, nullptr);
	}
	Rect newClip = deviceClip;
	if (!stack->empty()) {
		newClip = stack->top().Intersect(deviceClip);
	}
	stack->push(newClip);
}

void moz2d_draw_target_push_clip_rectangle (DrawTarget* drawTarget, float x, float y, float width, float height) {
	Rect clip = Rect(x,y, width, height);

	drawTarget->PushClipRect(clip);

	push_clip(drawTarget, clip);
}

void moz2d_draw_target_push_clip_path (DrawTarget* drawTarget, Path* aPath) {
	drawTarget->PushClip(aPath);

	push_clip(drawTarget, aPath->GetBounds(Matrix()));
}

/**
 * Return clip in device space
 */
Rect get_clip(DrawTarget* drawTarget) {
	std::stack<Rect> *stack = static_cast<std::stack<Rect>*>(drawTarget->GetUserData(&sClippingBounds));
	if (!stack) {
		IntSize extent = drawTarget->GetSize();
		return Rect(0,0, extent.width, extent.height);
	}

	if (stack->empty()) {
		IntSize extent = drawTarget->GetSize();
		return Rect(0,0, extent.width, extent.height);
	}

	return stack->top();
}

void moz2d_draw_target_clipping_bounds_global(DrawTarget* drawTarget, Rect* rectangle) {
	Rect clip = get_clip(drawTarget);

	rectangle->x = clip.x;
	rectangle->y = clip.y;
	rectangle->width = clip.width;
	rectangle->height = clip.height;
}

void moz2d_draw_target_clipping_bounds_local(DrawTarget* drawTarget, Rect* rectangle) {
	Rect clip = drawTarget->GetTransform().TransformBounds(get_clip(drawTarget));

	rectangle->x = clip.x;
	rectangle->y = clip.y;
	rectangle->width = clip.width;
	rectangle->height = clip.height;
}

/* --------------------------------------------------- */
/* -------------------- D R A W I N G ---------------- */
/* --------------------------------------------------- */

/*
 * Fill
 */
void moz2d_draw_target_fill_rect(DrawTarget* drawTarget, Rect* rect, Pattern* pattern, DrawOptions* drawOptions) {
	drawTarget->FillRect(*rect, *pattern, *drawOptions);
}

void moz2d_draw_target_fill_path(DrawTarget* drawTarget, Path* path, Pattern* pattern, DrawOptions* drawOptions) {
	drawTarget->Fill(path, *pattern, *drawOptions);
}

/*
 * Stroke
 */
void moz2d_draw_target_stroke_rect(DrawTarget* drawTarget, Rect* rect, Pattern* pattern, StrokeOptions* strokeOptions, DrawOptions* drawOptions) {
	drawTarget->StrokeRect(*rect, *pattern, *strokeOptions, *drawOptions);
}

void moz2d_draw_target_stroke_path(DrawTarget* drawTarget, Path* path, Pattern* pattern, StrokeOptions* strokeOptions, DrawOptions* drawOptions) {
	drawTarget->Stroke(path, *pattern, *strokeOptions, *drawOptions);
}

/*
 * Mask
 */
void moz2d_draw_target_mask_pattern(DrawTarget* drawTarget, Pattern* aSource, Pattern* aMask, DrawOptions* aOptions) {
	drawTarget->Mask(*aSource, *aMask, *aOptions);
}

void moz2d_draw_target_mask_surface(DrawTarget* drawTarget, Pattern* aSource, SourceSurface *aMask, Float offsetX, Float offsetY, DrawOptions* aOptions) {
	drawTarget->MaskSurface(*aSource, aMask, Point(offsetX, offsetY), *aOptions);
}

/*
 * Filter
 */
void moz2d_draw_target_draw_filter(DrawTarget* drawTarget, FilterNode* aFilter, Rect* sourceRect, Float destX, Float destY, DrawOptions* drawOptions) {
	drawTarget->DrawFilter(aFilter, *sourceRect, Point(destX, destY), *drawOptions);
}

/*
 * Path
 */
PathBuilder* moz2d_draw_target_create_path_builder(DrawTarget* drawTarget, FillRule aFillRule) {
	return drawTarget->CreatePathBuilder(aFillRule).take();
}

/* --------------------------------------------------- */
/* ----------- T R A N S F O R M A T I O N ----------- */
/* --------------------------------------------------- */

void moz2d_draw_target_transform_get(DrawTarget* drawTarget, float* array) {
	Matrix matrix = drawTarget->GetTransform();
	array[0] = matrix._11;
	array[1] = matrix._12;
	array[2] = matrix._21;
	array[3] = matrix._22;
	array[4] = matrix._31;
	array[5] = matrix._32;
}

void moz2d_draw_target_transform_set(DrawTarget* drawTarget, float* rawMatrix) {
	drawTarget->SetTransform(Matrix(rawMatrix[0],rawMatrix[1],rawMatrix[2],rawMatrix[3],rawMatrix[4],rawMatrix[5]));
}

void moz2d_draw_target_transform_concatenate(DrawTarget* drawTarget, float* rawMatrix) {
	drawTarget->ConcatTransform(Matrix(rawMatrix[0],rawMatrix[1],rawMatrix[2],rawMatrix[3],rawMatrix[4],rawMatrix[5]));
}

static UserDataKey sTransformKey;

void moz2d_draw_target_transform_push(DrawTarget* drawTarget) {
	Matrix transform = drawTarget->GetTransform().Copy();

	std::stack<Matrix> *stack = static_cast<std::stack<Matrix>*>(drawTarget->GetUserData(&sTransformKey));
	if (!stack) {
		stack = new std::stack<Matrix>();
		drawTarget->AddUserData(&sTransformKey, stack, nullptr);
	}

	stack->push(transform);
}

void moz2d_draw_target_transform_pop(DrawTarget* drawTarget) {
	std::stack<Matrix> *stack = static_cast<std::stack<Matrix>*>(drawTarget->GetUserData(&sTransformKey));
	if (!stack) {
		return;
	}

	if (stack->empty()) {
		return;
	}

	Matrix transform = stack->top();
	stack->pop();
	drawTarget->SetTransform(transform);
}
