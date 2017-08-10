/* @M3EXPLORER_LICENSE_START@
 *
 * This file is part of the Multicube Explorer tool.
 *
 * Authors: Vittorio Zaccaria, Gianluca Palermo, Giovanni Mariani, Fabrizio Castro, Stefano Bolli (2009) 
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


/* @additional_authors @, Stefano Bolli (2009) @ */

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <m3_gen_html.h>
#include <m3_design_space.h>
#include <m3_env.h>
#include <m3_point.h>
#include <m3_list.h>
#include <m3_parser.h>
#include <m3_sim_utils.h>
#include <list>
#include <vector>
#include <pthread.h>
#include <m3_common_utils.h>
#include <math.h>

#define REMOVE_FILES

// enable DEBUG on stdout
//#define DEBUG

// enable DEBUG for main effect function on file. 
// Is created a file for each metric due multithreading.
//#define DEBUG_MAIN_EFFECT_ON_FILE

#define STRMAX 100

#define MAXSIZE 1000

#include <iostream>
#include <fstream>
/*
string m3_read_html_template(string file_name);
bool m3_write_html_template(string file_name, string content);
string m3_substitute(string sentence, string tag, string to_embed);
bool m3_write_html_css(string directory);
*/

/*
    string s = m3_read_html_template("template_skeleton.html");
    s = m3_substitute(s, "@stuff@", "this is some stuff");
    m3_write_html_template("prova.html",s);
    m3_write_html_css(".");
*/

string b_folder;

#define BULLET_RED "<img src=\"template_results_page_files/Watermark_bullet_red.png\" style=\"height: 17px; width: 17px; \" />"
#define BULLET_GREEN "<img src=\"template_results_page_files/Watermark_bullet_green.png\" style=\"height: 17px; width: 17px; \" />"
#define BULLET_GRAY "<img src=\"template_results_page_files/Watermark_bullet_gray.png\" style=\"height: 17px; width: 17px; \" />"

#define BULLET_RED_L    "\\resizebox{0.20cm}{#esc}{\\includegraphics{"+b_folder+"/template#results#page#files/Watermark#bullet#red.png}}"
#define BULLET_GREEN_L  "\\resizebox{0.20cm}{#esc}{\\includegraphics{"+b_folder+"/template#results#page#files/Watermark#bullet#green.png}}"
#define BULLET_GRAY_L   "\\resizebox{0.20cm}{#esc}{\\includegraphics{"+b_folder+"/template#results#page#files/Watermark#bullet#gray.png}}"
#define BOXPLOT_SCHEMA_L   "\\begin{center} \\centering \\includegraphics[height=0.3\\textheight]{"+b_folder+"/template_results_page_files/boxplot.pdf}\\\\ \\end{center}"
#define INTERACTION_SCHEMA_L   "\\begin{center} \\centering \\includegraphics[width=0.8\\textwidth]{"+b_folder+"/template_results_page_files/interactions.pdf}\\\\ \\end{center}"


string red(string s)
{
    return "<FONT COLOR=\"#AB1212\">"+s+"</FONT>";
}

string green(string s)
{
    return "<FONT COLOR=\"#125512\">"+s+"</FONT>";
}

string bold(string s)
{
    return "<b>"+s+"</b>";
}

string safe(string x);

string boldl(string s)
{
    return "{\\bf "+safe(s) + "}";
}

string italic(string s)
{
    return "<i>"+s+"</i>";
}


string itos(int v);

string begin_table(int size)
{
    if(size>200)
        size = size - 150;
    return "<TABLE class=\"paragraph Body\" style=\"line-height: 20px; text-decoration: none; font-size: 12px; width: "+itos(size)+"px \" border=1; > <tr>";
}

string latex_content = "";

#define reset_latex_content() latex_content = "";
#define add_latex_content(s) latex_content = latex_content + s;

string begin_table_latex(int size)
{
    string s = "\n\n\\begin{center} \n \\scriptsize \\begin{longtable}{|";
    for(int i=0; i<size; i++)
    {
        s = s + "c|";
    }
    s = s + "} \n \\hline";
    return s;
}

string end_table()
{
    return "</TABLE>";
}

string end_table_latex()
{
    return "\\hline \\end{longtable} \n \\end{center}\n\n";
}

/*
The following table has been added in section IV.E, page 7: 

\begin{center}
\scriptsize
\begin{tabular}{|l|c|c|c|c|}
\hline \textbf{Processor issue width} 		& \textbf{1} & \textbf{2} & \textbf{4} & \textbf{8} \\
\hline
Fetch/Retire width 		& 1/2 & 2/4 & 4/6 & 8/10 \\
Fetch queue size 		& 8 & 16 & 32 & 64 \\
Integer issue queue size 	& 5 & 10 & 20 & 40 \\
Floating Point issue queue size 	& 2 & 5 & 15 & 35 \\
Reorder buffer size 		& 20 & 40 & 80 & 160 \\
LS/Integer/FP Units 		& 1/1/1 & 1/2/1 & 1/4/2 & 1/8/4 \\
L/S queue size 			& 8/8 & 16/16 & 32/32 & 64/64 \\
\hline
\end{tabular} \\
\vspace{0.2cm}
Table II: {Additional set of microarchitectural parameters dependent on the processor issue width.}
\end{center}
*/

string m3_substitute(string sentence, string tag, string to_embed);

string html_to_latex(string x)
{
    x = m3_substitute(x, "<b>", "\\textbf{");
    x = m3_substitute(x, "</b>", "}");
    x = m3_substitute(x, "<i>", "\\emph{");
    x = m3_substitute(x, "</i>", "}");
    x = m3_substitute(x, "<FONT COLOR=\"#AB1212\">", "");
    x = m3_substitute(x, "<FONT COLOR=\"#125512\">", "");
    x = m3_substitute(x, "</FONT>", "");
    x = m3_substitute(x, BULLET_RED, BULLET_RED_L);
    x = m3_substitute(x, BULLET_GREEN, BULLET_GREEN_L);
    x = m3_substitute(x, BULLET_GRAY, BULLET_GRAY_L);
    return x;
}

string table_head_latex(vector<string> v)
{
    string r;
    r = r + "\\hline ";
    for(int c=0; c<v.size(); c++)
    {
       r = r + "\\textbf{"+safe(html_to_latex(v[c]))+"} "; 
       if(c!=(v.size()-1))
           r = r + " & ";
    }
    r = r + "\\\\";
    r = r + "\n";
    return r;
        
}

string table_head(vector<string> v)
{
    string r;
    r = r + "<TR>";
    for(int c=0; c<v.size(); c++)
    {
       r = r + "<TD>"+v[c]+" "; 
    }
    r = r + "";
    r = r + "\n";
    r = r + "<TR>";
    return r;
}

string table_row_latex(vector<string> v)
{
    string r;
    r = r + "\\hline ";
    for(int c=0; c<v.size(); c++)
    {
       r = r + ""+safe(html_to_latex(v[c]))+""; 
       if(c!=(v.size()-1))
           r = r + " & " ;

    }
    r = r + "\\\\";
    r = r + "\n";
    return r;
        
}

string table_row(vector<string> v)
{
    string r;
    r = r + "<TR>";
    for(int c=0; c<v.size(); c++)
    {
       r = r + "<TD>"+v[c]+""; 
    }
    r = r + "";
    r = r + "\n";
    return r;
}

string itos(int v)
{
    ostringstream s;
    s << v;
    return s.str();
}

string dtos(double v)
{
    ostringstream s;
    s << v;
    return s.str();
}

string abbr(string s)
{
    return s;
    /*
    if(s.size()>=6)
        return s.substr(0,3)+".";
    else
        return s;
        */
}

static int ids = 0;
static string last_assigned_id;

string new_id()
{
    ostringstream s;
    s << "vzid" << ids ;
    ids++;
    last_assigned_id = s.str();
    return last_assigned_id;
}

char vz1[3] = {0xc2, 0xa0, 0x0};
string cr = "<div class=\"paragraph Body\" style=\"line-height: 20px; text-decoration: none;\"> "+string(vz1)+" </div>";
string crp = "<p>";

string frame_title(string name, string link)
{
	string template_title = cr + "<div class=\"paragraph Body\" style=\"line-height: 20px; font-size: 13px; font-weight: bold; line-height: 18px; color: #000000\"><a href=@the_link@ target=showframe >@page@</a></div>\n";
    string title = m3_substitute(template_title, "@the_link@", link);
    title = m3_substitute(title, "@page@", name);
    return title;
}

string frame_item(string name, string link)
{
    string template_item = "<div class=\"paragraph Body\" style=\"line-height: 20px; \"><span style=\"font-size: 13px; line-height: 18px; \"><a href=@the_link@ target=showframe >@page_item@</a></span><span style=\"font-size: 13px; font-weight: bold; line-height: 18px; \"></span></div>\n";
    string item= m3_substitute(template_item, "@the_link@", link);
    item = m3_substitute(item, "@page_item@", name);
    return item;
}

string frame_subitem(string name, string link)
{
    string template_item = "<div class=\"paragraph Body\" style=\"line-height: 20px; \"><span style=\"font-size: 13px; line-height: 18px; margin-left: 25px; width: 400px \"><a href=@the_link@ target=showframe >@page_item@</a></span><span style=\"font-size: 13px; font-weight: bold; line-height: 18px; \"></span></div>\n";
    string item= m3_substitute(template_item, "@the_link@", link);
    item = m3_substitute(item, "@page_item@", name);
    return item;
}

string safe(string x)
{
    x = m3_substitute(x, "_", "!");
    x = m3_substitute(x, "!", "\\_");
    x = m3_substitute(x, "%", "!");
    x = m3_substitute(x, "!", "\\%");
    x = m3_substitute(x, "#esc", "!");
    x = m3_substitute(x, "#", "_");
    return x;
}


#define create_latex_report(pg, architecture_name, architecture_owner) \
    string pg = ""; \
    pg = m3_read_latex_template("template_skeleton_report.tex"); \
    pg = m3_substitute(pg, "@architecturename@", safe(architecture_name)); \
    pg = m3_substitute(pg, "@architectureowner@", safe(architecture_owner)); \
    pg = m3_substitute(pg, "@snapshot@", safe(__M3EXPLORER_VERSION__)); 

#define create_latex_page(pg, pname, description) \
    string pg = "\\newpage \\section{@page_title@}\n@page_description@"; \
    pg = m3_substitute(pg, "@page_title@", pname); \
    pg = m3_substitute(pg, "@page_description@", (description)); 

#define create_latex_section(pg, pname, description) \
    string pg = "\\newpage \\subsection{@section_title@}\n@section_description@"; \
    pg = m3_substitute(pg, "@section_title@", pname); \
    pg = m3_substitute(pg, "@section_description@", (description)); 

#define create_summary_page(pg, architecture_name, architecture_owner, description) \
    string pg = ""; \
    pg = m3_read_html_template("template_skeleton_summary.html"); \
    pg = m3_substitute(pg, "@architecture_name@", architecture_name); \
    pg = m3_substitute(pg, "@architecture_owner@", architecture_owner); \
    pg = m3_substitute(pg, "@page_description@", (description)); \
    new_id(); \
    pg = m3_substitute(pg, "@name@", local_id()); 


#define create_page(pg, pname, description) \
    string pg = ""; \
    pg = m3_read_html_template("template_skeleton.html"); \
    pg = m3_substitute(pg, "@page_title@", pname); \
    pg = m3_substitute(pg, "@page_description@", (description)); \
    new_id(); \
    pg = m3_substitute(pg, "@name@", local_id()); 

#define create_page_variable(pg, pname, description, vs) \
    string pg = ""; \
    pg = m3_read_html_template("template_skeleton_variable.html"); \
    pg = m3_substitute(pg, "@page_title@", pname); \
    pg = m3_substitute(pg, "@page_description@", (description)); \
    pg = m3_substitute(pg, "@width@", itos(vs)); \
    new_id(); \
    pg = m3_substitute(pg, "@name@", local_id()); 

#define create_section(pg, pname, description) \
    string pg = m3_read_html_template("template_section.html"); \
    pg = m3_substitute(pg, "@section_title@", pname); \
    pg = m3_substitute(pg, "@section_description@", (description)); \
    new_id(); \
    pg = m3_substitute(pg, "@name@", local_id()); 

#define create_figure(fig, description, position) \
    string fig = m3_read_html_template("template_figure.html"); \
    fig = m3_substitute(fig, "@figure_description@", (description)); \
    fig = m3_substitute(fig, "@figure_position@", (position)); \
    new_id(); \
    fig = m3_substitute(fig, "@name@", local_id());

//string tmp_position = m3_substitute((position), "png", "pdf"); \

#define create_latex_figure(fig, description, position) \
    string fig = "\\begin{center}\\centering\\includegraphics[width=\\textwidth,height=0.4\\textheight,keepaspectratio=true]{@figure_position@}\\\\\n@figure_description@\n\\end{center}\\vspace{0.3cm}"; \
    fig = m3_substitute(fig, "@figure_description@", (description)); \
    fig = m3_substitute(fig, "@figure_position@", (position));

#define create_effects(fig, description, position) \
    string fig = m3_read_html_template("template_effects.html"); \
    fig = m3_substitute(fig, "@figure_description@", (description)); \
    fig = m3_substitute(fig, "@figure_position@", (position)); \
    new_id(); \
    fig = m3_substitute(fig, "@name@", local_id());

#define create_latex_effects(fig, description, position) \
    string fig = "\\begin{center}\\centering\\includegraphics[width=\\textwidth,height=0.4\\textheight,keepaspectratio=true]{@figure_position@}\\\\\n@figure_description@\n\\end{center}\\vspace{0.3cm}"; \
    fig = m3_substitute(fig, "@figure_description@", (description)); \
    fig = m3_substitute(fig, "@figure_position@", (position));


#define add_content(pg, content) \
    pg = m3_substitute(pg, "@stuff@", content);


#define add_link_item(lm, name) (lm = lm + frame_item((name), complete_id()));
#define add_link_title(lm, name) (lm  = lm + frame_title((name), complete_id()));
#define add_link_subitem(lm, name) (lm = lm + frame_subitem((name), complete_id()));

string frame_sep()
{
    return "<div class=\"paragraph Body\" style=\"line-height: 20px; text-decoration: none;\"> <p> </div>";
}

char html_objective_space_description[] = "The following graphs show a set of scatter plots of the objective space by considering two problem objectives at a time. The objective space is then analyzed in terms of each of the parameters levels.";
char html_bb_description[] = "The following graphs show a set of Box Plots for the objectives associated with the design points.";
char latex_bb_added_desctiption_1[] = "\\\\ \\vspace{0.2cm} \\\\ Boxplots (also known as a box-and-whisker plots) can be useful to display differences between populations without making any assumptions of the underlying statistical distribution. The spacings between the different parts of the box help indicate the degree of dispersion (spread) and skewness in the data, and identify outliers.\\\\ ";
char latex_bb_added_desctiption_2[] = " More precisely, the bottom and top of the box are the 25th and 75th percentile (the lower and upper quartiles, respectively), and the band within the box is the 50th percentile (the median). The ends of the whiskers represent the lowest datum still within 1.5 IQR of the lower quartile, and the highest datum still within 1.5 IQR (InterQuartile Range) of the upper quartile. Out of the whiskers are represented the outliers.";
char html_mef_description[] = "The following graphs show the impact of the parameters on each of the system objectives. The impact is computed as the average difference on the system objectives by passing from a low (-) to a high (+) parameter setting.";
char html_int_description[] = "The following graphs show the interaction between the parameters on each of the system objectives. The interaction between parameters is measured as the difference in slope between the associated effects.";
char html_int_description_s[] = "The following graphs show the interaction between the parameters on each of the system objectives by means of surface plots. Values have been averaged over the remaining set of parameters.";
char html_int_description_sc[] = "The following graphs show the interaction between the parameters on each of the system objectives by means of contour maps. Values have been averaged over the remaining set of parameters.";
char latex_int_added_desctiption_1[]="\\\\ \\vspace{0.2cm} \\\\ The presentce of the interaction between the factors is when the difference in response between the levels of one factor is not the same at all levels of the other factors, there is the presence of the interaction between the factors.\\\\";
char latex_int_added_desctiption_2[]=" The concept of interactions between the factors is illustrated graphically by using the previous three figures (called interaction plots) where three different interaction effect of the factor A against B are shown. The first figure shows the case of no interaction between the two factors. The case of no interaction occurs when the two line 'B+' and 'B-' are parallel. Two different cases of interaction between the two factors is illustrated with the other two figures. The case of \\emph{synergistic} interaction of the factor A against B is shown in the second figure, where the two lines present angles with different values and the same sign. The case of \\emph{antagonisitc} interaction of the factor A against B is shown in the third figure, where the two lines present angles with different sign.";
char html_objective_space_pareto_sets_description[] = "The following graphs show a set of scatter plots of the objective space by considering the <b>pareto sets</b> for two problem objectives at a time. The pareto sets of the objective space are then analyzed in terms of each of the parameters levels.";

bool m3_write_html_css(string directory)
{
   string oldd = directory;
   directory = directory + "/template_results_page_files";
   shell_command(("mkdir " + directory).c_str()); 
   shell_command(("cp -R "+m3_get_image_path(&current_environment) + "/html/template_results_page_files/* "+directory).c_str());
   shell_command(("cp "+m3_get_image_path(&current_environment) + "/html/m3_index.html "+ oldd));
}

bool m3_write_html_template(string file_name, string content)
{
    ofstream output(file_name.c_str());
    output << content;
    if(!output.good())
        return false;
    return true;
} 


string base_folder; 
string figs_folder;

//#define CREATE_PDF

bool m3_write_latex_and_create_pdf(string file_name, string content)
{
#ifdef CREATE_PDF
    ofstream output(file_name.c_str());
    output << content;
    if(!output.good())
        return false;
    shell_command("xelatex "+file_name);
    shell_command("xelatex "+file_name);
#endif
    return true;
}

string m3_read_latex_template(string file_name)
{
    string complete_file = m3_get_image_path(&current_environment) + "/latex/" + file_name; 
    ifstream input(complete_file.c_str());
    string whole;
    string line;
    while(!input.eof() && input.good())
    {
        getline(input, line);
        whole = whole + line + "\n";
    }
    return whole;
}

