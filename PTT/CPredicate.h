#if !defined CPREDICATE
#define CPREDICATE

#include <string>
#include <ctype.h>
#include  <deque>
#include <list>
#include <algorithm>

#include "CParameter.h"
#include "CTypes.h"
//#include "CAction.h"

using namespace std;

extern bool sh_arg_str_eq(sh_arg_str s1, sh_arg_str s2);
extern bool sh_arg_str_cmp(sh_arg_str s1, sh_arg_str s2);

typedef class CPredicateList;

class CPredicate  
{
public:
	void SetEntanglement(bool);
	void SetInnerEntanglement(short);
	bool IsEntangled();
	short GetInnerEntanglement();
	bool Subsume(CPredicate*, CTypes*, bool oneway=false);
	bool IsStatic();
	void SetStatic(bool);
	bool IsIndirect();
	void SetIndirect(bool);
	CPredicate* Clone();
	CParameter* GetPars();
	string GetName();
	int EqualGround(CPredicate*);
	int GetAttribute();
	void SetAttribute(int);
	CPredicate* Instantiate(CParameter *, CParameter*);
	bool Negative();
	void Negate();
	string GetPrologRepresentationVars();
	list<string> GetPrologRepresentationTypes();
	CPredicate();
	CPredicate(string,CParameter*);
	~CPredicate();
	

private:
	bool entangled; //in preconditions - by init, in effects - by goal
	short inner_entangled; //in precondition - previously, in effect - successively
	bool stat;
	int d;
	bool neg;
	CParameter* params;
	string name;
	void GetAllTypesCombinations(list<string>*, int , string);
public:
	bool Equal(CPredicate* p, CParameter *a1_pars, CParameter *a2_pars, vector<sh_arg_str>& sh_args, bool inc = true);
	void ReplacePars(list<par_repl_str>& par_repl);
	string ToString(bool types);
	string ToStringSpc();
	void SetName(string name);
	bool InstanceOf(CPredicate* p, CTypes* t);
	CPredicateList* MakeAllInstances(CTypes* obejcts, CTypes* types);
private:
	void MakeAllInstancesRec(vector<deque<string>*>* obj_list, CParameter* part_pars, int arg_no, CPredicateList* ret);
protected:
	bool indirect;
};


class CPredicateList  
{
public:
	int Find(CPredicate*);
	int FindProperPredicate(string);
	bool Subsume(CPredicate*,CTypes*);
	bool Disjunct(CPredicateList*);
	bool DisjunctU(CPredicateList*,CTypes*);
	CPredicateList* Clone() const;
	void ApplyEffects(CPredicateList*, int, bool noneg=true);
	bool PrecondFulfilled(CPredicateList*, list<int>&);
	bool BackwardStep(CPredicateList*, list<int>&);
	CPredicateList* Instantiate(CParameter*, CParameter*);
	int Count();
	list<string>* GetPrologRepresentation();
	void AddRecord(CPredicate*);
	CPredicateList();
	~CPredicateList();
	CPredicate* operator[](unsigned int);

private:
	deque<CPredicate*>* ppredlist;
public:
	bool Equal(CPredicateList* pl, CParameter* a1_pars, CParameter* a2_pars, vector<sh_arg_str>& sh_args, bool subset);
	void ReplacePars(list<par_repl_str> &par_repl);
	CPredicateList& operator-(CPredicateList& pl);
	CPredicateList& operator+(CPredicateList& pl);
	CPredicateList& operator*(CPredicateList& pl);
	//inline CPredicateList& operator=(const CPredicateList& pl) {return *pl.Clone();};
	void RemovePredicate(int i);
	string ToString(bool types);
	CPredicate* SubsumeP(CPredicate* p, CTypes* t);
	CPredicateList* Intersect(CPredicateList* p);
};

#endif