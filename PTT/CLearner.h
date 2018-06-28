// CLearner.h: interface for the CLearner class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLEARNER_H__ABA4C519_E1EE_4B6E_AEE3_B6FA6AF8B8D4__INCLUDED_)
#define AFX_CLEARNER_H__ABA4C519_E1EE_4B6E_AEE3_B6FA6AF8B8D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdlib.h>
#include "CPlan.h"	// Added by ClassView
#include "CAction.h"	// Added by ClassView
#include "CDependency.h"

#define FLAW_RATIO data.flawratio
#define OP_LIMIT data.oplimit
#define PAR_A data.par_a //how frequent are particular operators' instances consecutive
#define PAR_B data.par_b //how many pairs of actions will be assembled w.r.t plans length
#define PAR_C data.par_c //how many argument can macros have

#define NOT_CONSIDERED 0 //this must be zero !
#define PREC_CONSIDERED 1
#define SUCC_CONSIDERED 2
#define MACRO_PREC 4
#define MACRO_SUCC 8

#define CCC 0.4

struct pair_str {
int prob, first,second;
};


struct field_of_matrix {
int counter;  //number of pairs 
int score; //for raking purposes
// 0 - first act
// 1 - second act
vector<sh_arg_str> *shared_args;
// 0 - problem (index)
// 1 - first act
// 2 - second act
list<pair_str> *pairs;  
unsigned char* unused_acts; //for temporary reasons
};

struct ent_str {
	bool skip;
	int fail, total;
};

struct in_ent_str {
	int total;
	int *op_counts;
};

class CLearner  
{
public:
	void LearnEntanglements();
	void CreateMatrixOfCandidates();
	bool CreateDependencies();
	CLearner();
	virtual ~CLearner();

private:
	void ClearEntStr();
	void OutputMatrix(ostream &);
	int* frequency;
	bool OptimizePlan(CPlan*, CDependency*,ostream &);
	deque<CDependency*> dependencies;
	field_of_matrix  *matrix;
	int totalplanslength;
	vector<vector<ent_str>*> ent_data;
	vector<vector<in_ent_str>*> inner_ent_data;
	
	void MakeAchieverClasses(vector<achievers>*,vector<vector<int> >*);
	//void MakeAchieverSubsumedClasses(vector<achievers>*,vector<vector<int> >*);
	void FilterAchievers(vector<achievers>*,vector<achievers>*);
	void SubsumeAchievers(vector<achievers>*,vector<achievers>*);
	bool CompatibleClasses(vector< achievers >*, vector<int> *,vector<int> *);
	int WhichClass(int prob_no, vector<vector<int> > *classes);
	void CombineCompatibleClasses(vector< achievers >*, vector<vector<int> >*); //notice that the class will be modified
public:
	bool OptimizePlans(void);

	void FindInverseActions(CPlan* pi, CDependency* dep, list<int> &unnecessaty_acts);
	void FindShrinkableActions(CPlan* pi, CDependency* dep, list<int> &unnecessaty_acts, list<MacroReplStr> &replacable_acts);
	void Initialize(short par = 0);
	void LearnInnerEntanglements(void);
	void ClearInnerEntStr(void);
	void ApplyEntanglements(bool mcronly = false, bool actupd =true);
	// must be called after LearnInnerEntanglements()
	void ApplyInnerEntanglements(void);
	bool MarkUnnecessaryInverseActions(CPlan* pi, CDependency* dep, deque<inv_pair_str>& inverses, list<int>& unnecessary_acts);
	void MarkUnnecessaryNestedInverseActions(CPlan* pi, CDependency* dep, deque<inv_pair_str>& inverses, list<int>& unnecessary_acts);
	void GenerateMacrosFromInnerEntanglements(short *carry=NULL);
	bool IsSameAchiever(CAction* first, CAction* second, CPredicate* p);
	bool IsOneToOne(CAction* first, CAction* second);
	void OutputEnt(std::ostream& s);
	void OutputIntEnt(std::ostream& s);
	// Learrns macros according to the KER paper
	void LearnMacros(void);
	void LearnMacrosFromCA(void);
	
	void GuessEntanglements();
	void GuessMacros();
	
	void EliminateUselessMacros();
	
	void EliminateInfrequentMacros(int freq);

        void LearnMacrosFromFlips(bool no_im_args = false);
	void LearnMacrosFromAbsorbtions();
	void LearnMacrosFromStaticPreconditions();
	
	void CreateStats();
	void OutputAchievers(vector<achievers> &achs,ostream &s);
	
};



#endif // !defined(AFX_CLEARNER_H__ABA4C519_E1EE_4B6E_AEE3_B6FA6AF8B8D4__INCLUDED_)
