/*
 * filter_node.cpp
 *
 *  Created on: Jun 4, 2016
 *      Author: aliaksei
 */

#include "filter_node.h"

FilterNode* moz2d_filter_node_create(DrawTarget* drawTarget, FilterType filterType) {
	return drawTarget->CreateFilter(filterType).take();
}

void moz2d_filter_node_set_input_filter(FilterNode *filterNode, uint32_t aIndex, FilterNode* aSourceFilter) {
	filterNode->SetInput(aIndex, aSourceFilter);
}

void moz2d_filter_node_set_input_surface(FilterNode* filterNode, uint32_t aIndex, SourceSurface* aSurface) {
	filterNode->SetInput(aIndex, aSurface);
}

void moz2d_filter_node_set_attribute_float(FilterNode* filterNode, uint32_t aIndex, Float value) {
	filterNode->SetAttribute(aIndex, value);
}

void moz2d_filter_node_set_attribute_int(FilterNode *filterNode, uint32_t aIndex, uint32_t value) {
	filterNode->SetAttribute(aIndex, value);
}

void moz2d_filter_node_set_attribute_bool(FilterNode *filterNode, uint32_t aIndex, bool value) {
	filterNode->SetAttribute(aIndex, value);
}

void moz2d_filter_node_set_attribute_size(FilterNode *filterNode, uint32_t aIndex, Float width, Float height) {
	filterNode->SetAttribute(aIndex, Size(width, height));
}

void moz2d_filter_node_set_attribute_int_size(FilterNode *filterNode, uint32_t aIndex, int32_t width, int32_t height) {
	filterNode->SetAttribute(aIndex, IntSize(width, height));
}

void moz2d_filter_node_set_attribute_int_rect(FilterNode *filterNode, uint32_t aIndex, int32_t x, int32_t y, int32_t width, int32_t height) {
	filterNode->SetAttribute(aIndex, IntRect(x, y, width, height));
}

void moz2d_filter_node_set_attribute_color(FilterNode *filterNode, uint32_t aIndex, Float r, Float g, Float b, Float a) {
	filterNode->SetAttribute(aIndex, Color(r,g,b,a));
}

void moz2d_filter_node_set_attribute_float_array(FilterNode *filterNode, uint32_t aIndex, const Float* aFloats, uint32_t aSize) {
	filterNode->SetAttribute(aIndex, aFloats, aSize);
}

void moz2d_filter_node_set_attribute_matrix(FilterNode *filterNode, uint32_t aIndex, Float a11, Float a12, Float a21, Float a22, Float a31, Float a32) {
	filterNode->SetAttribute(aIndex, Matrix(a11, a12, a21, a22, a31, a32));
}

void moz2d_filter_node_set_attribute_matrix5x4(FilterNode *filterNode, uint32_t aIndex, const Float* a) {
	filterNode->SetAttribute(aIndex, Matrix5x4(
		a[0],  a[5],  a[10],  a[15],
		a[1],  a[6],  a[11],  a[16],
		a[2],  a[7],  a[12],  a[17],
		a[3],  a[8],  a[13],  a[18],
		a[4],  a[9],  a[14],  a[19]));
}

void moz2d_filter_node_set_attribute_point_3d(FilterNode *filterNode, uint32_t aIndex, Float x, Float y, Float z) {
	filterNode->SetAttribute(aIndex, Point3D(x,y,z));
}