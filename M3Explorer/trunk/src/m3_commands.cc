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






#include <m3_parser.h>
#include <m3_commands.h>
#include <m3_shell_variables.h>
#include <m3_point.h>
#include <m3_list.h>
#include <m3_doe_utils.h>
#include <m3_driver_utils.h>
#include <m3_opt_utils.h>
#include <m3_env.h>
#include <m3_sim_utils.h>
#include <m3_object.h>
#include <m3_gen_html.h>
#include <fstream>
#include <sstream>
#include <map>

/*
 * libxml is a C library, so we have to do this little hack
 * to make it work here, under C++
 */
extern "C"
{
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xmlschemas.h>
#include <libxml/tree.h>
#include <libxml/xmlstring.h>
}

pointer_trash prs_trash;
void prs_include_file(const char *name);

void prs_command_quit()
{
    if (current_environment.current_driver != NULL)     
        delete current_environment.current_driver;

    if (current_environment.current_optimizer != NULL)  
        delete current_environment.current_optimizer;

    if (current_environment.current_doe != NULL)  
        delete current_environment.current_doe;

    m3_shutdown_expression_eval();
    prs_display_message("Exiting from Multicube Explorer shell");
    exit(0);
}

void prs_command_set(m3_object *nameo, m3_object *value)
{
    if (!is_a<m3_string *>(nameo)) 
    { 
        prs_display_error("Syntax error"); 
        return; 
    }

    if (value != NULL)
    {
        m3_string *name = to<m3_string *>(nameo);
        current_environment.shell_variables.insert(name->get_string(), *value);
        prs_display_message("Assigned value " + value->print() + " to " + name->get_string());

        string nn = name->get_string();
        if(nn=="objectives")
        {
            if(current_environment.current_design_space == NULL)
            {
                prs_display_error("Design Space not defined, please load a driver before the setting objectives.");
                return;
            }
            current_environment.current_design_space->set_objectives(&current_environment, value);
        }
        if(nn=="architecture_info")
        {
            if(current_environment.current_design_space == NULL)
            {
                prs_display_error("Design Space not defined, please load a driver before the setting objectives.");
                return;
            }
            current_environment.current_design_space->set_architecture_info(&current_environment, value);
        }
        if(nn=="objectives_units")
        {
            if(current_environment.current_design_space == NULL)
            {
                prs_display_error("Design Space not defined, please load a driver before the setting objectives.");
                return;
            }
            current_environment.current_design_space->set_objectives_units(&current_environment, value);
        }
        if(nn=="objectives_names")
        {
            if(current_environment.current_design_space == NULL)
            {
                prs_display_error("Design Space not defined, please load a driver before the setting objectives.");
                return;
            }
            current_environment.current_design_space->set_objectives_names(&current_environment, value);
        }
   }
}

m3_object *prs_command_read_variable(m3_object *n)
{
    if (!is_a<m3_string *>(n)) 
    { 
        prs_display_error("Syntax error"); 
        return NULL; 
    }
    
    m3_string *name = to<m3_string *>(n);
    
    m3_object *p;
    if (! current_environment.shell_variables.get(name->get_string(), p))
    {
        prs_display_error("Variable " + name->get_string() + " does not exists");
        return NULL;
    }
    return prs_trash.insert(p->gen_copy());
}

void prs_command_db_insert_point(m3_object *p)
{
    if (!is_a<m3_point *>(p))  
    { 
        prs_display_error("Syntax error"); 
        return; 
    }
    current_environment.available_dbs[current_environment.current_db_name]->insert_point(to<m3_point *>(p));
}

void prs_command_db_write(string name)
{
    if(current_environment.current_design_space == NULL)
    {
	prs_display_error("Design Space not defined, format unknown");
	return;
    }

    prs_display_message("Writing the database to disk");
    if (!current_environment.available_dbs[current_environment.current_db_name]->write_to_file(name.c_str()))
        prs_display_error("There were problems writing to file");
    else 
        prs_display_message("Database correctly written");
}

