/*
 * exports.h
 *
 *  Created on: Oct 11, 2016
 *      Author: aliaksei
 */

#ifndef EXPORTS_H_
#define EXPORTS_H_

#ifdef _WIN32
#    ifdef LIBRARY_EXPORTS
#        define LIBRARY_API __declspec(dllexport)
#    else
#        define LIBRARY_API __declspec(dllimport)
#    endif
#elif
#    define LIBRARY_API
#endif

#endif /* EXPORTS_H_ */