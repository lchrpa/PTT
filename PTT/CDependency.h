// CDependency.h: interface for the CDependency class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CDEPENDENCY_H__3E7CE40E_547F_45D9_93D7_66BBEBB200B6__INCLUDED_)
#define AFX_CDEPENDENCY_H__3E7CE40E_547F_45D9_93D7_66BBEBB200B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdlib.h>
#include "CPlan.h"	// Added by ClassView
#include "CAction.h"	// Added by ClassView
#include "CDomain.h"

#define ACT_STRDEP 1
#define ACT_DEP 2
#define ACT_INDEP 4
#define ACT_ASSEMBLEABLE 8

struct inv_pair_str {
	int first,second;
	bool operator<(const inv_pair_str one) const {
		if (first>one.first) return true;
		if (first==one.first) return second<one.second;
		return false;
	}
	bool operator==(const inv_pair_str one){
		return (first == one.first && second == one.second);
	}
};

struct achiever {
  vector<int> *acts;
  int freq;
};

typedef vector<achiever> achievers;

class CDependency  
{
public:
	bool IsAssembleable(int,int);
	deque<int>* GetMovePred(int,int);
	bool FindPairsForAssemblage();
	void InversePairs(list<int>&);
	void GoalNotDependent(list<int>&);
	bool IsIndep(int,int);
	bool CreateIndependencies();
	bool IsStrDep(int,int);
	bool IsDep(int,int);
	void OutPut(ostream&);
	bool CreateDependencies();
	CDependency(CPlan*,CPredicateList*,CPredicateList*);
	virtual ~CDependency();
	void RetrieveAchievers(vector<achievers>&, CActionList*);

private:
	bool CanBeAdjacent(int,int,deque<int>&);
	CActionList *acts;
	CPlan *plan;
	CPredicateList *init,*goal;
	int position(int,int);
	unsigned char* dep_field;
	vector<deque<int>*> *move_pred_matrix;
	vector<CPredicateList*> states;
public:
	bool FindPairsForAssemblage(bool inc_dep);
	bool CreateDependenciesWithOptimization(CDomain* dom, deque<inv_pair_str> &inverses);
	bool Equal(CDependency*);
	void Clear(void);
};

#endif // !defined(AFX_CDEPENDENCY_H__3E7CE40E_547F_45D9_93D7_66BBEBB200B6__INCLUDED_)
