#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <string>
#include <iostream>
#include <ctype.h>
#include <list>
#include <vector>
#include <deque>
#include <set>

#include "CDomain.h"
#include "CProblem.h"
#include "CTypes.h"
#include "CParameter.h"
#include "CPredicate.h"
#include "CAction.h"
#include "CPlan.h"

using namespace std;

enum RequirementEnum{
	strips,
	adl,
	typing,
	act_costs,
	negativepreconditions,
	disjunctivepreconditions,
	equality,
	existentialpreconditions,
	universalpreconditions,
	quantifiedpreconditions,
	conditionaleffects,
	fluents, //=numericfluents+objectfluents
	numericfluents,
	objectfluents,
	durativeactions,
	derivedpredicates,
	timedinitialliterals,
	preferences,
	constraints
};

enum OperatorOutputToAsEnum{
	ot_default, //Type doesn't important
	ot_condition, //Output as a condition '=' is 'equal'
	ot_effect //Output as an effect '=' is 'assign'
};

const string TYPE_INT = "int";
const string TYPE_BOOL = "bool";
const string DEFAULT_FUNCTIONTYPE = TYPE_INT;
const string OP_ASSIGN = "assign";
const string OP_INCREASE = "increase";
const string OP_DECREASE = "decrease";
const string REL_LESSOREQUAL = "lessorequal";
const string REL_GREATEROREQUAL = "greaterorequal";
const string REL_LESS = "less";
const string REL_GREATER = "greater";
const string REL_EQUAL = "equal";
const string REL_ADD = "add";		
const string REL_SUBSTRACT = "substract";		
const string REL_MULTIPLY = "multiply";		
const string REL_DIVIDE = "divide";		

struct PTT_Data{ //contains planning domain, all training planning problems and all testing planning problems 
	CDomain *pdom;
	deque<CProblem*> *train;
	deque<CProblem*> *pprob;
	list<string> *plannercmd;
	float flawratio;
	int oplimit;
	bool inent_filter;
	bool optimization;
	bool goal,inv,grinv,repl;
	bool entanglements,macros;
	int macroentanglements;
	bool init, prec, succ;
	float par_a,par_b;
	int par_c;
	bool stats;
};


#endif
