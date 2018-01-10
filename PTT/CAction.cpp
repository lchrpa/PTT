// CAction.cpp: implementation of the CAction class.
//
//////////////////////////////////////////////////////////////////////

#include "CAction.h"
#include "CLearner.h"
//#include "CType.h"
#include "CProblem.h"

#include <strings.h>
#include <string.h>

extern string ToLower(string);
extern string ToUpper(string);
extern void revert_ah_arg_str(vector<sh_arg_str>&);
extern bool operator==(const sh_arg_str& lhs,const sh_arg_str& rhs);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

inline bool SizeSort(const vector<sh_arg_str> &lhs,const vector<sh_arg_str> &rhs){return rhs.size()<lhs.size();}

CAction::CAction()
{
}

CAction::CAction(string nam, CParameter *pars=NULL, CPredicateList *prec=NULL, CPredicateList *eff=NULL){
	
	int i,j,k;
	CPredicate *p;

	this->name=nam;
	this->params=pars;
	this->precondition=prec;
	this->effects=eff;
	this->negEffects=this->posEffects=NULL;
	this->stat_conn=new bool[pars->Count()*pars->Count()];
	memset(stat_conn,0,pars->Count()*pars->Count()*sizeof(bool));
	cost=0;

	for (i=0;i<this->precondition->Count();i++){
		p=(*this->precondition)[i];
		if (p->GetPars()!=NULL)
		for (j=0;j<p->GetPars()->Count();j++){	
			for (k=0;k<this->params->Count();k++){	
				if (!(*p->GetPars())[j]->name.empty()&&strcasecmp((*p->GetPars())[j]->name.data(),(*this->params)[k]->name.data())==0){
					(*p->GetPars())[j]->plisttypes=new deque<string>();
					*(*p->GetPars())[j]->plisttypes=*(*this->params)[k]->plisttypes;
					break;
				}
			}
		}
	}

	for (i=0;i<this->effects->Count();i++){
		p=(*this->effects)[i];
		if (p->GetName()=="total-cost"){
		    if (sscanf((*p->GetPars())[0]->constant.data(),"%i",&cost)!=1)  cost_s = (*p->GetPars())[0]->constant;
		    effects->RemovePredicate(i);
		    i--;
		    continue;
		}
		if (p->GetPars()!=NULL)
		for (j=0;j<p->GetPars()->Count();j++){	
			for (k=0;k<this->params->Count();k++){
				if (!(*p->GetPars())[j]->name.empty()&&strcasecmp((*p->GetPars())[j]->name.data(),(*this->params)[k]->name.data())==0){
					(*p->GetPars())[j]->plisttypes=new deque<string>();
					*(*p->GetPars())[j]->plisttypes=*(*this->params)[k]->plisttypes;
					break;
				}
			}
		}
	}
	
	
	//distribute positive and negative effects
	if (this->effects!=NULL){
	
		this->negEffects=new CPredicateList();
		this->posEffects=new CPredicateList();

		for (i=0;i<this->effects->Count();i++){
			if ((*this->effects)[i]->Negative()) {
				this->negEffects->AddRecord((*this->effects)[i]);
			} else {
				this->posEffects->AddRecord((*this->effects)[i]);
			}
		}
	}

}

CAction::~CAction()
{
	delete this->effects;
	delete this->precondition;
	delete this->params;
	delete this->stat_conn;
}

string CAction::ParamPrologList()
{
	string ret;
	ParamRecord *pr;
	int i,n;

	n=this->params->Count();
	ret+="[";
	for(i=0;i<n;i++){
		pr=(*this->params)[i];
		ret+=ToUpper(pr->name);
		if (i<n-1) ret+=","; else ret+="]";
	}

	return ret;
}

list<string>* CAction::GetPrologRepresentation()
{
	list<string> *ret=new list<string>();
	string s,name,prologpars;
	char a[256]; 
	int i,j,k,n,n2;
	ParamRecord *pr;

	/*
	name=ToLower(this->name);
	prologpars=this->ParamPrologList();
	//arity of operator
	n=this->params->Count();
	s="operator(";
	s+=name;
	s+=",";
	s+=itoa(n,a,10);
	s+=").";
	ret->push_back(s);
	
	//parameters description
	for(i=0;i<n;i++){
		pr=(*this->params)[i];
		n2=pr->plisttypes->size();
		for(j=0;j<n2;j++){
			s="parameter(";
			s+=name;
			s+=",";
			s+=itoa(i+1,a,10);
			s+=",";
			s+=ToLower((*pr->plisttypes)[j]);
			s+=").";
			ret->push_back(s);
		}
	}
	
	//precondition
	n=this->precondition->Count();
	for(i=0;i<n;i++){
		s="precondition(";
		s+=name;
		s+=",";
		s+=prologpars;
		s+=",";
		s+=(*this->precondition)[i]->GetPrologRepresentationVars();
		s+=").";
		ret->push_back(s);
	}
	//effects
	n=this->effects->Count();
	for(i=0;i<n;i++){
		if ((*this->effects)[i]->Negative()) s="effect_negative("; else s="effect_positive(";
		s+=name;
		s+=",";
		s+=prologpars;
		s+=",";
		s+=(*this->effects)[i]->GetPrologRepresentationVars();
		s+=").";
		ret->push_back(s);
	}
	*/
	return ret;
}

