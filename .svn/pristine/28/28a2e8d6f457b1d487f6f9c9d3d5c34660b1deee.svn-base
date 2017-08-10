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
#ifndef __DIFF_OP_H__
#define __DIFF_OP_H__

/* @additional_authors @, Matteo Bertasa (2008), Stefano Bolli (2008)@ */

#include "linalg.hpp"
#include "func.hpp"

namespace bvp{
  using namespace linalg;
  /*
   *Class graph: (other leaves may also be present)
   *
   *              diff_op----------                 Dirichlet
   *             /       \         \               /
   *linear_diff_op       diff_op2   bdry_diff_op---
   *            \        /                         \
   *           linear_diff_op2                      Neumann
   *                 |
   *                 |        
   *                 |        
   *                 |        
   *                 +-- del1   
   *                 |
   *                 +-- del2
   *                 |
   *                 +-- Laplacian
   *                 |
   *                 ... etc
   *
   */


  //A general differential operator. All it does is evaluate the
  //operator applied at a specified point for a real-valued function
  //that take a vector argument. This is a pure abstract class.
  class diff_op{ 
  public:
    diff_op();
    double operator()(const realfunc &f, const point &p) const;
    virtual double at(const realfunc &f, const point &p) const = 0;
    virtual ~diff_op(){};
  };

  //A linear diff_op. Also pure abstract class for now.  
  class linear_diff_op : virtual public diff_op{ 
  public:
    linear_diff_op(){};
  };

  //An at most 2nd-order differential operator. 
  class diff_op2 : virtual public diff_op{
  public:
    diff_op2(){};
  };

  //The heat, wave, and Laplace's equation use this kind of
  //operators: they're both linear and at most 2nd order. 
  class linear_diff_op2 : public linear_diff_op, public diff_op2{
  public:
    double operator()(const realfunc &f, const point &p) const;
  };
  

  //An operator for specifying boundary conditions
  class bdry_diff_op : public diff_op{
  public:
    double operator()(const realfunc &f, const point &p) const;
    double operator()(const realfunc &f, const point &p, 
		      const vector &n) const;
    virtual double at(const realfunc &f, 
		      const point &p) const =0;
    virtual double at(const realfunc &f, const point &p, 
		      const vector &n) const =0;
  };

  //Dirichlet boundary conditions
  class dirichlet_op : public bdry_diff_op{
  public:
    double at(const realfunc &f, const point &p) const;
    double at(const realfunc &f, const point &p, const vector &n) const;
  };


  //Neumann boundary conditions
  class neumann_op : public bdry_diff_op{
  public:
    double at(const realfunc &f, const point &p, const vector &n) const;    
  private:
    double at(const realfunc &f, const point &p) const {return f(p);};
  };

  //Identity operator
  class Id_op : public linear_diff_op2{
    double at(const realfunc &f, const point &p) const;
  }; 

  //Partial wrt to some direction. By default (i.e with the overloaded
  //at() function), wrt first coordinate. Else, last argument gives
  //the direction to evaluate the partial.
  class del1 : public linear_diff_op2{
  public:
    double at(const realfunc &f, const point &p) const;
    double at(const realfunc &f, const point &p, size_t i) const;
  };

  //Second partials wrt some direction. By default (i.e with the
  //overloaded at() function), wrt first coordinate, twice. Else, last
  //two arguments give the directions to evaluate the partials.
  class del2 : public linear_diff_op2{
  public:    
    double at(const realfunc &f, const point &p) const;
    double at(const realfunc &f, const point &p, size_t i, size_t j) const;
  };

  //The Laplacian
  class Laplacian : public linear_diff_op2{ 
  public:
    double at(const realfunc &f, const point &p) const;
  };
  
}

#endif //__DIFF_OP_H__
