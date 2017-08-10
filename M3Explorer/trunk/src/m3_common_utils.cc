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







#include <m3_opt_utils.h>
#include <m3_parser.h>
#include <iostream>
#include <dlfcn.h>
#include <map>
#include <cerrno>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sstream>
#include <fstream>
#include <m3_sim_utils.h>

/** FIXME: merge this function with the similar function in opt_select_optimizer */

bool m3_look_for_filename_in_search_path(m3_env *env, string & file_name, string & complete_path)
{
	string the_file_name= "lib"+file_name+".so";
    m3_object *search_path;
    m3_object *cbp;
    list<string> path_list;

	if(!env->shell_variables.get("search_path",search_path)) 
    { 
        path_list.push_back(".");
    }
    else
    {
        list<m3_object *>::iterator i;
        m3_list *sp;

	    if(!is_a<m3_list *>(search_path)) 
        {  prs_display_error("Invalid search path"); return false; }

        sp = to<m3_list *>(search_path);

        m3_list *spnc = const_cast<m3_list *>(sp);

        for(i=spnc->begin(); i!=spnc->end(); i++)
        {
            if(is_a<m3_string *>((*i)))
            {
                path_list.push_back(to<m3_string *>((*i))->get_string());
            }
        }
    }
        
	if(env->shell_variables.get("current_build_path",cbp)) 
    {
        m3_assert(is_a<m3_string *>(cbp)); 
        path_list.push_back(to<m3_string *>(cbp)->get_string() + "/lib");
    }
	

    list<string>::iterator it;
    for(it=path_list.begin(); it!=path_list.end(); it++)
    {
        string path=*it;
        string c_path=path+"/"+the_file_name;
        // cout << "Checking for " << c_path << endl;
        void *h=dlopen(c_path.c_str(),RTLD_NOW);
        if(h) 
        {
            complete_path = c_path;
            dlclose(h);
            return true;
        }
        else
        {
         //   cout << dlerror() << endl;
        }
    }
    return false;
}

string m3_get_image_path(m3_env *env)
{
    m3_object *cbp;
	if(env->shell_variables.get("current_build_path",cbp)) 
    {
        m3_assert(is_a<m3_string *>(cbp)); 
        return to<m3_string *>(cbp)->get_string();
    }
    return ".";
}

string m3_get_invocation_dir()
{
	char 	sz[400];
	string 	s = getcwd(sz, 400);
	return s;
}

bool m3_mkdir(string dir_path)
{
	if( mkdir( dir_path.c_str(), S_IRUSR | S_IWUSR | S_IXUSR)!=0 && errno!=17 )
	{		
		return false;
	}
	return true;
}

bool m3_rmdir(string dir_path)
{
	DIR 	*tdir;
	struct 	dirent *act_file;
	tdir 	= opendir(dir_path.c_str());

	if( tdir!=NULL )
	{
		act_file = readdir(tdir);
		while( act_file!=NULL )
		{
			if( strcmp(act_file->d_name, ".")!=0 && strcmp(act_file->d_name, "..")!=0 )			
			{
				if( act_file->d_type!=DT_DIR )
				{//deleting each file
					ostringstream file;
					file << dir_path.c_str() << "/" << act_file->d_name;
					remove( file.str().c_str() );
				}
				else
				{//recursion
					ostringstream temp;
					temp << dir_path.c_str() << "/" << act_file->d_name;
					if( !m3_rmdir(temp.str().c_str()) )
					{
						return false;
					}
				}
			}
			act_file = readdir(tdir);
		}
		closedir(tdir);
		//deleting empty directory
		remove(dir_path.c_str());
		return true;
	}
	else
	{
		return false;
	}
}

string m3_itos( int i )
{
	ostringstream str;
	str << i;
	return str.str();
}

string m3_dtos( double d )
{
	ostringstream str;
	str << d;
	return str.str();
}

