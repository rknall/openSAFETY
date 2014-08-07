/**
 * \file getopt.h
 *
 * \addtogroup CROSSWIN32
 * \{
 * \addtogroup osswin
 * \{
 * \details
 * \{
 * \}
 * \author Roland Knall, Bernecker + Rainer Industrie-Elektronik Ges.m.b.H
 */


#ifndef GETOPT_H
#define GETOPT_H

#include "config.h"

#include <string.h>
#include <errno.h>

#if !(osswin_EXPORTS)
extern char * optarg;		// global argument pointer
extern int optind; 	// global argv index
extern int opterr;
extern int optopt;

extern int getopt(int nargc, char* const nargv[], const char* ostr);
#endif

#endif //GETOPT_H

/**
 * \}
 * \}
 */
