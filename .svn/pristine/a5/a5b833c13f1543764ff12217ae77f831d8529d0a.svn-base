/* @GPL_LICENSE_START@
 *
 * Authors: Vittorio Zaccaria, Gianluca Palermo, Giovanni Mariani, Fabrizio Castro, Andrea Cuoccio (2009)
 * Copyright (c) 2008-2009, Politecnico di Milano, Universita' della Svizzera italiana
 * All rights reserved.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * @GPL_LICENSE_END@ */

/* @additional_authors @, Andrea Cuoccio (2009)@ */


#ifndef CONVERTER_H
#define CONVERTER_H
#include <vector>
#include <string>

using namespace std;

class converter
{
 public:
  //configuration and observation refers to training data set in which configuration contains the values of the domains 
  //for a corresponding observation
  //pred configuration contains the points in which evaluate the predicted function
  converter();
  converter(string obs_name,string pred_conf_name);
  ~converter();
  void convert_in(vector<vector<double> > configuration,vector<vector<double> > observation,vector<vector<double> > pred_configuration);
  void convert_out(string output);
};

#ifndef GOON_FUN
#define GOON_FUN
#define GOON(x) (x.good() && !x.eof())
#endif

#endif
