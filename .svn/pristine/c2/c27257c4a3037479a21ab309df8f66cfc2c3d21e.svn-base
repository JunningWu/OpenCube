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






#ifndef COMMANDS_H
#define COMMANDS_H

using namespace std;
 
#include <m3_object.h>
#include <m3_pointer_trash.h>
#include <m3_list.h>

extern pointer_trash prs_trash;
void prs_command_quit();
void prs_command_set(m3_object *,m3_object *);
//void prs_command_set_property(m3_object *,m3_object *,m3_object *);
m3_object *prs_command_read_variable(m3_object *);
void prs_command_db_insert_point(m3_object *);
void prs_command_db_write(m3_object *);
void prs_command_db_plot_objectives(string db_1,string db_2);
void prs_command_db_plot_2D(string db_to_plot);
void prs_command_read(m3_object *);
void prs_command_db_read(m3_object *);
void prs_command_drv_define_driver(m3_object *n);
void prs_command_show_variables();
void prs_command_tune();
void prs_command_drv_show_info();
void prs_command_opt_show_info();
void prs_command_define_optimizer(m3_object *n);
void prs_command_define_doe(m3_object *n);
void prs_command_doe_show_info();
void prs_command_define_application(m3_object *n);
void prs_command_filter_pareto(m3_object *,m3_object *);
void m3_init_parse_command();
void m3_parse_command(m3_string *command_name, m3_list *parameter_list);


#endif
