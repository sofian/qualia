/*
 * log_add.c
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
#include "log_add.h"

#ifdef USE_DOUBLE
#define MINUS_LOG_THRESHOLD -39.14
#else
#define MINUS_LOG_THRESHOLD -18.42
#endif

real logAdd(real log_a, real log_b)
{
  if (log_a < log_b) {
    real tmp = log_a;
    log_a = log_b;
    log_b = tmp;
  }

  real minusdif = log_b - log_a;

  Q_ASSERT_ERROR_MESSAGE (!isnan(minusdif), "LogAdd: minusdif (%f) log_b (%f) or log_a (%f) is nan",minusdif,log_b,log_a);

  if (minusdif < MINUS_LOG_THRESHOLD)
    return log_a;
  else
    return log_a + log1p(exp(minusdif));
}

real logSub(real log_a, real log_b)
{
  Q_ASSERT_ERROR_MESSAGE (log_a >= log_b, "LogSub: log_a (%f) should be greater than log_b (%f)", log_a, log_b);

  real minusdif = log_b - log_a;

  Q_ASSERT_ERROR_MESSAGE (!isnan(minusdif), "LogSub: minusdif (%f) log_b (%f) or log_a (%f) is nan",minusdif,log_b,log_a);

  if (log_a == log_b)
    return LOG_ZERO;
  else if (minusdif < MINUS_LOG_THRESHOLD)
    return log_a;
  else
    return log_a + log1p(-exp(minusdif));
}