void prs_command_db_plot_objectives(string db_1,string db_2)
{
	if(current_environment.current_design_space == NULL)
	{
		prs_display_error("Design Space not defined, format unknown");
		return;
	}

	if(current_environment.current_design_space->objectives.size() !=2 ){//check plot dimensions
		prs_display_error("plotting functionalities available only in 2D.");
		return;
	}
	if(!current_environment.available_dbs.count(db_1) ||
	!current_environment.available_dbs.count(db_2) ){//check existence of dbs
		prs_display_error("Required database not available");
		return;
	}

    bool on_file = false;
    int fn;
    if(get_integer_from_variables(&current_environment, "write_plot_on_file", fn))
    {
        on_file = fn;
    }

	string db_1_filename = db_1 ;
	string db_2_filename = db_2 ;
    //cout << "Writing plot for " << db_1 << " and " << db_2 << endl;
	current_environment.available_dbs[db_1]->write_objectives_to_file(&current_environment,db_1_filename.c_str());
	if(db_1!=db_2)
		current_environment.available_dbs[db_2]->write_objectives_to_file(&current_environment,db_2_filename.c_str());

	string O1 = current_environment.current_design_space->objectives[0];
	string O2 = current_environment.current_design_space->objectives[1];


    string u1 = current_environment.current_design_space->objectives_units[0];
    string u2 = current_environment.current_design_space->objectives_units[1];

	ofstream gnuplot_script("gnuplot_script_file.tmp",ios::out);
	gnuplot_script << "unset grid\n";
	gnuplot_script << "set key right\n";
	gnuplot_script << "set xlabel \"" << O1 << " [" << u1 << "]\"\n";
	gnuplot_script << "set ylabel \"" << O2 << " [" << u2 << "]\"\n";
    /* FIXME, ask for the title to the user */
    //gnuplot_script << "set title \"Objectives\";\n";
    if(on_file)
    {
        gnuplot_script << "set terminal postscript eps monochrome" << endl;
        gnuplot_script << "set output \"m3_explorer_plot.ps\"" << endl;
    }
	gnuplot_script << "plot \""<< db_1_filename <<"\" with points";
	if(db_1!=db_2)
		gnuplot_script << ", \""<< db_2_filename <<"\" with points";

	gnuplot_script.close();


	ostringstream run_gnuplot;
    if(!on_file)
	    run_gnuplot << "gnuplot -persist gnuplot_script_file.tmp\n";
    else
        run_gnuplot << "gnuplot gnuplot_script_file.tmp\n";

	run_gnuplot << "rm " << db_1_filename << endl;
	if(db_1!=db_2)
		run_gnuplot << "rm " << db_2_filename << endl;
	run_gnuplot << "rm gnuplot_script_file.tmp" << endl;

	if(system(run_gnuplot.str().c_str()) ==-1 ){
		prs_display_error("error running gnuplot");
		return;
	}
}

void prs_command_db_plot_2D(string db_to_plot)
{
	if(current_environment.current_design_space == NULL)
	{
		prs_display_error("Design Space not defined, format unknown");
		return;
	}

    if(!current_environment.available_dbs.count(db_to_plot))
    {//check existence of db
		prs_display_error("Required database not available");
		return;
	}

    string x_name,y_name;
    if(!get_string_from_variables(&current_environment, "X_axis",x_name))
    {
		prs_display_error("X_axis variable not defined");
		return;
    }
    if(!get_string_from_variables(&current_environment, "Y_axis",y_name))
    {
		prs_display_error("Y_axis variable not defined");
		return;
    }

    bool on_file = false;
    int fn;
    if(get_integer_from_variables(&current_environment, "write_plot_on_file", fn))
    {
        on_file = fn;
    }

    int x_axis_index;
    int y_axis_index;

    if((current_environment.current_design_space->metric_index.count(x_name)||current_environment.current_design_space->ds_parameters_index.count(x_name))&& (current_environment.current_design_space->metric_index.count(y_name)||current_environment.current_design_space->ds_parameters_index.count(y_name)))
    {
        if (current_environment.current_design_space->metric_index.count(x_name)) {
            x_axis_index=current_environment.current_design_space->metric_index[x_name]+current_environment.current_design_space->size()+1;
        }
        else {
            x_axis_index=current_environment.current_design_space->ds_parameters_index[x_name]+1;
        }
        if (current_environment.current_design_space->metric_index.count(y_name)) {
            y_axis_index=current_environment.current_design_space->metric_index[y_name]+current_environment.current_design_space->size()+1;
        }
        else {
            y_axis_index=current_environment.current_design_space->ds_parameters_index[y_name]+1;
        }
    }
    else{
        prs_display_error("Invalid X or Y axis specified. Design space is inconsistent at the moment.");
        return ;
    }

	string db_to_plot_filename = db_to_plot;
    ofstream file_out(db_to_plot_filename.c_str(),ios::out);

    m3_list *database_list = current_environment.available_dbs[db_to_plot]->get_list_of_points();
    list<m3_object *>::iterator c;
    for(c=database_list->begin();c!=database_list->end(); c++)
    { 
       m3_point & p =sim_get_point_at_iterator(c);
       if(p.check_consistency(&current_environment))
       {
          string point_repr = opt_transform_point_in_csv_string_vector(p, &current_environment);
          for(int i=0; i< current_environment.current_design_space->ds_parameters.size(); i++)
          {
              file_out << current_environment.current_design_space->get_parameter_representation(&current_environment, p, current_environment.current_design_space->ds_parameters[i].name) + " ";
          }
          for(int i=0; i<current_environment.current_design_space->metric_names.size(); i++)
          {
               string name = current_environment.current_design_space->metric_names[i];
               file_out << sim_get_metric_from_point(p, name, &current_environment) << " ";
          }
          file_out << endl;
       }
    }
    file_out.close();


	ofstream gnuplot_script("gnuplot_script_file.tmp",ios::out);
	gnuplot_script << "unset grid\n";
	gnuplot_script << "set key right\n";
	gnuplot_script << "set xlabel \"" << x_name << " \"\n";
	gnuplot_script << "set ylabel \"" << y_name << " \"\n";
    // FIXME, ask for the title to the user 
    //gnuplot_script << "set title \"Objectives\";\n";
    if(on_file)
    {
        gnuplot_script << "set terminal postscript eps monochrome" << endl;
        gnuplot_script << "set output \"m3_explorer_plot.ps\"" << endl;
    }

   /* gnuplot_script << "linear_fit(x) = a*x+b"<<endl;
    gnuplot_script << "fit linear_fit(x) \""<<  db_to_plot_filename <<"\" using "<< x_axis_index<<":"<< y_axis_index<<" via a,b"<<endl;
    gnuplot_script << "quadratic_fit(x) = aa*x*x+bb*x+cc"<<endl;
	gnuplot_script << "fit quadratic_fit(x) \""<<  db_to_plot_filename <<"\" using "<< x_axis_index<<":"<< y_axis_index<<" via aa,bb,cc"<<endl;*/
    gnuplot_script << "plot \""<< db_to_plot_filename <<"\" using "<< x_axis_index<<":"<< y_axis_index<< " with points";
	gnuplot_script.close();

	ostringstream run_gnuplot;
    if(!on_file)
	    run_gnuplot << "gnuplot -persist gnuplot_script_file.tmp\n";
    else
        run_gnuplot << "gnuplot gnuplot_script_file.tmp\n";

	run_gnuplot << "rm " << db_to_plot_filename << endl;
	run_gnuplot << "rm gnuplot_script_file.tmp" << endl;

	if(system(run_gnuplot.str().c_str()) ==-1 ){
		prs_display_error("error running gnuplot");
		return;
	}
   
}

