/*
 * services.cpp
 *
 *  Created on: Jul 31, 2016
 *      Author: Aliaksei Syrel
 */

#include "services.h"

#include "nsGkAtoms.h"
#include "nsAtomService.h"
#include "nsAtomTable.h"
#include "gfxPlatform.h"
#include "gfxConfig.h"

void moz2d_services_init_platform() {
	gfxPlatform::GetPlatform();
}

void moz2d_services_shutdown_platform() {
	gfxPlatform::Shutdown();
}

void moz2d_services_init_atom_table() {
	NS_InitAtomTable();
	nsGkAtoms::AddRefAtoms();
}

void moz2d_services_shutdown_atom_table() {
	NS_ShutdownAtomTable();
}

void moz2d_services_init_gfx_config() {
	mozilla::gfx::gfxConfig::Init();
}

void moz2d_services_shutdown_gfx_config() {
	mozilla::gfx::gfxConfig::Shutdown();
}