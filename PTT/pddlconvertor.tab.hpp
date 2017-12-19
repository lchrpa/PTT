/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_PDDLCONVERTOR_TAB_HPP_INCLUDED
# define YY_YY_PDDLCONVERTOR_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    DEFINE = 258,
    DOMAIN = 259,
    DDOMAIN = 260,
    PROBLEM = 261,
    ACTION = 262,
    IDENTIFIER = 263,
    VARIABLE = 264,
    PARAMETERS = 265,
    PRECONDITION = 266,
    EFFECT = 267,
    AND = 268,
    NOT = 269,
    PREDICATES = 270,
    TYPES = 271,
    REQUIREMENTS = 272,
    REQUIREMENT = 273,
    TOTAL_COST = 274,
    NUMBER_TYPE = 275,
    INCREASE = 276,
    OBJECTS = 277,
    INIT = 278,
    GOAL = 279,
    EITHER = 280,
    FUNCTIONS = 281,
    CONSTANTS = 282,
    NUMBER = 283,
    METRIC = 284,
    MINIMIZE = 285
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 23 "pddlconvertor.ypp" /* yacc.c:1909  */

//	RequirementEnum requirement;
	char str[255];
	void* pointer;
	int integer;

	/*if here comes during compilaton error, that the type doesn't exist
	  than remember, that the include must be not only in this .y file,
	  but also in .l file.
	*/

	deque<string>* pstringlist;
	TypesRecord*	  tr;
	CTypes*		  ptypes;
	ParamRecord*	  pr;
	CParameter*		  pparams;
	CPredicate*		  ppred;
	CPredicateList*		  ppredlist;
	CAction*		  pact;
	CActionList*		  pactlist;


/*	ActionDef* pactiondef;
	Parameter* pparameter;
	Predicate* ppredicate;
	PredicateDef* ppredicatedef;
	TypedList* ptypedlist;
	Operator* poperator;
	OperatorDef* poperatordef;
	
	list<Parameter*>* pparameterlist;
	list<string>* pstringlist;
	list<Predicate*>* ppredicatelist;
	list<PredicateDef*>* ppredicatedeflist;
	list<RequirementEnum>* prequirementlist;
	list<ActionDef*>* pactiondeflist;
	list<TypedList*>* ptypedlistlist;
	list<Object*>* pobjectlist;
	list<Operator*>* poperatorlist;
	list<OperatorDefParam*>* poperatordefparamlist;
	list<OperatorVariable*>* poperatorvariablelist;
	list<TypeDef*>* ptypedeflist;*/

#line 129 "pddlconvertor.tab.hpp" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PDDLCONVERTOR_TAB_HPP_INCLUDED  */
