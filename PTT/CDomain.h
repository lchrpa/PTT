#ifndef CDOMAIN
#define CDOMAIN

#include <string>
#include <ctype.h>
#include <fstream>
#include <ostream>
#include <algorithm>
#include <set>

#include "CTypes.h"
#include "CPredicate.h"
#include "CAction.h"


//#include "CDomain.h"

using namespace std;

typedef class CAction;
typedef class CActionList;

struct inv_matrix_field {

// 0 - first act
// 1 - second act
vector<sh_arg_str> *shared_args;

bool inv;

};

struct repl_matrix_field {

CAction *a;

// 0 - first act
// 1 - second act
vector<sh_arg_str> *shared_args;

bool repl;

};

struct flipping_data {
  CPredicate *p1,*p2;
  CActionList *starting;
  CActionList *finishing;
  
  vector<sh_arg_str> *shared_args;
};

typedef pair<string, vector<short> > act;
typedef pair<act*,vector<act*> > mcr;


class CDomain{
	
private:
	string origfilename;
	CPredicateList* ppreds;
	CPredicateList* pfunct;
	fstream knowledge_file;
	string name;
	CTypes*  ptypes;
	CActionList* pacts;
	inv_matrix_field *inv_matrix;
	repl_matrix_field *repl_matrix;
	vector<pair<short,vector<sh_arg_str>* > > incompatible_preds;
	vector<flipping_data> flipping;
	vector<flipping_data>::iterator flipping_iter;
	vector<vector<sh_arg_str>* > symmetric_args;
	void MergePredTypes(CPredicateList*);
	bool action_cost;
	int initial_inv_count,initial_pred_count;
	CTypes* constants;

public:
	void OutStatic(ostream &);
	void IdentifyStaticPredicates();
	CActionList* GetActions();
	string GetOrigFileName();
	void SetOrigFileName(string);
	void SetActions(CActionList*);
	void GeneratePrologFile();
	CPredicateList* GetPredicates();
	void SetPredicates(CPredicateList*);
	void SetFunctions(CPredicateList*);
	void SetConstants(CTypes*);
	CTypes* GetConstannts();
	CDomain();
	~CDomain();
	void SetName(char*);
	string GetName();
	void SetTypes(CTypes*);
	CTypes* GetTypes();
	inline void SetActionCost(bool x){action_cost=x;}
	inline bool GetActionCost(){return action_cost;}
	void MakeInvMatrix(void);
	void OutInv(ostream &s);
	void OutInc(ostream &s);
	void OutRepl(ostream &s);
	bool AreInverse(CAction* a1, CAction* a2);
	bool AreInverse(string a1, string a2,vector<sh_arg_str> &sh_arg);
	void InitializeReplMatrix(void);
	bool ReplacableBy(CAction* a1, CAction* a2, vector<sh_arg_str>& sh_args, repl_matrix_field &ret);
	void IdentifyIncomaptiblePredicates();
	void OutIncompatiblePreds(ostream &s);
	bool AreCompatible(CPredicate *p1,CPredicate *p2);
	void GetFlippingData();
	inline flipping_data* NextFlippingItem(){if (flipping_iter!=flipping.end()) return &(*(flipping_iter++)); else return NULL;}
	inline void ResetFlippingItem(){flipping_iter=flipping.begin();}
	void OutFlippingData(ostream &s);
	void GeneratePDDL(ostream& s);
	void ReformulateByEntanglements(CPredicateList* ent_init, CPredicateList* ent_goal);
	void ReformulateByInnerEntanglements(CPredicateList* ent_prev, CPredicateList* ent_succ);
	void ImportMacros(list<mcr*> *macros);
	void ReconstructEntanglements();
	void DetermineSymmetricArgs();
};


#endif