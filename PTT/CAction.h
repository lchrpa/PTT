// CAction.h: interface for the CAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CACTION_H__E0CF8871_5248_4893_AAFE_C70EA474EA33__INCLUDED_)
#define AFX_CACTION_H__E0CF8871_5248_4893_AAFE_C70EA474EA33__INCLUDED_

/*
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
*/
#include <stdlib.h>
#include <iostream>
#include "CParameter.h"
#include "CPredicate.h"
#include "CDomain.h"
//#include "CProblem.h"
//#include "CLearner.h"

using namespace std;

typedef class CLearner;
typedef class CProblem;
//typedef class CTypes;
typedef class CActionList;
typedef class CDomain;

class CAction  
{
public:
	CParameter* GetParams();
	void DetectSharedArgs(CAction*,vector<sh_arg_str>&);
	CPredicateList* GetNegEff();
	CPredicateList* GetPosEff();
	CPredicateList* GetEff();
	CPredicateList* GetPrec();
	CAction* Instantiate(CParameter*);
	string GetActName();
	list<string>* GetPrologRepresentation();
	CAction();
	CAction(string, CParameter*, CPredicateList*, CPredicateList*);
	~CAction();

protected:
	CPredicateList* negEffects;
	CPredicateList* posEffects;
	string ParamPrologList();
	CParameter* params;
	string name;
	CPredicateList* effects;
	CPredicateList* precondition;
	vector<list<int> > components;
	int cost;
	string cost_s;
public:
	bool InverseOps(CAction* o, vector<sh_arg_str>& sh_args);
	bool ReplacableBy(CAction* o, vector<sh_arg_str>& sh_args);
	bool Equal(CAction *a);
	virtual string ToString(void);
	virtual inline bool isMacro(){return false;}
	void MakeIndirectEffects(void);
	virtual inline CAction* FindPrimitiveForPredicate(CPredicate* p){return this;}
	// static predicates considered, entanglements optionally considered
	CActionList* MakeAllInstances(CTypes* types, CProblem* prob, bool ents);
	virtual inline int ActCount(void) {return 1;}
	inline int GetActionCost(){return cost;}
	bool AchieverFor(CAction *a, CTypes *t, vector<vector<sh_arg_str> >& sh_args_vec,bool stat = true);
	bool IndependentWith(CAction *a); //works for grounded actions (planning graph independence)
	inline bool AchieverForGnd(CAction *a){return !posEffects->Disjunct(a->GetPrec());} //works for grounded actions
protected:
	void MakeAllInstancesRec(vector<deque<string>*>* obj_list, CParameter* par_parts, int arg_no, CProblem* prob, bool ents, CActionList* ret);
	bool CheckInstance(CProblem* prob, bool ents);
	void AchieverForRec(vector<vector<sh_arg_str> >& sh_args_vec, CAction *a, CPredicateList *l1, CPredicateList *l2, int nth,int pos);
public:
	bool* stat_conn;
	void GenerateStatGraph(void);
	void GenerateComponents(void);
	int ComponentCount(void);
	bool HasMeaningfulInitEnt(void);
	bool HasMeaningfulGoalEnt(void);
	bool HasConnectedEnts();
};


class CActionList  
{
public:
	int Count();
	list<string>* GetPrologRepresentation();
	void AddRecord(CAction*);
	CActionList();
	 ~CActionList();
	CAction* FindProperAction(string name,int &);
	CAction* operator[](unsigned int);

private:
	deque<CAction*>* pactlist;
public:
	string ToString(void);
	void MakeIndirectEffects(void);
	void Replace(CAction* a, int pos);
	void Remove(int pos);
};

class CMacroAction : public CAction {
public:
	CMacroAction(CAction *a1,CAction *a2, vector<sh_arg_str> &sh_args, CTypes *types=NULL);
	~CMacroAction();
protected:
	CAction *first, *second;
	vector<sh_arg_str> sh_args;
	CPredicateList* MS;
	CPredicateList* CL;
	bool threat;
public:
	bool VerifyValidity(CActionList* acts, CTypes* types, CLearner*);
	virtual inline bool isMacro(){return true;}
	virtual CAction* FindPrimitiveForPredicate(CPredicate* p);
	virtual int ActCount(void);
	vector<pair<string,vector<short> > >* Unfold(vector<short>*,bool init=false);
	void DetermineInequalityConstraint();
protected:
	bool uninformative;
private:
	void Initialize(CTypes*);
	vector<string>* LinearizeActNames(void);
	vector<CAction*>* LinearizeActs(void);
public:
	bool IsUninformative(void);
	inline bool IsThreat(){return threat;}
	int StatGraphMatch(void);
	bool StatGraphConnect(void);
	bool IsRepetitive(void);
	void FindActionPositions(CActionList* acts, int& i, int& j);
	bool TestUsefulness(CDomain *dom);
};

#endif // !defined(AFX_CACTION_H__E0CF8871_5248_4893_AAFE_C70EA474EA33__INCLUDED_)
