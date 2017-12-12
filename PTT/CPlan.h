// CPlan.h: interface for the CPlan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CPLAN_H__67AF510E_DB98_4AF2_9784_E47DFCF70689__INCLUDED_)
#define AFX_CPLAN_H__67AF510E_DB98_4AF2_9784_E47DFCF70689__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include  <deque>
#include  <vector>
//#include <forward_list>
#include <fstream>
#include <ostream>
#include <algorithm>

#include "CParameter.h"
#include "CAction.h"

#define ACT_NEW 1
#define ACT_REMOVE 2
#define ACT_BEING_REPLACED 4

struct PlanRecord{
	//CAction *act;
	string actname;
	unsigned int pos;
	CParameter *params;
};

struct PlanModifer{
	CAction *a;
	unsigned char mod;
	unsigned int pos;
};

struct MacroReplStr{
	int i,j; //boundary actions
	deque<int> *move_pred; //intermediate actions moved before the first boundary action
	CAction *a; //macroaction
};


class CPlan  
{
public:
	void RemoveAction(int);
	int Length();
	void InstantiateActions(CActionList*);
	list<string>* GetPrologRepresentation(string);
	void ParsePlanFile(string);
	CPlan();
	virtual ~CPlan();
	CAction* operator[](unsigned int);

private:
	deque<PlanRecord>* plan;
	vector<CAction*>* act_plan;
public:
	bool ModifyPlan(list<int>& unnecessary_acts, list<MacroReplStr>& repl_acts, CPredicateList *init=NULL, CPredicateList *goal=NULL);
	void GeneratePDDLPlan(ostream& s);
};

#endif // !defined(AFX_CPLAN_H__67AF510E_DB98_4AF2_9784_E47DFCF70689__INCLUDED_)