void prs_command_read(string name)
{
    prs_display_message("Reading script " + name);
    prs_include_file(name.c_str());
}

void prs_command_db_read(string name)
{
   if(current_environment.current_design_space == NULL)
   {
      prs_display_error("Design space is undefined, unknown DB format");
   }
   else
   {
      current_environment.available_dbs[current_environment.current_db_name]->clear();
      prs_display_message("Reading database " + name);
      if(current_environment.available_dbs[current_environment.current_db_name]->read_from_file(name.c_str()))
      {
            prs_display_message("Database read");
      }
      else
      {
            prs_display_error("Database not correctly read");
      }
   }
}

void prs_command_define_optimizer(string name)
{
    if (!opt_select_optimizer(&current_environment, name))
        prs_display_error("specified optimizer does not exists; check for lib" + name + ".so in your search path");
    else
        prs_display_message("Current optimizer has been set to '" +  current_environment.current_optimizer->get_information() + "'");
}

void prs_command_define_doe(string name)
{
    if (!doe_select_doe(&current_environment, name))
        prs_display_error("specified doe does not exists; check for lib" + name + ".so in your search path");
    else
        prs_display_message("Current doe has been set to '" +  current_environment.current_doe->get_information() + "'");
}

void prs_command_define_new_database(string name)
{
	prs_display_message("Changing current DB to: " + (name));
	current_environment.current_db_name = (name);
	if (!current_environment.available_dbs.count(name)) //==NULL)
	{
		prs_display_message("Database not existing. Creating a new one.");
		current_environment.available_dbs[name]= new m3_database();
	}
	else
	{
		prs_display_message("Operation OK.");
	}
}

void prs_command_drv_define_driver(string n)
{

    string name = n;

    if (!drv_select_driver(&current_environment, name)) 
    {
        prs_display_error("Driver can not be loaded"); 
        return;
    }

    m3_design_space *new_design_space = current_environment.current_driver->get_design_space(&current_environment);
    m3_vector *parameter_names = new m3_vector();

    current_environment.current_design_space = new_design_space;

    /* new_design_space->print(&current_environment); */
    
    for (int i = 0; i < new_design_space->size(); i++)
    {
        m3_object *name_of_param = new m3_string(new_design_space->ds_parameters[i].name);
        parameter_names->insert(i, *name_of_param);

        if(new_design_space->ds_parameters[i].type == M3_DS_SCALAR)
        {
            m3_string *lower_bound = new m3_string(new_design_space->get_scalar_min_symbol(&current_environment, new_design_space->ds_parameters[i].name)); 
            m3_string *upper_bound = new m3_string(new_design_space->get_scalar_max_symbol(&current_environment, new_design_space->ds_parameters[i].name)); 

            m3_vector *param_range = new m3_vector();
            param_range->insert(0, *lower_bound);
            param_range->insert(1, *upper_bound);

            delete lower_bound;
            delete upper_bound;
            current_environment.shell_variables.insert(to<m3_string *>(name_of_param)->get_string(), *param_range);
            delete param_range;
			delete name_of_param;
        }
    }
    current_environment.shell_variables.insert("ds_parameters", *parameter_names);
    delete parameter_names;
}

