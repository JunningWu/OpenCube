//==========================================================================
//
// Author(s): Gerardo de Miguel González
// Contributors:
// Description: Metric Class implementation file
//
// Modifications:
//
//==========================================================================
//
//  The following code is derived, directly or indirectly, from XML plug-in for SCoPE,
//  released June 30, 2008. Copyright (C) 2008 Univeristy of Cantabria(UC)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License (GPL) or GNU Lesser General Public License(LGPL) as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License (GPL) or GNU Lesser General Public License(LGPL) for more details.
//
//  You should have received a copy of the GNU General Public License (GPL) or GNU Lesser General Public License(LGPL)
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//  For more information about SCoPE and the XML plug-in for SCoPE you can visit
//  http://www.teisa.unican.es/scope or write an e-mail to
//  the authors or a letter to SCoPE, GIM - TEISA, University
//  of Cantabria, AV. Los Castros s/n, ETSIIT, 39005, Santander, Spain
//
//==========================================================================

#include "string.h"
#include "metric.h"
#include <algorithm>

/**
 * \brief Metric default constructor
 *
 * \return A Metric Object
 */
Metric::Metric() {
  unit_multiplier = 1;
  setValue(0.0L);
}

/**
 * \brief Metric default destructor
 *
 */
Metric::~Metric () {
}

/**
 * \brief Metric constructor
 *
 * \param rname metric name (p.e execution_time)
 * \param rtype metric type (p.e float)
 * \param runit metric unit (p.e seconds)
 * \return A Metric object
 */
Metric::Metric(const string& rname, const string& rtype,
               const string& runit) : unit_multiplier(1), name(rname), type(rtype), unit(runit) {
  setValue(0.0L);
}

/**
 * \brief Metric copy constructor
 *
 * \param m a reference to a Metric object
 * \return A Metric object
 */
Metric::Metric(Metric const& m) : unit_multiplier(1), name (m.name), type (m.type), unit (m.unit)
{
   if (m.type.compare("float") == 0)
       value.real = m.getFloatValue();
   else if (m.type.compare("integer") == 0)
       value.integer = m.getIntegerValue();
   else
       value.real = 0.0L;
}

/**
 * \brief Metric name acccesor
 *
 * \return A string object which holds the name property content of the Metric object
 */
string Metric::getName() const {
  return name;
}

/**
 * \brief Metric type acccesor
 *
 * \return A string object which holds the type property content of the Metric object
 */
string Metric::getType() const  {
  return type;
}

/**
 * \brief Metric unit acccesor
 *
 * \return A string object which holds the unit property content of the Metric object
 */
string Metric::getUnit() const {
  return unit;
}

/**
 * \brief Metric object name property character case conversion function to lower case
 *
 */
void Metric::nameToLower() {
  std::transform(name.begin(), name.end(), name.begin(),
               (int(*)(int)) std::tolower);
}

/**
 * \brief Metric integer value acccesor
 *
 * \return An unsigned integer number which holds the integer value of the Metric object
 */
unsigned long int Metric::getIntegerValue () const {
  return value.integer;
}

/**
 * \brief Metric real value acccesor
 *
 * \return A floating point number which holds the real value of the Metric object
 */
long double Metric::getFloatValue () const {
  return value.real;
}

/**
 * \brief Metric name setting method
 *
 * \param rname a constant reference to a string object holding a name to set
 */
void Metric::setName(const string& rname) {
  name = rname;
}

/**
 * \brief Metric type setting method
 *
 * \param rtype a constant reference to a string object holding a type to set
 */
void Metric::setType(const string& rtype) {
  type = rtype;
}

/**
 * \brief Metric unit setting method
 *
 * \param runit a constant reference to a string object holding a unit to set
 */
void Metric::setUnit(const string& runit) {
  unit = runit;
  switch (unit[0]){
	case 'p':
		unit_multiplier = 1e-12;
		break;
	case 'n':
		unit_multiplier = 1e-9;
		break;
	case 'u':
		unit_multiplier = 1e-6;
		break;
	case 'm':
		if(strcasecmp("m",runit.c_str()) == 0 || strcasecmp("m2",runit.c_str()) == 0 || strcasecmp("m3",runit.c_str()) == 0 || strncasecmp("meter",runit.c_str(),5) == 0 ) break;
		unit_multiplier = 1e-3;
		break;
	case 'k':
	case 'K':
		unit_multiplier = 1e3;
		break;
	case 'M':
		unit_multiplier = 1e6;
		break;
	case 'g':
	case 'G':
		unit_multiplier = 1e9;
		break;
	case 'T':
		unit_multiplier = 1e12;
		break;
	default:
		unit_multiplier = 1;

  }

}
long double Metric::multiplier(){
	return unit_multiplier;
}

/**
 * \brief Metric integer value setting method
 *
 * \param rvalue a constant reference to an integer number holding a value to be set
 */
void Metric::setValue(const unsigned long int& rvalue) {
  if (type.compare("integer") == 0) {
	value.integer = rvalue;
  }
  else{
  	value.real = static_cast<long double>(rvalue);
  }

}

/**
 * \brief Metric floating point (real) value setting method
 *
 * \param rvalue a constant reference to floating-point number holding a value to be set
 */
void Metric::setValue(const long double& rvalue) {
  if (type.compare("integer") == 0) {
	value.integer = (unsigned long int)rvalue;
  }
  else{
  	value.real = rvalue;
  }
}

/**
 * \brief Metric assignment operator
 *
 * \param m a constant reference to a Metric object (rvalue)
 * \return a reference to a Metric object (lvalue)
 */
Metric& Metric::operator= (const Metric& m) {
  //::GMG::protect against invalid self-assignment
  if (this != &m) {
     name = m.getName();
     type = m.getType();
     unit = m.getUnit();
     if (m.type.compare("float") == 0)
         value.real = m.getFloatValue();
     else if (m.type.compare("integer") == 0)
         value.integer = m.getIntegerValue();
     else
         value.real = 0.0L;
  }
  //::GMG:: by convention, always return *this
  return *this;
}

/**
 * \brief XML output stream operator (Metric object XML serializer)
 *
 * \param os an output stream object reference
 * \param m a constant Metric object reference
 * \return a reference to an output stream object
 */
ostream& operator<<(ostream& os, const Metric& m) {
  os << "<system_metric name=\"" << m.name;
  if (m.type.compare("float") == 0) {
      if (m.getFloatValue() > 0 && m.getFloatValue() < 1)
          os << setiosflags(ios::fixed) << setprecision(9);
      else
        os << setiosflags(ios::scientific) << setprecision(9);

      os << "\" value=\"" << m.getFloatValue() << "\"/>";
  } else if (m.type.compare("integer") == 0)
      os << "\" value=\"" << m.getIntegerValue() << "\"/>";
    else
      os << "\" value=\"" << "NaN" << "\"/>";
  return os;
}

/**
 * \brief XML output stream operator (Metric object XML serializer)
 *
 * \param os an output stream object reference
 * \param m a constant Metric object pointer
 * \return a reference to an output stream object
 */
ostream& operator<<(ostream& os, const Metric* m) {
  os << "<system_metric name=\"" << m->name;
  if (m->type.compare("float") == 0)
      os << setiosflags(ios::scientific) << setprecision(9)
         << "\" value=\"" << m->getFloatValue() << "\"/>";
  else if (m->type.compare("integer") == 0)
      os << "\" value=\"" << m->getIntegerValue() << "\"/>";
  else
      os << "\" value=\"" << "NaN" << "\"/>";
  return os;
}
