//==========================================================================
//	CxxToken.hxx
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
 * \file CxxToken.hxx
 * \brief Static Parser for execution time estimation and cache modeling
 */

#ifndef CXXTOKEN_HXX
#define CXXTOKEN_HXX

#include <iostream>
#include <stdlib.h>
using namespace std;

#define YYSTYPE CxxTokenType
#define YY_parse_STYPE CxxTokenType
#define YACC_BANG_TYPE size_t
#define YACC_MARK_TYPE size_t

#define YACC_BANG() push_bang()
#define YACC_UNBANG(bangValue, msg) pop_bang(bangValue); yyerrok; yyclearin; yyerror(msg);

#define ERRMSG(a) do { cout << "/*ERROR -- " << a << "*/"<< endl; increment_error_count(); } while (0)
#define WRNMSG(a) do { cout << "/*WARNING -- " << a << "*/"<< endl; increment_warning_count(); } while (0)


#ifdef NEEDS_BOOL
enum bool { false, true };
#endif

#ifdef BISON_PP_CLASS
#define PARSE_TOKEN(a) BISON_PP_CLASS::a
#else
#define PARSE_TOKEN(a) a
#endif

extern int line_number;
extern void increment_error_count();
extern void increment_warning_count();
extern int tokenMarkDepth;

enum var_type {int_t, long_t, short_t, long_long_t, float_t, double_t, bool_t, char_t, string_t, wide_char_t, wide_string_t, pointer_t, array_t, lit_int_t, lit_double_t, lit_bool_t, lit_string_t, lit_char_t, lit_wide_char_t, lit_wide_string_t, lit_number_t, struct_t, class_t, union_t, user_t, func_t, no_type};

/*!
 * \class CxxToken
 * \brief Class to model token characteristics
 */
class CxxToken {
public:
	//Constructor(s) & destructor
	//CxxToken(int tokenValue = 0, var_type type = no_type, const char* name=NULL,int leng=-1 ) : _value(tokenValue),_type(type) {
	CxxToken(); //Default
	CxxToken(const CxxToken&);
	CxxToken(int tokenValue , var_type type , const char* name,int leng );
	virtual ~CxxToken();

	int value() const;
	void set_value(int val);
	enum var_type type() const;
	void set_type(enum var_type type);
	bool is_name(const char *name);

	char * name() const;
	void set_name(const char *name);

	char * scope() const;
	void set_scope(const char *scope);

	int _value;					//!< Token id
	enum var_type _type;		//!< Type of variable stored in token (int, char, etc)
	char *_name;
	int _leng;
	char *_scope;

private:
	CxxToken& operator=(const CxxToken&);
};


enum CxxIsTemplate { IS_DEFAULT, IS_TEMPLATE };
enum CxxIsTree { IS_SCALAR, IS_TREE };
typedef CxxToken CxxTreeArgument;

class CxxStatement : public CxxToken {};
typedef CxxStatement CxxDeclaration;
class CxxExpression : public CxxStatement {};
class CxxName : public CxxExpression {};
class CxxTokens : public CxxToken {};
class CxxMetaObject : public CxxExpression {};
class CxxMetaStatement : public CxxStatement {};

class CxxStatements : public CxxStatement {};
typedef CxxStatements CxxDeclarations;
typedef CxxDeclarations CxxMemberDeclarations;
typedef CxxExpression CxxTreeExpression;

class CxxKeyword : public CxxName {};
class CxxDeclSpecifierId : public CxxKeyword {};

