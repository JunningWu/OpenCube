/* @M3EXPLORER_LICENSE_START@
 *
 * This file is part of the Multicube Explorer tool.
 *
 * Authors: Vittorio Zaccaria, Gianluca Palermo, Giovanni Mariani, Fabrizio Castro
 * Copyright (c) 2008-2009, Politecnico di Milano, Universita' della Svizzera italiana
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 
 *  * Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 * 
 *  * Redistributions in binary form must reproduce the above copyright notice, 
 *    this list of conditions and the following disclaimer in the documentation 
 *    and/or other materials provided with the distribution.
 * 
 * Neither the name of Politecnico di Milano nor Universita' della Svizzera Italiana 
 * nor the names of its contributors may be used to endorse or promote products 
 * derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This work was supported by the EC under grant MULTICUBE FP7-216693.
 * 
 * @M3EXPLORER_LICENSE_END@ */
#include <map>
#include <fstream>
#include <m3_xdr_api.h>
#include <iostream>

using namespace std;

void m3_xdr_write_design(ofstream &fout, vector<int> & data, vector<double> & values)
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

void m3_xdr_write_raw_data(ofstream &fout, vector<double> & data, vector<double> & values)
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

bool m3_xdr_read_vector(ifstream &input, vector<double> & x)
{
    x.clear();
    int num_of_elements;
    if(input.eof() || !input.good())
        return false;
    input >> num_of_elements;
    if(input.eof() || !input.good())
        return false;
    int n=0;
    while(!input.eof() && input.good() && n<num_of_elements)
    {
          double delta;
          input >> delta; 
          x.push_back(delta); 
          n++;
    }
    if(n<num_of_elements)
        return false;
    return true;
}

#define GO_AHEAD(fin) (fin.good() && !fin.eof())

bool m3_xdr_read_design(ifstream &fin, vector<double> & data, vector<double> & values)
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