string CAction::GetActName()
{
	return this->name;
}

CAction* CAction::Instantiate(CParameter *cpars)
{
	
	CAction *ret;
	CParameter *cpars_new;
	CPredicateList *precond_new, *effect_new; 
	
	//parameter mismatch
	if (this->params->Count()!=cpars->Count()) return NULL;

	//copying parameters
	//cpars_new=new CParameter();
	cpars_new=cpars->Clone();

	//instantiating precondition and effect predicates
 	precond_new=this->precondition->Instantiate(this->params,cpars);
	effect_new=this->effects->Instantiate(this->params,cpars);

	ret=new CAction(this->name,cpars_new,precond_new,effect_new);
	
	return ret;

}

CPredicateList* CAction::GetPrec()
{
  	return this->precondition;
}

CPredicateList* CAction::GetEff()
{
	return this->effects;
}

CPredicateList* CAction::GetPosEff()
{
	return this->posEffects;
}

CPredicateList* CAction::GetNegEff()
{
	return this->negEffects;
}

CParameter* CAction::GetParams()
{
	return this->params;
}

void CAction::DetectSharedArgs(CAction *a, vector<sh_arg_str> &sh)
{
	this->params->DetectShared(a->GetParams(),sh);
}


bool CAction::InverseOps(CAction* o, vector<sh_arg_str>& sh_args)
{
	return negEffects->Equal(o->GetPosEff(),params,o->GetParams(),sh_args,false) &&
		posEffects->Equal(o->GetNegEff(),params,o->GetParams(),sh_args,false);
	
}

bool CAction::ReplacableBy(CAction* o, vector<sh_arg_str>& sh_args)
{
	bool b;
	b=posEffects->Equal(o->GetPosEff(),params,o->GetParams(),sh_args,true);
	if (!b) return false;
	revert_ah_arg_str(sh_args);
	b=o->GetNegEff()->Equal(negEffects,o->GetParams(),params,sh_args,true) &&
		o->GetPrec()->Equal(precondition,o->GetParams(),params,sh_args,true);
	if (!b) return false;
	revert_ah_arg_str(sh_args);
	return true;
}

bool CAction::Equal(CAction* a)
{
       if (strcasecmp(this->name.data(),a->GetActName().data())!=0) return false;
       if (this->params->Count()!=a->GetParams()->Count()) return false;
       
       vector<sh_arg_str> sh_args;
       
       if (this->ReplacableBy(a,sh_args)){
	 //cout << "equal: " << a->GetActName() << "   " << sh_args.size() << " " << params->Count() << endl; //debug reasons 
	 //return sh_args.size()==params->Count();
	 return true;
       }
       
       return false;
}


string CAction::ToString(void)
{
	string ret;
	char buf[100];
	
	ret+="(:action ";
	ret+=name;
	ret+="\n";
	ret+=":parameters (";
	ret+=params->ToString(true);
	ret+=")\n";
	ret+=":precondition (and ";
	ret+=GetPrec()->ToString(false);
	ret+=")\n";
	ret+=":effect (and ";
	ret+=GetEff()->ToString(false);
	if (cost!=0){
	    sprintf(buf,"(increase (total-cost) %i)",cost);;
	    ret+=buf;
	}
	if (!cost_s.empty()){
	    ret+="(increase (total-cost) ";
	    ret+=cost_s;
	    ret+=")";
	}
	ret+=")\n";
	ret+=")";

	return ret;
}

void CAction::MakeIndirectEffects(void)
{
	CPredicateList& pl = ((*precondition)-(*negEffects));
	for (int i=0;i<pl.Count();i++) {pl[i]->SetIndirect(true); if(pl[i]->Negative()) pl[i]->Negate();}
	(*posEffects) = (*posEffects)+pl;
	(*effects) = (*posEffects)+(*negEffects);
}

