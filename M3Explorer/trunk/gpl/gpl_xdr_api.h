/* @GPL_LICENSE_START@
 *
 * Authors: Vittorio Zaccaria, Gianluca Palermo, Giovanni Mariani, Fabrizio Castro
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

#ifndef GPL_XDR_API
#define GPL_XDR_API

#include <vector>
#include <map>
#include <fstream>

using namespace std;
bool gpl_xdr_read_design(std::ifstream &, vector<double> &, vector<double> &);
void gpl_xdr_write_vector(std::ofstream &, vector<double> & x);
void gpl_xdr_write_raw_data(ofstream &fout, vector<double> & data, vector<double> & values);

#define GO_AHEAD(fin) (fin.good() && !fin.eof())

#endif
