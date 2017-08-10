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
/*! \file utils.hpp
 * \brief Defines some miscellaneous functions.
 */

/* @additional_authors @, Matteo Bertasa (2008), Stefano Bolli (2008) @ */

#ifndef __UTILS_H__ 
#define __UTILS_H__

#include <string>
#include <map>
#include <set>
#include "linalg.hpp"

/*! \namespace utils
 *  \brief A few helpful functions that didn't seem to be readily
 *  classified anywhere else.   
 */ 
namespace utils{
  ///Clears whitespace from front and back of string s.
  std::string trim(const std::string& s);

  ///Does map m contain thing?
  template<typename K, typename V> 
  bool contains(const std::map<K,V>& m, K thing);
  ///Does set s contain thing?
  template<typename E>
  bool contains(const std::set<E>& s, E thing);

  ///Does set s1 include set s2?
  template<typename E>
  bool includes(const std::set<E>& s1, const std::set<E>& s2);

  ///Reads matrices from filenames.
  linalg::matrix read_matrix(std::string filename);
  ///Reads vectors from filenames.
  linalg::vector read_vector(std::string filename);

  /*! \brief Reads map<point,double> from a matrix.  
   *
   *Last column is the value at each point which is represented in
   *turn by the rest of the row.
   */
  std::map<linalg::point, double> read_pd_map(std::string filename);

  ///Outputs some information about generic exceptions.
  void show_exception(error_handling::error exc);

}

#endif
