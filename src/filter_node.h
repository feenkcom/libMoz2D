/*
 * filter_node.h
 *
 *  Created on: Jun 4, 2016
 *      Author: aliaksei
 */

#ifndef FILTER_NODE_H_
#define FILTER_NODE_H_

#include "2d/2D.h"
#include "2d/Filters.h"

using namespace mozilla::gfx;

#ifdef __cplusplus
extern "C" {
#endif

FilterNode* moz2d_filter_node_create(DrawTarget *drawTarget, FilterType filterType);
void moz2d_filter_node_delete(FilterNode *filterNode);
void moz2d_filter_node_set_input_filter(FilterNode *filterNode, uint32_t aIndex, FilterNode* aSourceFilter);
void moz2d_filter_node_set_input_surface(FilterNode *filterNode, uint32_t aIndex, SourceSurface* aSurface);
void moz2d_filter_node_set_attribute_float(FilterNode *filterNode, uint32_t aIndex, Float value);
void moz2d_filter_node_set_attribute_int(FilterNode *filterNode, uint32_t aIndex, uint32_t value);
void moz2d_filter_node_set_attribute_bool(FilterNode *filterNode, uint32_t aIndex, bool value);
void moz2d_filter_node_set_attribute_size(FilterNode *filterNode, uint32_t aIndex, Size value);
void moz2d_filter_node_set_attribute_int_rect(FilterNode *filterNode, uint32_t aIndex, IntRect value);

#ifdef __cplusplus
}
#endif

#endif /* FILTER_NODE_H_ */
