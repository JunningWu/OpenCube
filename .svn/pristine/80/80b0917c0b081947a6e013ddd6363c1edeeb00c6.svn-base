/*
 * ExpatMM - C++ Wrapper for Expat available at http://expat.sourceforge.net/
 * Copyright (c) 2007, 2008 IntelliTree Solutions llc
 * Author: Coleman Kane <ckane@intellitree.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, please feel free
 * to contact the author listed above.
 */

#include <cstdio>
#include <string>
#include "expat.h"
#include "expatmm.h"

using namespace expatmm;

std::string
expatmm::getExpatMMVersion(void) {
  char buf[24];
  snprintf(buf, 24, "%d.%d.%d", EXPATMM_LIBRARY_MAJOR, EXPATMM_LIBRARY_MINOR, EXPATMM_LIBRARY_REVISION);
  return std::string(buf);
}
