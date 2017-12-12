%{
#include <stdlib.h>
#include <stdio.h>
//#include <iostream>
//#include <list>
#include "common.h"
//#include "pddlfile.h"
#include "pddlconvertor.tab.hpp"

extern "C"
{
	enum RequirementEnum;
}

%}


%option noyywrap

D                       [0-9]
L                       [a-zA-Z]
NL								[^a-z^A-Z]

%%
define						{return DEFINE;}
domain						{return DOMAIN;}
problem						{return PROBLEM;}
either						{return EITHER;}
:domain						{return DDOMAIN;}
:action						{return ACTION;}
:parameters					{return PARAMETERS;}
:precondition				{return PRECONDITION;}
:effect						{return EFFECT;}
:predicates					{return PREDICATES;}
:types						{return TYPES;}
:requirements				{return REQUIREMENTS;}
:functions					{return FUNCTIONS;}
:constants					{return CONSTANTS;}

:strips							{yylval.integer=strips; 						return REQUIREMENT;}
:typing							{yylval.integer=typing; 						return REQUIREMENT;}
:action-costs							{yylval.integer=act_costs; 						return REQUIREMENT;}
:negative-preconditions 	{yylval.integer=negativepreconditions; 	return REQUIREMENT;}
:disjunctive-preconditions	{yylval.integer=disjunctivepreconditions; return REQUIREMENT;}
:equality 						{yylval.integer=equality; 						return REQUIREMENT;}
:existential-preconditions {yylval.integer=existentialpreconditions; return REQUIREMENT;}
:universal-preconditions 	{yylval.integer=universalpreconditions; 	return REQUIREMENT;}
:quantified-preconditions	{yylval.integer=quantifiedpreconditions; 	return REQUIREMENT;}
:conditional-effects 		{yylval.integer=conditionaleffects; 		return REQUIREMENT;}
:fluents 						{yylval.integer=fluents; 						return REQUIREMENT;}
:numeric-fluents				{yylval.integer=numericfluents; 				return REQUIREMENT;}
:object-fluents				{yylval.integer=objectfluents; 				return REQUIREMENT;}
:adl								{yylval.integer=adl; 							return REQUIREMENT;}
:durative-actions 			{yylval.integer=durativeactions; 			return REQUIREMENT;}
:derived-predicates			{yylval.integer=derivedpredicates; 			return REQUIREMENT;}
:timed-initial-literals 	{yylval.integer=timedinitialliterals; 		return REQUIREMENT;}
:preferences					{yylval.integer=preferences; 					return REQUIREMENT;}
:constraints					{yylval.integer=constraints; 					return REQUIREMENT;}

:objects													{return OBJECTS;}
:init														{return INIT;}
:goal														{return GOAL;}
:metric													{return METRIC;}
and														{return AND;}
not														{return NOT;}
total-cost	{return TOTAL_COST;}
number		{return NUMBER_TYPE;}
increase	{return INCREASE;}
minimize	{return MINIMIZE;}
{L}({L}|{D}|\-|\_)*|\<\=|\>\=|\<|\>|\=|\+		{strcpy(yylval.str, yytext); return IDENTIFIER;}
[\?]{L}({L}|{D}|\-|\_)*								{strcpy(yylval.str, yytext); return VARIABLE;}
\-{D}+|{D}+														{strcpy(yylval.str, yytext); return NUMBER;}
[ \t\n\r]+													/*ignore white chars */
;.*$														/*ignore comments */
.															{return *yytext; }

%%