string m3_read_html_template(string file_name)
{
    string complete_file = m3_get_image_path(&current_environment) + "/html/" + file_name; 
    ifstream input(complete_file.c_str());
    string whole;
    string line;
    while(!input.eof() && input.good())
    {
        getline(input, line);
        whole = whole + line + "\n";
    }
    return whole;
}

string m3_substitute(string sentence, string tag, string to_embed)
{
    int pos;
    while((pos = sentence.find(tag))!= string::npos)
    {
        sentence.replace(pos, tag.size(), to_embed); 
    }
    return sentence;
}

#define CHAR_SIZE_PX 8 
string current_file;
string link_material;

#define complete_id() (current_file + "#" + last_assigned_id)
#define local_id() ("\""+ last_assigned_id + "\"")

int row_size(vector<string> & r);
double get_effect(m3_env *env, int par);
int get_significance(m3_env *env, int par);
string perc(double x);
string get_effect_string(m3_env *env, int par, int obj);
string print_interaction_effects(m3_env *env, vector<double> & y_min, vector<double> & y_max, int &size_px, string &);

void generate_summary_page(m3_env *env, vector<double> & y_min, vector<double> & y_max)
{
    current_file = "m3_design_space_summary.html";
    string summary_link_material;

    string title;
    string remaining_report = latex_content;
    reset_latex_content();

    string property;
   
    if(env->current_design_space->architecture_owner=="")
       property = "";
    else
       property = "Property of "+env->current_design_space->architecture_owner;
    
    create_summary_page(page, env->current_design_space->architecture_name, property, "This page shows the summary of the architectural exploration performed on the current use case.");

    create_latex_report(ltx_report, env->current_design_space->architecture_name, property);
    create_latex_page(ltx_page, "Summary of the architectural exploration", "This section shows the summary of the architectural exploration performed on the current use case.");

    add_link_title(summary_link_material, "Summary");
    
    create_section(paretos, "Overall objective space Pareto sets", 
            "This section shows the overall objective space composition by using a series of scatter plots for each pair of problem objectives.");
    add_link_item(summary_link_material, "Overall Pareto analysis");
    create_latex_section(ltx_ov, "Overall objective space Pareto sets", "This subsection shows the overall objective space composition by using a series of scatter plots for each pair of problem objectives.");
  
    add_latex_content(ltx_page);  

    string xml_design_space_file_name;
    if(get_string_from_variables(&current_environment, "xml_design_space_file",xml_design_space_file_name))
    {
        add_latex_content("\\\\ \\vspace{0.2cm} \\\\  The XML design space file used to generate this report is the following: ");
        add_latex_content("\\lstset{ language=XML, basicstyle=\\scriptsize, tabsize=4} \\lstinputlisting{" + xml_design_space_file_name+"} ");
    }
    
    add_latex_content(ltx_ov);  

    int obj_n = env->current_design_space->objectives.size();
    for(int i=0; i<obj_n-1; i++)
    {
        for(int j=i+1; j<obj_n; j++)
        {	
            string pareto_fig_ref = "figs/pareto_"+itos(i)+"_"+itos(j)+".png";

            create_figure(vz_fig, "Objective space Pareto sets, projection on '"+bold(current_environment.current_design_space->objectives_names[i])+"' - '"+ bold(current_environment.current_design_space->objectives_names[j])+"'",  pareto_fig_ref);

            add_link_subitem(summary_link_material, (current_environment.current_design_space->objectives_names[i])+" - "+ (current_environment.current_design_space->objectives_names[j]));

            create_latex_figure(vz_ltx_fig, "Objective space Pareto sets, projection on '"+boldl(current_environment.current_design_space->objectives_names[i])+"' - '"+ boldl(current_environment.current_design_space->objectives_names[j])+"'",  b_folder+"/"+pareto_fig_ref);

            add_latex_content(vz_ltx_fig);
            paretos = paretos + vz_fig;
        }
    }
    
    vector<string> row;
    string table;
    string table_latex;
    int size_px;

    row.push_back(bold("Parameter"));
    row.push_back(bold("Type"));
    row.push_back(bold("Minimum"));
    row.push_back(bold("Maximum"));
    for(int i=0;i < env->current_design_space->objectives_names.size();i++)
    {
        row.push_back(bold(env->current_design_space->objectives_names[i]));
    }
    size_px = row_size(row);
    table = table_head(row);
    table_latex = table_head_latex(row);

    size_px = size_px*CHAR_SIZE_PX; 

    for(int i=0; i<env->current_design_space->ds_parameters.size();i++)
    {
        row.clear();
        string par = env->current_design_space->ds_parameters[i].name;
        if(env->current_design_space->ds_parameters[i].type == M3_DS_SCALAR) 
        {
            row.push_back(par);
            row.push_back("scalar");
            row.push_back(env->current_design_space->get_scalar_min_symbol(env, par));	
            row.push_back(env->current_design_space->get_scalar_max_symbol(env, par));	
            for(int j=0;j<env->current_design_space->objectives_names.size();j++)
            {
                row.push_back(get_effect_string(env, i, j));
            }
            size_px = max(size_px, row_size(row));
        }
        if(env->current_design_space->ds_parameters[i].type == M3_DS_PERMUTATION)
        {
            row.push_back(par);
            row.push_back("permutation");
            row.push_back("N/A");	
            row.push_back("N/A");	
            for(int j=0;j<env->current_design_space->objectives_names.size();j++)
            {
                row.push_back("N/A");
            }

        }
        if(env->current_design_space->ds_parameters[i].type == M3_DS_ON_OFF_MASK)
        {
            row.push_back(par);
            row.push_back("on/off mask");
            row.push_back("N/A");	
            row.push_back("N/A");	
            for(int j=0;j<env->current_design_space->objectives_names.size();j++)
            {
                row.push_back("N/A");
            }
        }
       table = table+table_row(row);
       table_latex = table_latex + table_row_latex(row);
    }

    table = begin_table(size_px) + table + end_table();
    table_latex = begin_table_latex(4+env->current_design_space->objectives_names.size()) + table_latex + end_table_latex();

    create_section(sign_section, "Parameter space significance analysis", "This section shows the parameters used within the design space exploration and the impact of each parameter on the problem objective functions.");
    add_link_item(summary_link_material, "Significance");
    
    create_latex_section(ltx_sign, "Parameter space significance analysis", "This section shows the parameters used within the design space exploration and the impact of each parameter on the problem objective functions.");
    add_latex_content(ltx_sign);
    add_latex_content(table_latex);



    string table_int = print_interaction_effects(&current_environment, y_min, y_max, size_px, table_latex);

    table_int = begin_table(size_px) + table_int + end_table();
    string table_int_latex = begin_table_latex(3) + table_latex + end_table_latex();
    
    create_section(int_section, "Parameter space interactions analysis", "This section shows the main interactions between parameters and the qualitative impact on the problem objective functions. The interaction strength is qualitatively evalued with a strength ranging from 1 to 5. Green stregth indicates synergistic interaction while red strength indicates antagonistic interaction.");
    create_latex_section(ltx_int_section, "Parameter space interactions analysis", "This section shows the main interactions between parameters and the qualitative impact on the problem objective functions. The interaction strength is qualitatively evalued with a strength ranging from 1 to 5. Green stregth indicates synergistic interaction while red strength indicates antagonistic interaction.");
    add_latex_content(ltx_int_section);
    add_latex_content(table_int_latex);
    add_content(ltx_report, latex_content+remaining_report);
    m3_write_latex_and_create_pdf("report.tex", ltx_report);
    add_link_item(summary_link_material, "Interactions");

    /** GENERATE PAGE */
    add_content(page, paretos+sign_section+table+cr+cr+int_section+table_int);

    m3_write_html_template(b_folder+"/m3_design_space_summary.html",page);
    link_material = summary_link_material + link_material;
}


int row_size(vector<string> & r)
{
    int c;
    int s=0;
    for(c=0;c<r.size();c++)
    {
        s = s + r[c].size();
    }
    return s;
}

string vz_db_export_local_db_html(m3_env* env, m3_database* local_db, int & table_size)
{
    string table;
    int size_px;
    if(env->current_design_space)
    {
        vector<string> row;
        int n = 0;
        int wrong_formatted_points = 0;

        /** Writes out the header i.e. the parameter names */
        for(int i=0; i< env->current_design_space->ds_parameters.size(); i++)
        {
            row.push_back(abbr(env->current_design_space->ds_parameters[i].name));
        }

        /** Metrics will be inserted into a separate file */
        /*
        for(int i=0; i<env->current_design_space->metric_names.size(); i++)
        {
            row.push_back(italic(abbr(env->current_design_space->metric_names[i])) + 
                        "[" + abbr(env->current_design_space->unit_names[i]) + "]");
        }*/
        
        for(int i=0; i<env->current_design_space->objectives.size(); i++)
        {
            row.push_back(bold(abbr(env->current_design_space->objectives_names[i]) + 
                        "[" + abbr(env->current_design_space->objectives_units[i]) + "]"));
        }
        size_px = row_size(row);
//        size_px = max(size_px, row_size(row));
        table = table + table_head(row);

        /** Write out the database list */
        m3_list *database_list = local_db->get_list_of_points();
        list<m3_object *>::iterator c;
        for(c=database_list->begin();c!=database_list->end(); c++)
        { 
            m3_point & p =sim_get_point_at_iterator(c);
            if(p.check_consistency(env))
            {
                row.clear();
                for(int i=0; i< env->current_design_space->ds_parameters.size(); i++)
                {
                    row.push_back(env->current_design_space->get_parameter_representation(env, p,
                                                            env->current_design_space->ds_parameters[i].name));
                }
                /*
                for(int i=0; i<env->current_design_space->metric_names.size(); i++)
                {
                    string name = env->current_design_space->metric_names[i];
                    row.push_back(italic(dtos(sim_get_metric_from_point(p, name,env))));
                }*/
                for(int i=0; i<env->current_design_space->objectives.size(); i++)
                {
                    row.push_back(bold(dtos(p.get_objective(env,i))));
                }
                size_px = max(size_px, row_size(row));
                table = table + table_row(row);
                n++;
            }
            else wrong_formatted_points++;
        }
    }
    size_px = size_px*CHAR_SIZE_PX; 
    table = begin_table(size_px) + table + end_table();
    table_size = size_px;
    return table;
}



void db_export_local_db_html(m3_env* env, m3_database* local_db, ofstream& file_name)
{
    if(env->current_design_space == NULL)
    {
        file_name << "Design Space not defined, format unknown <p> " << endl;
        return;
    }
    else
    {
        int n = 0;
        int wrong_formatted_points = 0;
        file_name << " <p> Current database contents \n";
        file_name << " <p>" << endl;
        file_name << " <table border=""2""> <tr>" << endl;

        /** Writes out the header i.e. the parameter names */
        for(int i=0; i< env->current_design_space->ds_parameters.size(); i++)
        {
            file_name << "<td> <b> " << env->current_design_space->ds_parameters[i].name << " </b> </td> ";

        }
        /** Write out the metric names */
        for(int i=0; i<env->current_design_space->metric_names.size(); i++)
        {
            file_name << "<td> <b> " << env->current_design_space->metric_names[i] << " </b> </td> ";
        }
        file_name << "</tr>" << endl;

        /** Write out the database list */
        m3_list *database_list = local_db->get_list_of_points();
        list<m3_object *>::iterator c;
        for(c=database_list->begin();c!=database_list->end(); c++)
        { 
            m3_point & p =sim_get_point_at_iterator(c);
            if(p.check_consistency(env))
            {
                file_name << "<tr>";
                for(int i=0; i< env->current_design_space->ds_parameters.size(); i++)
                {
                    file_name << "<td>" << env->current_design_space->get_parameter_representation(env, p, 
                            env->current_design_space->ds_parameters[i].name) << "</td>";
                }
                for(int i=0; i<env->current_design_space->metric_names.size(); i++)
                {
                    string name = env->current_design_space->metric_names[i];
                    file_name << "<td>" << sim_get_metric_from_point(p, name, env) << "</td>";
                }
                file_name << "</tr>";
                n++;
            }
            else wrong_formatted_points++;
        }

        file_name << " </table> <p> " << endl;
        file_name << "Number of points in the DB: " << n << " <p>\n";
        if (wrong_formatted_points>0)
            file_name << wrong_formatted_points << " were not consistent with design space format. They have been discarded" << endl;
        file_name << " <p> " << endl;
    }
}

bool monochrome = true;

#define pred(p, command) p = (p) ? shell_command(string(command)) : false

bool generate_png_and_pdf(ostringstream & script, string gnuplot_script_name, string fig_name, vector<string> files_to_remove)
{
    string the_script_png = script.str();
    
    string the_script_pdf;
    if(monochrome)
       the_script_pdf = m3_substitute(the_script_png,  "set terminal png", "set terminal postscript eps monochrome");
    else
       the_script_pdf = m3_substitute(the_script_png,  "set terminal png", "set terminal postscript eps color");

    the_script_pdf = m3_substitute(the_script_pdf, "pt 13 ps 0.7", "");
    the_script_pdf = m3_substitute(the_script_pdf,  ".png", ".ps");

    string n_png = gnuplot_script_name+"_png"; 
    string n_pdf = gnuplot_script_name+"_pdf"; 


    ofstream gs_png(n_png.c_str() ,ios::out);
    ofstream gs_pdf(n_pdf.c_str() ,ios::out);

    gs_png << the_script_png;
    gs_pdf << the_script_pdf;

    gs_png.close();
    gs_pdf.close();

    bool p = true;

    pred(p, "gnuplot "+n_png);
#ifdef CREATE_PDF
    pred(p, "gnuplot "+n_pdf);
    pred(p, "pstopdf "+m3_substitute(fig_name, ".png", ".ps"));
#endif
#ifdef REMOVE_FILES
    pred(p, "rm "+n_png);
    pred(p, "rm "+n_pdf);
    for(int i=0; i<files_to_remove.size(); i++)
    {
         pred(p, "rm "+files_to_remove[i]);
    }
#endif
    return p;
}


void plot_map_to_figure(m3_database *the_database, int obj1, int par1, int par2)
{
    string local_file = "map_"+itos(par1)+"_"+itos(par2)+"_"+itos(obj1)+".data";
    //string local_file = current_environment.current_design_space->objectives_names[obj1];
    string figure_name = figs_folder + "/map_"+itos(par1)+"_"+itos(par2)+"_"+itos(obj1)+".png";
    the_database->write_param_obj_to_file(&current_environment , local_file.c_str(), obj1, par1, par2);
    string O1 = current_environment.current_design_space->objectives_names[obj1];
    string u1 = current_environment.current_design_space->objectives_units[obj1];
    string p1n = current_environment.current_design_space->ds_parameters[par1].name;
    string p2n = current_environment.current_design_space->ds_parameters[par2].name;

    string gnuplot_script_name = "map_"+itos(par1)+"_"+itos(par2)+"_"+itos(obj1)+".scr";

    ostringstream gnuplot_script;
    m3_env *env = &current_environment;

    string p1_min = env->current_design_space->get_scalar_min_symbol(env, p1n);
    string p1_max = env->current_design_space->get_scalar_max_symbol(env, p1n);

    string p2_min = env->current_design_space->get_scalar_min_symbol(env, p2n);
    string p2_max = env->current_design_space->get_scalar_max_symbol(env, p2n);

    /*
     * set xtics ("low" -3, "mid" 0, "high" 3)
     * set ytics ...
     */ 
    vector<string> xt = env->current_design_space->get_scalar_range(env, p1n);
    vector<string> yt = env->current_design_space->get_scalar_range(env, p2n);

    int multipl = 10;
    int xts = xt.size(); 
    int yts = yt.size();

    ostringstream dg;
    dg << "set dgrid3d " << xts*multipl << ", " << yts*multipl << ", 4";

    string dgrid = dg.str();
    string pm = "set pm3d ";

//#define trick(k) (((double) (k)) + 0.1 )

    gnuplot_script << "set xtics (";
    for(int i=0; i<xt.size(); i++) 
        gnuplot_script << "\"" << xt[i] << "\" " << (i) << ((i!=(xt.size()-1)) ? "," : "");
    gnuplot_script << ")" << endl;
    
    gnuplot_script << "set ytics (";
    for(int i=0; i<yt.size(); i++) 
        gnuplot_script << "\"" << yt[i] << "\" " << (i) << ((i!=(yt.size()-1)) ? "," : "");
    gnuplot_script << ")" << endl;

    //gnuplot_script << "set xrange [" << p1_min << ":" << p1_max << "]" << endl;
    //gnuplot_script << "set yrange [" << p2_min << ":" << p2_max << "]" << endl;
    gnuplot_script << "set key off" << endl;
    gnuplot_script << "set xlabel \"" << p1n << "\"" << endl;
    gnuplot_script << "set ylabel \"" << p2n << "\"" << endl;
    //gnuplot_script << "set logscale z" << endl;
    //gnuplot_script << "set zlabel \"" << O1 << " [" << u1 << "]" << "\"" << endl;
    //gnuplot_script << "set zlabel \"" << O1 << "\"" << endl;
    //gnuplot_script << "set label 1 \"" << O1 << " [" << u1 << "]" << "\" center rotate by 90 at graph 0, graph 0, graph 0.5 offset -6 " << endl;
    gnuplot_script << "unset surface" << endl;
    gnuplot_script << dgrid << endl;
    gnuplot_script << pm << endl;
    gnuplot_script << "set terminal png" << endl;
    gnuplot_script << "set output \""<< figure_name << "\"" << endl;
    gnuplot_script << "splot '"+local_file+"' with dots" << endl;

    vector<string> remove;

    monochrome = false;
    if(!generate_png_and_pdf(gnuplot_script, gnuplot_script_name, figure_name, remove))
    {
        prs_display_error("Error running gnuplot");
    }
    gnuplot_script_name = gnuplot_script_name+".flat";
    figure_name = figs_folder + "/map_"+itos(par1)+"_"+itos(par2)+"_"+itos(obj1)+"_flat.png";
    
    ostringstream gnuplot_script2;
    
    gnuplot_script2 << "set xtics (";
    for(int i=0; i<xt.size(); i++) 
        gnuplot_script2 << "\"" << xt[i] << "\" " << i << ((i!=(xt.size()-1)) ? "," : "");
    gnuplot_script2 << ")" << endl;
    
    gnuplot_script2 << "set ytics (";
    for(int i=0; i<yt.size(); i++) 
        gnuplot_script2 << "\"" << yt[i] << "\" " << i << ((i!=(yt.size()-1)) ? "," : "");
    gnuplot_script2 << ")" << endl;
    
    /*gnuplot_script2 << "set yrange [" << p2_min << ":" << p2_max << "]" << endl;*/
    gnuplot_script2 << "set key off" << endl;
    gnuplot_script2 << "set xlabel \"" << p1n << "\"" << endl;
    gnuplot_script2 << "set ylabel \"" << p2n << "\"" << endl;
    gnuplot_script2 << "set palette color positive" << endl;
    gnuplot_script2 << dgrid << endl;
    // gnuplot_script2 << "set logscale z" << endl;
    gnuplot_script2 << pm << " map" << endl;
    gnuplot_script2 << "set terminal png" << endl;
    gnuplot_script2 << "set output \""<< figure_name << "\"" << endl;
    gnuplot_script2 << "splot '"+local_file+"' " << endl;

    remove.push_back(local_file);
    if(!generate_png_and_pdf(gnuplot_script2, gnuplot_script_name, figure_name, remove))
    {
        prs_display_error("Error running gnuplot");
    }
    /* 
    gnuplot_script_name = gnuplot_script_name+".cntr";
    figure_name = figs_folder + "/map_fig_"+itos(par1)+"_"+itos(par2)+"_"+itos(obj1)+"_cntr.png";

    ostringstream gnuplot_script3;
    
    gnuplot_script3 << "set yrange [" << p2_min << ":" << p2_max << "]" << endl;
    gnuplot_script3 << "set key off" << endl;
    gnuplot_script3 << "set xlabel \"" << p1n << "\"" << endl;
    gnuplot_script3 << "set ylabel \"" << p2n << "\"" << endl;
    gnuplot_script3 << "set dgrid3d" << endl;
    gnuplot_script3 << "set contour base" << endl;
    gnuplot_script3 << "set view 0,0" << endl;
    gnuplot_script3 << "set terminal png" << endl;
    gnuplot_script3 << "set output \""<< figure_name << "" << endl;
    gnuplot_script3 << "splot '"+local_file+"' " << endl;
    if(!generate_png_and_pdf(gnuplot_script3, gnuplot_script_name, figure_name, remove))
    {
        prs_display_error("Error running gnuplot");
    }
    */
    monochrome = true;
}

    

