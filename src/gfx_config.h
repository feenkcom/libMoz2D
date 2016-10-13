/*
 * gfx_config.h
 *
 *  Created on: Oct 12, 2016
 *      Author: Aliaksei Syrel
 */

#ifndef GFX_CONFIG_H_
#define GFX_CONFIG_H_

#include "exports.h"
#include "config/gfxConfig.h"

using namespace mozilla::gfx;

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API void moz_gfx_config_for_each_feature(void(*aCallback)(const char*,const char*, FeatureState&));
LIBRARY_API bool moz_gfx_config_feature_is_enabled(FeatureState* aFeature);
LIBRARY_API FeatureState& moz_gfx_config_get_feature(Feature aFeature);
LIBRARY_API FeatureStatus moz_gfx_config_get_feature_status(FeatureState* aFeature);
LIBRARY_API void moz_gfx_config_feature_enable_by_default(FeatureState* aFeature);

#ifdef __cplusplus
}
#endif

#endif /* GFX_CONFIG_H_ */