void prs_command_db_export(string file_name)
{
    if(current_environment.current_design_space == NULL)
    {
	prs_display_error("Design Space not defined, format unknown");
	return;
    }
    prs_display_message("Saving the database in CSV format..");
    
    /** Opens the file */
    ofstream file_out(file_name.c_str(),ios::out);
    m3_assert(!file_out.fail());

    /** Writes out the header i.e. the parameter names */
    for(int i=0; i< current_environment.current_design_space->ds_parameters.size(); i++)
    {
        file_out << current_environment.current_design_space->ds_parameters[i].name << ";";

    }

    /** Write out the metric names */
    for(int i=0; i<current_environment.current_design_space->metric_names.size(); i++)
    {
        file_out << current_environment.current_design_space->metric_names[i] << ";";
    }
    file_out << endl;

    /** Write out the database list */
    m3_list *database_list = current_environment.available_dbs[current_environment.current_db_name]->get_list_of_points();
    int wrong_formatted_points = 0;
    list<m3_object *>::iterator c;
    for(c=database_list->begin();c!=database_list->end(); c++)
    { 
       m3_point & p =sim_get_point_at_iterator(c);
       if(p.check_consistency(&current_environment))
       {
          string point_repr = opt_transform_point_in_csv_string_vector(p, &current_environment);
          file_out << point_repr;
          for(int i=0; i<current_environment.current_design_space->metric_names.size(); i++)
          {
               string name = current_environment.current_design_space->metric_names[i];
               file_out << sim_get_metric_from_point(p, name, &current_environment) << ";";
          }
          file_out << endl;
       }
       else wrong_formatted_points++;
    }
    if(wrong_formatted_points>0)
       prs_display_error("Some points were not consistent with the design space. They are not exported");
}

class m3_sortable_point: public m3_point
{
    public:


    m3_sortable_point(m3_point &p) : m3_point(p) {};

    bool operator<(const m3_sortable_point& p) const{
        m3_vector *sorting_metrics;
            vector<string> sorting_metric_names;
        if(!get_vector_from_variables(&current_environment, "sorting_metrics", sorting_metrics) )
            sorting_metric_names = current_environment.current_design_space->objectives;
        else
            sorting_metric_names = get_string_vector(sorting_metrics);

        m3_point this_point(*this);
        m3_point other_point(p);

        for(int m=0;m<sorting_metric_names.size();m++){
            int m_index = current_environment.current_design_space->metric_index[sorting_metric_names[m]];
            if( this_point.get_metric(m_index) < other_point.get_metric(m_index) )
                return true;
            else if( this_point.get_metric(m_index) > other_point.get_metric(m_index) )
                return false;
        }

        return false;
    }
};