void pareto_plot_to_file_local_db_obj(m3_database* local_db, string fig_name, int obj1, int obj2, bool is_local)
{
    if(current_environment.current_design_space == NULL)
    {
        prs_display_error("Design Space not defined, format unknown");
        return;
    }

    //Need a unique name for temp file due to multithreading
    string db_filename;
    if(!is_local) 
        db_filename =  "g_db_"+itos(obj1)+"_"+itos(obj2)+".tmp";
    else 
        db_filename =  "l_db_"+itos(obj1)+"_"+itos(obj2)+".tmp";

    local_db->write_objectives_to_file(&current_environment, db_filename.c_str(), obj1, obj2);
    string O1 = current_environment.current_design_space->objectives_names[obj1];
    string O2 = current_environment.current_design_space->objectives_names[obj2];
    string u1 = current_environment.current_design_space->objectives_units[obj1];
    string u2 = current_environment.current_design_space->objectives_units[obj2];

    //Need a unique name for temp file due to multithreading
    string gnuplot_script_name;
    if(!is_local) 
        gnuplot_script_name = "g_pareto_"+itos(obj1)+"_"+itos(obj2)+".tmp.scr";
    else 
        gnuplot_script_name = "l_pareto_"+itos(obj1)+"_"+itos(obj2)+".tmp.scr";

    ostringstream gnuplot_script;
    gnuplot_script << "unset grid\n";
    gnuplot_script << "set key right\n";
	gnuplot_script << "set xlabel \"" << O1 << " [" << u1 << "]\"\n";
	gnuplot_script << "set ylabel \"" << O2 << " [" << u2 << "]\"\n";
    gnuplot_script << "set terminal png" << endl;
    gnuplot_script << "set output \""<< fig_name << "" << endl;
    gnuplot_script << "plot \""<< db_filename <<"\" notitle with points pt 13";

    vector<string> remove;
    remove.push_back(db_filename);

    if(!generate_png_and_pdf(gnuplot_script, gnuplot_script_name, fig_name, remove))
    {
        prs_display_error("Error running gnuplot");
    }

}



void pareto_plot_to_file_obj(string db, string fig_name, int obj1, int obj2)
{
    if(!current_environment.available_dbs.count(db))
    {//check existence of dbs
        prs_display_error("Required database not available");
        return;
    }
    pareto_plot_to_file_local_db_obj(current_environment.available_dbs[db], fig_name, obj1, obj2, 0);
}



void level_analysis_plot_to_file_local_db_obj(m3_env* env, m3_database* local_db, string fig_name, string par, int obj1, int obj2, bool is_local)
{
    int min_level, max_level, curr_level, level_num;
    int curr_index;
    string min_symbol, max_symbol, curr_symbol;


    if(env->current_design_space == NULL)
    {
        prs_display_error("Design Space not defined, format unknown");
        return;
    }


    min_symbol = env->current_design_space->get_scalar_min_symbol(env, par);	
    min_level = env->current_design_space->get_scalar_level(env, par, min_symbol);
    max_symbol = env->current_design_space->get_scalar_max_symbol(env, par);	
    max_level = env->current_design_space->get_scalar_level(env, par, max_symbol);
    level_num = (max_level - min_level) +1;

    m3_database *new_database[level_num];
    string level_value[level_num];
    char no_points[40];
    for(int z=0; z<level_num; z++)
    {
        new_database[z] = new m3_database();
        level_value[z] = "no_points";
    }


    m3_list *database_list = local_db->get_list_of_points();
    list<m3_object *>::iterator c;
    for(c=database_list->begin();c!=database_list->end();)
    {
        m3_point & p =sim_get_point_at_iterator(c);
        curr_symbol = env->current_design_space->get_parameter_representation(env, p, par); 
        curr_level = env->current_design_space->get_scalar_level(env, par, curr_symbol);
        new_database[curr_level-min_level]->insert_point(&p);
        curr_index = curr_level - min_level;
        level_value[curr_index]= curr_symbol;
        c++;
    }

    string O1 = env->current_design_space->objectives_names[obj1];
    string O2 = env->current_design_space->objectives_names[obj2];
    string u1 = current_environment.current_design_space->objectives_units[obj1];
    string u2 = current_environment.current_design_space->objectives_units[obj2];
    //Need a unique name for temp file due to multithreading
    string gnuplot_script_name;
    if(!is_local) 
        gnuplot_script_name = "g_scatter_"+par+"_"+itos(obj1)+"_"+itos(obj2)+".tmp.scr";
    else 
        gnuplot_script_name = "l_scatter_"+par+"_"+itos(obj1)+"_"+itos(obj2)+".tmp.scr";

    ostringstream gnuplot_script;
    gnuplot_script << "unset grid\n";
    gnuplot_script << "set key right\n";
	gnuplot_script << "set xlabel \"" << O1 << " [" << u1 << "]\"\n";
	gnuplot_script << "set ylabel \"" << O2 << " [" << u2 << "]\"\n";
    gnuplot_script << "set terminal png" << endl;
    gnuplot_script << "set output \""<< fig_name << "" << endl;
    gnuplot_script << "plot  ";


    int has_previous = 0;
    string db_filename[level_num];
    vector<string> rm;
    for(int t=0; t<level_num; t++)
    {
        //Checking existence of points having the current level
        if(level_value[t] != "no_points")
        {
            //Need a unique name for temp file due to multithreading
            string curr_db_filename;
            if(!is_local) 
                curr_db_filename = "g_"+par+"_"+level_value[t]+"_"+itos(obj1)+"_"+itos(obj2);
            else 
                curr_db_filename = "l_"+par+"_"+level_value[t]+"_"+itos(obj1)+"_"+itos(obj2);

            new_database[t]->write_objectives_to_file(env, curr_db_filename.c_str(), obj1, obj2);
            db_filename[t] = curr_db_filename;
            if(has_previous == 0) 
            {
                gnuplot_script << "\"" << db_filename[t] << "\" title \""<< par <<" = "<< level_value[t].c_str() <<"\"";
                gnuplot_script <<" with points pt 13 ps 0.7";
                has_previous = 1;
            }
            else 
            {
                gnuplot_script << ", \"" << db_filename[t] << "\" title \""<< par <<" = "<< level_value[t].c_str() <<"\"";
                gnuplot_script <<" with points pt 13 ps 0.7";
            }
            rm.push_back(db_filename[t]);
        }
    }

    if(!generate_png_and_pdf(gnuplot_script, gnuplot_script_name, fig_name, rm))
    {
        prs_display_error("Error running gnuplot");
    }

// clean up memory
    for(int z=0; z<level_num; z++)
    {
        delete new_database[z];
    }
}



void level_analysis_plot_to_file_obj(m3_env* env, string fig_name, string par, int obj1, int obj2)
{
    if(!env->available_dbs.count(env->current_db_name))
    {//check existence of dbs
        prs_display_error("Required database not available");
        return;
    }
    level_analysis_plot_to_file_local_db_obj(env, env->available_dbs[env->current_db_name], fig_name, par, obj1, obj2, 0);
}



void parameter_objective_boxplot_to_file(m3_env* env, string fig_name, string par, int obj_index)
{
    string objective_name = env->current_design_space->objectives_names[obj_index];
    int min_level, max_level, curr_level, level_num;
    double min_objective_value, max_objective_value, curr_objective_value;
    int curr_index;
    string min_symbol, max_symbol, curr_symbol;

    if(env->current_design_space == NULL)
    {
        prs_display_error("Design Space not defined, format unknown");
        return;
    }


    min_level = env->current_design_space->get_scalar_min(env, par);
    max_level = env->current_design_space->get_scalar_max(env, par);    
    level_num = (max_level - min_level) +1;
    m3_database *new_database[level_num];
    string level_value[level_num];
    char no_points[40];
    for(int z=0; z<level_num; z++)
    {
        new_database[z] = new m3_database();
        level_value[z] = "no_points";
        //cout << "Created database " << z << endl;
    }


    m3_list *database_list = env->available_dbs[env->current_db_name]->get_list_of_points();
    list<m3_object *>::iterator c;

    //Initializing min_objective_value and max_objective_value with random values (from the first point)
    m3_point & init_point =sim_get_point_at_iterator(database_list->begin());
    min_objective_value = init_point.get_objective(env, obj_index);  
    max_objective_value = min_objective_value;
    for(c=database_list->begin();c!=database_list->end();)
    {
        m3_point & p =sim_get_point_at_iterator(c);
        curr_symbol = env->current_design_space->get_parameter_representation(env, p, par); 
        curr_level = env->current_design_space->get_scalar_level(env, par, curr_symbol);

        int accessed = curr_level-min_level;

        if((curr_level >= min_level) && (curr_level <= max_level))
        {
            if(new_database[accessed]==NULL)
            {
                /*
                 * List size is 1
                 * Curr level is 0
                 * Curr level symbol is 16
                 * Min level is 1
                 */
                cout << "Failure while accessing " << accessed << endl;
                cout << "List size is " << level_num << endl;
                cout << "Curr level is " << curr_level << endl;
                cout << "Curr level symbol is " << curr_symbol << endl;
                cout << "Min level is " << min_level << endl;
            }
            new_database[accessed]->insert_point(&p);
            curr_index = accessed;
            level_value[curr_index]= curr_symbol;

            //Assigning min_objective_value and max_objective_value actual min and max values present in the db
            curr_objective_value = p.get_objective(env, obj_index);
            if(curr_objective_value < min_objective_value) min_objective_value = curr_objective_value;
            if(curr_objective_value > max_objective_value) max_objective_value = curr_objective_value;
        }
        c++;
    }

    double margin;
    if(max_objective_value == min_objective_value)
        margin = max_objective_value *0.1;
    else 
        margin = (max_objective_value - min_objective_value)/20;



    //Need a unique name for temp file due to multithreading
    string gnuplot_script_name = "box_plot_"+par+"_"+itos(obj_index)+".tmp.scr";

    ofstream gnuplot_script(gnuplot_script_name.c_str() ,ios::out);
    gnuplot_script << "unset grid\n";
    gnuplot_script << "unset key\n";
    gnuplot_script << "set boxwidth 0.2 absolute\n";
    gnuplot_script << "set xlabel \"" << par << " level""\n";
    gnuplot_script << "set xrange [" << min_level-0.5 <<":"<< max_level+0.5 << "]\n";
    string u = current_environment.current_design_space->objectives_units[obj_index];
    gnuplot_script << "set ylabel \"" << objective_name << " [" << u << "]\"\n";
    gnuplot_script << "set yrange ["<< min_objective_value-margin <<":"<< max_objective_value+margin <<"]\n";	
    gnuplot_script << "set terminal png" << endl;
    gnuplot_script << "set output \""<< fig_name << "\"" << endl;


    gnuplot_script << "set xtic (\"\" 0";
    if(env->current_design_space->scalar_parameters[par].type != M3_SCALAR_TYPE_INTEGER)
    {
        //Generating xtics in gnuplot script 
        for(int tic=0; tic< env->current_design_space->scalar_parameters[par].list_size; tic++)
        {
            gnuplot_script << ", \"" << env->current_design_space->scalar_parameters[par].list[tic] << "\" " << tic+min_level;
        }
    }
    else
    {
        for(int tic = env->current_design_space->scalar_parameters[par].min;
                tic <= env->current_design_space->scalar_parameters[par].max;
                tic++)
        {
            gnuplot_script << ", \"" << tic << "\" " << tic;
        }
    }
    gnuplot_script << ")\n";
    gnuplot_script << "plot  ";


    int has_points = 0;
    int has_previous = 0;
    string curr_db_bp;
    string curr_db_bp_outliers;
    double first_quartile, second_quartile, third_quartile, sample_min, sample_max, IQR;
    string db_filename[level_num];
    string db_outliers_filename[level_num];
    vector <float> objective_values;

    for(int t=0; t<level_num; t++)
    {	
	objective_values.clear();
        //Checking existence of points having the current level
        if(level_value[t] != "no_points")
        {
            database_list = new_database[t]->get_list_of_points();
            list<m3_object *>::iterator c;
            for(c=database_list->begin();c!=database_list->end(); c++)
            { 		
                m3_point & p =sim_get_point_at_iterator(c);


                if(p.check_consistency(env))
                {
                    objective_values.push_back(p.get_objective(env, obj_index));
                }
            }


            //Calculating box-plot data

	    sort(objective_values.begin(), objective_values.end());
	    vector<double> ordered_vector( objective_values.begin(), objective_values.end() );
	    first_quartile	= m3_get_percentile_value( 25, ordered_vector );
	    second_quartile	= m3_get_percentile_value( 50, ordered_vector );
	    third_quartile	= m3_get_percentile_value( 75, ordered_vector );
            sample_min		= objective_values[0];
            sample_max 		= objective_values[objective_values.size()-1];

            //Limiting whiskers to 1.5 Inter-Quartile Range
            IQR = third_quartile-first_quartile;
            if(sample_min<(first_quartile-(1.5*IQR)))
	    {
	      sample_min = first_quartile-(1.5*IQR);

	    }
            if(sample_max>(third_quartile+(1.5*IQR))) 
	    {
	      sample_max = third_quartile+(1.5*IQR);

	    }

            //Writing box-plot data files (box-polt)
            curr_db_bp = par+"_"+level_value[t]+"_"+itos(obj_index);
            db_filename[t] = curr_db_bp;
            ofstream curr_db_boxplot;
            curr_db_boxplot.open(curr_db_bp.c_str(), ios::out);
            m3_assert(!curr_db_boxplot.fail());
            curr_db_boxplot << t+min_level << "  " << first_quartile << "  " << sample_min << "  " << second_quartile << "  ";
            curr_db_boxplot << sample_max << "  " << third_quartile << "  \n";
            curr_db_boxplot.close();
            //Writing box-plot data files (outliers)
            curr_db_bp_outliers = par+"_"+level_value[t]+"_"+itos(obj_index)+"_o";
            db_outliers_filename[t] = curr_db_bp_outliers;
            ofstream curr_db_boxplot_o;
            curr_db_boxplot_o.open(curr_db_bp_outliers.c_str(), ios::out);
            m3_assert(!curr_db_boxplot_o.fail());
             for (int b=0; b<objective_values.size(); b++)
             {
                 if(objective_values[b]< sample_min || objective_values[b]> sample_max)
                 {
                     curr_db_boxplot_o << t+min_level << "  " << objective_values[b] <<"\n";
                     has_points = 1;
                 }
             }
            curr_db_boxplot_o.close();


            //Adding plotting commands to gnuplot script
            if(has_previous == 0) 
            {
                has_previous = 1;
                gnuplot_script << "\"" << db_filename[t] << "\" using 1:2:3:5:6 with candlesticks lt 3 lw 2 ";
                gnuplot_script << "title \""<< par <<" = "<< level_value[t].c_str() <<"\" whiskerbars, ";
                gnuplot_script << "\"" << db_filename[t] << "\" using 1:4:4:4:4 with candlesticks lt -1 lw 2 notitle";
                if(has_points != 0)
                    gnuplot_script << ", \"" << db_outliers_filename[t] << "\" with points lt 3 pt 6 ps 0.7";

            }
            else
            {
                gnuplot_script << ", \"" << db_filename[t] << "\" using 1:2:3:5:6 with candlesticks lt 3 lw 2 ";
                gnuplot_script << "title \""<< par <<" = "<< level_value[t].c_str() <<"\" whiskerbars, ";
                gnuplot_script << "\"" << db_filename[t] << "\" using 1:4:4:4:4 with candlesticks lt -1 lw 2 notitle";
                if(has_points != 0)
                    gnuplot_script << ", \"" << db_outliers_filename[t] << "\" with points lt 3 pt 6 ps 0.7";
            }
            has_points = 0;
        }
    }
    gnuplot_script.close();

    ostringstream run_gnuplot;
    run_gnuplot << "gnuplot \""<< gnuplot_script_name <<"\"\n";

#ifdef REMOVE_FILES
     for(int x=0; x<level_num; x++)
     {
         if(level_value[x] != "no_points") 
         {
             run_gnuplot << "rm " << db_filename[x] << endl;
             run_gnuplot << "rm " << db_outliers_filename[x] << endl;
         }
     }
     run_gnuplot << "rm \""<< gnuplot_script_name <<"\"\n"<< endl;
#endif

// clean up memory
    for(int z=0; z<level_num; z++)
    {
        delete new_database[z];
    }

    if(!shell_command(run_gnuplot.str())){
        prs_display_error("error running gnuplot");
        return;
    }
}



