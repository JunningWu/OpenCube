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
#ifndef __DDM_H__
#define __DDM_H__

/* @additional_authors @, Matteo Bertasa (2008), Stefano Bolli (2008) @*/

#include <vector>
#include <map>
#include <set>
#include <boost/shared_ptr.hpp>
#include "bvp.hpp"
#include "interpolator.hpp"
#include "diff_op.hpp"
#include "func.hpp"

namespace bvp{
  using std::map;
  using std::set;
  using boost::shared_ptr;

  //Boundary differential operators that may be specified in peculiar
  //ways depending on the ddm we're working on.
  //
  //On interior boundary points as specified by the domain
  //decomposition, the alternate Bprime operator is applied instead of the
  //B operator.
  class ddm_bdry_diff_op : public bdry_diff_op{
  public:
    ddm_bdry_diff_op(shared_ptr<const bdry_diff_op> B_in, 
		     shared_ptr<const bdry_diff_op> Bprime_in,
		     const set<point>& ibps);
    double at(const realfunc &f, const point &p) const;
    double at(const realfunc &f, const point &p, const vector &n) const;
  private:
    shared_ptr<const bdry_diff_op> B;
    shared_ptr<const bdry_diff_op> Bprime;
    set<point> intr_bdry_pts;
  };

  //************************ ddm ********************************************

  //A generic domain decomposition method
  template<typename RBF>
  class ddm{
  public:
    //What are the domains of this ddm, and what's the bvp?
    ddm(const set<shared_ptr<const domain> >& ds, 
	shared_ptr<const BVP> thebvp);
        
    //Relative tolerance. Defaults to 1e-5.
    void set_tolerance(double tol);
    virtual double at(const point& p) const = 0;
    double operator()(const point& p) const;

    virtual ~ddm();

  protected:
    shared_ptr<const BVP> bvp;
    set<shared_ptr<const domain> > domains;
    double tolerance;

    //Each specific ddm has its own way of doing things.
    virtual void solve() = 0;
  };

  class overlapping_domain;
  
  //***************** additive_schwarz_ddm ***********************************

  //This method described in Li and Chen [2003].
  template <typename RBF>
  class additive_schwarz_ddm : public ddm<RBF>{
  public:
    additive_schwarz_ddm(const set<shared_ptr<const domain> >&, 
			 shared_ptr<const linear_BVP2> bvp);
    double at(const point& p) const;
  private:
    map<shared_ptr<const overlapping_domain>, 
	shared_ptr<interpolator<RBF> > > phis;

    set<shared_ptr<const interpolator<RBF> > > 
    which_interps(const point& p) const;

    void solve();

    //Evaluate using the average of relevant interpolators.
    double avg_interp(set<shared_ptr<const interpolator<RBF> > > 
		      relevant_interpolators,
		      const point& p) const;


    vector at_all_points() const;
    
  };

  //An overlapping domain's constructor, besides needing the same
  //arguments as required for an ordinary domain, also needs to know
  //which domains it will overlap with, and on which domain its
  //boundary points lie. These two extra pieces of information are
  //given by
  // 
  //        set<const shared_ptr<overlapping_domain> >
  //        map<point, shared_ptr<const overlapping_domain> >
  // 
  //respectively. By default, if these arguments are omitted, it is
  //assumed that the overlapping domain doesn't overlap with anything
  //at all. If a point in the domain's boundary doesn't overlap with
  //another domain, then it shouldn't be specified in the map<point,
  //overlapping_domain> data structure (or it can quite harmlessly be
  //assigned  to itself).
  //
  //Alternatively, the set_overlapper_info(...) function can handle
  //all of the details.
  class overlapping_domain : public domain{
  public:
    overlapping_domain(string intr, string bdry, string ns);
    overlapping_domain(string intr, string bdry, string ns, 
		       const set<shared_ptr<const overlapping_domain> >& ols,
		       const map<point, 
		       shared_ptr<const overlapping_domain> >& bdry_asst);

    overlapping_domain(size_t dimension);
    overlapping_domain(size_t dimension, set<point> intr, 
	   set<point> bdry, map<point, vector> ns);
      
    set<shared_ptr<const overlapping_domain> > get_domains() const;
    
    //With what domain this point overlap? If none, return 0.
    shared_ptr<const overlapping_domain> which_domain(const point& p) const; 
    
    //Once domains are defined via ordinary domain constructors, this
    //function will grab a set of domains and say which domain
    //overlaps with which one. 
    friend void set_overlapper_info(set<shared_ptr<overlapping_domain> > 
				    domains);

    //Set overlapper info manually.
    void set_overlapper_info(const point& p, 
			     const shared_ptr<overlapping_domain> o);

  private:
    set<shared_ptr<const overlapping_domain> > overlappers;
    map<point, shared_ptr<const overlapping_domain> > boundary_assignments;
  };
}

#endif // __DDM_H__