void prs_command_db_export_xml(string file_name)
{
    if(current_environment.current_design_space == NULL)
    {
	prs_display_error("Design Space not defined, format unknown");
	return;
    }
    prs_display_message("Saving the database in XML format..");

    /** Initi file and variable */
	xmlDocPtr output;
	xmlNodePtr root, xpoint, xparams, xparam, xitem, xmetrics, xmetric;
    string error;
	map<string, int>::iterator it;
	int i=0, j;
	m3_design_space *design_space=current_environment.current_design_space;

	// output preparation
    xmlKeepBlanksDefault(1);
    xmlThrDefIndentTreeOutput(2);
    xmlThrDefTreeIndentString("\t");
    output=xmlNewDoc((xmlChar *) "1.0");
    root=xmlNewNode(NULL, (xmlChar *) "points");
    xmlNewProp(root, (xmlChar *) "xmlns", (xmlChar *) "http://www.multicube.eu/");
    xmlNewProp(root, (xmlChar *) "version", (xmlChar *) "1.3");
    xmlDocSetRootElement(output, root);


    /** Sort out the database list */
    set<m3_sortable_point> sorted_db;
    m3_list *database_list = current_environment.available_dbs[current_environment.current_db_name]->get_list_of_points();
    list<m3_object *>::iterator db_iter;
    for(db_iter=database_list->begin();db_iter!=database_list->end(); db_iter++)
    {
       m3_point & point =sim_get_point_at_iterator(db_iter);
       if(point.check_consistency(&current_environment) && !point.get_error(error))
       {
           sorted_db.insert(m3_sortable_point(point));
       }
    }

    /** Write out the database list */
    set<m3_sortable_point>::iterator c;
    for(c=sorted_db.begin();c!=sorted_db.end(); c++)
    {
       m3_point p(*c);
       if(p.check_consistency(&current_environment) && !p.get_error(error))
       {
           /** Generate xml structure for a point */
            xpoint=xmlNewChild(root, NULL, (xmlChar *) "point", NULL);
            xparams=xmlNewChild(xpoint, NULL, (xmlChar *) "parameters", NULL);
            xmetrics=xmlNewChild(xpoint, NULL, (xmlChar *) "system_metrics", NULL);

           /** Generate xml structure for parameters */
            for(it=design_space->ds_parameters_index.begin(); it!=design_space->ds_parameters_index.end(); it++)
            {
                xparam=xmlNewChild(xparams, NULL, (xmlChar *) "parameter", NULL);
                xmlNewProp(xparam, (xmlChar *) "name", (xmlChar *) it->first.c_str());

                int type=design_space->ds_parameters[it->second].type;
                if(type==M3_DS_PERMUTATION || type==M3_DS_ON_OFF_MASK)
                {
                    vector<int> v;
                    if (type==M3_DS_PERMUTATION)
                        v=design_space->get_permutation(&current_environment,&p,it->first);
                    else
                        v=design_space->get_mask(&current_environment,&p,it->first);
                    for(j=0; j<v.size(); ++j)
                    {
                        xitem=xmlNewChild(xparam, NULL, (xmlChar *) "item", NULL);

                        ostringstream temp;
                        temp << j+1;
                        string jj = temp.str();
                        ostringstream temp1;
                        temp << v[j];
                        string vj = temp1.str();
                        xmlNewProp(xitem, (xmlChar *) "position", (xmlChar *) jj.c_str());
                        xmlNewProp(xitem, (xmlChar *) "value", (xmlChar *) vj.c_str());
                    }
                }
                else // scalar value, we can insert it as is
                {
                    string c_rep = design_space->get_parameter_representation(&current_environment,p,it->first);
                    xmlNewProp(xparam, (xmlChar *) "value", (xmlChar *) c_rep.c_str());
                }
            }

           /** Generate xml structure for system_metrics */
            for(it=design_space->metric_index.begin(); it!=design_space->metric_index.end(); it++)
            {
                xmetric=xmlNewChild(xmetrics, NULL, (xmlChar *) "system_metric", NULL);
                xmlNewProp(xmetric, (xmlChar *) "name", (xmlChar *) it->first.c_str());

                char m_rep[32];
                sprintf(m_rep,"%f",p.get_metric(it->second));
                xmlNewProp(xmetric, (xmlChar *) "value", (xmlChar *) m_rep);
            }
       }
    }
    //output->save(file_name, true, 'UTF-8');
	xmlSaveFileEnc(file_name.c_str(), output, "UTF-8");
}

void prs_command_tune()
{
    int res;
    if (!current_environment.current_driver) { prs_display_error("Please define the driver"); return;}
    if (!current_environment.current_optimizer) { prs_display_error("Please define the optimizer"); return;}
    if (!current_environment.current_doe) { prs_display_error("Please define the doe"); return;}
    current_environment.current_optimizer->explore(&current_environment);
}

void prs_command_opt_show_info()
{
    if (!current_environment.current_optimizer) { prs_display_error("Please define the optimizer"); return;}
    cout << current_environment.current_optimizer->get_information() << endl;
}

void prs_command_drv_show_info()
{
    if (!current_environment.current_driver) { prs_display_error("Please define the driver"); return;}
    cout << current_environment.current_driver->get_information() << endl;
}


void prs_command_doe_show_info()
{
    if (!current_environment.current_doe) { prs_display_error("Please define the doe"); return;}
    cout << current_environment.current_doe->get_information() << endl;
}


void prs_command_show_variables()
{
    cout << "\nShell variables:\n";
    printf("%-20s  %-10s \n", "Name", "Value");
    printf("%-20s  %-10s \n", "----------", "------------------------");
    m3_map_const_iterator i;
    for (i = current_environment.shell_variables.begin();i != current_environment.shell_variables.end();i++)
    {
        string name = i->first;
        if(name != "current_build_path" && name != "xml_design_space_file")
            printf("%-20s  %-10s  \n", i->first.c_str(), i->second->print().c_str());
    }
    cout << endl;

    cout << "Databases in memory\n";
    printf("%-15s  %-10s \n", "Name", "size");
    printf("-----------------------------\n");
    map<string, m3_database *>::iterator j;
    for (j = current_environment.available_dbs.begin();j != current_environment.available_dbs.end();j++)
    {
        printf("%-15s  %-10s  %d\n", j->first.c_str(), "(available) ", j->second->size());
    }
}

void prs_command_filter_pareto()
{
    prs_display_message("Filtering the database for pareto points..");
    sim_compute_pareto(&current_environment);
}

