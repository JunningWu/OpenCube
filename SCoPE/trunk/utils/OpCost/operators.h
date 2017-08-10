//==========================================================================
//	operators.h
//	Author: Hector Posadas, Juan Castillo, David Quijano
//	Date: 12-12-2007
//	Description: Static Parser for execution time estimation and cache modeling.
//  Original grammar obtained from http://willink.me.uk/projects/fog/index.html
//  These files comprises a pre-built demonstration of the superset C++ grammar
//  from FOG.
//==========================================================================
//  Copyright (C) 2008 Design of Systems on Silicon(DS2)
//  Main developer: University of Cantabria
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
//  For more information about SCoPE you can visit
//  http://www.teisa.unican.es/scope or write an e-mail to
//  scope@teisa.unican.es or a letter to SCoPE, GIM - TEISA, University
//  of Cantabria, AV. Los Castros s/n, ETSIIT, 39005, Santander, Spain
//==========================================================================

/*!
 * \file operators.h
 * \brief Defines for operators handling
 */
#ifndef _OPERATORS_H_
#define _OPERATORS_H_

	/* Number of parameters */

	#define TOTAL_DATA_OP 			35
	#define TOTAL_ACCESS_OP 		8
	#define TOTAL_CONTROL_OP 		5
	#define TOTAL_DATA_TYPES		4
	#define TOTAL_CRTL_OP			13

	/* Data Types */

	#define INT_TYPE_OP   			0
	#define FLOAT_TYPE_OP 			1
	#define CHAR_TYPE_OP  			2
	#define BOOL_TYPE_OP  			3

	/* Data-type dependent operators */

	#define INC_OP					0
	#define DEC_OP					1
	#define PLUS_UN_OP 				2
	#define MINUS_UN_OP 			3
	#define NOT_OP					4
	#define INV_OP					5
	#define ASSIGN_OP 				6
	#define PLUS_OP 				7
	#define MINUS_OP 				8
	#define MULT_OP					9
	#define DIV_OP					10
	#define MOD_OP					11

	#define ASS_ADD_OP				12
	#define ASS_SUB_OP				13
	#define ASS_MUL_OP				14
	#define ASS_DIV_OP				15
	#define ASS_MOD_OP				16

	#define EQUAL_OP				17
	#define NEQ_OP					18
	#define GT_OP					19
	#define LT_OP					20
	#define GE_OP					21
	#define LE_OP					22

	#define LOG_AND_OP				23
	#define LOG_OR_OP				24
	#define AND_OP					25
	#define XOR_OP					26
	#define OR_OP					27
	#define ASS_AND_OP				28
	#define ASS_XOR_OP				29
	#define ASS_OR_OP				30

	#define SHL_OP					31
	#define ASS_SHL_OP				32
	#define SHR_OP					33
	#define ASS_SHR_OP				34

	/* Data-type independent operations */

	#define ARRAY_OP				35 /*0*/
	#define DOT_OP					36 /*1*/
	#define ARROW_OP				37 /*2*/
	#define REFERENCE_OP			38 /*3*/
	#define STAR_OP					39 /*4*/
	#define TERNARY_OP				40 /*5*/
	#define NEW_OP					41 /*6*/
	#define DELETE_OP				42 /*7*/

	#define START_LOOP_ST			43 /*0*/
	#define JUMP_ST					44 /*1*/
	#define FUNC_CALL				45 /*2*/
	#define MEMB_FUNC_CALL			46 /*3*/
	#define END_FUNC				47 /*4*/

#endif
