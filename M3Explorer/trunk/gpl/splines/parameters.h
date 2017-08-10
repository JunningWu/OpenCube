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


#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <string>
#include <map>
#include <fstream>
#include <stdexcept>
#include <sstream>

  template<typename T> T to_string( const T & obj )
         {
          std::stringstream os; 
          os << obj; 
          return os.str(); 
         } 

  template<typename T> T from_string(const std::string & str) 
         {
          std::istringstream is( str ); 
          T res;
          is >> std::dec >> res;
          return res; 
         } 
/**
* \brief Implement Generic parameter collection
*/
class Parameters{
private:
	std::map<std::string, std::string> Options;



public:
	/**
	* \brief Costruct empty collection
	*/
	Parameters(){};
	/**
	* \brief Return value for given parameter as specified type
	* \return the option's value
	*/
	template<typename G> G getOption(const std::string& name) const{
		if(Options.find(name) == Options.end()) throw std::runtime_error("Option \"" + name + "\" not stored");
		return from_string<G>(Options.find(name)->second);
	}

	/**
	* \brief Return value for given parameter as specified type
	* \return the option's value
	*/
	template<typename G> G getOption(const char* name) const{
		return getOption<G>(std::string(name));
	}

	/**
	* \brief add parameter to Collection
	*/
	template<typename G> void setOption(const std::string& name, const G& value){
                
		Options[name] =  to_string<G>(value);
	}

	/**
	* \brief add parameter to Collection
	*/
	template<typename G> void setOption(const char* name, const G& value){     
		Options[std::string(name)] =  to_string<G>(value);
	}
	
	/**
	* \brief Check if parameter is available
	* \return return true if parameter is available
	*/
	bool isOption(const std::string& name) const;

	/**
	* \brief Check if parameter is available
	* \return return true if parameter is available
	*/
	bool isOption(const char* name) const;
        
        /**
        * \print help
        */
        void help();

};


#endif
