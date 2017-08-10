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






#include <iostream>
#include <sstream>
#include <m3_shell_variables.h>
#include <string>
#include <m3_parser.h>
#include <m3_sim_utils.h>
#include <m3_driver.h>

using namespace std;

class m3_test_driver: public m3_driver
{
    public:
        string get_information();
        m3_design_space *get_design_space(m3_env *);
        m3_vector * get_metrics();
        m3_vector * get_statistics();
        m3_point  * simulate(m3_point &, m3_env *);
        bool is_valid(m3_point &, m3_env *) { return true;};
        string get_name();
        m3_test_driver();
        ~m3_test_driver();
};


string m3_test_driver::get_information()
{
    string info = "";
    info.append("Test driver written by G. Palermo and V. Zaccaria");
    return info;
}

m3_design_space *m3_test_driver::get_design_space(m3_env *env)
{
    m3_design_space *ds = new m3_design_space();

    vector<string> vsa; vsa.push_back("item1"); vsa.push_back("item2");
    vector<string> vsb; vsb.push_back("2"); vsb.push_back("4"); vsb.push_back("8");

    ds->insert_scalar(env, "Scalar1", M3_SCALAR_TYPE_INTEGER, 2, 3, vector<string>());
    ds->insert_scalar(env, "Scalar2", M3_SCALAR_TYPE_LIST, 0, 0, vsa);
    ds->insert_scalar(env, "Scalar3", M3_SCALAR_TYPE_LIST, 0, 0, vsb);

    ds->insert_permutation(env, "Perm1", true, "Scalar1", 0);
    ds->insert_on_off_mask(env, "Mask1 FIX-ONS-FIX", false, "", 3, false, false, "", 2);
    ds->insert_on_off_mask(env, "Mask2 FIX-NO-ONS", false, "", 3, true, false, "", 0);
    ds->insert_on_off_mask(env, "Mask3 FIX-ONS-VAR", false, "", 3, false, true, "Scalar1", 0);
    ds->insert_on_off_mask(env, "Mask4 VAR-NO-ONS", true, "Scalar1", 0, true, false, "", 0);

    ds->insert_metric(env, "M1", "cycles");
    ds->insert_metric(env, "M2", "J");
    ds->insert_metric(env, "M3", "W");
    
    return ds;
}

m3_point *m3_test_driver::simulate(m3_point &point, m3_env *env)
{
    m3_vector metrics;
    m3_vector stats;
    m3_point *simulated_point = new m3_point(point);

    double M1, M2, M3;

    int value[8];
    for (int j = 0; j < point.size(); j++)
    {
        value[j] = point[j];
    }
    double av[3], xv[3], yv[3];
    av[0] = 0.0001;
    xv[0] = 139;
    yv[0] = 6;
    av[1] = 0.001;
    xv[1] = 58;
    yv[1] = 12;
    av[2] = 0.03;
    xv[2] = 83;
    yv[2] = 33;

    M1 = (value[0] + value[1]) * (value[0] + value[1]) * av[0] - 2 * av[0] * xv[0] * (value[0] + value[1]) + av[0] * xv[0] * xv[0] + yv[0];
    M2 = ((value[2]) * (value[2]) * av[1] - 2 * av[1] * xv[1] * (value[2]) + av[1] * xv[1] * xv[1] + yv[1]) / 300;
    M3 = (value[1] - value[2]) * (value[1] - value[2]) * av[2] - 2 * av[2] * xv[2] * (value[1] - value[2]) + av[2] * xv[2] * xv[2] + yv[2];

    m3_double M31(M1);
    m3_double M32(M2);
    m3_double M33(M3);

    metrics.insert(0, (M31));
    metrics.insert(1, (M32));
    metrics.insert(2, (M33));

    simulated_point->set_properties("metrics", metrics);
    simulated_point->set_properties("statistics", stats);

    return simulated_point;
}

string m3_test_driver::get_name()
{
    string name = "test_driver";
    return name;
}

m3_test_driver::m3_test_driver()
{
    prs_display_message("Loading the test_driver");
}


m3_test_driver::~m3_test_driver()
{
    prs_display_message("Removing test_driver");
}


extern "C"
{

    m3_test_driver *drv_generate_driver()
    {
        prs_display_message("Creating the test_driver");
        return new m3_test_driver();
    }

}