// static predicates considered, entanglements optionally considered
CActionList* CAction::MakeAllInstances(CTypes* types, CProblem* prob, bool ents)
{
	vector<deque<string>*> lists;
	CActionList *ret = new CActionList();
	int i;

	for (i=0;i<params->Count();i++){
		lists.push_back(types->FindAllObjects((*params)[i]->plisttypes,prob->GetObjects()));
	}

	MakeAllInstancesRec(&lists,new CParameter(),0,prob,ents,ret);
	
	return ret;
}

bool CAction::CheckInstance(CProblem* prob, bool ents)
{
	int i;

	for(i=0;i<precondition->Count();i++){
		if ((*precondition)[i]->IsStatic() || (ents && (*precondition)[i]->IsEntangled())) 
			if (prob->GetInit()->Find((*precondition)[i])==-1) return false;
	}
	if (ents)
		for(i=0;i<posEffects->Count();i++){
			if ((*posEffects)[i]->IsEntangled())
				if (prob->GetGoal()->Find((*posEffects)[i])==-1) return false;
		}
	return true;
}

void CAction::MakeAllInstancesRec(vector<deque<string>*>* obj_list, CParameter* par_parts, int arg_no, CProblem* prob, bool ents, CActionList* ret)
{
	int n = obj_list->size();
	
	if (arg_no >= n){
/*		if (this->name=="Drive_mcr_Unload_mcr_Drop"){

			int x =5 ;
		}*/
		CAction *a = this->Instantiate(par_parts);
		if (a->CheckInstance(prob,ents)) ret->AddRecord(a); else delete a;
	} else {
		deque<string>::iterator it;
		ParamRecord *pr;
		for(it = (*obj_list)[arg_no]->begin();it != (*obj_list)[arg_no]->end(); it++){
			CParameter *tmp_pars = par_parts->Clone();
			pr = new ParamRecord();
			pr->constant = (*it);
			pr->plisttypes = NULL;
			tmp_pars->AddRecord(pr);

			MakeAllInstancesRec(obj_list,tmp_pars,arg_no+1,prob,ents,ret);
		}

	}
}

void CAction::GenerateStatGraph(void)
{
	int n=params->Count();
	map<string,int> par_map;
	//CPredicateList *tmppl;
	
	memset(stat_conn,0,n*n*sizeof(bool));

	for(int i=0;i<n;i++) par_map[(*params)[i]->name]=i;
	
	for(int i=0;i<precondition->Count();i++){
		if (((*precondition)[i]->IsEntangled() || (*precondition)[i]->IsStatic()) && (*precondition)[i]->GetPars()->Count()>1){
			CParameter *tmppars = (*precondition)[i]->GetPars();
			for(int j=0;j<tmppars->Count();j++)
				for(int k=j+1;k<tmppars->Count();k++){
					*(stat_conn+par_map[(*tmppars)[j]->name]*n+par_map[(*tmppars)[k]->name]) = true;
					*(stat_conn+par_map[(*tmppars)[k]->name]*n+par_map[(*tmppars)[j]->name]) = true;
				}

		}
	}

	for(int i=0;i<posEffects->Count();i++){
		if ((*posEffects)[i]->IsEntangled() && (*posEffects)[i]->GetPars()->Count()>1){
			CParameter *tmppars = (*posEffects)[i]->GetPars();
			for(int j=0;j<tmppars->Count();j++)
				for(int k=j+1;k<tmppars->Count();k++){
					*(stat_conn+par_map[(*tmppars)[j]->name]*n+par_map[(*tmppars)[k]->name]) = true;
					*(stat_conn+par_map[(*tmppars)[k]->name]*n+par_map[(*tmppars)[j]->name]) = true;
				}

		}
	}
}

void CAction::GenerateComponents(void)
{
	list<int> pars;
	list<int>::iterator it;
	int n=params->Count();
	int x;

	components.clear();

	for(int i=0;i<n;i++) pars.push_back(i);

	list<int> currentcomp;

	while (!pars.empty()){
		x=pars.front();
		pars.pop_front();
		currentcomp.clear();
		currentcomp.push_back(x);
		for(it=currentcomp.begin();it!=currentcomp.end();it++){
			for(int j=0;j<n;j++){
				if (*(stat_conn+(*it)*n+j)&&find(currentcomp.begin(),currentcomp.end(),j)==currentcomp.end()){
					currentcomp.push_back(j);
					if (find(pars.begin(),pars.end(),j)!=pars.end()) pars.erase(find(pars.begin(),pars.end(),j));
				}
			}
			
		}
		components.push_back(currentcomp);
	}
}