class CxxAccessSpecifier : public CxxKeyword {};
class CxxBaseSpecifier : public CxxToken {};
class CxxBaseSpecifiers : public CxxTokens {};
class CxxBrace : public CxxToken {};
class CxxBuiltInId : public CxxName {
/*	CxxBuiltInId(CxxName &rhs){
		_value= rhs._value;
		_type= rhs._type;
		_name= rhs._name;
		_leng= rhs._leng;
		_scope= rhs._scope;
	}*/
};
class CxxCharacterLiteral : public CxxToken {};
class CxxClass : public CxxToken {};
class CxxClassKey : public CxxKeyword {};
class CxxCondition : public CxxExpression {};
class CxxCvQualifiers : public CxxDeclSpecifierId {};
class CxxDeclarator : public CxxToken {};
typedef CxxExpression CxxDeleteExpression;
//class CxxDerived : public CxxToken {};
//class CxxEnum : public CxxToken {};
class CxxEnumerator : public CxxToken {};
class CxxEnumerators : public CxxTokens {};
class CxxExceptionDeclaration : public CxxToken {};
class CxxExceptionSpecification : public CxxToken {};
class CxxExpressions : public CxxExpression {};
class CxxFileId : public CxxToken {};
class CxxFileIds : public CxxTokens {};
class CxxFileName : public CxxToken {};
class CxxFloatingLiteral : public CxxToken {};
class CxxFunctionBody : public CxxStatement {};
class CxxFunctionDeclarations : public CxxDeclarations {};
class CxxHandler : public CxxToken {};
class CxxHandlers : public CxxTokens {};
class CxxIdentifier : public CxxName {};
//class CxxIds : public CxxTokens {};
class CxxInitializerClause : public CxxExpression {};
class CxxInitializerClauses : public CxxInitializerClause {};
class CxxIntegerLiteral : public CxxToken {};
class CxxLine : public CxxToken {};
//class CxxList : public CxxTokens {};
class CxxMemInitializer : public CxxToken {};
class CxxMemInitializers : public CxxTokens {};
class CxxMetaClass : public CxxStatement {};
class CxxMetaFunction : public CxxMetaObject {};
class CxxMetaInitializer : public CxxToken {};
class CxxMetaInitializers : public CxxTokens {};
class CxxMetaParameter : public CxxToken {};
class CxxMetaParameters : public CxxTokens {};
//class CxxMetaPrototype : public CxxToken {};
//class CxxMetaPrototypes : public CxxTokens {};
class CxxMetaType : public CxxName {};
class CxxMetaVariable : public CxxMetaObject {};
class CxxNamespace : public CxxToken {};
typedef CxxExpression CxxNewExpression;
class CxxNumberLiteral : public CxxExpression {};
class CxxParameter : public CxxExpression {};
class CxxParameters : public CxxExpression {};
class CxxParenthesised : public CxxToken {};
class CxxPointerDeclarator : public CxxDeclarator {};
class CxxPosition : public CxxName {};
class CxxSegment : public CxxName {};
class CxxSpacing : public CxxToken {};
class CxxStrings : public CxxToken {};
typedef CxxStrings CxxStringLiteral;
class CxxSubspace : public CxxToken {};
class CxxSyntaxMacroParameter : public CxxToken {};
class CxxSyntaxMacroParameters : public CxxTokens {};
class CxxTemplateArgument : public CxxToken {};
class CxxTemplateArguments : public CxxTokens {};
class CxxTemplateParameter : public CxxToken {};
class CxxTemplateParameters : public CxxTokens {};
class CxxSimpleTypeParameter : public CxxTemplateParameter {};
class CxxTemplatedTypeParameter : public CxxTemplateParameter {};
class CxxTokenStatements : public CxxTokens {};
class CxxTreeArguments : public CxxTokens {};
class CxxType1Parameters : public CxxTokens {};
class CxxTypeId : public CxxToken {};
class CxxTypeIds : public CxxTokens {};
class CxxUtility : public CxxToken {};

#define FOGPARSERVALUE_ENUM(T,N) \
	const T *name2(u_,N); \
	const T& N() const { return *name2(u_,N); } \
	const T* & N() { return name2(u_,N); }
#define FOGPARSERVALUE_POINTER(T,N) T *N;
#define FOGPARSERVALUE_VALUE(T,N) T N;

union CxxTokenType
{
		CxxToken *_token;

		FOGPARSERVALUE_VALUE(bool, _bool)
		FOGPARSERVALUE_VALUE(long, _long)
		FOGPARSERVALUE_POINTER(CxxBrace, brace)
		FOGPARSERVALUE_POINTER(CxxSpacing, spacing)

