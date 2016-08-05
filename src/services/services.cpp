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


void moz2d_services_init() {
	moz2d_services_init_atom_table();
	moz2d_services_init_platform();
}

void moz2d_services_shutdown() {
	moz2d_services_shutdown_platform();
	moz2d_services_shutdown_atom_table();
}

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
