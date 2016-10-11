/*
 * filter_node.h
 *
 *  Created on: Jun 4, 2016
 *      Author: aliaksei
 */

#ifndef FILTER_NODE_H_
#define FILTER_NODE_H_

#include "exports.h"
#include "2d/2D.h"
#include "2d/Filters.h"
#include "2d/FilterNodeSoftware.h"

using namespace mozilla::gfx;

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API FilterNode* moz2d_filter_node_create(DrawTarget *drawTarget, FilterType filterType);
LIBRARY_API void moz2d_filter_node_set_input_filter(FilterNode *filterNode, uint32_t aIndex, FilterNode* aSourceFilter);
LIBRARY_API void moz2d_filter_node_set_input_surface(FilterNode *filterNode, uint32_t aIndex, SourceSurface* aSurface);
LIBRARY_API void moz2d_filter_node_set_attribute_float(FilterNode *filterNode, uint32_t aIndex, Float value);
LIBRARY_API void moz2d_filter_node_set_attribute_int(FilterNode *filterNode, uint32_t aIndex, uint32_t value);
LIBRARY_API void moz2d_filter_node_set_attribute_bool(FilterNode *filterNode, uint32_t aIndex, bool value);
LIBRARY_API void moz2d_filter_node_set_attribute_size(FilterNode *filterNode, uint32_t aIndex, Float width, Float height);
LIBRARY_API void moz2d_filter_node_set_attribute_int_size(FilterNode *filterNode, uint32_t aIndex, int32_t width, int32_t height);
LIBRARY_API void moz2d_filter_node_set_attribute_int_rect(FilterNode *filterNode, uint32_t aIndex, int32_t x, int32_t y, int32_t width, int32_t height);
LIBRARY_API void moz2d_filter_node_set_attribute_color(FilterNode *filterNode, uint32_t aIndex, Float r, Float g, Float b, Float a);
LIBRARY_API void moz2d_filter_node_set_attribute_float_array(FilterNode *filterNode, uint32_t aIndex, const Float* aFloats, uint32_t aSize);
LIBRARY_API void moz2d_filter_node_set_attribute_matrix(FilterNode *filterNode, uint32_t aIndex, Float a11, Float a12, Float a21, Float a22, Float a31, Float a32);
LIBRARY_API void moz2d_filter_node_set_attribute_matrix5x4(FilterNode *filterNode, uint32_t aIndex, const Float* elements);
LIBRARY_API void moz2d_filter_node_set_attribute_point_3d(FilterNode *filterNode, uint32_t aIndex, Float x, Float y, Float z);

#ifdef __cplusplus
}
#endif

#endif /* FILTER_NODE_H_ */
