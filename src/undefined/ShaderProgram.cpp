/*
 * ShaderProgram.cpp
 *
 *  Created on: Aug 5, 2016
 *      Author: Aliaksei Syrel
 */

#include "OGLShaderProgram.h"

using namespace mozilla::layers;
using namespace mozilla;

//static inline ShaderConfigOGL
//ShaderConfigFromTargetAndFormat(GLenum aTarget,
//                                gfx::SurfaceFormat aFormat)
//{
//  ShaderConfigOGL config;
//  config.SetTextureTarget(aTarget);
//  config.SetRBSwap(aFormat == gfx::SurfaceFormat::B8G8R8A8 ||
//                   aFormat == gfx::SurfaceFormat::B8G8R8X8);
//  config.SetNoAlpha(aFormat == gfx::SurfaceFormat::B8G8R8X8 ||
//                    aFormat == gfx::SurfaceFormat::R8G8B8X8 ||
//                    aFormat == gfx::SurfaceFormat::R5G6B5_UINT16);
//  return config;
//}