void add_main_effect(m3_env *env, int parameter, int objective, double min, double max);

void main_effects_plot_to_file(m3_env* env, string fig_name, int obj_index)
{

    string objective_name = env->current_design_space->objectives_names[obj_index];

    int min_level, max_level, curr_level;
    double min_objective_value, max_objective_value, delta_objective_value;
    double objective_min_tot, objective_max_tot;
    int min_level_n, max_level_n;
    string curr_symbol, curr_par;


    if(env->current_design_space == NULL)
    {
        prs_display_error("Design Space not defined, format unknown");
        return;
    }

    string level_filename;
    int par_num = 0;
    double left_x = 0.25;
    for(int count=0; count<env->current_design_space->ds_parameters.size(); count++)
    {
        if(env->current_design_space->ds_parameters[count].type == M3_DS_SCALAR) 
        {
            par_num++;
        }
        if(env->current_design_space->ds_parameters[count].type == M3_DS_ON_OFF_MASK) 
        {
            par_num++;
        }
        if(env->current_design_space->ds_parameters[count].type == M3_DS_PERMUTATION) 
        {
            par_num++;
        }
    }
    string curr_level_filename[par_num];


    //Need a unique name for temp file due to multithreading
    string gnuplot_script_name = "main_effects_"+objective_name+".tmp.scr";

    #ifdef DEBUG_MAIN_EFFECT_ON_FILE
        string debug_out_name = "debug_main_"+objective_name+".dbg.txt";
        ofstream debug_out(debug_out_name.c_str() ,ios::out);
	debug_out << "START DEBUG " << objective_name << endl<<endl;
    #endif


    ofstream gnuplot_script(gnuplot_script_name.c_str() ,ios::out);
    gnuplot_script << "unset grid\n";
    gnuplot_script << "unset key\n";
    string u = current_environment.current_design_space->objectives_units[obj_index];
    gnuplot_script << "set ylabel \"" << objective_name << " [" << u << "]\"\n";

    if(par_num < 5)
    {
        gnuplot_script << "set terminal png size 640,540" << endl;
        gnuplot_script << "set output \""<< fig_name << "\"" << endl;
        gnuplot_script << "set multiplot\n";
        gnuplot_script << "set size 1,0.9\n";
        gnuplot_script << "set origin 0, 0.1\n";
    }
    else
    {
        gnuplot_script << "set terminal png size 900,540" << endl;
        gnuplot_script << "set output \""<< fig_name << "\"" << endl;
        gnuplot_script << "set multiplot\n";
        gnuplot_script << "set size 1,0.9\n";
        gnuplot_script << "set origin 0, 0.1\n";
    }


    m3_list *database_list = env->available_dbs[env->current_db_name]->get_list_of_points();
    list<m3_object *>::iterator c;

    int initialized = 0;
    int initialized_delta = 0;
    double min, max, temp;
    for(int i=0; i<env->current_design_space->ds_parameters.size(); i++)
    {
        objective_min_tot = 0;
        objective_max_tot = 0;		
        min_level_n = 0;
        max_level_n = 0;

        //-----------------------------------------------------------------------
        //                SCALAR main effects graph generation
        //-----------------------------------------------------------------------

        if(env->current_design_space->ds_parameters[i].type == M3_DS_SCALAR)
        {
            curr_par = env->current_design_space->ds_parameters[i].name;

            //Initializing min_level and max_level with random values (from the first point)
            m3_point & init_point =sim_get_point_at_iterator(database_list->begin());
            curr_symbol = env->current_design_space->get_parameter_representation(env, init_point, curr_par); 
            min_level = env->current_design_space->get_scalar_level(env, curr_par, curr_symbol); 
            max_level = env->current_design_space->get_scalar_level(env, curr_par, curr_symbol);

            //Assigning min_level and max_level actual min and max values present in the db
            for(c=database_list->begin();c!=database_list->end();)
            {
                m3_point & p =sim_get_point_at_iterator(c);
                curr_symbol = env->current_design_space->get_parameter_representation(env, p, curr_par); 
                curr_level = env->current_design_space->get_scalar_level(env, curr_par, curr_symbol);
                if(curr_level < min_level) min_level = curr_level;
                if(curr_level > max_level) max_level = curr_level;
                c++;
            }

            for(c=database_list->begin();c!=database_list->end();)
            {
                m3_point & p =sim_get_point_at_iterator(c);
                curr_symbol = env->current_design_space->get_parameter_representation(env, p, curr_par); 
                curr_level = env->current_design_space->get_scalar_level(env, curr_par, curr_symbol);
                if(curr_level == min_level)
                {	
                    objective_min_tot = objective_min_tot + p.get_objective(env, obj_index);
                    min_level_n++;		
                }
                if(curr_level == max_level)
                {
                    objective_max_tot = objective_max_tot + p.get_objective(env, obj_index);
                    max_level_n++;		
                }
                c++;
            }	

            level_filename = curr_par+"_"+objective_name;
            curr_level_filename[i] = level_filename;
            ofstream level_file;
            level_file.open(level_filename.c_str(), ios::out);
            m3_assert(!level_file.fail());
            level_file << left_x << "  " << objective_min_tot/min_level_n << endl;
            level_file << left_x+1 << "  " << objective_max_tot/max_level_n << endl;
            /** Adding the main effect to the central structure */
            add_main_effect(env, i, obj_index, objective_min_tot/min_level_n, objective_max_tot/max_level_n);
            /****/

            level_file.close();
            left_x = left_x+1.5;	


            //Initializing and assigning max_objective_value and min_objective_value
            min = objective_min_tot/min_level_n;
            max = objective_max_tot/max_level_n;
            //- could have a objective value greater than +. In that case values are swapped
            if(min>max)
            {
                temp = min;
                min = max;
                max = temp;
            }	
            if(initialized==0) 
            {
                min_objective_value = min;
                max_objective_value = max;
                delta_objective_value = max-min;
                initialized = 1;
            }
            else
            {
                if((min) < min_objective_value) min_objective_value = min;
                if((max) > max_objective_value) max_objective_value = max; 
                if((max-min) > delta_objective_value ) delta_objective_value = max -min;
            }
            if(initialized_delta==0) 
            {
                delta_objective_value = max-min;
                initialized_delta = 1;
            }
            else
            {
                if((max-min) > delta_objective_value ) delta_objective_value = max -min;
            }
        }  //end SCALAR if
        //-----------------------------------------------------------------------
        //                MASK main effects graph generation
        //-----------------------------------------------------------------------
        if(env->current_design_space->ds_parameters[i].type == M3_DS_ON_OFF_MASK)
        {
            curr_par = env->current_design_space->ds_parameters[i].name;
            int index_min = randint(0, (database_list->size()-1));

            // Assigning min_level
            int index_i = 0;
            vector <int> min_level_vector, scrambled_min_level_vector, max_level_vector, curr_level_vector, max_constrained_level_vector;
            int scrambled_min_level;
            int osize;
            int dimension;
            m3_mask_low_high_parameter x;
            bool satisfy_constraints = false;
            bool at_least_one_satisfy_constraints=false;
                                
            for(c=database_list->begin();c!=database_list->end();)
            {
                // obtain the min_level and the expected scramble(min_level)
                if(index_i == index_min)
                {
                    m3_point &p = sim_get_point_at_iterator(c);
                    min_level_vector = env->current_design_space->get_mask(env, &p, curr_par);
                    min_level = bit_vector_to_int(min_level_vector);
                    
                    m3_on_off_mask &mask = env->current_design_space->on_off_mask_parameters[curr_par];
                    osize = on_set_size(min_level_vector);      	    
                    dimension = env->current_design_space->get_mask_dimension(env, curr_par, &p);
               
                    x.mask_name = curr_par;
                    x.no_on_set_size = mask.no_on_set_size;
                    x.variable_dimension = mask.variable_dimension;
                    x.variable_on_set_size = mask.variable_on_set_size;
                    x.on_set_size = osize;
                    x.dimension = dimension; 
                    x.index=-1;
 
                    scrambled_min_level_vector = env->current_design_space->mask_scrambler(min_level_vector, x);
                    scrambled_min_level = bit_vector_to_int(scrambled_min_level_vector);
                    break;
                }
                
                index_i++;
                c++;
            }

            #ifdef DEBUG
	        cout << "Selected MIN mask --------> : ";
	        cout << env->current_design_space->get_vector_representation(env, min_level_vector);
	        cout << " dimension: "<<dimension<<" osize: "<<osize<<endl; 
	        cout << "corresponding scrambled --> : ";
	        cout << env->current_design_space->get_vector_representation(env, scrambled_min_level_vector);
	        cout << endl;
   	     #endif

            #ifdef DEBUG_MAIN_EFFECT_ON_FILE
	        debug_out << "Selected MIN mask --------> : ";
	        debug_out << env->current_design_space->get_vector_representation(env, min_level_vector);
	        debug_out << " dimension: "<<dimension<<" osize: "<<osize<<endl; 
	        debug_out << "corresponding scrambled --> : ";
	        debug_out << env->current_design_space->get_vector_representation(env, scrambled_min_level_vector);
	        debug_out << endl;
   	     #endif

  
            // Assigning max_level 
            max_level = -1;
            for(c=database_list->begin();c!=database_list->end();)
            {
                // read an element
                m3_point & p = sim_get_point_at_iterator(c);                
                curr_level_vector = env->current_design_space->get_mask(env, &p, curr_par);
                curr_level = bit_vector_to_int(curr_level_vector);
                if(curr_level == scrambled_min_level)
                {
                    // if is equal to the scramble of min_level select that as max
                    max_level = curr_level;
                    break;
                }
                index_i++;
                c++;
            }
            
            // if max is not foud search for the nearest...among the available point in the DB
            if(max_level==-1)
            {   
                #ifdef DEBUG
                    cout<<"** not find exact scramble search for the nearest.."<<endl;
                    cout << "curr_mask     satisfy_constraints    distance    min_distance    min_constrained_distance  *  x.no_on_set_size   curr_on_set_size   curr_dimension"<<endl<<endl;
                #endif

                #ifdef DEBUG_MAIN_EFFECT_ON_FILE
                    debug_out<<"** not find exact scramble search for the nearest.."<<endl;
                    debug_out << "curr_mask     satisfy_constraints    distance    min_distance    min_constrained_distance  *  x.no_on_set_size   curr_on_set_size   curr_dimension"<<endl<<endl;
                #endif



                double min_distance, distance;
                double min_constrained_distance;
		bool exist_at_least_one = false;

       		// note that distance is normalized, so is always between 0 and 1
		min_distance = 2.00;
                min_constrained_distance = 2.00;
                //max_level_vector = curr_level_vector;
        
                
                for(c=database_list->begin();c!=database_list->end();)
                {
                    // read an element		 
		    m3_point & p = sim_get_point_at_iterator(c);                
                    curr_level_vector = env->current_design_space->get_mask(env, &p, curr_par);
		   
		    if(curr_level_vector == min_level_vector)
		    {  

                        #ifdef DEBUG 
	                    cout << env->current_design_space->get_vector_representation(env, curr_level_vector)<<" == min_level_vector - SKIP - "<<endl;
                        #endif

                        #ifdef DEBUG_MAIN_EFFECT_ON_FILE 
	                    debug_out << env->current_design_space->get_vector_representation(env, curr_level_vector)<<" == min_level_vector - SKIP - "<<endl;
                        #endif
 
                        c++; 
                        continue;
		    }

                    distance = normalized_average_mask_distance(scrambled_min_level_vector, curr_level_vector);                    
                    
                    int curr_on_set_size = on_set_size(curr_level_vector);
                    int curr_dimension = curr_level_vector.size();

                    // constraints analysis    
                    if (  (x.no_on_set_size==true && curr_on_set_size == curr_dimension-osize && curr_dimension == dimension) || 
                          (x.no_on_set_size==false && curr_on_set_size==osize && curr_dimension == dimension)// ||
//                          ( x.no_on_set_size==false && x.variable_on_set_size==true && curr_on_set_size==osize)
                        )
                        // last conditions remain like the previous, because the sensitivity to the dimension is given by the main effect over the parameter that 
                        // manage the on_set_size_length
                    {
                        satisfy_constraints=true;
                        at_least_one_satisfy_constraints=true;
                    }
                    
                    if(satisfy_constraints == true && distance < min_constrained_distance)
                    {
                        max_constrained_level_vector = curr_level_vector;
                        min_constrained_distance = distance;
                    }

                     #ifdef DEBUG 
	                cout << env->current_design_space->get_vector_representation(env, curr_level_vector);
                        cout << " "<<satisfy_constraints<<"  "<<distance<<" "<<min_distance<<" "<<min_constrained_distance<<"*"<<x.no_on_set_size<<" "<<curr_on_set_size <<" "<<curr_dimension<<" "<<endl;
                    #endif

                    #ifdef DEBUG_MAIN_EFFECT_ON_FILE 
	                debug_out << env->current_design_space->get_vector_representation(env, curr_level_vector);
                        debug_out << " "<<satisfy_constraints<<"  "<<distance<<" "<<min_distance<<" "<<min_constrained_distance<<"*"<<x.no_on_set_size<<" "<<curr_on_set_size <<" "<<curr_dimension<<" "<<endl;
                    #endif
                                        

                    if(distance < min_distance)
                    {
                        max_level_vector = curr_level_vector;
                        min_distance = distance;
                    }

                    exist_at_least_one = true;
		    satisfy_constraints = false;
                    c++;
                }   

                if (exist_at_least_one == true)
		{
		    if (at_least_one_satisfy_constraints==true)
		    {
			max_level = bit_vector_to_int(max_constrained_level_vector);
		    }
		    else
		    {
			max_level = bit_vector_to_int(max_level_vector);
		    }
	        }
		else
		{
		    max_level = min_level;
		}

                #ifdef DEBUG
                    cout << endl << "** Selected NEAREST to min scrambled (called max) : "<<max_level<<"  -->  ";
	            cout << env->current_design_space->get_vector_representation(env, int_to_bit_vector(max_level, dimension))<<endl;
                    cout << "exist_at_least_one: "<<exist_at_least_one<<" at least one sat constraints: "<<at_least_one_satisfy_constraints<<endl<<endl;
                #endif
 
                #ifdef DEBUG_MAIN_EFFECT_ON_FILE
                    debug_out << endl << "** Selected NEAREST to min scrambled (called max) : "<<max_level<<"  -->  ";
	            debug_out << env->current_design_space->get_vector_representation(env, int_to_bit_vector(max_level, dimension))<<endl;
                    debug_out << "exist_at_least_one: "<<exist_at_least_one<<" at least one sat constraints: "<<at_least_one_satisfy_constraints<<endl<<endl;
                #endif
            
            }  //  if "nearest search"
            
            
            // look for the min max occurence and compute the objective sum
            for(c=database_list->begin();c!=database_list->end();)
            {
                m3_point & p = sim_get_point_at_iterator(c);
                curr_symbol = env->current_design_space->get_parameter_representation(env, p, curr_par); 
                curr_level = bit_vector_to_int(env->current_design_space->get_mask(env, &p, curr_par));

		//debug_out << "p: "<<p.get_objective(env, obj_index)<<endl;

                if(curr_level == min_level)
                {	
                    objective_min_tot = objective_min_tot + p.get_objective(env, obj_index);
                    min_level_n++;		
                }
                if(curr_level == max_level)
                {
                    objective_max_tot = objective_max_tot + p.get_objective(env, obj_index);
                    max_level_n++;		
                }
                c++;
            }	

            level_filename = curr_par+"_"+objective_name;
            curr_level_filename[i] = level_filename;
            ofstream level_file;
            level_file.open(level_filename.c_str(), ios::out);
            m3_assert(!level_file.fail());
            
            // This is for the line management [OLD like scalar]
            // level_file << left_x << "  " << objective_min_tot/min_level_n << endl;
            // level_file << left_x+1 << "  " << objective_max_tot/max_level_n << endl;
            
	    double delta = fabs(objective_min_tot/min_level_n - objective_max_tot/max_level_n);

            
              #ifdef DEBUG 
	        cout << "### SUMMARY ###"<<endl;
                cout << "TYPE: mask - OBJ NAME: "<<objective_name<<endl<<endl;;
		cout << "   objective min tot: "<<objective_min_tot<<endl;
		cout << "   objective max tot: "<<objective_max_tot<<endl<<endl;
                cout << "   min_level_n: "<<min_level_n<<endl;
		cout << "   max_level_n: "<<max_level_n<<endl<<endl;
                cout << "   AVG-low_level: "<<objective_min_tot/min_level_n<<endl;
		cout << "   AVG-high_level: "<<objective_max_tot/max_level_n<<endl<<endl;
		cout << "   delta: "<<delta<<endl<<endl;  
             #endif

             #ifdef DEBUG_MAIN_EFFECT_ON_FILE
	        debug_out << "### SUMMARY ###"<<endl;
                debug_out << "TYPE: mask - OBJ NAME: "<<objective_name<<endl<<endl;;
		debug_out << "   objective min tot: "<<objective_min_tot<<endl;
		debug_out << "   objective max tot: "<<objective_max_tot<<endl<<endl;
                debug_out << "   min_level_n: "<<min_level_n<<endl;
		debug_out << "   max_level_n: "<<max_level_n<<endl<<endl;
                debug_out << "   AVG-low_level: "<<objective_min_tot/min_level_n<<endl;
		debug_out << "   AVG-high_level: "<<objective_max_tot/max_level_n<<endl<<endl;
		debug_out << "   delta: "<<delta<<endl<<endl;  
             #endif

	    
            // A vertical line in the center, no like scalar on - and +
            level_file << (2*left_x+1)/2 << "  " << delta << endl;
            
//          Adding the main effect to the central structure 
            add_main_effect(env, i, obj_index, objective_min_tot/min_level_n, objective_max_tot/max_level_n);            
            
            level_file.close();
            left_x = left_x+1.5;	

            // compute the max delta used later for setting the y-axis scale
            if(initialized_delta==0) 
            {
                delta_objective_value = delta;
                initialized_delta = 1;
            }
            else
            {
                if(delta > delta_objective_value )
		    delta_objective_value = delta;
            }
            level_file.close();
        }  // end MASK if
        
        //-----------------------------------------------------------------------
        //                PERMUTATION main effects graph generation
        //-----------------------------------------------------------------------
        if(env->current_design_space->ds_parameters[i].type == M3_DS_PERMUTATION)
        {
            curr_par = env->current_design_space->ds_parameters[i].name;
            
            // Randomly select the index of the min permutation
            int index_min = randint(0, (database_list->size()-1));
            
            // Assigning min_level
            int index_i = 0;
            vector <int> min_level_vector, scrambled_min_level_vector, max_level_vector, curr_level_vector, max_constrained_level_vector;
            int scrambled_min_level;
            int dimension;
            m3_permutation_low_high_parameter x;
            bool satisfy_constraints = false;
            bool at_least_one_satisfy_constraints=false;
                                
            
            for(c=database_list->begin();c!=database_list->end();)
            {
                // obtain the min_level and the expected scramble scrambled_min_level = scramble(min_level)
                if(index_i == index_min)
                {
                    m3_point &p = sim_get_point_at_iterator(c);
                    min_level_vector = env->current_design_space->get_permutation(env, &p, curr_par);
                    min_level = permutation_to_factoradic(min_level_vector);
                    m3_permutation &perm = env->current_design_space->permutation_parameters[curr_par];
                    dimension = env->current_design_space->get_permutation_dimension(env, curr_par, &p);

                    x.permutation_name = curr_par;
                    x.variable_dimension = perm.variable_dimension;
                    x.dimension = dimension; 
                    x.index=-1;
            
                    scrambled_min_level_vector = env->current_design_space->permutation_scrambler(min_level_vector, x);
                    scrambled_min_level = permutation_to_factoradic(scrambled_min_level_vector);
                    break;
                }
                
                index_i++;
                c++;
            }
            #ifdef DEBUG
	        cout << "Selected MIN permutation  --------> : ";
	        cout << env->current_design_space->get_vector_representation(env, min_level_vector);
	        cout << " dimension: "<<dimension<<endl; 
	        cout << "corresponding scrambled ----------> : ";
	        cout << env->current_design_space->get_vector_representation(env, scrambled_min_level_vector);
	        cout << endl;
   	    #endif

            #ifdef DEBUG_MAIN_EFFECT_ON_FILE
	        debug_out << "Selected MIN permutation  --------> : ";
	        debug_out << env->current_design_space->get_vector_representation(env, min_level_vector);
	        debug_out << " dimension: "<<dimension<<endl; 
	        debug_out << "corresponding scrambled ----------> : ";
	        debug_out << env->current_design_space->get_vector_representation(env, scrambled_min_level_vector);
	        debug_out << endl;
   	     #endif

            // Assigning max_level 
            max_level = -1;
            for(c=database_list->begin();c!=database_list->end();)
            {
                // read an element
                m3_point & p = sim_get_point_at_iterator(c);                
                curr_level_vector = env->current_design_space->get_permutation(env, &p, curr_par);
                curr_level = permutation_to_factoradic(curr_level_vector);
                if(curr_level == scrambled_min_level)
                {
                    // if is equal to the scramble of min_level select that as max
                    max_level = curr_level;
                    break;
                }
                index_i++;
                c++;
            }
            
            // if max is not foud search for the nearest...among the available point in the DB
            if(max_level==-1)
            {   
                #ifdef DEBUG
                    cout<<"** not find exact scramble search for the nearest.."<<endl;
                    cout << "curr_permu     satisfy_constraints    distance    min_distance    min_constrained_distance  * curr_dimension"<<endl<<endl;
                #endif
                
                #ifdef DEBUG_MAIN_EFFECT_ON_FILE
                    debug_out<<"** not find exact scramble search for the nearest.."<<endl;
                    debug_out << "curr_permu     satisfy_constraints    distance    min_distance    min_constrained_distance  * curr_dimension"<<endl<<endl;
                #endif

                double min_distance, distance;
                double min_constrained_distance;
		bool exist_at_least_one = false;
 
                // note that distance is normalized, so is always between 0 and 1
		min_distance = 2.00;
                min_constrained_distance = 2.00;
                //max_level_vector = curr_level_vector;
 
                for(c=database_list->begin();c!=database_list->end();)
                {
                    // read an element
                    m3_point & p = sim_get_point_at_iterator(c);                
                    curr_level_vector = env->current_design_space->get_permutation(env, &p, curr_par);
                    if(curr_level_vector == min_level_vector)
		    {  

                        #ifdef DEBUG 
	                    cout << env->current_design_space->get_vector_representation(env, curr_level_vector)<<" == min_level_vector - SKIP - "<<endl;
                        #endif

                        #ifdef DEBUG_MAIN_EFFECT_ON_FILE
	                    debug_out << env->current_design_space->get_vector_representation(env, curr_level_vector)<<" == min_level_vector - SKIP - "<<endl;
                        #endif

                        c++; 
                        continue;
		    }

                    distance = normalized_average_permutation_distance(scrambled_min_level_vector, curr_level_vector);                    
                    
                    int curr_dimension = curr_level_vector.size();
                   
                    // constraints analysis    
                    if (curr_dimension == dimension) 
                        // last conditions remain like the previous, because the sensitivity to the dimension is given by the main effect over the parameter that 
                        // manage the on_set_size_length
                    {
                        satisfy_constraints=true;
                        at_least_one_satisfy_constraints=true;
                    }
                    
                    if(satisfy_constraints == true && distance < min_constrained_distance)
                    {
                        max_constrained_level_vector = curr_level_vector;
                        min_constrained_distance = distance;
                    }

                    #ifdef DEBUG 
	                cout << env->current_design_space->get_vector_representation(env, curr_level_vector);
                        cout << " "<<satisfy_constraints<<"  "<<distance<<" "<<min_distance<<" "<<min_constrained_distance<<"*"<<" "<<curr_dimension<<" "<<endl;
                    #endif

                    #ifdef DEBUG_MAIN_EFFECT_ON_FILE 
	                debug_out << env->current_design_space->get_vector_representation(env, curr_level_vector);
                        debug_out << " "<<satisfy_constraints<<"  "<<distance<<" "<<min_distance<<" "<<min_constrained_distance<<"*"<<" "<<curr_dimension<<" "<<endl;
                    #endif

                    if(distance < min_distance)
                    {
                        max_level_vector = curr_level_vector;
                        min_distance = distance;
                    }
                    exist_at_least_one = true;
		    satisfy_constraints = false;
                    c++;
                }                                    
                if (exist_at_least_one == true)
		{
		    if (at_least_one_satisfy_constraints==true)
		    {
			max_level = permutation_to_factoradic(max_constrained_level_vector);
		    }
		    else
		    {
			max_level = permutation_to_factoradic(max_level_vector);
		    }
	        }
		else
		{
		    max_level = min_level;
		}

                #ifdef DEBUG
                    cout << endl << "** Selected NEAREST to min scrambled (called max) : "<<max_level<<"  -->  ";
	            cout << env->current_design_space->get_vector_representation(env, factoradic_to_permutation(max_level, dimension))<<endl;
                    cout << "exist_at_least_one: "<<exist_at_least_one<<" at least one sat constraints: "<<at_least_one_satisfy_constraints<<endl<<endl;
                #endif

                #ifdef DEBUG_MAIN_EFFECT_ON_FILE
                    debug_out << endl << "** Selected NEAREST to min scrambled (called max) : "<<max_level<<"  -->  ";
	            debug_out << env->current_design_space->get_vector_representation(env, factoradic_to_permutation(max_level, dimension))<<endl;
                    debug_out << "exist_at_least_one: "<<exist_at_least_one<<" at least one sat constraints: "<<at_least_one_satisfy_constraints<<endl<<endl;
                #endif
 
 
            }  //  if "nearest search"
            
            // look for the min max occurence and compute the objective sum
            for(c=database_list->begin();c!=database_list->end();)
            {
                m3_point & p = sim_get_point_at_iterator(c);
                curr_symbol = env->current_design_space->get_parameter_representation(env, p, curr_par); 
                curr_level = permutation_to_factoradic(env->current_design_space->get_permutation(env, &p, curr_par));
                if(curr_level == min_level)
                {	
                    objective_min_tot = objective_min_tot + p.get_objective(env, obj_index);
                    min_level_n++;		
                }
                if(curr_level == max_level)
                {
                    objective_max_tot = objective_max_tot + p.get_objective(env, obj_index);
                    max_level_n++;		
                }
                c++;
            }	

            level_filename = curr_par+"_"+objective_name;
            curr_level_filename[i] = level_filename;
            
            ofstream level_file;
            level_file.open(level_filename.c_str(), ios::out);
            m3_assert(!level_file.fail());

            // This is for the line management [OLD like scalar]
            // level_file << left_x << "  " << objective_min_tot/min_level_n << endl;
            // level_file << left_x+1 << "  " << objective_max_tot/max_level_n << endl;
            
            double delta = fabs(objective_min_tot/min_level_n - objective_max_tot/max_level_n);
            #ifdef DEBUG 
	        cout << "### SUMMARY ###"<<endl;
                cout << "TYPE: permutation - OBJ NAME: "<<objective_name<<endl<<endl;;
		cout << "   objective min tot: "<<objective_min_tot<<endl;
		cout << "   objective max tot: "<<objective_max_tot<<endl<<endl;
                cout << "   min_level_n: "<<min_level_n<<endl;
		cout << "   max_level_n: "<<max_level_n<<endl<<endl;
                cout << "   AVG-low_level: "<<objective_min_tot/min_level_n<<endl;
		cout << "   AVG-high_level: "<<objective_max_tot/max_level_n<<endl<<endl;
		cout << "   delta: "<<delta<<endl<<endl;  
            #endif

            #ifdef DEBUG_MAIN_EFFECT_ON_FILE 
	        debug_out << "### SUMMARY ###"<<endl;
                debug_out << "TYPE: permutation - OBJ NAME: "<<objective_name<<endl<<endl;;
		debug_out << "   objective min tot: "<<objective_min_tot<<endl;
		debug_out << "   objective max tot: "<<objective_max_tot<<endl<<endl;
                debug_out << "   min_level_n: "<<min_level_n<<endl;
		debug_out << "   max_level_n: "<<max_level_n<<endl<<endl;
                debug_out << "   AVG-low_level: "<<objective_min_tot/min_level_n<<endl;
		debug_out << "   AVG-high_level: "<<objective_max_tot/max_level_n<<endl<<endl;
		debug_out << "   delta: "<<delta<<endl<<endl;  
            #endif



            level_file << (2*left_x+1)/2 << "  " << delta << endl;

            // Adding the main effect to the central structure 
            add_main_effect(env, i, obj_index, objective_min_tot/min_level_n, objective_max_tot/max_level_n);            
            
            level_file.close();
            left_x = left_x+1.5;
	
            // compute the max delta used later for setting the y-axis scale
            if(initialized_delta==0) 
            {
                delta_objective_value = delta;
                initialized_delta = 1;
            }
            else
            {
                if(delta > delta_objective_value) 
		    delta_objective_value = delta;
            }
          level_file.close();
        }  // end PERMUTATION if
    } // end for over all parameters in the design space
    #ifdef DEBUG
        cout <<" BEFORE elaboration for OBJ-NAME: "<<objective_name<<endl;
        cout <<" min objective value "<<min_objective_value<<endl;
        cout <<" max objective value "<<max_objective_value<<endl;
	cout <<" delta objective value "<<delta_objective_value<<endl<<endl;
    #endif

    #ifdef DEBUG_MAIN_EFFECT_ON_FILE
        debug_out <<" BEFORE elaboration for OBJ-NAME: "<<objective_name<<endl;
        debug_out <<" min objective value "<<min_objective_value<<endl;
        debug_out <<" max objective value "<<max_objective_value<<endl;
	debug_out <<" delta objective value "<<delta_objective_value<<endl<<endl;
    #endif

		       
    if( (max_objective_value - min_objective_value) < delta_objective_value)
    {
        double delta_center = (max_objective_value - min_objective_value)/2.00 + min_objective_value;

        min_objective_value = delta_center - (delta_objective_value/2.00);
        max_objective_value = delta_center + (delta_objective_value/2.00);
    }
    
    // If no scalar are available, but only mask or permutation
    if(initialized == 0)
    {   
        double delta_center = delta_objective_value/2.00;
                
        min_objective_value = delta_center - (delta_objective_value/2.00);
        max_objective_value = delta_center + (delta_objective_value/2.00);
    }
    #ifdef DEBUG
        cout <<" AFTER elaboration for OBJ-NAME: "<<objective_name<<endl;
        cout <<" min objective value "<<min_objective_value<<endl;
        cout <<" max objective value "<<max_objective_value<<endl;
	cout <<" delta objective value "<<delta_objective_value<<endl<<endl;
    #endif

    #ifdef DEBUG_MAIN_EFFECT_ON_FILE
        debug_out <<" AFTER elaboration for OBJ-NAME: "<<objective_name<<endl;
        debug_out <<" min objective value "<<min_objective_value<<endl;
        debug_out <<" max objective value "<<max_objective_value<<endl;
	debug_out <<" delta objective value "<<delta_objective_value<<endl<<endl;
    #endif
    //Setting xrange and yrange
    double margin;
    if(max_objective_value == min_objective_value)
    {
        // to prevent gnuplot error "can't plot with an empty y range !"
	if(max_objective_value==0)
             margin = 0.00001;
	else
             margin = max_objective_value *0.1;
    }
    else
    {   	
        margin = (max_objective_value - min_objective_value)/20;
    }

    gnuplot_script << "set yrange ["<< min_objective_value-margin <<":"<< max_objective_value+margin <<"]\n";	
    gnuplot_script << "set xrange [0:"<< left_x <<"]\n";

    //Setting xtics
    left_x = 0.25;
    gnuplot_script << "set xtic (\"\" 0";
    for(int tic=0; tic<par_num; tic++ )
    {
        if(env->current_design_space->ds_parameters[tic].type == M3_DS_SCALAR)
            gnuplot_script << ", \"|\" " << left_x; 
        else
            gnuplot_script << ", \" \" " << left_x; 
        
        gnuplot_script << ", \"" << env->current_design_space->ds_parameters[tic].name << "\" " << left_x+0.5;
        
        if(env->current_design_space->ds_parameters[tic].type == M3_DS_SCALAR)
            gnuplot_script << ", \"+\" "<< left_x+1;
        else
            gnuplot_script << ", \" \" "<< left_x+1;
        
        left_x = left_x + 1.5;	
    }
    gnuplot_script << ") rotate\n";

    gnuplot_script << "plot ";

    double bs_ycenter = min_objective_value + (max_objective_value-min_objective_value)/2;
     
    if(env->current_design_space->ds_parameters[0].type == M3_DS_SCALAR)
        gnuplot_script << "\"" << curr_level_filename[0] << "\" with lines lt -1 lw 2";
    else
    {

        double bs_x;
        double bs_y;
        
        // read from file
        ifstream bs_tmp_file_in;
        bs_tmp_file_in.open (curr_level_filename[0].c_str(), ios::in);
        
        bs_tmp_file_in >> bs_x;
        bs_tmp_file_in >> bs_y;                    
        
        bs_tmp_file_in.close();
        
        // write to file
        ofstream bs_tmp_file_out;
        bs_tmp_file_out.open (curr_level_filename[0].c_str(), ios::out);

        bs_tmp_file_out << bs_x << "  " << bs_ycenter-(bs_y/2) << endl;
        bs_tmp_file_out << bs_x << "  " << bs_ycenter+(bs_y/2) << endl;
        
        bs_tmp_file_out.close();
        
        gnuplot_script << "\"" << curr_level_filename[0] << "\" with lines lt -1 lw 2";
    }
        
    for(int z=1; z<par_num; z++)
    {
        if(env->current_design_space->ds_parameters[z].type == M3_DS_SCALAR)
            gnuplot_script << ", \"" << curr_level_filename[z] << "\" with lines lt -1 lw 2";
        else
        {
            double bs_x;
            double bs_y;
            string bs_filename = curr_level_filename[z].c_str();
            
            // read from file
            ifstream bs_tmp_file_in;
            bs_tmp_file_in.open (curr_level_filename[z].c_str(), ios::in);
        
            bs_tmp_file_in >> bs_x;
            bs_tmp_file_in >> bs_y;                    
        
            bs_tmp_file_in.close();
        
            // write to file
            ofstream bs_tmp_file_out;
            bs_tmp_file_out.open (curr_level_filename[z].c_str(), ios::out);
 
            bs_tmp_file_out << bs_x << "  " << bs_ycenter-(bs_y/2) << endl;
            bs_tmp_file_out << bs_x << "  " << bs_ycenter+(bs_y/2) << endl;
        
            bs_tmp_file_out.close();
        
            gnuplot_script << ", \"" << curr_level_filename[z] << "\" with lines lt -1 lw 2";
        }
    }

    gnuplot_script << "\nunset multiplot\n";
    gnuplot_script.close();

    ostringstream run_gnuplot;
    run_gnuplot << "gnuplot \""<< gnuplot_script_name <<"\"\n";
#ifdef REMOVE_FILES
    for(int l=0; l<par_num; l++)
    {
        run_gnuplot << "rm " << curr_level_filename[l] << endl;
    }
    run_gnuplot << "rm \""<< gnuplot_script_name <<"\"\n"<< endl;
#endif

    if(!shell_command(run_gnuplot.str()))
    {
        prs_display_error("error running gnuplot");
        return;
    }
}

