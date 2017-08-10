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
#include "utils.hpp"
#include <string>
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "linalg.hpp"
#include "error.hpp"
/* @additional_authors @, Matteo Bertasa (2008), Stefano Bolli (2008) @ */

namespace utils{
  std::string trim(const std::string& s){
    if(s.length() == 0)
      return s;
    std::size_t beg = s.find_first_not_of(" \a\b\f\n\r\t\v");
    std::size_t end = s.find_last_not_of(" \a\b\f\n\r\t\v");
    if(beg == std::string::npos) // No non-spaces
      return "";
    return std::string(s, beg, end - beg + 1);
  }

  template<typename K, typename V> 
  bool contains(const std::map<K,V>& m, K thing){
    return m.find(thing) != m.end();
  }

  template<typename E>
  bool contains(const std::set<E>& s, E thing){
    return s.find(thing) != s.end();
  }
  
  template<typename E>
  bool includes(const std::set<E>& s1, const std::set<E>& s2){
    return std::includes(s2.begin(), s2.end(), s1.begin(), s1.end());
  }
  
  using linalg::vector;
  using linalg::matrix;

  matrix read_matrix(std::string filename){
    std::ifstream ifs(filename.c_str());
    if(!ifs){
      error_handling::badArgument exc;
      exc.reason = "Cannot open file ";
      exc.reason += filename;
      exc.line = __LINE__;
      exc.file = __FILE__;
      throw exc;      
    }
    matrix v;
    ifs >> v;
    return v;
  }

  vector read_vector(std::string filename){
    std::ifstream ifs(filename.c_str());
    if(!ifs){
      error_handling::badArgument exc;
      exc.reason = "Cannot open file ";
      exc.reason += filename;
      exc.line = __LINE__;
      exc.file = __FILE__;
      throw exc;      
    }
    vector v;
    ifs >> v;
    return v;
  }

  std::map<linalg::point, double> read_pd_map(std::string filename){
    std::ifstream ifs(filename.c_str());
    if(!ifs){
      error_handling::badArgument exc;
      exc.reason = "Cannot open file ";
      exc.reason += filename;
      exc.line = __LINE__;
      exc.file = __FILE__;
      throw exc;      
    }
    matrix M;
    ifs >> M;

    if(M.cols() < 2){
      error_handling::badArgument exc;
      exc.reason = 
	"Input matrix to read_pd_map is too narrow. \n"
	"Need at least two columns in the input matrix";
      exc.line = __LINE__;
      exc.file = __FILE__;
      throw exc;
    }
    
    std::map <linalg::point, double> result;
    size_t m = M.cols();
    linalg::slice s(1,m-1);
    for(size_t i = 1; i <= M.rows(); i++)
      result[M(i,s)] = M(i,m);
    return result;
  }

  void show_exception(error_handling::error exc){
    using namespace std;

    cout << "Caught an exception!" << endl;
    cout << exc.reason << endl;
    cout << "On line " << exc.line << endl;
    cout << "From file " << exc.file << endl;
  }

}

#include <boost/shared_ptr.hpp>
#include "ddm.hpp"
//Instantiations
namespace utils{
  using boost::shared_ptr;
  template bool contains(const std::set<linalg::point>&, linalg::point E);
  template bool contains(const std::map<linalg::point,linalg::vector>& m, 
			 linalg::point thing);
  template bool includes(const std::set<linalg::point>& s1, 
			 const std::set<linalg::point>& s2);
  template bool contains(const std::map<linalg::point, 
			 shared_ptr<const bvp::overlapping_domain> >&,
			 linalg::point );

  template bool contains(const std::set<shared_ptr
			 <const bvp::overlapping_domain> >&, 
			 shared_ptr<const bvp::overlapping_domain> E); 

}
