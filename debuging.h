/** debuging.h --- General file for print debug information
 *
 * Copyright (C) 2011 Egorov N.V. <khenargot@gmail.com>
 *
 * Author: Egorov N.V. <egorov@rti-mints.ru>
 * $Id: debuging.h,v 0c3654d776ba  2012/05/28 14:10:40  egorov@rti-mints.ru $
 *

 * Redistribution and use in source and binary forms,
 * with or without modificat ion, are permitted provided that the
 * following conditions are met:
 * 
 *  * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 
 *  * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 *  * Neither the name of the <ORGANIZATION> nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#ifndef DEBUGING_H
#define DEBUGING_H 1

#define DEBUGMSG1(message)                                              
#define DEBUGOUT1(string, ...)                                 
#define DEBUGPARAMETRS1(enumeration, ...)                               
#define DEBUGMSG2(message)                                              
#define DEBUGOUT2(string, ...)                                 
#define DEBUGPARAMETRS2(enumeration, ...)                               

#ifndef NDEBUG

#ifndef OUTSTREAM
#define OUTSTREAM stderr
#endif

#define DEBUGMSG(message)                                               \
    fprintf(OUTSTREAM,"(DD)[%s@%d]: %s\n", __FILE__, __LINE__, message)
#define DEBUGOUT(string, ...) {                                \
    fprintf(OUTSTREAM, "(DD)[%s@%d]: ", __FILE__, __LINE__);   \
    fprintf(OUTSTREAM, string, ##__VA_ARGS__);                 \
    fputc('\n', OUTSTREAM); }
#define DEBUGPARAMETRS(enumeration, ...){                               \
    fprintf(OUTSTREAM, "(DD)[%s@%d]{%s}->{", __FILE__, __LINE__, #__VA_ARGS__); \
    fprintf(OUTSTREAM, enumeration, ##__VA_ARGS__);                     \
    fputs("}\n", OUTSTREAM); }
#define perror(string)                                                  \
    fprintf(stderr, "(EE)[%s@%d]: %s: %s\n", __FILE__, __LINE__, string, strerror(errno))
#if DEBUGLVL >= 1
#undef  DEBUGMSG1
#define DEBUGMSG1(message)                                              \
    fprintf(OUTSTREAM,"(DD)[%s@%d]: %s\n", __FILE__, __LINE__, message)
#undef  DEBUGOUT1
#define DEBUGOUT1(string, ...){                                \
    fprintf(OUTSTREAM, "(DD)[%s@%d]: ", __FILE__, __LINE__);   \
    fprintf(OUTSTREAM, string, ##__VA_ARGS__);                 \
    fputc('\n', OUTSTREAM); }
#undef  DEBUGPARAMETRS1
#define DEBUGPARAMETRS1(enumeration, ...) {                             \
    fprintf(OUTSTREAM, "(DD)[%s@%d]{%s}->{", __FILE__, __LINE__, #__VA_ARGS__); \
    fprintf(OUTSTREAM, enumeration, ##__VA_ARGS__);                     \
    fputs("}\n", OUTSTREAM); }
#endif // DEBUGLVL >=1
#if DEBUGLVL >= 2
#pragma message "Level two of debug is activated"
#undef  DEBUGMSG2
#define DEBUGMSG2(message)                                              \
    fprintf(OUTSTREAM,"(DD)[%s@%d]: %s\n", __FILE__, __LINE__, message)
#undef  DEBUGOUT2
#define DEBUGOUT2(string, ...) {                               \
    fprintf(OUTSTREAM, "(DD)[%s@%d]: ", __FILE__, __LINE__);   \
    fprintf(OUTSTREAM, string, ##__VA_ARGS__);                 \
    fputc('\n', OUTSTREAM); }
#undef  DEBUGPARAMETRS2
#define DEBUGPARAMETRS2(enumeration, ...) {                             \
    fprintf(OUTSTREAM, "(DD)[%s@%d]{%s}->{", __FILE__, __LINE__, #__VA_ARGS__); \
    fprintf(OUTSTREAM, enumeration, ##__VA_ARGS__);                     \
    fputs("}\n", OUTSTREAM); }
#endif // DEBUGLVL >= 2
#else //NDEBUG
#define DEBUGMSG(message)                                               
#define DEBUGOUT(string, ...)                                  
#define DEBUGPARAMETRS(enumeration, ...)                                
#endif //NDEBUG

#endif // DEBUGING_H