void prs_command_rsm_train( string prediction_db_name, string rsm_name )
{
    if( !current_environment.current_rsms.count(rsm_name) )
    {
        prs_display_error("The RSM model '" + rsm_name  + "' doesn't exists");
        return;
    }
    else
    {
        if(current_environment.available_dbs[ current_environment.current_db_name ]->points->size() != 0)
        {
            prs_display_message("Training \"" + rsm_name + "\" RSM");
            m3_object *ret=current_environment.current_rsms[rsm_name]->train_rsm(&current_environment,prediction_db_name);
            if( !((m3_integer*)ret)->get_integer() )
                prs_display_error("RSM training aborted");
            delete ret;
        }
        else
            prs_display_error("Training DB empty");
    }
}

void prs_command_rsm_validate( string rsm_name, bool reuse_trainers )
{
    	m3_object 	*obj;
	bool 		requirements 		= true;
	m3_database	*current_database	= current_environment.available_dbs[ current_environment.current_db_name ];   	
	if( !current_environment.shell_variables.get( "trainers", obj ) )
	{
		prs_display_error( "Please define the shell array \"trainers\" before launching this command" );
		requirements = false;
	}
	if( current_database->size() == 0 )
	{
		prs_display_error( "Load a database before launching this command" );
		requirements = false;
	}
	if( !current_environment.current_rsms.count(rsm_name) )
	{
		prs_display_error( "The RSM model '" + rsm_name  + "' doesn't exists" );
		requirements = false;
	}
	if( requirements )
	{	
    		rsm_validate_rsm( &current_environment, rsm_name, current_database, reuse_trainers );
	}
}

typedef enum m3_command_type
{
    no_par,
    simple_par,
    multiple_pars
} m3_command_type;


typedef class m3_command
{
   public:
   m3_command_type type;   
   string command_help;
   string command_long_help;
   string command_parameters_name;
   m3_command(  m3_command_type i_type, 
                const char *i_command_help, 
                const char *i_command_long_help, 
                const char *i_command_parameters_name)
   {
        type = i_type;
        command_help = i_command_help;
        command_long_help = i_command_long_help;
        command_parameters_name = i_command_parameters_name;
   }
   m3_command() {};

} m3_command;

map<string, m3_command> m3_commands;

void m3_init_parse_command()
{
    m3_commands["help"]        = m3_command( no_par, "general help on m3explorer commands", "general help on m3explorer commands", "" );
    m3_commands["quit"]        = m3_command( no_par, "quit the current m3explorer session", "quit the current m3explorer session", "" );
    m3_commands["exit"]        = m3_command( no_par, "quit the current m3explorer session", "quit the current m3explorer session", "" );
    m3_commands["read_script"] = m3_command( simple_par, "read script from file", "reads the script from FILE executing all the commands", "FILE" );
    m3_commands["set"]         = m3_command( multiple_pars, "set a variable to a specific value", "set variable VAR to a specific VALUE", "VAR=VALUE");
    m3_commands["db_insert_point"]     = m3_command( simple_par, "insert a point in the current database", "insert the point POINT in the current database", "POINT");
    m3_commands["db_read"]     = m3_command( simple_par, "reads a database from disk", "reads database with name DB_NAME from disk", "DB_NAME");
    m3_commands["db_compute_ADRS"]     = m3_command( simple_par, "Computes ADRS w.r.t. a specified reference database", "Computes ADRS w.r.t. DB_NAME", "DB_NAME");
    m3_commands["db_write"]    = m3_command( simple_par, "writes a database on disk", "writes a database on disk", "DB_NAME");
    m3_commands["db_plot_objectives"]    = m3_command( simple_par, "plot objectives for given databases", "plot objectives for given databases. DB2_NAME is optional parameter", "DB1_NAME DB2_NAME");
    m3_commands["db_plot_2D"]    = m3_command( simple_par, "plot objectives for given databases", "plot objectives for given databases. DB2_NAME is optional parameter", "DB1_NAME DB2_NAME");
    m3_commands["opt_tune"]    = m3_command( no_par, "start the exploration process", "start the exploration process", "");
    m3_commands["opt_define_optimizer"]  = m3_command( simple_par, "define the optimizer module", "define the optimizer module as OPT_NAME", "OPT_NAME");
    m3_commands["opt_show_info"]         = m3_command( no_par, "shows information about current optimizer", "shows information about current optimizer", "");
    m3_commands["doe_define_doe"]  = m3_command( simple_par, "define the doe module", "define the doe module as DOE_NAME", "DOE_NAME");
    m3_commands["doe_show_info"]         = m3_command( no_par, "shows information about current doe", "shows information about current doe", "");
    m3_commands["drv_define_driver"]     = m3_command( simple_par, "define the driver module", "define the driver module as DRV_NAME", "DRV_NAME");
    m3_commands["drv_show_info"]         = m3_command( no_par, "shows information about current driver", "shows information about current driver", "");
    m3_commands["db_change_current"] = m3_command( simple_par, "change the current database", "change the current database to DB_NAME", "DB_NAME");
    m3_commands["db_report"]         = m3_command( no_par, "reports the contents of a database", "reports the content of a database", "");
    m3_commands["db_report_html"] = m3_command( simple_par, "generates a html report of the database", "generates a html report of the darabase into FOLDER", "FOLDER");
    m3_commands["db_filter_pareto"]  = m3_command( no_par, "filters the current database for pareto points", "filters the current database for pareto points", "");
    m3_commands["show_vars"]         = m3_command( no_par, "shows the variables in the current shell", "shows the variable in the current shell", "");
    m3_commands["db_export"]         = m3_command( no_par, "exports the db into a csv file", "exports the db into FILE", "FILE");
    m3_commands["db_export_xml"]         = m3_command( no_par, "exports the db into a xml file. You can sort the output by setting \"sorting_metrics\"", "exports the db into FILE. You can sort the output by setting \"sorting_metrics\"", "FILE");    
    m3_commands["rsm_train"]	= m3_command( multiple_pars, "trains an RSM and makes predictions", "trains RSM_NAME using the current db and puts the predictions into PREDICTIONS_DB", "PREDICTIONS_DB RSM_NAME" );
    m3_commands["rsm_validate"]	= m3_command( multiple_pars, "validates an RSM on the current db", "validates RSM_NAME on the current db using training sets with size defined in the shell vector \"trainers\"", "RSM_NAME" );
}