pthread_mutex_t sh_lock;

typedef struct me_parameter
{
   vector<pair<double, double> > effect_per_objective; 
} me_parameter;

map<int, me_parameter> main_effect_per_parameter;


void add_main_effect(m3_env *env, int parameter, int objective, double min, double max)
{
    pthread_mutex_lock(&sh_lock);
    if(!main_effect_per_parameter.count(parameter))
    {
        me_parameter me;
        me.effect_per_objective.resize(env->current_design_space->objectives.size());
        main_effect_per_parameter.insert(make_pair(parameter, me));
        main_effect_per_parameter[parameter].effect_per_objective[objective]=make_pair(min, max);
    }
    else
    {
        main_effect_per_parameter[parameter].effect_per_objective[objective]=make_pair(min, max);
    }
    pthread_mutex_unlock(&sh_lock);
}

#define LOWER_SAT_DELTA 0.001
//#define UPPER_SAT_DELTA (1/(LOWER_SAT_DELTA)


string perc(double x)
{
    ostringstream s;
    s.setf(ios::fixed,ios::floatfield);   // floatfield set to fixed
    s.precision(2);
    s << (x*100);
    return s.str()+ " %";
}

double saturated_delta(double a, double b)
{
    if(fabs(a)<LOWER_SAT_DELTA)
        return (b>0) ? 1.0 : -1.0;
    else
    {
        double sat = (b-a)/a;
        if(fabs(sat)>=1.0)
            return (sat>0) ? 1.0 : -1.0;
        else
            return sat;
    }

}

