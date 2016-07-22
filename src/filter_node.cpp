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

void moz2d_filter_node_delete(FilterNode* filterNode) {
	delete filterNode;
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

void moz2d_filter_node_set_attribute_size(FilterNode *filterNode, uint32_t aIndex, Size value) {
	filterNode->SetAttribute(aIndex, value);
}

void moz2d_filter_node_set_attribute_int_rect(FilterNode *filterNode, uint32_t aIndex, IntRect value) {
	filterNode->SetAttribute(aIndex, value);
}
