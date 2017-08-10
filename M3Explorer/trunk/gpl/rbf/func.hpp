/* @GPL_LICENSE_START@
 *
 * Authors: Vittorio Zaccaria, Gianluca Palermo, Giovanni Mariani, Fabrizio Castro, Matteo Bertasa (2008), Stefano Bolli (2008) 
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
#ifndef __FUNC_H__
#define __FUNC_H__

/* @additional_authors @, Matteo Bertasa (2008), Stefano Bolli (2008) @ */

#include "linalg.hpp"
#include <gsl/gsl_deriv.h>

namespace bvp{
  using namespace linalg;

  //A real-valued function from R^n to R.
  class realfunc{
  public:
    realfunc(); 
    realfunc( double(*f)(const point&));
    virtual ~realfunc(){};

    void set_function_ptr(double (f_in)(const point &p));
    
    double operator()(const point& p) const;
    virtual double at(const point& p) const;

    //Derivatives in directions k, k1, k2.
    virtual double d(const point& x, size_t k) const; 
    virtual double d2(const point& x, size_t k1, size_t k2) const ; 
  protected:
    static double eps; 
    static double sqrteps;
    static double root3eps;
    static bool initialised ;
  private:
    double (*myfunc)(const point &p);
    void no_init(int line, string file) const; //For throwing exceptions
  };


  //A function wrapper for calling GSL derivatives. 
  class gsl_function_wrapper{
  public:
    gsl_function_wrapper(const realfunc &f, point p, size_t idx);
    void set_params( const realfunc &f, point p, size_t idx);
    gsl_function* get_gsl_function() const;

    static double takemyaddress(double xi, void* nothing);
  private:
    gsl_function_wrapper();
    
    static point x;
    static size_t index;    
    static realfunc myfunc;

    static gsl_function* f;
  };
}

#endif //__FUNC_H__
