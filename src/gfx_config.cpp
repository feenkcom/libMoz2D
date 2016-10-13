/*
 * gfx_config.cpp
 *
 *  Created on: Oct 12, 2016
 *      Author: Aliaksei Syrel
 */

#include "gfx_config.h"

void moz_gfx_config_for_each_feature(void(*aCallback)(const char*, const char*, FeatureState&)) {
	gfxConfig::ForEachFeature(gfxConfig::FeatureIterCallback(aCallback));
}

bool moz_gfx_config_feature_is_enabled(FeatureState* aFeature) {
	return aFeature->IsEnabled();
}

FeatureState& moz_gfx_config_get_feature(Feature aFeature) {
	return gfxConfig::GetFeature(aFeature);
}

FeatureStatus moz_gfx_config_get_feature_status(FeatureState* aFeature) {
	return aFeature->GetValue();
}

void moz_gfx_config_feature_enable_by_default(FeatureState* aFeature) {
	aFeature->EnableByDefault();
}