int CAction::ComponentCount(void)
{
	return components.size();
}

bool CAction::HasMeaningfulInitEnt(int pars_count)
{
	for(int i=0;i<precondition->Count();i++){
		if ((*precondition)[i]->IsEntangled() && !(*precondition)[i]->IsStatic() && (*precondition)[i]->GetPars()->Count()>=pars_count) return true;
	}
	
	return false;
}

bool CAction::HasMeaningfulGoalEnt(int pars_count)
{
	for(int i=0;i<posEffects->Count();i++){
		if ((*posEffects)[i]->IsEntangled() && (*posEffects)[i]->GetPars()->Count()>=pars_count) return true;
	}
	
	return false;
}

bool CAction::HasConnectedEnts()
{
    for(int i=0;i<posEffects->Count();i++){
	if ((*posEffects)[i]->IsEntangled() && (*posEffects)[i]->GetPars()->Count()>=2){
	  for(int j=0;j<precondition->Count();j++){
	    if ((*precondition)[j]->IsEntangled() && !(*precondition)[j]->IsStatic() && (*precondition)[j]->GetPars()->Count()>=2){
	      vector<sh_arg_str> x;
	      (*posEffects)[i]->GetPars()->DetectShared((*precondition)[j]->GetPars(),x);
	      //cout << GetActName() << " --- " << x.size() << endl;
	      if (!x.empty()) return true;
	    }
	  }
	}
    }
    return false;
}


void CAction::AchieverForRec(vector< vector< sh_arg_str > >& sh_args_vec, CAction *a, CPredicateList* l1, CPredicateList* l2, int nth, int pos)
{
  if (nth>l1->Count()) return;
  int n=sh_args_vec.size();
  for (int i=pos;i<n;i++){
    for(int j=0;j<l1->Count();j++){
      vector<sh_arg_str> v = sh_args_vec[i];
      (*l1)[j]->Equal((*l2)[j],GetParams(),a->GetParams(),v);
      if (find(sh_args_vec.begin(),sh_args_vec.end(),v)==sh_args_vec.end()) sh_args_vec.push_back(v);
    }
  }
  AchieverForRec(sh_args_vec,a,l1,l2,nth+1,n);
}


bool CAction::AchieverFor(CAction* a, CTypes *t, vector< vector< sh_arg_str > >& sh_args_vec, bool stat)
{
  if (this->GetPosEff()->DisjunctU(a->GetPrec(),t)) return false;
  
  vector< sh_arg_str > sh_args;
  
  if (stat){
    CPredicateList *tmplist = a->GetPrec()->Clone();
    for (int i=0;i<GetPrec()->Count();i++){
	if ((*GetPrec())[i]->IsStatic()){
	  CPredicate *tmp = tmplist->SubsumeP((*GetPrec())[i],t);
	  if (tmp!=NULL){
	    (*GetPrec())[i]->Equal(tmp,GetParams(),a->GetParams(),sh_args);
	    tmplist->RemovePredicate(tmplist->Find(tmp));
	
	  }
	}
	  
    }
    
  }
  
  sh_args_vec.push_back(sh_args);
  
  CPredicateList *l1,*l2;
  
  l1=new CPredicateList();
  l2=new CPredicateList();
  
  for (int i=0;i<GetPosEff()->Count();i++){
    CPredicate *tmp = a->GetPrec()->SubsumeP((*GetPosEff())[i],t);
    if (tmp!=NULL){
       l1->AddRecord((*GetPosEff())[i]->Clone()); 
       l2->AddRecord(tmp->Clone());
    }
  }
  
  AchieverForRec(sh_args_vec,a,l1,l2,1,0);
  sort(sh_args_vec.begin(),sh_args_vec.end(),SizeSort);
  
  return true;
}

bool CAction::IndependentWith(CAction* a)
{
  if (!negEffects->Disjunct(a->GetPosEff())) return false;
  if (!negEffects->Disjunct(a->GetPrec())) return false;
  if (!posEffects->Disjunct(a->GetNegEff())) return false;
  if (!precondition->Disjunct(a->GetNegEff())) return false;
  return true;
}



//////////////////////////////////////////////////////////////////////
// CMacroAction Class
//////////////////////////////////////////////////////////////////////

