/*
 * rect.cpp
 *
 *  Created on: Jun 4, 2016
 *      Author: aliaksei
 */

#include "rect.h"

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
