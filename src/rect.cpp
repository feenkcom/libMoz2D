/*
 * rect.cpp
 *
 *  Created on: Jun 4, 2016
 *      Author: aliaksei
 */

#include "rect.h"

int32_t moz2d_int_rect_x(IntRect* rect) {
	return rect->x;
}

int32_t moz2d_int_rect_y(IntRect* rect){
	return rect->y;
}

int32_t moz2d_Int_rect_width(IntRect* rect) {
	return rect->width;
}

int32_t moz2d_Int_rect_height(IntRect* rect) {
	return rect->height;
}

bool moz2d_rect_is_empty(Rect* rect) {
	return rect->IsEmpty();
}

void moz2d_rect_set_empty(Rect* rect) {
	rect->SetEmpty();
}

bool moz2d_rect_is_finite(Rect* rect) {
	return rect->IsFinite();
}

bool moz2d_rect_contains_rect(Rect* rect, Rect* anotherRect) {
	return rect->Contains(*anotherRect);
}

bool moz2d_rect_contains(Rect* rect, Float aX, Float aY) {
	return rect->Contains(aX, aY);
}

bool moz2d_rect_contains_point(Rect* rect, Point *aPoint) {
	return rect->Contains(*aPoint);
}

void moz2d_rect_set_rect(Rect* rect, Float aX, Float aY, Float aWidth, Float aHeight) {
	rect->SetRect(aX, aY, aWidth, aHeight);
}
