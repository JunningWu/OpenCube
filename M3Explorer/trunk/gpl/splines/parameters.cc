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

#include "parameters.h"
#include <iostream>
using namespace std;


bool Parameters::isOption(const std::string& name) const{
	return Options.find(name) != Options.end();
}

bool Parameters::isOption(const char* name) const{
	return isOption(std::string(name));
}

void Parameters::help()
{
 cout << "R spline script generator\n"; 
 cout << "Usage: rsm [options] file..\n";
 cout << "Options:\n";
 cout << "--help 			: show this help\n";
 cout << "--trai 			: specify the training set file (required)\n";
 cout << "--pred 			: specify the prediction set file\n";
 cout << "--inte 			: specify the interaction file\n";
 cout << "--subs 			: specify a subset of the training set file to be used as training, the remainig is used as prediction set\n";
 cout << "--lmco 			: specify to compare spline model with linear models of the 1st and 2nd order\n";
 cout << "--eran			: produce error analysis file. Available only when the prediction is not specified\n";
 cout << "--output		: specify output file name. Default result.db\n";
 cout << "--clean			: delete unuseful file at the end of the process\n";
}