string m3_parse_command_get_param(m3_list *parameter_list, int n)
{
   int no = 0;
   list<m3_object *>::iterator i;
   for(i=parameter_list->begin(); i!=parameter_list->end() ; i++)
   {
        if(no == n)
        {
            m3_string *p = to<m3_string *>((*(i)));
            if(p == NULL)
            {
                prs_display_error("Expected a string as parameter");
                return "";
            }
            else
                return p->get_string();
        }
        no++;
   }

}

void m3_parse_command(m3_string *command_name, m3_list *parameter_list)
{

    if(command_name->get_string() == "help")
    {
        if(parameter_list->empty())
        {
            map<string, m3_command>::iterator i;
            for(i=m3_commands.begin(); i!= m3_commands.end(); i++)
            {
                printf("%-30s  %-10s \n", i->first.c_str(), i->second.command_help.c_str());
            }
        }
        else
        {
            string par =  m3_parse_command_get_param(parameter_list, 0);
            if(par == "--long")
            {
                map<string, m3_command>::iterator i;
                for(i=m3_commands.begin(); i!= m3_commands.end(); i++)
                {
                    printf("%-30s %-20s  %-10s \n", i->first.c_str(), 
                            i->second.command_parameters_name.c_str(), i->second.command_long_help.c_str());
                }
            }
        }
    }
    else if(command_name->get_string() == "read_script")
    {
        if(parameter_list->empty())
           prs_display_error("Please specify a script name");
        else
        {
            string par =  m3_parse_command_get_param(parameter_list, 0);
            prs_command_read(par);
        }
    }
    else if(command_name->get_string() == "db_insert_point")
    {
        if(parameter_list->empty())
           prs_display_error("Please specify the point to insert");
        else
        {   
            m3_point *par = to<m3_point *>((*(parameter_list->begin())));
                if(par == NULL)
                {
                    prs_display_error("Expected a full object representation as parameter");
                }
            prs_command_db_insert_point(par);
        }
    }
    else if(command_name->get_string() == "db_read")
    {
        if(parameter_list->empty())
           prs_display_error("Please specify a database name");
        else
        {
            string par =  m3_parse_command_get_param(parameter_list, 0);
            prs_command_db_read(par);
        }
    }
    else if(command_name->get_string() == "db_report_html")
    {
        if(parameter_list->empty())
           prs_display_error("Please specify a destination folder name");
        else
        {
            string folder_name =  m3_parse_command_get_param(parameter_list, 0);
            db_generate_html(folder_name);
        }
    }
    else if(command_name->get_string() == "db_compute_ADRS")
    {
        if(parameter_list->empty())
           prs_display_error("Please specify a database name");
        else
        {
            string reference_db =  m3_parse_command_get_param(parameter_list, 0);
            string current_db = current_environment.current_db_name;
            double ADRS;
            if(!sim_compute_ADRS(&current_environment, reference_db, current_db, ADRS))
                prs_display_error("Invalid databases specified for computing ADRS");
            else
            {
                m3_double m3ADRS(ADRS);
                current_environment.shell_variables.insert("last_ADRS", m3ADRS);
            }
        }
    }
    else if(command_name->get_string() == "db_write")
    {
        if(parameter_list->empty())
           prs_display_error("Please specify a database name");
        else
        {
            string par =  m3_parse_command_get_param(parameter_list, 0);
            prs_command_db_write(par);
        }
    }
    else if(command_name->get_string() == "db_plot_objectives")
    {
	string current_db = current_environment.current_db_name;
	string db_1;
	string db_2;
	switch(parameter_list->size()){
	case 0:
		prs_display_message("plotting the current db");
		prs_command_db_plot_objectives(current_db,current_db);
		break;

	case 1:
		db_1 = m3_parse_command_get_param(parameter_list, 0);
		prs_command_db_plot_objectives(db_1,db_1);
		break;

	case 2:
		db_1 = m3_parse_command_get_param(parameter_list, 0);
		db_2 = m3_parse_command_get_param(parameter_list, 1);
		prs_command_db_plot_objectives(db_1,db_2);
		break;
		
	default:
		prs_display_message("maximum two database parameters");
	}
    }
    else if(command_name->get_string() == "db_plot_2D")
    {
	string current_db = current_environment.current_db_name;
	string db_to_plot;
	switch(parameter_list->size()){
	case 0:
		prs_display_message("plotting the current db");
		prs_command_db_plot_2D(current_db);
		break;

	case 1:
		db_to_plot = m3_parse_command_get_param(parameter_list, 0);
		prs_command_db_plot_2D(db_to_plot);
		break;

	default:
		prs_display_message("maximum one database parameters");
	}
    }
    else if(command_name->get_string() == "opt_tune")
    {
        prs_command_tune();
    }
    else if(command_name->get_string() == "db_report")
    {	
    	if(current_environment.current_design_space == NULL)
    	{
	   prs_display_error("Design Space not defined, format unknown");
	   return;
    	}
        current_environment.db_report();
    }
    else if(command_name->get_string() == "show_vars")
    { 
    	prs_command_show_variables();
    }       
    else if(command_name->get_string() == "opt_define_optimizer")
    {
        if(parameter_list->empty())
           prs_display_error("Please specify an optimizer name");
        else
        {
            string par =  m3_parse_command_get_param(parameter_list, 0);
            prs_command_define_optimizer(par);
        }
    }
    else if(command_name->get_string() == "opt_show_info")
    {
        prs_command_opt_show_info();
    }
    else if(command_name->get_string() == "doe_define_doe")
    {
        if(parameter_list->empty())
           prs_display_error("Please specify a doe name");
        else
        {
            string par =  m3_parse_command_get_param(parameter_list, 0);
            prs_command_define_doe(par);
        }
    }
    else if(command_name->get_string() == "doe_show_info")
    {
        prs_command_doe_show_info();
    }
    else if(command_name->get_string() == "drv_define_driver")
    {
        if(parameter_list->empty())
           prs_display_error("Please specify a driver name");
        else
        {
            string par =  m3_parse_command_get_param(parameter_list, 0);
            prs_command_drv_define_driver(par);
        }
    }
    else if(command_name->get_string() == "drv_show_info")
    {
        prs_command_drv_show_info();
    }
    else if(command_name->get_string() == "db_change_current")
    {
        if(parameter_list->empty())
           prs_display_error("Please specify a database name");
        else
        {
            string par =  m3_parse_command_get_param(parameter_list, 0);
            prs_command_define_new_database(par);
        }
    }
    else if(command_name->get_string() == "db_filter_pareto")
    {
        prs_command_filter_pareto();
    }
    else if(command_name->get_string() == "db_export")
    {
        if(parameter_list->empty())
           prs_display_error("Please specify the file name");
        else
        {
            prs_command_db_export(m3_parse_command_get_param(parameter_list, 0));
        }
    }

    else if(command_name->get_string() == "db_export_xml")
    {
        if(parameter_list->empty())
           prs_display_error("Please specify the file name");
        else
        {
            prs_command_db_export_xml(m3_parse_command_get_param(parameter_list, 0));
        }
    }

    else if( command_name->get_string() == "rsm_train" )
    {
	if( parameter_list->size() < 2 )
	{
		prs_display_error( "Please specify both PREDICTION_DB and RESPONSE_SURFACE_MODEL" );
	}
	else
	{
		string prediction_db_param	= m3_parse_command_get_param(parameter_list, 0);
		string rsm_param		= m3_parse_command_get_param(parameter_list, 1);
		prs_command_rsm_train( prediction_db_param, rsm_param );
	}
    }
    else if( command_name->get_string() == "rsm_validate" )
    {
	if( parameter_list->size()<1 )
	{
		prs_display_error( "Please specify the RESPONSE_SURFACE_MODEL" );
		return;
	}	
	string	rsm_name 	= m3_parse_command_get_param( parameter_list, 0 );
	bool	reuse_trainers	= false;
	if( parameter_list->size()>1 )
	{
		string reuse_param = m3_parse_command_get_param( parameter_list, 1 );
		if( reuse_param == "reuse" )
			reuse_trainers = true;
	}
	prs_command_rsm_validate( rsm_name, reuse_trainers );
    }
    else if(command_name->get_string() == "quit" || command_name->get_string()=="exit")
    {
    	prs_command_quit();
    }
    else
    {
    	prs_display_error( command_name->get_string() + ": command not found" );
    }
}



