/*
 * services.h
 *
 *  Created on: Jul 31, 2016
 *      Author: Aliaksei Syrel
 */

#ifndef SERVICES_H_
#define SERVICES_H_

#include "exports.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Initialize Thebes and start platform dependent services
 * such as gfxPlatformList and others
 */
LIBRARY_API void moz2d_services_init_platform();

/*
 * Shutdown Thebes and free resources
 */
LIBRARY_API void moz2d_services_shutdown_platform();

/*
 * Initialize nsAtom table and register all atoms
 */
LIBRARY_API void moz2d_services_init_atom_table();

/*
 * Shutdown and clean atom table
 */
LIBRARY_API void moz2d_services_shutdown_atom_table();

LIBRARY_API void moz2d_services_init_gfx_config();
LIBRARY_API void moz2d_services_shutdown_gfx_config();

#ifdef __cplusplus
}
#endif


#endif /* SERVICES_H_ */
