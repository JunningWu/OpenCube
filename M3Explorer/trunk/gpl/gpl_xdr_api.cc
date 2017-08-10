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

#include <map>
#include <fstream>
#include <gpl_xdr_api.h>
#include <iostream>

using namespace std;


bool gpl_xdr_read_design(ifstream &fin, vector<double> & data, vector<double> & values)
{
   if(!GO_AHEAD(fin))
   {
      return false;
   }

   data.clear();

   int dsize = 0;
   fin >> dsize; 
#if defined(DEBUG)
   cout << " [" << dsize << "] ";
#endif
   int i;
   for(i=0; i<dsize && GO_AHEAD(fin); i++)
   {
       double k; fin >> k;
       data.push_back(k); 
#if defined(DEBUG)
       cout << " " << k;
#endif
   }
   if(i!=dsize)
   {
       return false;
   }
   
   if(!GO_AHEAD(fin))
   {

       return false;
   }
   values.clear();

   fin >> dsize;
#if defined(DEBUG)
   cout << " [" << dsize << "] " ;
#endif
   for(i=0; i<dsize && GO_AHEAD(fin); i++)
   {
       double x; fin >> x;
       values.push_back(x);
#if defined(DEBUG)
       cout << " " << x;
#endif
   }
#if defined(DEBUG)
   cout << endl;
#endif
   if(i!=dsize)
   { 
       return false;
   }

   return true;
}

//file("kool.cpp",ios::in|ios::out);
//

void gpl_xdr_write_vector(ofstream &fout, vector<double> & x)
{

    fout << x.size() << " ";
    for(int i=0; i<x.size(); i++)
    {
        fout << x[i] << " ";
    }
    fout << endl;
}

void gpl_xdr_write_raw_data(ofstream &fout, vector<double> & data, vector<double> & values)
{
   fout << data.size() << " ";
   for(int i=0; i<data.size(); i++)
   {
        fout << data[i] << " ";
        //cout << data[i] << " ";
   }
   fout << values.size() << " ";
   for(int i=0; i<values.size(); i++)
   {
        fout << values[i] << " ";
        //cout << tmp_val << " ";
   }
   fout << endl;
   //cout << endl;
}