#define FAIR_EFFECT         0.05
#define SIGNIFICANT_EFFECT  0.2 

bool consider_max_effect=true;

string get_effect_string(m3_env *env, int par, int obj)
{
     double omin = main_effect_per_parameter[par].effect_per_objective[obj].first;
     double omax = main_effect_per_parameter[par].effect_per_objective[obj].second;
     double effect = saturated_delta(omin, omax);
     if(fabs(effect)>FAIR_EFFECT)
     {
             if(effect>0)
                return red(bold(perc(effect)));
             else
                return green(bold(perc(effect)));     
     }
     else
         return perc(effect);
}

double get_effect(m3_env *env, int par)
{
    if(!main_effect_per_parameter.count(par))
        return 0.0;

    double effect = 0;
    for(int j=0; j<env->current_design_space->objectives.size(); j++)
    { 
            double omin = main_effect_per_parameter[par].effect_per_objective[j].first;
            double omax = main_effect_per_parameter[par].effect_per_objective[j].second;
            if(consider_max_effect)
               effect = max(effect, fabs(saturated_delta(omin, omax)));
            else
               effect = effect + fabs(saturated_delta(omin, omax))/(env->current_design_space->objectives.size());
    }
    return effect;
}

int get_significance(m3_env *env, int par)
{
    if(fabs(get_effect(env, par))<FAIR_EFFECT)
        return 0;
    else
    {
        if(fabs(get_effect(env, par)) > SIGNIFICANT_EFFECT)
            return 2;
        else
            return 1;
    }
}

typedef struct line
{
    double x_mm, x_mp;
    double x_pm, x_pp;
} line;

typedef struct int_parameters
{
   vector< line > slopes_per_objective; 
} int_parameters;

map< pair< int, int >, int_parameters > slopes_per_parameter_pair;

void add_interaction_effect(m3_env *env, int par, int par_i, int objective, double x_mm, double x_mp, double x_pm, double x_pp) 
{
    pthread_mutex_lock(&sh_lock);
    if(!slopes_per_parameter_pair.count(make_pair(par, par_i)))
    {
        int_parameters inp;
        inp.slopes_per_objective.resize(env->current_design_space->objectives.size());
        slopes_per_parameter_pair.insert(make_pair(make_pair(par, par_i), inp));
    }
    line l;
    l.x_mm = x_mm;
    l.x_mp = x_mp;
    l.x_pm = x_pm;
    l.x_pp = x_pp;
    slopes_per_parameter_pair[make_pair(par, par_i)].slopes_per_objective[objective]=l;
    pthread_mutex_unlock(&sh_lock);
}

bool sg(double x)
{
    if(x>=0)
        return true;
    else
        return false;
}

double interaction(double pos_height, double neg_height, bool & anti)
{
    if((sg(pos_height) && !sg(neg_height)) ||
       !sg(pos_height) && sg(neg_height))
    {
       anti=true; 
    }
    else
       anti=false;

    return fabs(pos_height-neg_height);
}

#define avg(o) ((y_max[o]+y_min[o])/2)
#define norm(x,o) (2*(((x)-avg(o))/(y_max[o]-y_min[o])))

#define angle(xm, xp, o) (atan(norm(xp,o) - norm(xm,o))/M_PI*90.0)

typedef struct sign_int
{
    int p1;
    int p2;
    double strength;
    int objective;
    bool anti;
} sign_int;

bool compare_s(sign_int x, sign_int y)
{
    return (x.strength>y.strength);
}

vector<sign_int> sint; 
vector<sign_int> sint_filtered; 

sign_int similar_int;

bool similar(sign_int the_int)
{
    if(( (the_int.p1 == similar_int.p1) && (the_int.p2 == similar_int.p2) ) ||
       ( (the_int.p1 == similar_int.p2) && (the_int.p2 == similar_int.p1) ) &&
       ( the_int.objective == similar_int.objective ))
        return true;
    else
        return false;
}

#include <algorithm>

string print_interaction_effects(m3_env *env, vector<double> & y_min, vector<double> & y_max, int &size_px, string &table_latex)
{
    map< pair< int, int >, int_parameters >::iterator i;
    for(i=slopes_per_parameter_pair.begin(); i!=slopes_per_parameter_pair.end();i++)
    {
        int p1 = i->first.first;
        int p2 = i->first.second;
        for(int o=0; o<env->current_design_space->objectives.size();o++)
        {
            double angle_1 = angle(i->second.slopes_per_objective[o].x_mm, i->second.slopes_per_objective[o].x_mp, o);
            double angle_2 = angle(i->second.slopes_per_objective[o].x_pm, i->second.slopes_per_objective[o].x_pp, o);
            bool anti;
            sign_int ss;
            ss.p1 = p1;
            ss.p2 = p2;
            ss.strength = interaction(angle_1, angle_2, ss.anti);
            ss.objective = o;
            sint.push_back(ss);
            /*
            cout <<  "   " << env->current_design_space->objectives_names[o];
            cout <<  "   " << perc(interaction(angle_1, angle_2, anti)) << " % ---- " ;
            cout << ((anti) ? "anti":"syn") << endl;
            cout << " [ " << angle_1<< " " << angle_2<< " ] " << endl;
            */
        }
    }
    sort(sint.begin(), sint.end(), compare_s);
    vector<sign_int>::iterator ord;
    for(ord=sint.begin(); ord!=sint.end();ord++)
    {
        similar_int = *ord;
        if(find_if(sint_filtered.begin(), sint_filtered.end(), similar)==sint_filtered.end() &&
                ord->strength>1.0)
        {
            sint_filtered.push_back(*ord);
        }
    }

    vector<string> row;
    string table;

    row.push_back(bold("Parameter Interaction"));
    row.push_back(bold("Objective"));
    row.push_back(bold("Strength"));

    size_px = row_size(row);
    table = table_head(row);
    table_latex = table_head_latex(row);

#define MAX_BULLETS         5
#define BULLETS_CHAR_SIZE   5 
    for(int k=0; k<sint_filtered.size();k++)
    {
        row.clear();
        row.push_back(bold(env->current_design_space->ds_parameters[sint_filtered[k].p1].name)+" - "+
                bold(env->current_design_space->ds_parameters[sint_filtered[k].p2].name));
        row.push_back(env->current_design_space->objectives_names[sint_filtered[k].objective]);
        row.push_back("");
        int nb = (sint_filtered[k].strength / 5)+1;
        nb = (nb>=MAX_BULLETS) ? MAX_BULLETS : nb;
        size_px = max(size_px, row_size(row)+MAX_BULLETS*BULLETS_CHAR_SIZE);
        int z=0;
        for(; z<nb; z++)
        {
            row[2] = row[2]+ ((sint_filtered[k].anti) ? BULLET_RED : BULLET_GREEN);
        }
        if(nb<MAX_BULLETS)
        {
            for(; z<MAX_BULLETS; z++)
            {
                row[2] = row[2]+ BULLET_GRAY;
            }
        }
        table = table + table_row(row);
        table_latex = table_latex + table_row_latex(row);
    }
    size_px = size_px*CHAR_SIZE_PX; 
    return table;
}


void interaction_effects_write_to_files(m3_env* env, string par, int par_index, int obj_index, double& y_min, double& y_max, vector<string>& data_files)
{

    int min_level, max_level, curr_level;
    int min_level_i_pmin, max_level_i_pmin, min_level_i_pmax, max_level_i_pmax, curr_level_i;
    double min_objective_value, max_objective_value;
    double pp_tot = 0;
    double pm_tot = 0;
    double mp_tot = 0;
    double mm_tot = 0;
    int pp_n = 0;
    int pm_n = 0;
    int mp_n = 0;
    int mm_n = 0;
    string curr_symbol, curr_symbol_i;
    string curr_par_i;

    if(env->current_design_space == NULL)
    {
        prs_display_error("Design Space not defined, format unknown");
        return;
    }

    string level_filename_m;
    string level_filename_p;
    int temp_files_num = 0;
    for(int count=0; count<env->current_design_space->ds_parameters.size(); count++)
    {
        if(env->current_design_space->ds_parameters[count].type == M3_DS_SCALAR && 
                env->current_design_space->ds_parameters[count].name != par) 
        {
            temp_files_num++;
        }
    }
    temp_files_num = (temp_files_num)*2;
    string curr_level_filename[temp_files_num];


    m3_list *database_list = env->available_dbs[env->current_db_name]->get_list_of_points();
    list<m3_object *>::iterator c;

    //Initializing min_level and max_level with random values (from the first point)
    m3_point & init_point =sim_get_point_at_iterator(database_list->begin());
    curr_symbol = env->current_design_space->get_parameter_representation(env, init_point, par); 
    min_level = env->current_design_space->get_scalar_level(env, par, curr_symbol); 
    max_level = env->current_design_space->get_scalar_level(env, par, curr_symbol);			

    //Assigning min_level and max_level actual min and max values present in the db
    for(c=database_list->begin();c!=database_list->end();)
    {
        m3_point & p = sim_get_point_at_iterator(c);
        curr_symbol = env->current_design_space->get_parameter_representation(env, p, par); 
        curr_level = env->current_design_space->get_scalar_level(env, par, curr_symbol);
        if(curr_level < min_level) min_level = curr_level;
        if(curr_level > max_level) max_level = curr_level;
        c++;
    }	

    /* At this point, min_level and max_level correspond 
     * to the minimum and maximum values found in the DB for "par".
     **/

    int count = 0;
    double left_x = 0.25;
    double objective_values[4];
    int initialized = 0;
    for(int i=0; i<env->current_design_space->ds_parameters.size(); i++)
    {
        mm_tot = 0;
        mp_tot = 0;
        pm_tot = 0;
        pp_tot = 0;
        mm_n = 0;	
        mp_n = 0;
        pm_n = 0;
        pp_n = 0;	

        if(env->current_design_space->ds_parameters[i].type == M3_DS_SCALAR && 
           env->current_design_space->ds_parameters[i].name != par)
        {
            curr_par_i = env->current_design_space->ds_parameters[i].name;

            //Initializing min_level_i and max_level_i with random values (from the first point having min_level or max_level
            //value for main parameter). This grants the existence of each level combination (--, -+, +-, ++) in the db

            /* 
             * This cycle assigns to the minimum and maximum value of curr_par_i the 
             * last level found by scanning the database. It is actually the same for both the + and - associated 
             * with curr_par_i.
             *
             */

            for(c=database_list->begin();c!=database_list->end();)
            {
                m3_point & p =sim_get_point_at_iterator(c);
                curr_symbol = env->current_design_space->get_parameter_representation(env, p, par); 
                curr_level = env->current_design_space->get_scalar_level(env, par, curr_symbol);
                curr_symbol_i = env->current_design_space->get_parameter_representation(env, p, curr_par_i); 
                curr_level_i = env->current_design_space->get_scalar_level(env, curr_par_i, curr_symbol_i); 
                if(curr_level == min_level)
                {
                    min_level_i_pmin = curr_level_i;
                    max_level_i_pmin = curr_level_i;

                }
                if(curr_level == max_level)
                {
                    min_level_i_pmax = curr_level_i;
                    max_level_i_pmax = curr_level_i;
                }
                c++;
            }				

            //Assigning min_level_i and max_level_i actual min and max values present in the db for interaction par.
            //Only when point has min_level or max_level for main parameter
            
            /* 
             * This cycle actually refines the the minimum and maximum value of curr_par_i the 
             * last level found by scanning the database. 
             *
             */

            for(c=database_list->begin();c!=database_list->end();)
            {
                m3_point & p =sim_get_point_at_iterator(c);
                curr_symbol = env->current_design_space->get_parameter_representation(env, p, par); 
                curr_level = env->current_design_space->get_scalar_level(env, par, curr_symbol);
                curr_symbol_i = env->current_design_space->get_parameter_representation(env, p, curr_par_i); 
                curr_level_i = env->current_design_space->get_scalar_level(env, curr_par_i, curr_symbol_i);
                if(curr_level == min_level)
                {
                    if(curr_level_i < min_level_i_pmin) min_level_i_pmin = curr_level_i;
                    if(curr_level_i > max_level_i_pmin) max_level_i_pmin = curr_level_i;
                }
                if(curr_level == max_level)
                {
                    if(curr_level_i < min_level_i_pmax) min_level_i_pmax = curr_level_i;
                    if(curr_level_i > max_level_i_pmax) max_level_i_pmax = curr_level_i;
                }
                c++;
            }

            //Calculating plottable values
            for(c=database_list->begin();c!=database_list->end();)
            {
                m3_point & p =sim_get_point_at_iterator(c);
                curr_symbol = env->current_design_space->get_parameter_representation(env, p, par); 
                curr_symbol_i = env->current_design_space->get_parameter_representation(env, p, curr_par_i); 
                curr_level = env->current_design_space->get_scalar_level(env, par, curr_symbol);
                curr_level_i = env->current_design_space->get_scalar_level(env, curr_par_i, curr_symbol_i);
                if(curr_level == min_level && curr_level_i == min_level_i_pmin)
                {
                    mm_tot = mm_tot + p.get_objective(env, obj_index);
                    mm_n++;		
                }
                if(curr_level == min_level && curr_level_i == max_level_i_pmin)
                {
                    mp_tot = mp_tot + p.get_objective(env, obj_index);
                    mp_n++;		
                }
                if(curr_level == max_level && curr_level_i == min_level_i_pmax)
                {
                    pm_tot = pm_tot + p.get_objective(env, obj_index);
                    pm_n++;		
                }	
                if(curr_level == max_level && curr_level_i == max_level_i_pmax)
                {
                    pp_tot = pp_tot + p.get_objective(env, obj_index);
                    pp_n++;		
                }
                c++;
            }	
            
            level_filename_m = par + "-_" + curr_par_i + "_" + itos(obj_index);
            curr_level_filename[2*count] = level_filename_m;
            ofstream level_file_m;
            level_file_m.open(level_filename_m.c_str(), ios::out);
            m3_assert(!level_file_m.fail());
            level_file_m << left_x << "  " << mm_tot/mm_n << endl;
            level_file_m << left_x+1 << "  " << mp_tot/mp_n << endl;
            level_file_m.close();

            data_files.push_back(level_filename_m);

            level_filename_p = par + "+_" + curr_par_i + "_" + itos(obj_index);

            curr_level_filename[2*count+1] = level_filename_p;
            ofstream level_file_p;
            level_file_p.open(level_filename_p.c_str(), ios::out);
            m3_assert(!level_file_p.fail());
            level_file_p << left_x << "  " << pm_tot/pm_n << endl;
            level_file_p << left_x+1 << "  " << pp_tot/pp_n << endl;
            level_file_p.close();

            data_files.push_back(level_filename_p);

            left_x = left_x + 1.5;
            count++;


            objective_values[0] = mm_tot/mm_n;
            objective_values[1] = mp_tot/mp_n;
            objective_values[2] = pm_tot/pm_n;
            objective_values[3] = pp_tot/pp_n;

            double xmp = (mp_tot/mp_n);
            double xmm = (mm_tot/mm_n);

            double xpp = (pp_tot/pp_n);
            double xpm = (pm_tot/pm_n);

            add_interaction_effect(env, par_index, i, obj_index, xmm, xmp, xpm, xpp);
            //Initializing and assigning max_objective_value and min_objective_value
            if(initialized==0)
            {
                min_objective_value = objective_values[0];
                max_objective_value = objective_values[0];
                initialized = 1;
            }
            //-- could have a objective value greater than ++ (for all combinations --,-+,+-,++)
            for(int m=0; m<4; m++)
            {
                if (objective_values[m]<min_objective_value) min_objective_value = objective_values[m];
                if (objective_values[m]>max_objective_value) max_objective_value = objective_values[m];
            }			
        }
    }

    double margin;
    if(max_objective_value == min_objective_value)
        margin = max_objective_value *0.1;
    else 
        margin = (max_objective_value - min_objective_value)/20;

    y_min = min_objective_value-margin;
    y_max = max_objective_value+margin;	
}



