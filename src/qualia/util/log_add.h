/*
 * log_add.h
 *
 * This file is part of Qualia https://github.com/sofian/qualia
 *
 * (c) 2011 Sofian Audry -- info(@)sofianaudry(.)com
 *
 * Adapted from Torch 3.1 library
 * Copyright (C) 2003--2004 Samy Bengio (bengio@idiap.ch)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LOG_ADD_INC
#define LOG_ADD_INC

#include <qualia/core/common.h>

#include <math.h>

#ifndef log1p
#warning "log1p is not defined in this environment which might result in problems on your system"
#define log1p(x) log(1+(x))
#endif

/** Some simple functions for log operations.

    @author Samy Bengio (bengio@idiap.ch)
*/
//@{
#define LOG_2_PI 1.83787706640934548355
#define LOG_ZERO -INF
#define LOG_ONE 0

/** logAdd(log_a,log_b) = log(a+b) = log(exp(log_a)+exp(log_b))
    but done in a smart way so that if log_a or log_b are large
    but not their difference the computation works correctly.
*/
real logAdd(real log_a,real log_b);

/// logSub(log_a,log_b) = log(a-b)
real logSub(real log_a,real log_b);
//@}

#endif
