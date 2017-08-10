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



#ifndef _GEN_HTML_H
#define _GEN_HTML_H


#include <m3_design_space.h>



using namespace std;



/** 
 * Prints show_vars on a file.
 *
 * @param file_name the ofstream of the file in which to print.
 */
void show_variables_html(ofstream& file_name);

/** 
 * Prints the content of a db on a file.
 *
 * @param env the M3 env.
 * @param local_db the M3 db to print.
 * @param file_name the ofstream of the file in which to print.
 */
void db_export_local_db_html(m3_env* env, m3_database* local_db, ofstream& file_name);

/** 
 * Prints the points in a db on a png figure using gnuplot.
 *
 * @param local_db the M3 db to print.
 * @param fig_name the name of the output figure.
 * @param obj1 the index of x axis objective.
 * @param obj2 the index of y axis objective.
 */
void pareto_plot_to_file_local_db_obj(m3_database* local_db, string fig_name, int obj1, int obj2);

/** 
 * Prints the points in the current db on a png figure using gnuplot.
 *
 * @param db the name of the M3 db to print.
 * @param fig_name the name of the output figure.
 * @param obj1 the index of x axis objective.
 * @param obj2 the index of y axis objective.
 * @param is_local true if the db is local, that is not registered in the environment.
 */
void pareto_plot_to_file_obj(string db, string fig_name, int obj1, int obj2, bool is_local);

/** 
 * Prints the level analysis of a db wrt one parameter on a png figure using gnuplot.
 *
 * @param env the M3 env.
 * @param local_db the M3 db to print.
 * @param fig_name the name of the output figure.
 * @param par the name of the parameter.
 * @param obj1 the index of x axis objective.
 * @param obj2 the index of y axis objective.
 */
void level_analysis_plot_to_file_local_db_obj(m3_env* env, m3_database* local_db, string fig_name, string par, int obj1, int obj2);

/** 
 * Prints the level analysis of the current db wrt one parameter on a png figure using gnuplot.
 *
 * @param env the M3 env.
 * @param fig_name the name of the output figure.
 * @param par the name of the parameter.
 * @param obj1 the index of x axis objective.
 * @param obj2 the index of y axis objective.
 */
void level_analysis_plot_to_file_obj(m3_env* env, string fig_name, string par, int obj1, int obj2);

/** 
 * Prints the parameter-metric box-plots of the current db on a png figure using gnuplot.
 *
 * @param env the M3 env.
 * @param fig_name the name of the output figure.
 * @param par the name of the parameter.
 * @param metric_name the name of the metric.
 */
void parameter_metric_boxplot_to_file(m3_env* env, string fig_name, string par, string metric_name);

/** 
 * Prints the main effects graph of the current db wrt one metric on a png figure using gnuplot.
 *
 * @param env the M3 env.
 * @param fig_name the name of the output figure.
 * @param metric_name the name of the metric.
 */
void main_effects_plot_to_file(m3_env* env, string fig_name, string metric_name);

/** 
 * Writes the interaction effects values of the current db wrt one metric and one main parameter on temp files.
 *
 * @param env the M3 env.
 * @param par the name of the main parameter.
 * @param metric_name the name of the metric.
 * @param y_min lowest metric value on y axis.
 * @param y_max highest metric value on y axis.
 * @param data_files vector storing the names of the temp files generated.
 */
void interaction_effects_write_to_files(m3_env* env, string par, string metric_name, double& y_min, double& y_max, vector<string>& data_files);

/** 
 * Prints the interaction effects graph of the current db wrt one metric and one main parameter on a png figure using gnuplot.
 *
 * @param env the M3 env.
 * @param fig_name the name of the output figure.
 * @param par the name of the main parameter.
 * @param metric_name the name of the metric.
 * @param y_min y axis lower range.
 * @param y_max y axis higher range.
 * @param data_files vector storing the names of the temp files containing the values to print.
 */
void interaction_effects_plot_to_file(m3_env* env, string fig_name, string par, string metric_name, double y_min, double y_max, vector<string> data_files);

/** 
 * Generates an html report of the current db in a specified folder.
 *
 * @param folder_name the name of the destination folder.
 */
void db_generate_html(string folder_name);


#endif