void interaction_effects_plot_to_file(m3_env* env, string fig_name, string par, int obj_index, double y_min, double y_max, vector<string> data_files)
{

    //Need a unique name for temp file due to multithreading
    string gnuplot_script_name;
    gnuplot_script_name = "interaction_"+par+"_"+itos(obj_index)+".tmp.scr";
    ofstream gnuplot_script(gnuplot_script_name.c_str() ,ios::out);
    gnuplot_script << "unset grid\n";
    gnuplot_script << "set key right\n";
    string u = current_environment.current_design_space->objectives_units[obj_index];
    gnuplot_script << "set ylabel \"" << current_environment.current_design_space->objectives_names[obj_index] << " [" << u << "]\"\n";


    if((data_files.size()/2) < 5)
    {
        gnuplot_script << "set terminal png size 640,540" << endl;
        gnuplot_script << "set output \""<< fig_name << "\"" << endl;
        gnuplot_script << "set multiplot\n";
        gnuplot_script << "set size 1,0.9\n";
        gnuplot_script << "set origin 0, 0.1\n";
    }
    else
    {
        gnuplot_script << "set terminal png size 900,540" << endl;
        gnuplot_script << "set output \""<< fig_name << "\"" << endl;
        gnuplot_script << "set multiplot\n";
        gnuplot_script << "set size 1,0.9\n";
        gnuplot_script << "set origin 0, 0.1\n";
    }

    string curr_par_i;
    double left_x = 0.25;
    gnuplot_script << "set xtic (\"\" 0";
    for(int tic=0; tic<env->current_design_space->ds_parameters.size(); tic++)
    {
        if(env->current_design_space->ds_parameters[tic].type == M3_DS_SCALAR && env->current_design_space->ds_parameters[tic].name != par)
        {
            curr_par_i = env->current_design_space->ds_parameters[tic].name;
            gnuplot_script << ", \"|\" " << left_x; 
            gnuplot_script << ", \"" << curr_par_i << "\" " << left_x+0.5;
            gnuplot_script << ", \"+\" "<< left_x+1;
            left_x = left_x + 1.5;	
        }
    }

    gnuplot_script << ") rotate\n";

    gnuplot_script << "set yrange ["<< y_min <<":"<< y_max <<"]\n";	
    gnuplot_script << "set xrange [0:"<< left_x <<"]\n";


    gnuplot_script << "plot ";
    gnuplot_script << "\"" << data_files[1] << "\" title \""<< "+" << "\" with lines lt 1 lw 2";
    gnuplot_script << ", \"" << data_files[0] << "\" title \""<< "-" << "\" with lines lt -1 lw 2";
    for (int z=2; z<data_files.size(); z=z+2)
    {
        gnuplot_script << " ,\"" << data_files[z+1] << "\" notitle with lines lt 1 lw 2";
        gnuplot_script << " ,\"" << data_files[z] << "\" notitle with lines lt -1 lw 2";

    }
    gnuplot_script << "\nunset multiplot\n";
    gnuplot_script.close();

    ostringstream run_gnuplot;
    run_gnuplot << "gnuplot \""<< gnuplot_script_name <<"\"\n";
#ifdef REMOVE_FILES
    for (int l=0; l<data_files.size(); l++)
    {
        run_gnuplot << "rm " << data_files[l] << endl;
    }
#endif
    data_files.erase(data_files.begin(), data_files.end());
#ifdef REMOVE_FILES
    run_gnuplot << "rm \""<< gnuplot_script_name <<"\"\n"<< endl;
#endif

    if(!shell_command(run_gnuplot.str()))
    {
        prs_display_error("error running gnuplot");
        return;
    }
}




struct pareto_thread_data
{
    string db;
    string fig_name;
    int obj1;
    int obj2;
};

void *print_pareto_thread(void* pareto_threadarg)
{
    struct pareto_thread_data* mydata;
    mydata = (struct pareto_thread_data *) pareto_threadarg;
    pareto_plot_to_file_obj(mydata->db, mydata->fig_name, mydata->obj1, mydata->obj2);
    pthread_exit(NULL);
}



struct la_thread_data
{
    m3_env* env;
    string fig_name;
    string par;
    int obj1;
    int obj2;
};

void *print_level_analysis_thread(void* la_threadarg)
{
    struct la_thread_data* mydata;
    mydata = (struct la_thread_data *) la_threadarg;
    level_analysis_plot_to_file_obj(mydata->env, mydata->fig_name, mydata->par, mydata->obj1, mydata->obj2);
    pthread_exit(NULL);
}



struct pm_thread_data
{
    m3_env* env;
    string fig_name;
    string par;
    int obj_index;
};

void *print_parameter_objective_boxplot_thread(void* pm_threadarg)
{
    struct pm_thread_data* mydata;
    mydata = (struct pm_thread_data *) pm_threadarg;
    parameter_objective_boxplot_to_file(mydata->env, mydata->fig_name, mydata->par, mydata->obj_index);

    pthread_exit(NULL);

}



struct me_thread_data
{
    m3_env* env;
    string fig_name;
    int obj_index;
};

void *print_main_effects_thread(void* me_threadarg)
{
    struct me_thread_data* mydata;
    mydata = (struct me_thread_data *) me_threadarg;
    main_effects_plot_to_file(mydata->env, mydata->fig_name, mydata->obj_index);
    pthread_exit(NULL);
}



struct i_thread_data
{
    m3_env* env;
    string par;
    int par_index;
    int obj_index;
    double y_min;
    double y_max;
    vector <string> data_files;
};

void *print_interaction_effects_thread(void* i_threadarg)
{
    struct i_thread_data* mydata;
    mydata = (struct i_thread_data *) i_threadarg;
    interaction_effects_write_to_files(mydata->env, mydata->par, mydata->par_index, mydata->obj_index, mydata->y_min, mydata->y_max, mydata->data_files);
    pthread_exit(NULL);
}

m3_database *old_root;

void change_current_to_root(m3_env *env)
{
   old_root = env->available_dbs["root"];
   env->available_dbs["root"] = env->available_dbs[env->current_db_name];  
}

void restore_root(m3_env *env)
{
    env->available_dbs["root"] = old_root;
}

m3_database *original_db;

bool filter_current_db(m3_env *env)
{
    change_current_to_root(env);
    dont_look_at_objectives = true;
    m3_database *filtered_db= opt_execute_doe_and_optimizer(env, "m3_full_doe", "m3_linear_scan");
    if(!filtered_db)
        return false;
    dont_look_at_objectives = false;
    restore_root(env);
    original_db = env->available_dbs[env->current_db_name];
    env->available_dbs[env->current_db_name] = filtered_db;

    prs_display_message("Original database has "+itos(original_db->size())+ " points");
    prs_display_message("Reported database has "+itos(filtered_db->size())+ " points");
    return true;
}

void restore_current_db(m3_env *env)
{
    env->available_dbs[env->current_db_name] = original_db;
}


#define add_l_content(s) l_content = l_content + (s)

