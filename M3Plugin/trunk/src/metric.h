//==========================================================================
//
// Author(s): Gerardo de Miguel González
// Contributors:
// Description: Metric Class header file
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


#ifndef _METRIC_H_
#define _METRIC_H_

#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

/**
 * \class Metric
 * \brief Metric Class which captures the design metric concept
 *
 * \author Gerardo de Miguel González
 * \version 1.0
 * \date    June 2008
 */
class Metric {
public:
  Metric ();
  Metric(const string& rname, const string& rtype, const string& runit);
  Metric(Metric const& metric);
  ~Metric ();

  string getName() const;
  string getType() const;
  string getUnit() const;
  unsigned long int getIntegerValue () const;
  long double getFloatValue () const;

  void nameToLower();
  long double multiplier();

  void setName(const string& rname);
  void setType(const string& rtype);
  void setUnit(const string& runit);
  void setValue(const unsigned long int& rvalue);
  void setValue(const long double& rvalue);

  Metric& operator= (const Metric& m);
  friend ostream& operator<<(ostream& os, const Metric& m);
  friend ostream& operator<<(ostream& os, const Metric* m);

private:
  string name;
  string type;
  string unit;
  long double unit_multiplier;

  union Value {
    unsigned long int integer;
    long double real;
  };

  Value value;
};

#endif