CMacroAction::CMacroAction(CAction *a1, CAction *a2, vector<sh_arg_str> &sh_args, CTypes *types): MS(NULL)
, CL(NULL)
, threat(false)
, uninformative(false)
{
	
	

	first = a1;
	second = a2;

	this->sh_args = sh_args;

	name = a1->GetActName()+"_mcr_"+a2->GetActName();

	cost = a1->GetActionCost()+a2->GetActionCost();
	
	Initialize(types);
	//DetermineInequalityConstraint();
	
	this->stat_conn=new bool[params->Count()*params->Count()];
	memset(stat_conn,0,params->Count()*params->Count()*sizeof(bool));
}

void CMacroAction::Initialize(CTypes *types)
{
	int i,j,q;
	list<par_repl_str> *par_repl = new list<par_repl_str>();
	par_repl_str x;
	CParameter *a2pars = second->GetParams()->Clone();

/*	if (second->GetActName()=="Unload_mcr_Drop"){

		int shit =5 ;
	}*/

	q=0;
	params = first->GetParams()->Clone();
	for(i=0;i<a2pars->Count();i++){
		for(j=0;j<sh_args.size();j++){
			if (sh_args[j].second == i) {
				x.second =  (*(first->GetParams()))[sh_args[j].first];
				x.first =  (*(a2pars))[sh_args[j].second];
				par_repl->push_back(x);
				if (types!=NULL){ //unify types - ungrounded macros only
				   if (!types->InFamilyLine((*(params))[sh_args[j].first]->plisttypes->front(), (*(a2pars))[sh_args[j].second]->plisttypes->front(), true)){ 
				        //cout << name << endl;     
				        (*(*params)[sh_args[j].first]->plisttypes)[0] = (*(a2pars))[sh_args[j].second]->plisttypes->front();
				   }
				}
				break;
			}
		}
		if (j==sh_args.size()) {
			char* suffix=new char[10];
			sprintf(suffix,"x%i",q+first->GetParams()->Count());
			params->AddRecord((*(a2pars))[i],suffix);
			x.first =  (*(second->GetParams()))[i];
			x.second =  (*(params))[params->Count()-1];
			par_repl->push_back(x);
			q++;
		}
	}

	CPredicateList *a2pre = second->GetPrec()->Clone();
	CPredicateList *a2poseff = second->GetPosEff()->Clone();
	CPredicateList *a2negeff = second->GetNegEff()->Clone();

	a2pre->ReplacePars(*par_repl);
	a2poseff->ReplacePars(*par_repl);
	a2negeff->ReplacePars(*par_repl);

	threat = !first->GetNegEff()->Disjunct(a2pre);
	MS = &((*a2pre)-((*first->GetPosEff())+((*first->GetPrec())-(*first->GetNegEff()))));
	CL = &((*a2negeff)*(*first->GetPosEff()));
	
	this->precondition = &((*first->GetPrec())+((*a2pre)-(*first->GetPosEff())));
	this->negEffects = &(((*first->GetNegEff())+(*a2negeff))-(*a2poseff));
	this->posEffects = &(((*first->GetPosEff())+(*a2poseff))-(*a2negeff));
	this->effects = &((*posEffects)+(*negEffects));

	uninformative = precondition->Intersect(posEffects)->Count() == posEffects->Count();

}

CMacroAction::~CMacroAction(){
	//CAction::~CAction();
}

vector<pair<string,vector<short> > >* CMacroAction::Unfold(vector<short> *pars,bool init){
  
  vector<pair<string,vector<short> > > *ret=new vector<pair<string,vector<short> > >();
  vector<pair<string,vector<short> > > *x;
  
  pars->clear();
  if (!init) {
    for(int i=0;i<params->Count();i++) pars->push_back(i);
  }
  vector<short> one,two;
  //macro
  if (!init) ret->push_back(pair<string,vector<short> >(GetActName(),*pars));
  
  //first one
  for(int i=0;i<first->GetParams()->Count();i++) one.push_back((*pars)[i]);
  
  //second one (more tricky)
  int j=0;
  int k=first->GetParams()->Count();
  for(int i=0;i<second->GetParams()->Count();i++){
		for(j=0;j<sh_args.size();j++){
			if (sh_args[j].second == i){ 
			  two.push_back((*pars)[sh_args[j].first]);
			  break;
			}
		}
		if (j==sh_args.size()) {
		     two.push_back((*pars)[k]);
		     k++;
		}
  }
   if (!first->isMacro()) ret->push_back(pair<string,vector<short> >(first->GetActName(),one)); else {
	x=((CMacroAction*) first)->Unfold(&one,true);
	ret->insert(ret->end(),x->begin(),x->end());
   }
   if (!second->isMacro()) ret->push_back(pair<string,vector<short> >(second->GetActName(),two)); else {
	x=((CMacroAction*) second)->Unfold(&two,true);
	ret->insert(ret->end(),x->begin(),x->end());
   }
  return ret; 
}