void db_generate_html(string folder_name)
{

    string l_content = "";
    reset_latex_content();

    if(!current_environment.available_dbs.count(current_environment.current_db_name))
        return;

    if(!filter_current_db(&current_environment))
        prs_display_error("Can't pre-process database");

    if( current_environment.current_design_space == NULL )
    {
		prs_display_error("Can't generate HTML report");
		return;
    }

    pthread_mutex_init (&sh_lock, NULL);


    //Creating base and figs folders and main page reference
    base_folder = folder_name;
    figs_folder = folder_name+"/figs";

    shell_command("mkdir "+base_folder);
    shell_command("mkdir "+figs_folder);

    /** DESIGN SPACE */
    string lm1;
    string lm2;
    b_folder = base_folder;


    current_file = "m3_design_space_graphs.html";
    create_page(os, "Objectives space", html_objective_space_description);
    create_latex_page(l_os, "Objectives space", html_objective_space_description);

    add_link_title(link_material, "Objectives space");

    create_section(os_section, "Overall objective space", "This section shows the overall objective space composition by using a series of scatter plots for each pair of problem objectives.");
    add_link_item(lm1, "Overall analysis");
    
    create_latex_section(l_os_section, "Overall objective space", "This section shows the overall objective space composition by using a series of scatter plots for each pair of problem objectives.");


    create_section(os_la_section, "Level analysis of the objective space", "This section shows the contribution of the "+bold("levels")+" of each parameters to the objective space by using a series of scatter plots for each pair of problem objectives.");
    add_link_item(lm2, "Level analysis");

    create_latex_section(l_os_la_section, "Level analysis of the objective space", "This section shows the contribution of the "+boldl("levels")+" of each parameters to the objective space by using a series of scatter plots for each pair of problem objectives.");

    string content;
    string section1 = os_section;
    string section2 = os_la_section;
    
    string l_section1 = l_os_section;
    string l_section2 = l_os_la_section;

    int obj_n = current_environment.current_design_space->objectives.size();
    string designspace_fig,  designspace_fig_ref, pareto_files_ref, level_analysis_fig, level_analysis_fig_ref;
    string current_par_name;

    //Calculating thread number and creating thread array and passing parameter structure
    int return_code;
    int p_idx = 0;
    int idx = 0;
    int la_threads_num = 0;
    int pareto_threads_num = 0;
    for(int i=0; i<obj_n-1; i++)
    {
        for(int j=i+1; j<obj_n; j++)
        {	
            pareto_threads_num++;
            for(int k=0; k< current_environment.current_design_space->ds_parameters.size(); k++)
            {
                if(current_environment.current_design_space->ds_parameters[k].type == M3_DS_SCALAR)
                    la_threads_num++;
            }
        }
    }
    pthread_t pareto_thread[pareto_threads_num];
    pareto_thread_data pareto_thread_data_array[pareto_threads_num];
    pthread_t la_thread[la_threads_num];
    la_thread_data la_thread_data_array[la_threads_num];

    for(int i=0; i<obj_n-1; i++)
    {
        for(int j=i+1; j<obj_n; j++)
        {
            designspace_fig_ref = "figs/ds_"+itos(i)+"_"+itos(j)+".png";
                
            pareto_thread_data_array[p_idx].db = current_environment.current_db_name;
            pareto_thread_data_array[p_idx].fig_name = base_folder+"/"+designspace_fig_ref;
            pareto_thread_data_array[p_idx].obj1 = i;
            pareto_thread_data_array[p_idx].obj2 = j;
            return_code = pthread_create(&pareto_thread[p_idx], NULL, print_pareto_thread, (void*) &pareto_thread_data_array[p_idx]);
            if(return_code) 
                prs_display_error("ERROR creating thread\n");

            p_idx++;

            create_figure(vz_fig, "Objectives space, projection on '"+bold(current_environment.current_design_space->objectives_names[i])+"' - '"+ bold(current_environment.current_design_space->objectives_names[j])+"'",  designspace_fig_ref);
            add_link_subitem(lm1, (current_environment.current_design_space->objectives_names[i])+" - "+ (current_environment.current_design_space->objectives_names[j]));
            create_latex_figure(l_vz_fig, "Objectives space, projection on '"+boldl(current_environment.current_design_space->objectives_names[i])+"' - '"+ boldl(current_environment.current_design_space->objectives_names[j])+"'",  b_folder+"/"+designspace_fig_ref);

            section1 = section1 + vz_fig;
            l_section1 = l_section1 + l_vz_fig;

            for(int k=0; k< current_environment.current_design_space->ds_parameters.size(); k++)
            {
                if(current_environment.current_design_space->ds_parameters[k].type == M3_DS_SCALAR)
                {			
                    current_par_name = current_environment.current_design_space->ds_parameters[k].name;
                    level_analysis_fig_ref = "figs/dsla_"+itos(k)+"_"+itos(i)+"_"+itos(j)+".png";

                    la_thread_data_array[idx].env = &current_environment;
                    la_thread_data_array[idx].fig_name = base_folder+"/"+level_analysis_fig_ref;
                    la_thread_data_array[idx].par = current_par_name;
                    la_thread_data_array[idx].obj1 = i;
                    la_thread_data_array[idx].obj2 = j;
                    return_code = pthread_create(&la_thread[idx], NULL, print_level_analysis_thread, (void*) &la_thread_data_array[idx]);
                    if(return_code)	
                        prs_display_error("ERROR creating thread\n");

                    idx++;

                    create_figure(vz_fig, "Objectives space, focus on '"+bold(current_par_name)+"', projection on '"+bold(current_environment.current_design_space->objectives_names[i])+"' - '"+ bold(current_environment.current_design_space->objectives_names[j])+"'",  level_analysis_fig_ref);
                    
                    create_latex_figure(l_vz_fig, "Objectives space, focus on '"+boldl(current_par_name)+"', projection on '"+boldl(current_environment.current_design_space->objectives_names[i])+"' - '"+ boldl(current_environment.current_design_space->objectives_names[j])+"'",  b_folder+"/"+level_analysis_fig_ref);

                    add_link_subitem(lm2, (current_par_name)+": "+(current_environment.current_design_space->objectives_names[i])+" - "+ (current_environment.current_design_space->objectives_names[j]));

                    section2 = section2 + vz_fig;
                    l_section2 = l_section2 + l_vz_fig;

                }
            }		
        }
    }

    content = section1 + section2;
    link_material = link_material + lm1 + lm2;
    add_content(os, content);

    add_latex_content(l_os+l_section1+l_section2);
            
    m3_write_html_template(b_folder+"/m3_design_space_graphs.html",os);
    m3_write_html_css(b_folder);


    current_file = "m3_design_space_graphs_pareto.html";

    prs_display_message("HTML Analysis per level correctly written");

    /** VZ ++++ */
    create_page(osp, "Objectives space, pareto sets", html_objective_space_pareto_sets_description);
    create_latex_page(l_osp, "Objectives space, pareto sets", html_objective_space_pareto_sets_description);
    add_link_title(link_material, "Pareto sets");

    create_section(osp_section, "Overall objective space Pareto sets", "This section shows the overall objective space composition by using a series of scatter plots for each pair of problem objectives.");
    create_latex_section(l_osp_section, "Overall objective space Pareto sets", "This section shows the overall objective space composition by using a series of scatter plots for each pair of problem objectives.");
    lm1 = "";
    add_link_item(lm1, "Overall Pareto sets");

    section1 = osp_section;
    l_section1 = l_osp_section;

    create_section(ospla_section, "Objectives space Pareto sets, level analysis", "This section shows the contribution of the "+bold("levels")+" of each parameters to the objective space Pareto sets by using a series of scatter plots for each pair of problem objectives.");
    create_latex_section(l_ospla_section, "Objectives space Pareto sets, level analysis", "This section shows the contribution of the "+boldl("levels")+" of each parameters to the objective space Pareto sets by using a series of scatter plots for each pair of problem objectives.");
    lm2 = "";
    add_link_item(lm2, "Pareto sets, level analysis");
            
    section2= ospla_section;
    l_section2= l_ospla_section;

    /** VZ --- */

    string pareto_files, pareto_fig, pareto_fig_ref;

    m3_database* filtered_db; 
    filtered_db = sim_compute_pareto_on_local_db(&current_environment, current_environment.available_dbs[current_environment.current_db_name]);

    for(int i=0; i<obj_n-1; i++)
    {
        for(int j=i+1; j<obj_n; j++)
        {	
            pareto_fig_ref = "figs/pareto_"+itos(i)+"_"+itos(j)+".png";

            pareto_plot_to_file_local_db_obj(filtered_db, base_folder+"/"+pareto_fig_ref, i, j, 1);


            create_figure(vz_fig, "Objective space Pareto sets, projection on '"+bold(current_environment.current_design_space->objectives_names[i])+"' - '"+ bold(current_environment.current_design_space->objectives_names[j])+"'",  pareto_fig_ref);
            create_latex_figure(l_vz_fig, "Objective space Pareto sets, projection on '"+boldl(current_environment.current_design_space->objectives_names[i])+"' - '"+ boldl(current_environment.current_design_space->objectives_names[j])+"'",  b_folder+"/"+pareto_fig_ref);

            add_link_subitem(lm1, (current_environment.current_design_space->objectives_names[i])+" - "+ (current_environment.current_design_space->objectives_names[j]));

            section1 = section1 + vz_fig;
            l_section1 = l_section1 + l_vz_fig;

            for(int k=0; k< current_environment.current_design_space->ds_parameters.size(); k++)
            {
                if(current_environment.current_design_space->ds_parameters[k].type == M3_DS_SCALAR)
                {			
                    current_par_name = current_environment.current_design_space->ds_parameters[k].name;
                    level_analysis_fig_ref = "figs/la_"+itos(k)+"_"+itos(i)+"_"+itos(j)+".png";

                    level_analysis_plot_to_file_local_db_obj(&current_environment, filtered_db, 
                            base_folder+"/"+level_analysis_fig_ref, current_par_name, i, j, 1);

                    create_figure(vz_fig, "Objective space, focus on '"+bold(current_par_name)+"', projection on '"+bold(current_environment.current_design_space->objectives_names[i])+"' - '"+ bold(current_environment.current_design_space->objectives_names[j])+"'",  level_analysis_fig_ref);
                    create_latex_figure(l_vz_fig, "Objective space, focus on '"+boldl(current_par_name)+"', projection on '"+boldl(current_environment.current_design_space->objectives_names[i])+"' - '"+ boldl(current_environment.current_design_space->objectives_names[j])+"'",  b_folder+"/"+level_analysis_fig_ref);

                    add_link_subitem(lm2, (current_par_name)+": "+(current_environment.current_design_space->objectives_names[i])+" - "+ (current_environment.current_design_space->objectives_names[j]));

                    section2 = section2 + vz_fig;
                    l_section2 = l_section2 + l_vz_fig;

                }
            }
        }
    }
    
    link_material = link_material + lm1 + lm2;
    content = section1 + section2;
    add_latex_content(l_section1+l_section2);
    add_content(osp, content);
           
    m3_write_html_template(b_folder+"/m3_design_space_graphs_pareto.html",osp);
    
    /** VZ ++ */
    current_file = "m3_design_space_pareto_design_table.html";
    int table_size_px;
    string design_table = vz_db_export_local_db_html(&current_environment, filtered_db, table_size_px );
    delete filtered_db;
#define MIN_PAGE_SZ 820

    create_page_variable(vz_dt, "Pareto sets, design table", "This table shows the composition of the Pareto set of the objective space. Objective values are indicated in bold.", (table_size_px < MIN_PAGE_SZ) ? MIN_PAGE_SZ: table_size_px);
    add_link_title(link_material, "Pareto sets, design table");

    add_content(vz_dt, design_table);
    
    m3_write_html_template(b_folder+"/m3_design_space_pareto_design_table.html",vz_dt);

    
    section1 = "";
    l_section1 = "";
    section2 = "";
    lm1 = "";
    lm2 = "";

    current_file = "m3_design_space_box_plots.html";
    create_page(vz_bb, "Objectives Box plots", html_bb_description);
    create_latex_page(l_vz_bb, "Objectives Box plots", html_bb_description);
    add_link_title(link_material, "Objectives Box plots");

    /** VZ -- */

    string parameter_objective_fig_ref;
    string current_parameter;

    //Calculating thread number and creating thread array and passing parameter structure
    idx = 0;
    int pm_threads_num = 0;

	for(int i=0; i<current_environment.current_design_space->ds_parameters.size(); i++)
	{
		if(current_environment.current_design_space->ds_parameters[i].type == M3_DS_SCALAR)
		{
			for(int j=0; j<current_environment.current_design_space->objectives.size(); j++)
			{
				pm_threads_num++;
			}
		}
	}
    pthread_t pm_thread[pm_threads_num];
    pm_thread_data pm_thread_data_array[pm_threads_num];

	for(int i=0; i<current_environment.current_design_space->ds_parameters.size(); i++)
	{
		if(current_environment.current_design_space->ds_parameters[i].type == M3_DS_SCALAR)
		{
			current_parameter = current_environment.current_design_space->ds_parameters[i].name;

			/** VZ ++ */
			create_section(bb_section, "Box plots for parameter '"+bold(current_parameter)+"'", "The following Box Plots shows the behavior of each of the objectives with respect to parameter '"+bold(current_parameter)+"'");
			create_latex_section(l_bb_section, "Box plots for parameter '"+boldl(current_parameter)+"'", "The following Box Plots shows the behavior of each of the objectives with respect to parameter '"+boldl(current_parameter)+"'");

			add_link_item(lm1, (current_parameter));

			section1 = section1 + bb_section;
			l_section1 = l_section1 + l_bb_section;

			/** VZ -- */

			for(int j=0; j<current_environment.current_design_space->objectives.size(); j++)
			{
				parameter_objective_fig_ref = "figs/pm_"+itos(i)+"_"+itos(j)+".png";

				string objective_name = current_environment.current_design_space->objectives_names[j];

				//Generating parameter-objective boxplot graphs in parameter_objective file using threads
				pm_thread_data_array[idx].env = &current_environment;
				pm_thread_data_array[idx].fig_name = base_folder+"/"+parameter_objective_fig_ref;
				pm_thread_data_array[idx].par = current_parameter;
				pm_thread_data_array[idx].obj_index = j;
				return_code = pthread_create(&pm_thread[idx], NULL, print_parameter_objective_boxplot_thread, (void*) &pm_thread_data_array[idx]);
				if(return_code) prs_display_error("ERROR creating thread\n");
				idx++;

				create_figure(vz_fig, "Box plot of '"+bold(objective_name)+"', focus on '"+bold(current_parameter)+"'.",  parameter_objective_fig_ref);
				create_latex_figure(l_vz_fig, "Box plot of '"+boldl(objective_name)+"', focus on '"+boldl(current_parameter)+"'.",  b_folder+"/"+parameter_objective_fig_ref);
				add_link_subitem(lm1, objective_name);

				section1 = section1 + vz_fig;
				l_section1 = l_section1 + l_vz_fig;
			}
		}
	}
    link_material = link_material + lm1;
    content = section1;
    add_content(vz_bb, content);
    add_latex_content(l_vz_bb+latex_bb_added_desctiption_1+BOXPLOT_SCHEMA_L+latex_bb_added_desctiption_2+l_section1);

    m3_write_html_template(b_folder+"/m3_design_space_box_plots.html",vz_bb);
    /** VZ -- **/

    prs_display_message("HTML Box plots correctly generated");


    //Creating and writing main_effects file
    string main_effects_fig_ref;

    //Calculating thread number and creating thread array and passing parameter structure
    idx = 0;
    int me_threads_num = 0;
   	me_threads_num = current_environment.current_design_space->objectives.size();
    pthread_t me_thread[me_threads_num];
    me_thread_data me_thread_data_array[me_threads_num];
    
    /** VZ +++ */
    current_file = "m3_design_space_main_effects.html";

    create_page(vz_mef, "Parameter main effects", html_mef_description);
    create_latex_page(l_vz_mef, "Parameter main effects", html_mef_description);
    add_link_title(link_material, "Parameter main effects");

    section1 = "";
    l_section1 = "";
    lm1="";
    /** VZ --- */

	for(int j=0; j<current_environment.current_design_space->objectives.size(); j++)
	{
		main_effects_fig_ref = "figs/me_"+itos(j)+".png";

		string current_objective_name = current_environment.current_design_space->objectives_names[j];
		int current_objective_index = j;

		create_figure(vz_fig, "Impact of parameters on '"+bold(current_objective_name)+"'.", main_effects_fig_ref);
		create_latex_figure(l_vz_fig, "Impact of parameters on '"+boldl(current_objective_name)+"'.", b_folder+"/"+main_effects_fig_ref);
		add_link_subitem(lm1, current_objective_name);

		section1 = section1 + vz_fig;
		l_section1 = l_section1 + l_vz_fig;

		me_thread_data_array[idx].env = &current_environment;
		me_thread_data_array[idx].fig_name = base_folder+"/"+main_effects_fig_ref;
		me_thread_data_array[idx].obj_index = current_objective_index;

		return_code = pthread_create(&me_thread[idx], NULL, print_main_effects_thread, (void*) &me_thread_data_array[idx]);

		if(return_code)
			prs_display_error("ERROR creating thread\n");

		idx++;
	}
    content = section1;
    link_material = link_material + lm1;
    add_content(vz_mef, content);
    add_latex_content(l_vz_mef+l_section1);
           
    m3_write_html_template(b_folder+"/m3_design_space_main_effects.html",vz_mef);

    prs_display_message("HTML Main effects correctly generated");

    //Creating and writing inter_files files
    string interactions_fig_ref;
    string interactions_fig;

    //Calculating thread number and creating thread array and passing parameter structure
    idx = 0;
    int i_threads_num = 0;
	for(int j=0; j<current_environment.current_design_space->objectives.size(); j++)
	{
		for(int i=0; i<current_environment.current_design_space->ds_parameters.size(); i++)
		{
			if(current_environment.current_design_space->ds_parameters[i].type == M3_DS_SCALAR)
				i_threads_num++;
		}
	}
    pthread_t i_thread[i_threads_num];
    i_thread_data i_thread_data_array[i_threads_num];
    
    /** VZ +++ */

    current_file = "m3_design_space_interactions.html";
    create_page(vz_int, "Parameter interactions", html_int_description);
    create_latex_page(l_vz_int, "Parameter interactions", html_int_description);
    add_link_title(link_material, "Parameter interactions");

    section1 = "";
    l_section1 = "";
    lm1 = "";
    /** VZ --- */

	for(int j=0; j<current_environment.current_design_space->objectives.size(); j++)
	{
		string current_objective = current_environment.current_design_space->objectives_names[j];

		create_section(int_section, "Parameter interactions for objective '"+bold(current_objective)+"'", "This table shows the interaction effects for objective '"+bold(current_objective)+"'. Parallel lines represent no interaction between parameters on determining '"+bold(current_objective)+"'. Diverging lines represent synergistic effects while converging lines represent antagonistic effects between parameters.");
		create_latex_section(l_int_section, "Parameter interactions for objective '"+boldl(current_objective)+"'", "This table shows the interaction effects for objective '"+boldl(current_objective)+"'. Parallel lines represent no interaction between parameters on determining '"+boldl(current_objective)+"'. Diverging lines represent synergistic effects while converging lines represent antagonistic effects between parameters.");

		add_link_item(lm1, (current_objective));

		section1 = section1+ int_section;
		l_section1 = l_section1+ l_int_section;
		/** VZ --- */
		for(int i=0; i<current_environment.current_design_space->ds_parameters.size(); i++)
		{
			if(current_environment.current_design_space->ds_parameters[i].type == M3_DS_SCALAR)
			{
				interactions_fig_ref = "figs/int_"+itos(j)+"_"+itos(i)+".png";
				current_parameter = current_environment.current_design_space->ds_parameters[i].name;

				//Generating interaction graphs in inter_files file using threads
				i_thread_data_array[idx].env = &current_environment;
				i_thread_data_array[idx].par = current_parameter;
				i_thread_data_array[idx].obj_index = j;
				i_thread_data_array[idx].y_min = 0;
				i_thread_data_array[idx].y_min = 0;
				i_thread_data_array[idx].par_index = i;
				return_code = pthread_create(&i_thread[idx], NULL, print_interaction_effects_thread, (void*) 																&i_thread_data_array[idx]);
				if(return_code)
					prs_display_error("ERROR creating thread\n");

				idx++;

				create_effects(vz_fig, "Impact of parameter '" + bold(current_parameter) + "' against the others on '"+bold(current_objective)+"' by considering interactions.", interactions_fig_ref);
				create_latex_effects(l_vz_fig, "Impact of parameter '" + boldl(current_parameter) + "' against the others on '"+boldl(current_objective)+"' by considering interactions.", b_folder+"/"+interactions_fig_ref);
				add_link_subitem(lm1, current_parameter);

				section1 = section1 + vz_fig;
				l_section1 = l_section1 + l_vz_fig;
				/** VZ --- */

			}
		}
	}

    content = section1;
    link_material = link_material + lm1;
    add_content(vz_int, content);
    add_latex_content(l_vz_int+latex_int_added_desctiption_1+INTERACTION_SCHEMA_L+latex_int_added_desctiption_2 +l_section1);

    m3_write_html_template(b_folder+"/m3_design_space_interactions.html",vz_int);


    prs_display_message("HTML Interaction effects correctly generated");


    current_file = "m3_design_space_interactions_surface.html";
    create_page(vz_int_s, "Surface plots", html_int_description_s);
    create_latex_page(l_vz_int_s, "Surface plots", html_int_description_s);

    add_link_title(link_material, "Surface plots");

    section1 = "";
    l_section1 = "";
    lm1 = "";
    /** VZ --- */

	for(int j=0; j<current_environment.current_design_space->objectives.size(); j++)
	{
		string current_objective = current_environment.current_design_space->objectives_names[j];

		create_section(int_section_s, "Surface plot for objective '"+bold(current_objective)+"'", "The following graphs show the surface plots of " + bold(current_objective) + " for each pair of parameters.");
		create_latex_section(l_int_section_s, "Surface plot for objective '"+boldl(current_objective)+"'", "The following graphs show the surface plots of " + boldl(current_objective) + " for each pair of parameters.");

		add_link_item(lm1, (current_objective));

		section1 = section1+ int_section_s;
		l_section1 = l_section1+ l_int_section_s;
		/** VZ --- */
		for(int alpha=0; alpha<current_environment.current_design_space->ds_parameters.size();alpha++)
		{
			if(current_environment.current_design_space->ds_parameters[alpha].type == M3_DS_SCALAR)
			{
				for(int beta=0; beta<current_environment.current_design_space->ds_parameters.size(); beta++)
				{
					if((alpha!=beta) && current_environment.current_design_space->ds_parameters[beta].type == M3_DS_SCALAR)
					{
						string alphan = current_environment.current_design_space->ds_parameters[alpha].name;
						string betan  = current_environment.current_design_space->ds_parameters[beta].name;
						string surf_ref = "figs/map_"+itos(alpha)+"_"+itos(beta)+"_"+itos(j)+".png";
						create_figure(vz_fig, "Surface plot for '"+bold(current_objective)+"', '"+bold(alphan)+"'-'"+bold(betan)+"' parameters.", surf_ref);
						create_latex_figure(l_vz_fig, "Surface plot for '"+boldl(current_objective)+"', '"+boldl(alphan)+"'-'"+boldl(betan)+"' parameters.", b_folder+"/"+surf_ref);

						add_link_subitem(lm1, (alphan)+"-"+(betan));

						section1 = section1 + vz_fig;
						l_section1 = l_section1 + l_vz_fig;
				/** VZ --- */
						plot_map_to_figure(current_environment.available_dbs[current_environment.current_db_name], j, alpha, beta);
					}
				}
			}

		}
	}

    content = section1;
    link_material = link_material + lm1;
    add_content(vz_int_s, content);
    add_latex_content(l_vz_int_s+l_section1);
           
    m3_write_html_template(b_folder+"/m3_design_space_interactions_surface.html",vz_int_s);

    current_file = "m3_design_space_interactions_surface_contour.html";
    create_page(vz_int_sc, "Contour maps", html_int_description_sc);
    create_latex_page(l_vz_int_sc, "Contour maps", html_int_description_sc);

    add_link_title(link_material, "Contour maps");

    section1 = "";
    l_section1 = "";
    lm1 = "";
    /** VZ --- */

	for(int j=0; j<current_environment.current_design_space->objectives.size(); j++)
	{
		string current_objective = current_environment.current_design_space->objectives_names[j];

		create_section(int_section_sc, "Contour map for objective '"+bold(current_objective)+"'", "The following graphs show the contour plots of " + bold(current_objective) + " for each pair of parameters.");
		create_latex_section(l_int_section_sc, "Contour map for objective '"+boldl(current_objective)+"'", "The following graphs show the contour plots of " + boldl(current_objective) + " for each pair of parameters.");

		add_link_item(lm1, (current_objective));

		section1 = section1+ int_section_sc;
		l_section1 = l_section1+ l_int_section_sc;
		/** VZ --- */
		for(int alpha=0; alpha<current_environment.current_design_space->ds_parameters.size();alpha++)
		{
			if(current_environment.current_design_space->ds_parameters[alpha].type == M3_DS_SCALAR)
			{
				for(int beta=0; beta<current_environment.current_design_space->ds_parameters.size(); beta++)
				{
					if((alpha!=beta) && current_environment.current_design_space->ds_parameters[beta].type == M3_DS_SCALAR)
					{
						string alphan = current_environment.current_design_space->ds_parameters[alpha].name;
						string betan  = current_environment.current_design_space->ds_parameters[beta].name;
						string surf_ref = "figs/map_"+itos(alpha)+"_"+itos(beta)+"_"+itos(j)+"_flat.png";
						create_figure(vz_fig, "Contour map for '"+bold(current_objective)+"', '"+bold(alphan)+"'-'"+bold(betan)+"' parameters.", surf_ref);
						create_latex_figure(l_vz_fig, "Contour map for '"+boldl(current_objective)+"', '"+boldl(alphan)+"'-'"+boldl(betan)+"' parameters.", b_folder+"/"+surf_ref);

						add_link_subitem(lm1, (alphan)+"-"+(betan));

						section1 = section1 + vz_fig;
						l_section1 = l_section1 + l_vz_fig;
					}
				}
			}

		}
	}

    content = section1;
    link_material = link_material + lm1;
    add_content(vz_int_sc, content);
    add_latex_content(l_vz_int_sc+l_section1);
           
    m3_write_html_template(b_folder+"/m3_design_space_interactions_surface_contour.html",vz_int_sc);


    prs_display_message("HTML Interaction effects correctly generated");

    //Waiting for all threads to finish execution
    //Waiting for all pareto_threads to finish execution (join)
    void* status;
    for(int s=0; s<pareto_threads_num; s++)
    {
        return_code = pthread_join(pareto_thread[s], &status);	
    }
    //Waiting for all la_threads to finish execution (join)
    for(int t=0; t<la_threads_num; t++)
    {
        return_code = pthread_join(la_thread[t], &status);	
    }
    //Waiting for all pm_threads to finish execution (join)
    for(int u=0; u<pm_threads_num; u++)
    {
        return_code = pthread_join(pm_thread[u], &status);	
    }
    //Waiting for all me_threads to finish execution (join)
    for(int v=0; v<me_threads_num; v++)
    {
        return_code = pthread_join(me_thread[v], &status);	
    }





    //Waiting for all i_threads to finish execution (join) and calculating actual y axis range for each metric
	vector<double> y_min; y_min.resize(current_environment.current_design_space->objectives.size());
	vector<double> y_max; y_max.resize(current_environment.current_design_space->objectives.size());
	vector<bool> is_initialized; is_initialized.resize(current_environment.current_design_space->objectives.size());

   	for (int w=0; w<current_environment.current_design_space->objectives.size(); w++)
	{
		is_initialized[w] = 0;
	}

    int curr_objective_idx;
    for(int z=0; z<i_threads_num; z++)
    {
        return_code = pthread_join(i_thread[z], &status);
        curr_objective_idx = i_thread_data_array[z].obj_index;
        if(!is_initialized[curr_objective_idx])
        {
            y_min[curr_objective_idx] = i_thread_data_array[z].y_min;
            y_max[curr_objective_idx] = i_thread_data_array[z].y_max;
            is_initialized[curr_objective_idx] = 1;
        }
        else
        {
            if(i_thread_data_array[z].y_min < y_min[curr_objective_idx]) y_min[curr_objective_idx] = i_thread_data_array[z].y_min;
            if(i_thread_data_array[z].y_max > y_max[curr_objective_idx]) y_max[curr_objective_idx] = i_thread_data_array[z].y_max;
        }
    }


    string vz_link_template= m3_read_html_template("template_skeleton_link.html");
    link_material = m3_substitute(vz_link_template, "@link_material@", link_material);
    m3_write_html_template(b_folder+"/m3_link.html",link_material);

    //Printing interaction figures
    idx = 0;
	for(int j=0; j<current_environment.current_design_space->objectives.size(); j++)
	{
		curr_objective_idx = j;
		for(int i=0; i<current_environment.current_design_space->ds_parameters.size(); i++)
		{
			if(current_environment.current_design_space->ds_parameters[i].type == M3_DS_SCALAR)
			{
				current_parameter = current_environment.current_design_space->ds_parameters[i].name;
				interactions_fig = base_folder+"/figs/int_"+itos(j)+"_"+itos(i)+".png";
				interaction_effects_plot_to_file(&current_environment, interactions_fig, current_parameter,
						j, y_min[curr_objective_idx], y_max[curr_objective_idx], i_thread_data_array[idx].data_files);
				idx++;
			}

		}
	}


    generate_summary_page(&current_environment, y_min, y_max);
    //Exiting ang notifying user of success
    prs_display_message("HTML report correctly generated");
    restore_current_db(&current_environment);
}
