// CProblem.h: interface for the CProblem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CPROBLEM_H__393C4F54_6D2A_4A58_8C22_AFB58288E5E1__INCLUDED_)
#define AFX_CPROBLEM_H__393C4F54_6D2A_4A58_8C22_AFB58288E5E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <set>
#include <ctype.h>
#include <fstream>


#include "CTypes.h"
#include "CPredicate.h"
#include "CPlan.h"
#include "CDomain.h"

struct ComponentType{
set<string> consts;
CPredicateList *preds;
ComponentType(){preds=new CPredicateList();}
ComponentType & operator=(const ComponentType &ct){
	ComponentType ret;
	ret.consts=ct.consts;
	ret.preds=ct.preds->Clone();
	return ret;
}
};

struct xcomp {
  bool operator() (const pair<string,int>& lhs, const pair<string,int>& rhs) const
  {return lhs.second<rhs.second;}
};

class CProblem  
{
public:
	CPlan* GetPlan();
	CPredicateList* GetGoal();
	CPredicateList* GetInit();
	string GetOrigFileName();
	void SetOrigFileName(string);
	void AssignPlan(CPlan*);
	void SetGoal(CPredicateList*);
	void SetInit(CPredicateList*);
	void GeneratePrologFile();
	void SetObjects(CTypes*);
	inline CTypes* GetObjects(){return objects;}
	string GetName();
	void SetName(string);
	CProblem();
	~CProblem();

private:
	string origfilename;
	CPlan* plan;
	list<string>*	GetState(bool);
	CPredicateList* goal;
	CPredicateList* init;
	CTypes *objects, *prob_objects;
	string name;
	map<string,int> objmap;
	map<string,int> predcounts_init, predcounts_goal;
	list<ComponentType> AC;
	set<pair<string,int>,xcomp> basicTypes;
	map<string,string> obj_type_map;
public:
	void ProblemToPDDL(ostream& s, string dom_name);
	void AssignTypes(void);
	void ReformulateByEntangelments(CPredicateList* ent_init, CPredicateList* ent_goal, CTypes* types);
	void ReformulateByInnerEntanglements(CPredicateList* ent_succ,CTypes *types);
	void AssignDomain(CDomain* dom);
	void DetermineAllInstancesPredicates();
	int  NumberOfPredicates(CPredicate* p,bool init);
	int  ObjectsUpperBound(CPredicate *p);
	vector<string> allinstpreds_init, allinstpreds_goal;
private:
	CDomain* dom;
	CPredicate* static_graph;
	map<string,CPredicateList*> types_to_predicate_map;
	static list<ComponentType> globalAC;
	void NumberOfPredicatesRec(vector< deque <string>* > *t,int arg_no,string part,int *ret,bool init);
public:
	void BuildStaticGraph(void);
	void BuildAbstractComponents(void);
	// debug reasons
	void OutputAC(ostream& s);
	// after building ACs for every training problem
	static void GetGlobalAC(deque<CProblem*>* training);
	static bool CheckLocalityRule(CAction* a,CTypes*);
};

#endif // !defined(AFX_CPROBLEM_H__393C4F54_6D2A_4A58_8C22_AFB58288E5E1__INCLUDED_)
