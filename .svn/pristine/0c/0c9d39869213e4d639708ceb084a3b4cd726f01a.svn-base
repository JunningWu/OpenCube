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

#ifndef R_WRAPPER_HPP
#define R_WRAPPER_HPP

#include "parameters.h"
#include <string>
#include <vector>

using namespace std;
class r_wrapper
{
 public:
  r_wrapper(Parameters& params,vector<vector<double> > training_configurations, vector<vector<double> > training_observations);
  r_wrapper(Parameters& params,vector<vector<double> > training_configurations, vector<vector<double> > training_observations,vector<vector<double> > prediction_configurations);
  ~r_wrapper();
  void start();
 private:
  void delete_temp();
  void delete_temp_prob();
  vector<vector<double> > training_configurations;
  vector<vector<double> > training_observations;
  vector<vector<double> > prediction_configurations;
  Parameters param;
};
#endif
