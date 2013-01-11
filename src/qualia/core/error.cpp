/* error.cpp
 *
 * This file is part of Qualia https://github.com/sofian/qualia
 *
 * (c) 2011 Sofian Audry -- info(@)sofianaudry(.)com
 * This file was originally part of Drone (http://drone.ws)
 *   (c) 2004 Mathieu Guindon, Julien Keable, Jean-Sebastien Senecal
 * Error messages code adapted from Torch
 *   (c) 2004 Ronan Collobert (collober@idiap.ch)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "error.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

// These implementations were taken from Torch 3 (http://www.torch.ch).

char xxpetit_message_pour_melanie[10000];

#if DEBUG_ERROR
void ASSERT_ERROR_MESSAGE(bool expr, const char* msg, ...)
{
  if (!expr)
  {
    va_list ap;
    va_start(ap, msg);
    errormsg(msg, ap);
    va_end(ap);
  }
}
#endif

#if DEBUG_WARNING
void ASSERT_WARNING_MESSAGE(bool expr, const char* msg, ...)
{
  if (!expr)
  {
    va_list ap;
    va_start(ap, msg);
    warningmsg(msg, ap);
    va_end(ap);
  }
}
#endif

#if DEBUG_NOTICE
void ASSERT_NOTICE_MESSAGE(bool expr, const char* msg, ...)
{
  if (!expr)
  {
    va_list ap;
    va_start(ap, msg);
    noticemsg(msg, ap);
    va_end(ap);
  }
}
#endif

void errormsg(const char* msg, ...)
{
  va_list args;
  va_start(args,msg);
  vsprintf(xxpetit_message_pour_melanie, msg, args);
  printf("\nE: %s\n\n", xxpetit_message_pour_melanie);
  fflush(stdout);
  va_end(args);
  exit(-1);
}

void warningmsg(const char* msg, ...)
{
  va_list args;
  va_start(args,msg);
  vsprintf(xxpetit_message_pour_melanie, msg, args);
  printf("W: %s\n", xxpetit_message_pour_melanie);
  fflush(stdout);
  va_end(args);
}

void noticemsg(const char* msg, ...)
{
  va_list args;
  va_start(args,msg);
  vsprintf(xxpetit_message_pour_melanie, msg, args);
  printf("N: %s\n",  xxpetit_message_pour_melanie);
  fflush(stdout);
  va_end(args);
}

void message(const char* msg, ...)
{
  va_list args;
  va_start(args,msg);
  vsprintf(xxpetit_message_pour_melanie, msg, args);
  printf("# %s\n", xxpetit_message_pour_melanie);
  fflush(stdout);
  va_end(args);
}

void print(const char* msg, ...)
{
  va_list args;
  va_start(args,msg);
  vsprintf(xxpetit_message_pour_melanie, msg, args);
  printf("%s", xxpetit_message_pour_melanie);
  fflush(stdout);
  va_end(args);
}