bool CMacroAction::IsUninformative(void)
{
	return uninformative;
}

//for preconditions only
CAction* CMacroAction::FindPrimitiveForPredicate(CPredicate* p)
{
	int pos = GetPrec()->Find(p);
	if (pos>=first->GetPrec()->Count()) return second->isMacro() ? ((CMacroAction*)second)->FindPrimitiveForPredicate(p) : second;
	else return first->isMacro() ? ((CMacroAction*)first)->FindPrimitiveForPredicate(p) : first;

}

bool CMacroAction::VerifyValidity(CActionList* acts, CTypes *types, CLearner *learn)
{
	//uninformative
	if (uninformative) 
	{
		//debug reasons
		std::cout << "uninformative " << std::endl;
		return false;
	}

	//thread rule
	if (threat){
		//debug reasons
		std::cout << "threat rule " << std::endl;
		return false;
	}

	int f1,s1;

	int nact = acts->Count();

	for (int i=0;i<nact;i++){
		if (strcasecmp((*acts)[i]->GetActName().data(),second->GetActName().data()) == 0) s1=i;
		if (strcasecmp((*acts)[i]->GetActName().data(),first->GetActName().data()) == 0) f1=i;
	}

	//MS rule
	for (int i=0;i<nact;i++){
		//if (strcasecmp((*acts)[i]->GetActName().data(),second->GetActName().data()) == 0) continue; //perhaps (it seems that it works only for ent by succeeding_)
		for (int j=0;j<(*acts)[i]->GetPosEff()->Count();j++){
			CPredicate *tmppred = MS->SubsumeP((*(*acts)[i]->GetPosEff())[j],types);
			if (tmppred!=NULL && (tmppred->GetInnerEntanglement()==-1 || tmppred->GetInnerEntanglement()==i) && ((*(*acts)[i]->GetPosEff())[j]->GetInnerEntanglement()==-1 || (*(*acts)[i]->GetPosEff())[j]->GetInnerEntanglement()==f1)/*&& !(*(*acts)[i]->GetPosEff())[j]->IsIndirect() */ /*&& first->GetPrec()->Subsume(tmppred,types)*/) {
				bool clob = false;
				for (int l=0;l<(*acts)[i]->GetNegEff()->Count();l++){
					CPredicate *tmppred3 = first->GetPrec()->SubsumeP((*(*acts)[i]->GetNegEff())[l],types);
					if (tmppred3!=NULL && (*acts)[i]->GetPosEff()->Find(tmppred3)==-1){
						clob = true;
						break;
					}
				}
				if (clob){
					//debug reasons 
					std::cout << "MS failed - " << tmppred->ToString(true) << std::endl;
					//trying to recover - not a BULLSHIT
					
					tmppred = second->GetPrec()->SubsumeP(tmppred,types);
					CPredicate* tmppred2 = ((*first->GetPrec())-(*first->GetNegEff())).SubsumeP(tmppred,types);
			
					if ( tmppred2!=NULL){
						tmppred2->Equal(tmppred,first->GetParams(),second->GetParams(),sh_args);
						if (learn->IsSameAchiever(first->FindPrimitiveForPredicate(tmppred2),second->FindPrimitiveForPredicate(tmppred),tmppred2)){
							Initialize(types); //reinitialize
							if (uninformative||threat) return false;
						
							continue;
						}
						
					}
				
					return false; 
				}
			}
		}
	}
	
	
	for (int k=0;k<CL->Count();k++){
		if ((*CL)[k]->GetInnerEntanglement()!=-1 && strcasecmp((*acts)[(*CL)[k]->GetInnerEntanglement()]->GetActName().data(),second->GetActName().data())==0)
			CL->RemovePredicate(k);
			
	}
	

	//CL rule (might be optimized by adding it into MS rule..)
	if (CL!=NULL && CL->Count()>0)
	for (int i=0;i<nact;i++){
		if (strcasecmp((*acts)[i]->GetActName().data(),second->GetActName().data()) == 0) continue;
		for (int j=0;j<(*acts)[i]->GetPrec()->Count();j++){
			short x = (*(*acts)[i]->GetPrec())[j]->GetInnerEntanglement();
			CPredicate* tmppred = CL->SubsumeP((*(*acts)[i]->GetPrec())[j],types);
			CPredicate* tmppred2 = (tmppred!=NULL) ? second->GetPrec()->SubsumeP(tmppred,types) : NULL;
			if (tmppred!=NULL && (x==-1 || x==s1 /*strcasecmp((*acts)[x]->GetActName().data(),(*acts)[i]->GetActName().data()) == 0*/) && (tmppred2==NULL || tmppred2->GetInnerEntanglement()==-1 || tmppred2->GetInnerEntanglement()==i)){
				//debug reasons
				std::cout << "CL failed - " << (*(*acts)[i]->GetPrec())[j]->ToString(true) << std::endl;
				return false; 
			}
		}
	}
	return true;
}