bool m3_path_already_exists( string path )
{
	struct stat buf;
	if( stat( path.c_str(), &buf) == 0 )
		return true;
	return false;
}

string m3_get_unique_temp_dir_name( string base_name )
{
	ostringstream	dir_name;
	string 		name;
	int		i 	= 1;
	int		max 	= 20000;
	if( base_name == "" )
		dir_name << "/m3explorer_tmp_" << getpid(); 
	else
	{
		if( base_name[0]!='/' )
			dir_name << "/";
		dir_name << base_name << "_" << getpid();
	}
	name = m3_get_invocation_dir() + dir_name.str();
	while( m3_path_already_exists( name ) && i<=max )
	{
		name = m3_get_invocation_dir() + dir_name.str() + "_" + m3_itos( i );
		i++;
	}
	if( i>max )
		return "";
	else
		return name;
}

//#define DEBUG

bool m3_plot_vectors( map< string, vector< pair< double, double > > > *to_plot, string filename, string title, string x_axis_name, bool logscale_x, double x_scale, string y_axis_name, bool logscale_y,	double y_scale, bool monochrome)
{
	string temp_path = m3_get_unique_temp_dir_name( "m3explorer_vectors_plot" );
	if( temp_path == "" )
		return false;
	if ( !m3_mkdir( temp_path ) )
		return false;
	string file_out_path =  temp_path + "/m3explorer_gplot_script.scr";
	ofstream file_out( file_out_path.c_str(), ios::out );
	file_out << "unset grid" << endl;
	file_out << "set key right" << endl;
	file_out << "set xlabel \"" << x_axis_name << "\"" << endl;
	file_out << "set ylabel \"" << y_axis_name << "\"" << endl;
	string logscale_axes = "";
	if( logscale_x )
		logscale_axes = "x";
	if( logscale_y )
		logscale_axes = logscale_axes + "y";
	if( logscale_x || logscale_y )
		file_out << "set logscale " << logscale_axes << " 10"<<endl;
	file_out << "set title \"" << title << "\"" << endl;
	file_out << "set size "<< x_scale << ", " << y_scale <<endl;
	if( monochrome )
		file_out << "set terminal postscript eps monochrome" << endl;
	else	
		file_out << "set terminal postscript eps color" << endl;
	file_out << "set output \"" << filename << ".ps\"" << endl;
	map< string, vector< pair< double, double > > >::iterator map_i = to_plot->begin();
	int index = 0;
	for( ; map_i!=to_plot->end(); map_i++ )
	{
		string file_outdata_name = temp_path + "/data_file_" + m3_itos(index);
		if( index == 0 )
			file_out << "plot \"" 	<< file_outdata_name << "\" with " << "linespoints" << " title \""<<map_i->first<<"\"";
		else 
			file_out << ", \"" 	<< file_outdata_name << "\" with " << "linespoints" << " title \""<<map_i->first<<"\"";
		ofstream file_outdata( file_outdata_name.c_str(), ios::out );
		for( int i=0; i<map_i->second.size() ; i++ )
		{
			file_outdata << map_i->second[i].first << " " << map_i->second[i].second << endl;
		}
		file_outdata.close();
		index++;
	}
	file_out << endl;
	file_out << endl;
	file_out.close();
	shell_command( "gnuplot " + file_out_path );
	#ifndef DEBUG
		m3_rmdir( temp_path );
	#endif
	return true;
}

double	m3_get_percentile_value(double percentile, vector<double> ordered_vector)
{
	int last_element_index = ordered_vector.size() - 1;
	if( percentile<=0 )
		return ordered_vector[0];
	if( percentile>=100 )
		return ordered_vector[ last_element_index ];
	double k;
	double p 	= percentile / 100;
	double n 	= p * last_element_index;
	double d 	= modf( n, &k );
	double Vk 	= ordered_vector[ (int) k ];
	double Vk_next 	= ordered_vector[ (int) ( k + 1 ) ];
	return Vk + d * ( Vk_next - Vk );
}