		FOGPARSERVALUE_POINTER(CxxAccessSpecifier, access_specifier)
		FOGPARSERVALUE_POINTER(CxxBaseSpecifier, base_specifier)
		FOGPARSERVALUE_POINTER(CxxBaseSpecifiers, base_specifiers)
		FOGPARSERVALUE_POINTER(CxxBuiltInId, built_in_id)
		FOGPARSERVALUE_POINTER(CxxCharacterLiteral, character_literal)
		FOGPARSERVALUE_POINTER(CxxClass, _class)
		FOGPARSERVALUE_POINTER(CxxClassKey, class_key)
		FOGPARSERVALUE_POINTER(CxxCondition, condition)
		FOGPARSERVALUE_POINTER(CxxCvQualifiers, cv_qualifiers)
		FOGPARSERVALUE_POINTER(CxxDeclSpecifierId, decl_specifier_id)
		FOGPARSERVALUE_POINTER(CxxDeclaration, declaration)
		FOGPARSERVALUE_POINTER(CxxDeclarations, declarations)
		FOGPARSERVALUE_POINTER(CxxDeclarator, declarator)
		FOGPARSERVALUE_POINTER(CxxDeleteExpression, delete_expression)
		FOGPARSERVALUE_POINTER(CxxEnumerator, enumerator)
		FOGPARSERVALUE_POINTER(CxxEnumerators, enumerators)
		FOGPARSERVALUE_POINTER(CxxExceptionDeclaration, exception_declaration)
		FOGPARSERVALUE_POINTER(CxxExceptionSpecification, exception_specification)
		FOGPARSERVALUE_POINTER(CxxExpression, expression)
		FOGPARSERVALUE_POINTER(CxxExpressions, expressions)
		FOGPARSERVALUE_POINTER(CxxFileId, file_id)
		FOGPARSERVALUE_POINTER(CxxFileIds, file_ids)
		FOGPARSERVALUE_POINTER(CxxFileName, file_name)
		FOGPARSERVALUE_POINTER(CxxFloatingLiteral, floating_literal)
		FOGPARSERVALUE_POINTER(CxxFunctionBody, function_body)
		FOGPARSERVALUE_POINTER(CxxHandler, handler)
		FOGPARSERVALUE_POINTER(CxxHandlers, handlers)
		FOGPARSERVALUE_POINTER(CxxIdentifier, identifier)
		FOGPARSERVALUE_POINTER(CxxInitializerClause, initializer_clause)
		FOGPARSERVALUE_POINTER(CxxInitializerClauses, initializer_clauses)
		FOGPARSERVALUE_POINTER(CxxIntegerLiteral, integer_literal)
		FOGPARSERVALUE_POINTER(CxxKeyword, keyword)
		FOGPARSERVALUE_POINTER(CxxLine, line)
		FOGPARSERVALUE_POINTER(CxxMemInitializer, mem_initializer)
		FOGPARSERVALUE_POINTER(CxxMemInitializers, mem_initializers)
		FOGPARSERVALUE_POINTER(CxxMemberDeclarations, member_declarations)
		FOGPARSERVALUE_POINTER(CxxMetaClass, meta_class)
		FOGPARSERVALUE_POINTER(CxxMetaFunction, meta_function)
		FOGPARSERVALUE_POINTER(CxxMetaInitializer, meta_initializer)
		FOGPARSERVALUE_POINTER(CxxMetaInitializers, meta_initializers)
		FOGPARSERVALUE_POINTER(CxxMetaObject, meta_object)
		FOGPARSERVALUE_POINTER(CxxMetaStatement, meta_statement)
		FOGPARSERVALUE_POINTER(CxxMetaType, meta_type)
		FOGPARSERVALUE_POINTER(CxxMetaVariable, meta_variable)
		FOGPARSERVALUE_POINTER(CxxName, name)
		FOGPARSERVALUE_POINTER(CxxNewExpression, new_expression)
		FOGPARSERVALUE_POINTER(CxxNumberLiteral, number_literal)
		FOGPARSERVALUE_POINTER(CxxParameter, parameter)
		FOGPARSERVALUE_POINTER(CxxParameters, parameters)
		FOGPARSERVALUE_POINTER(CxxParenthesised, parenthesised)
		FOGPARSERVALUE_POINTER(CxxPointerDeclarator, pointer_declarator)
		FOGPARSERVALUE_POINTER(CxxPosition, position)
		FOGPARSERVALUE_POINTER(CxxSegment, segment)
		FOGPARSERVALUE_POINTER(CxxSimpleTypeParameter, simple_type_parameter)
		FOGPARSERVALUE_POINTER(CxxStatement, statement)
		FOGPARSERVALUE_POINTER(CxxStatements, statements)
		FOGPARSERVALUE_POINTER(CxxStringLiteral, string_literal)
		FOGPARSERVALUE_POINTER(CxxStrings, strings)
		FOGPARSERVALUE_POINTER(CxxSubspace, subspace)
		FOGPARSERVALUE_POINTER(CxxSyntaxMacroParameter, syntax_macro_parameter)
		FOGPARSERVALUE_POINTER(CxxSyntaxMacroParameters, syntax_macro_parameters)
		FOGPARSERVALUE_POINTER(CxxTemplateArgument, template_argument)
		FOGPARSERVALUE_POINTER(CxxTemplateArguments, template_arguments)
		FOGPARSERVALUE_POINTER(CxxTemplateParameter, template_parameter)
		FOGPARSERVALUE_POINTER(CxxTemplateParameters, template_parameters)
		FOGPARSERVALUE_POINTER(CxxTemplatedTypeParameter, templated_type_parameter)
		FOGPARSERVALUE_POINTER(CxxToken, token)
		FOGPARSERVALUE_POINTER(CxxTokenStatements, token_statements)
		FOGPARSERVALUE_POINTER(CxxTokens, tokens)
		FOGPARSERVALUE_POINTER(CxxTreeArgument, tree_argument)
		FOGPARSERVALUE_POINTER(CxxTreeArguments, tree_arguments)
		FOGPARSERVALUE_POINTER(CxxTreeExpression, tree_expression)
		FOGPARSERVALUE_POINTER(CxxType1Parameters, type1_parameters)
		FOGPARSERVALUE_POINTER(CxxUtility, utility)

		FOGPARSERVALUE_VALUE(int, bang)
		FOGPARSERVALUE_VALUE(CxxIsTemplate, is_template)
		FOGPARSERVALUE_VALUE(YACC_MARK_TYPE, mark)
		FOGPARSERVALUE_VALUE(size_t, nest)
};

#endif