//after generating statgraphs...
int CMacroAction::StatGraphMatch(void)
{
	int n=GetParams()->Count();
	int ret=0;

	//for a1
	int n1=first->GetParams()->Count();
	for(int i=0;i<n1;i++){
		for(int j=i+1;j<n1;j++){
			if (*(stat_conn+i*n+j)!=*(first->stat_conn+i*n1+j)){
				ret |=1;
				break;
			}
		}
	}
	
	//for a2
	int n2=second->GetParams()->Count();
	vector<int> argmatch(n2,-1);
	for(int x=0;x<sh_args.size();x++){
		argmatch[sh_args[x].second]=sh_args[x].first;
	}
	int inc=n1;
	for(int x=0;x<n2;x++){
		if (argmatch[x]==-1){
			argmatch[x]=inc;
			inc++;
		}
	}
	for(int i=0;i<n2;i++){
		for(int j=i+1;j<n2;j++){
			if (*(stat_conn+argmatch[i]*n+argmatch[j])!=*(second->stat_conn+i*n2+j)){
				ret |=2;
				break;
			}
		}
	}
	return ret;
}

//DOESN'T WORK !!! prejebat !!
bool CMacroAction::StatGraphConnect(void)
{
	int n=GetParams()->Count();
	int n1=first->GetParams()->Count();
	
	for(int i=0;i<n1;i++){
		for(int j=n1;j<n;j++){
			if (*(stat_conn+i*n+j)){
				return true;
			}
		}
	}

	return false;
}

vector<string>* CMacroAction::LinearizeActNames(void)
{
	vector<string> *ret;
	
	if (first->isMacro()){
		ret=((CMacroAction*) first)->LinearizeActNames();
	} else {
		ret=new vector<string>();
		ret->push_back(first->GetActName());
	}
	
	if (second->isMacro()){
		vector<string> *ret2=((CMacroAction*) second)->LinearizeActNames();
		ret->insert(ret->end(),ret2->begin(),ret2->end());
	} else {
		ret->push_back(second->GetActName());
	}
	
	return ret;
}

vector<CAction*>* CMacroAction::LinearizeActs()
{
    vector<CAction*> *ret=new vector<CAction*>();
	
	if (first->isMacro()){
		ret=((CMacroAction*) first)->LinearizeActs();
	} else {
		ret->push_back(first);
	}
	
	if (second->isMacro()){
		vector<CAction*> *ret2=((CMacroAction*) second)->LinearizeActs();
		ret->insert(ret->end(),ret2->begin(),ret2->end());
	} else {
		ret->push_back(second);
	}
	
	return ret;
}

int CMacroAction::ActCount(){
	return first->ActCount()+second->ActCount();
}

bool CMacroAction::IsRepetitive(void)
{
	vector<string> *acts = LinearizeActNames();
	int no1 = first->ActCount();
	int n=acts->size();
	int i=no1;

	while (i-no1<no1 && i<n){
		bool fail=false;
		for (int j=i;j>=no1;j--){
			if ((*acts)[j]!=(*acts)[no1-1-(i-j)]) {fail=true;break;}
		}
		if (!fail) return true;
		i++;
	}
	return false;
}

void CMacroAction::FindActionPositions(CActionList* acts, int& i, int& j)
{
	acts->FindProperAction(first->GetActName(),i);
	acts->FindProperAction(second->GetActName(),j);
}

bool CMacroAction::TestUsefulness(CDomain *dom)
{
      if (threat||uninformative) return false;
      //test compatibility of preconditions
      for (int i=0;i<GetPrec()->Count();i++)
	for (int j=i+1;j<GetPrec()->Count();j++){
	  if (!dom->AreCompatible((*GetPrec())[i],(*GetPrec())[j])) return false;
	}
      
      //vector<CAction*> *linacts = LinearizeActs();
      vector<short> pars;
      vector<sh_arg_str> v;
      vector<pair<string,vector<short> > > *linacts = Unfold(&pars);
      for (vector<pair<string,vector<short> > >::iterator it=linacts->begin()+1;it!=linacts->end()-1;it++){
	for (vector<pair<string,vector<short> > >::iterator it2=it+1;it2!=linacts->end();it2++){
	  if (it->first == it2->first) return false;
	  v.clear();
	  for (int i=0;i<it->second.size();i++)
	    for (int j=0;j<it2->second.size();j++){
	      if (it->second[i]==it2->second[j]){	      
	       sh_arg_str *sa = new sh_arg_str();
	       sa->first=i;
	       sa->second=j;
	       v.push_back(*sa);
	      }
	    }
	  
	  if (dom->AreInverse(it->first,it2->first,v)) return false;
	}
      }
      return true;
}

