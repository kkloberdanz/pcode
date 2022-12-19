/*	$NetBSD: getopt.c,v 1.26 2003/08/07 16:43:40 agc Exp $	*/

/*
 * Copyright (c) 1987, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vm_getopt.h"

int vm_opterr = 1,     /* if error message should be printed */
    vm_optind = 1,     /* index into parent argv vector */
    vm_optopt,         /* character checked for validity */
    vm_optreset;       /* reset getopt */
const char *vm_optarg; /* argument associated with option */

#define BADCH (int)'?'
#define BADARG (int)':'
#define EMSG ""

/*
 * getopt --
 *	Parse argc/argv argument vector.
 */
int vm_getopt(int nargc, char *const nargv[], const char *ostr) {
    static const char *place = EMSG; /* option letter processing */
    char *oli;                       /* option letter list index */

    if (vm_optreset || *place == 0) { /* update scanning pointer */
        vm_optreset = 0;
        place = nargv[vm_optind];
        if (vm_optind >= nargc || *place++ != '-') {
            /* Argument is absent or is not an option */
            place = EMSG;
            return (-1);
        }
        vm_optopt = *place++;
        if (vm_optopt == '-' && *place == 0) {
            /* "--" => end of options */
            ++vm_optind;
            place = EMSG;
            return (-1);
        }
        if (vm_optopt == 0) {
            /* Solitary '-', treat as a '-' option
               if the program (eg su) is looking for it. */
            place = EMSG;
            if (strchr(ostr, '-') == NULL)
                return (-1);
            vm_optopt = '-';
        }
    } else
        vm_optopt = *place++;

    /* See if option letter is one the caller wanted... */
    if (vm_optopt == ':' || (oli = strchr(ostr, vm_optopt)) == NULL) {
        if (*place == 0)
            ++vm_optind;
        if (vm_opterr && *ostr != ':')
            (void)fprintf(stderr, "vm: illegal option -- %c\n", vm_optopt);
        return (BADCH);
    }

    /* Does this option need an argument? */
    if (oli[1] != ':') {
        /* don't need argument */
        vm_optarg = NULL;
        if (*place == 0)
            ++vm_optind;
    } else {
        /* Option-argument is either the rest of this argument or the
           entire next argument. */
        if (*place)
            vm_optarg = place;
        else if (nargc > ++vm_optind)
            vm_optarg = nargv[vm_optind];
        else {
            /* option-argument absent */
            place = EMSG;
            if (*ostr == ':')
                return (BADARG);
            if (vm_opterr)
                (void)fprintf(
                    stderr,
                    "vm: option requires an argument -- %c\n",
                    vm_optopt
                );
            return (BADCH);
        }
        place = EMSG;
        ++vm_optind;
    }
    return (vm_optopt); /* return option letter */
}
