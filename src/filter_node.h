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
#include "2d/FilterNodeSoftware.h"

using namespace mozilla::gfx;

template<typename Tag, typename Tag::type M>
struct Rob {
  friend typename Tag::type get(Tag) {
    return M;
  }
};

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
void moz2d_filter_node_set_attribute_size(FilterNode *filterNode, uint32_t aIndex, Float width, Float height);
void moz2d_filter_node_set_attribute_int_size(FilterNode *filterNode, uint32_t aIndex, int32_t width, int32_t height);
void moz2d_filter_node_set_attribute_int_rect(FilterNode *filterNode, uint32_t aIndex, int32_t x, int32_t y, int32_t width, int32_t height);
void moz2d_filter_node_set_attribute_color(FilterNode *filterNode, uint32_t aIndex, Float r, Float g, Float b, Float a);
void moz2d_filter_node_set_attribute_float_array(FilterNode *filterNode, uint32_t aIndex, const Float* aFloats, uint32_t aSize);

uint32_t moz2d_filter_node_turbulence_software_get_num_octaves(FilterNodeTurbulenceSoftware *filterNode);
uint32_t moz2d_filter_node_turbulence_software_get_seed(FilterNodeTurbulenceSoftware *filterNode);
Float moz2d_filter_node_turbulence_software_get_frequency_width(FilterNodeTurbulenceSoftware *filterNode);
Float moz2d_filter_node_turbulence_software_get_frequency_height(FilterNodeTurbulenceSoftware *filterNode);
int32_t moz2d_filter_node_turbulence_software_get_rect_x(FilterNodeTurbulenceSoftware *filterNode);
int32_t moz2d_filter_node_turbulence_software_get_rect_y(FilterNodeTurbulenceSoftware *filterNode);
int32_t moz2d_filter_node_turbulence_software_get_rect_width(FilterNodeTurbulenceSoftware *filterNode);
int32_t moz2d_filter_node_turbulence_software_get_rect_height(FilterNodeTurbulenceSoftware *filterNode);

#ifdef __cplusplus
}
#endif

#endif /* FILTER_NODE_H_ */