void CMacroAction::DetermineInequalityConstraint()
{
   //using a "trick" - when two predicates are the same with different params
   vector<sh_arg_str> sh_args;
   
   vector<sh_arg_str> *sh_args_tmp; 
   
   for (int i=0;i<precondition->Count();i++)
     for (int j=i+1;j<precondition->Count();j++){
        sh_args_tmp=new vector<sh_arg_str>(); 
        if (!(*precondition)[i]->IsStatic() && (*precondition)[i]->Equal((*precondition)[j],params,params,*sh_args_tmp)){
	   sh_args.insert(sh_args.end(),sh_args_tmp->begin(),sh_args_tmp->end());
	   
	}
     }
   
  // cout << "IE passed for " << name << endl;
   
   for (vector<sh_arg_str>::iterator it=sh_args.begin();it!=sh_args.end();it++){
     CParameter *tmp_pars=new CParameter();
     if ((*params)[it->first]->name == (*params)[it->second]->name) continue;
     CParameter *cloned_pars=params->Clone(); //not very clean..
     tmp_pars->AddRecord((*cloned_pars)[it->first]);
     tmp_pars->AddRecord((*cloned_pars)[it->second]);
     CPredicate *tmp_pred=new CPredicate("=",tmp_pars);
     tmp_pred->Negate();
     tmp_pred->SetStatic(true);
     precondition->AddRecord(tmp_pred);
   }
   
}

int CMacroAction::HasIntermediateAdditionalArgs()
{
   vector<short> px;
   int additional=0;
   
   vector<pair<string,vector<short> > > *pars=Unfold(&px);
   
   for (vector<short>::iterator it=px.begin();it!=px.end();it++){
     //cout << "Macro: " << name << " par: $" << *it << "   " << (++pars->begin())->first << " " << pars->rbegin()->first <<  endl;
     if ( find((++pars->begin())->second.begin(),(++pars->begin())->second.end(),*it) == (++pars->begin())->second.end() &&
          find(pars->rbegin()->second.begin(),pars->rbegin()->second.end(),*it) == pars->rbegin()->second.end() ) {
         additional++;
     }
     
   }
   
   return additional;
}


//////////////////////////////////////////////////////////////////////
// CActionList Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CActionList::CActionList()
{
	this->pactlist=new deque<CAction*>();
}

CActionList::~CActionList()
{
	delete this->pactlist;
}

void CActionList::AddRecord(CAction *act)
{
	this->pactlist->push_back(act);
}


list<string>* CActionList::GetPrologRepresentation()
{
	list<string>  *ret=new list<string>();
	list<string> *p;
	int n=this->pactlist->size();

	for (int i=0;i<n;i++){
		
		p=(*this->pactlist)[i]->GetPrologRepresentation();
		ret->merge(*p);
	}
	
	return ret;
}



CAction* CActionList::FindProperAction(string name,int &pos)
{
	string s;
	int n=this->pactlist->size();

	for (int i=0;i<n;i++){
		
		s=(*this->pactlist)[i]->GetActName();
		if (strcasecmp(s.data(),name.data())==0) {
			pos=i;	
			return (*this->pactlist)[i];
		}
	}
	pos=-1;
	return NULL;
}

int CActionList::Count()
{
	return this->pactlist->size();
}

CAction* CActionList::operator[](unsigned int i){

	return (*this->pactlist)[i];
}




string CActionList::ToString(void)
{
	string ret;
	deque<CAction*>::iterator it = pactlist->begin();

	while (it!=pactlist->end()){
		ret+=(*it)->ToString();
		ret+="\n";
		it++;
	}
	return ret;
}





void CActionList::MakeIndirectEffects(void)
{
	deque<CAction*>::iterator it; 
	for(it=pactlist->begin();it!=pactlist->end();it++){
		(*it)->MakeIndirectEffects();
	}

}



void CActionList::Replace(CAction* a, int pos)
{
	(*pactlist)[pos]=a;
}

void CActionList::Remove(int pos)
{
	pactlist->erase(pactlist->begin()+pos);
}
