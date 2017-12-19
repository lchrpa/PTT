#include "CDomain.h"

extern string ToLower(string);
extern bool compatible_sh_arg_str(vector<sh_arg_str>&);

CDomain::CDomain(){
	inv_matrix = NULL;
	repl_matrix = NULL;
	pfunct = NULL;
	constants = NULL;
	action_cost = false;
	
}

CDomain::~CDomain(){
	if (inv_matrix!=NULL) delete inv_matrix;
	if (repl_matrix!=NULL) delete repl_matrix;
	knowledge_file.close();
}

void CDomain::SetName(char *s){
	this->name=s;
}

string CDomain::GetName(){
	return this->name;
}

void CDomain::SetTypes(CTypes *pt){
	this->ptypes=pt;
	this->ptypes->MakeTree();
}


CTypes* CDomain::GetTypes(){
	return this->ptypes;
}

void CDomain::SetConstants(CTypes* cons)
{
   constants=cons;
}

CTypes* CDomain::GetConstannts()
{
   return constants;
}

void CDomain::SetPredicates(CPredicateList *pp)
{
	this->ppreds=pp;
}

void CDomain::SetFunctions(CPredicateList *pf)
{
	this->pfunct=pf;
}

CPredicateList* CDomain::GetPredicates()
{
	return this->ppreds;
}

void CDomain::GeneratePrologFile()
{
	fstream f;
	string fname;
	list<string> *pins;

	fname=ToLower(this->name);
	fname+=".pl";

	f.open(fname.data(),fstream::out | fstream::trunc);
	//domain name
	f << "domain(" << ToLower(this->name) << ")." <<endl;
	//types inherits
	pins=this->ptypes->GetPrologRepresentation();
	//predicates (definitions)
	pins->merge(*this->ppreds->GetPrologRepresentation());
	//actions
	pins->merge(*this->pacts->GetPrologRepresentation());

	//storing of the list of instruction into the Prolog file
	while (!pins->empty()){
		f << pins->front() <<endl;
		pins->pop_front();
	}

	f.close();

}

void CDomain::SetActions(CActionList *acts)
{
	this->pacts=acts;
}

void CDomain::SetOrigFileName(string s)
{
	this->origfilename=s;
}

string CDomain::GetOrigFileName()
{
	return this->origfilename;
}


CActionList* CDomain::GetActions()
{
	return this->pacts;
}

//identifies and marks all static predicates in operators' preconditions
void CDomain::IdentifyStaticPredicates()
{
	int i,j,k;
	CAction *op;
	CPredicate *p;
	bool ineff;

	for (i=0;i<this->pacts->Count();i++){
		op=(*this->pacts)[i];
		for (j=0;j<op->GetPrec()->Count();j++){
			p=(*op->GetPrec())[j];
			ineff=false;
			for (k=0;k<this->pacts->Count();k++){
				ineff=(*this->pacts)[k]->GetEff()->Subsume(p,this->GetTypes());
				if (ineff) break;
			}
			p->SetStatic(!ineff);
		}
	}

}

void CDomain::MakeInvMatrix(void)
{
	int i,j;
	int n=GetActions()->Count();
	initial_inv_count=n;
	CAction *a1,*a2;
	vector<sh_arg_str> *sh_args;
	
	this->inv_matrix = new inv_matrix_field[n*n];

	for (i=0;i<n;i++){
		for (j=0;j<n;j++){
			a1 = (*pacts)[i];
			a2 = (*pacts)[j];
			sh_args = new vector<sh_arg_str>();
			if (a1->InverseOps(a2,*sh_args)){
				(*(inv_matrix+i*n+j)).inv = true;
				(*(inv_matrix+i*n+j)).shared_args=sh_args;
			} else {
				(*(inv_matrix+i*n+j)).inv = false;
				(*(inv_matrix+i*n+j)).shared_args=NULL;
			}
		}
	}
}

void CDomain::IdentifyIncomaptiblePredicates()
{
  int i,j,k;
  int n=GetActions()->Count();
  int n2=GetPredicates()->Count();
  initial_pred_count=n2;
  incompatible_preds.resize(n2*n2,make_pair(0,(vector<sh_arg_str>*)NULL));
  CAction *a;
  set<int> visited;
  
  for (i=0;i<n;i++){
     a = (*pacts)[i];
     visited.clear();
     for (j=0;j<a->GetPosEff()->Count();j++){
       int p1 = ppreds->FindProperPredicate((*a->GetPosEff())[j]->GetName());
       for (k=0;k<a->GetNegEff()->Count();k++){
	 int p2 = ppreds->FindProperPredicate((*a->GetNegEff())[k]->GetName());
	 
	 if (incompatible_preds[p1<p2 ? p1*n2+p2 : p2*n2+p1].first==0){
	   incompatible_preds[p1<p2 ? p1*n2+p2 : p2*n2+p1].first = 1;
	   incompatible_preds[p1<p2 ? p1*n2+p2 : p2*n2+p1].second = new vector<sh_arg_str>();
	   if (p1<p2) (*a->GetPosEff())[j]->GetPars()->DetectShared((*a->GetNegEff())[k]->GetPars(),*incompatible_preds[p1*n2+p2].second);
	   else (*a->GetNegEff())[k]->GetPars()->DetectShared((*a->GetPosEff())[j]->GetPars(),*incompatible_preds[p2*n2+p1].second);
	 } else if (incompatible_preds[p1<p2 ? p1*n2+p2 : p2*n2+p1].first==1){
	   if (p1<p2) {
	     if (!incompatible_preds[p1*n2+p2].second->empty()) (*a->GetPosEff())[j]->GetPars()->DetectShared((*a->GetNegEff())[k]->GetPars(),*incompatible_preds[p1*n2+p2].second);
	   } else {
	     if (!incompatible_preds[p2*n2+p1].second->empty()) (*a->GetNegEff())[k]->GetPars()->DetectShared((*a->GetPosEff())[j]->GetPars(),*incompatible_preds[p2*n2+p1].second);
	   }
	   
	 }
	 visited.insert(p2);
       }
       for (k=0;k<n2;k++) {
	  if (visited.find(k)==visited.end()) 
	   //if (incompatible_preds[p1<k ? p1*n2+k : k*n2+p1].first) {
	      incompatible_preds[p1<k ? p1*n2+k : k*n2+p1].first=2;
	      //incompatible_preds[p1<k ? p1*n2+k : k*n2+p1].second->clear();
	    //}
       }
     }
  }
  
}

void CDomain::OutIncompatiblePreds(ostream& s)
{
  int n=GetPredicates()->Count();
  for (int i=0;i<n;i++){
    for (int j=i+1;j<n;j++){
      if (incompatible_preds[i*n+j].first==1)
	s << (*ppreds)[i]->GetName() << " - mutex with - " << (*ppreds)[j]->GetName() <<endl;
    }
  }

}



bool CDomain::AreCompatible(CPredicate* p1, CPredicate* p2)
{
     int k1 = ppreds->FindProperPredicate(p1->GetName());
     int k2 = ppreds->FindProperPredicate(p2->GetName());
     
     pair<short,vector<sh_arg_str>* > *tmp;
     
     tmp = &incompatible_preds[k1<k2 ? k1*initial_pred_count+k2 : k2*initial_pred_count+k1];
     
     if (tmp->first!=1) return true;
     if (p1->GetPars()->Count()==0 || p2->GetPars()->Count()==0) return false;
     if (tmp->second->empty()) return true;
     
     return k1<k2 ? !p1->GetPars()->IsCorresponding(p2->GetPars(),*tmp->second) : !p2->GetPars()->IsCorresponding(p1->GetPars(),*tmp->second);
     
}

void CDomain::GetFlippingData()
{
    int n=GetPredicates()->Count();
    for (int i=0;i<n;i++){
      for (int j=i+1;j<n;j++){
        if (incompatible_preds[i*n+j].first==1 && (*ppreds)[i]->GetPars()->Count()!=(*ppreds)[j]->GetPars()->Count()){
	  CPredicate* tmp_p1 = ((*ppreds)[i]->GetPars()->Count() < (*ppreds)[j]->GetPars()->Count()) ? (*ppreds)[i] : (*ppreds)[j];
	  CPredicate* tmp_p2 = ((*ppreds)[i]->GetPars()->Count() > (*ppreds)[j]->GetPars()->Count()) ? (*ppreds)[i] : (*ppreds)[j];
	  if (tmp_p1->GetPars()->Count() == incompatible_preds[i*n+j].second->size()){//thesecond predicate must "extend" the first one
	      flipping_data *tmpdata = new flipping_data();
	      tmpdata->p1=tmp_p1;
	      tmpdata->p2=tmp_p2;
	      tmpdata->shared_args=incompatible_preds[i*n+j].second;
	      for (int k=0; k<pacts->Count();k++){
		if ((*pacts)[k]->GetNegEff()->FindProperPredicate(tmp_p1->GetName())!=-1 && (*pacts)[k]->GetPosEff()->FindProperPredicate(tmp_p2->GetName())!=-1) tmpdata->starting.push_back((*pacts)[k]);
		if ((*pacts)[k]->GetNegEff()->FindProperPredicate(tmp_p2->GetName())!=-1 && (*pacts)[k]->GetPosEff()->FindProperPredicate(tmp_p1->GetName())!=-1) tmpdata->finishing.push_back((*pacts)[k]);
	      }
	      if (!tmpdata->starting.empty() && !tmpdata->finishing.empty()){//no "flipping" actions nothing to connect
	      
	           flipping.push_back((*tmpdata));
	      }
	  }
	}
      }
    }
    
}

void CDomain::OutFlippingData(ostream& s)
{
   for (vector<flipping_data>::iterator it=flipping.begin();it!=flipping.end();it++){
      s << it->p1->ToString(false) << " - flips with - " << it->p2->ToString(false) << endl;
      s << "Actions start the flip: ";
      for (vector<CAction*>::iterator it2=it->starting.begin();it2!=it->starting.end();it2++) s << (*it2)->GetActName() << " ";
      s << endl;
      s << "Actions finish the flip: ";
      for (vector<CAction*>::iterator it2=it->finishing.begin();it2!=it->finishing.end();it2++) s << (*it2)->GetActName() << " ";
      s << endl;
   }
  
  
}



bool CDomain::AreInverse(CAction* a1, CAction* a2)
{
	int i,j,k;
	int n = initial_inv_count;
	vector<sh_arg_str> *sh_args;
	
	pacts->FindProperAction(a1->GetActName(),i);
	pacts->FindProperAction(a2->GetActName(),j);

	if ((*(inv_matrix+i*n+j)).inv){
		sh_args = (*(inv_matrix+i*n+j)).shared_args;
		if (!a1->GetParams()->IsCorresponding(a2->GetParams(),*sh_args)) return false;
	} else {
		return false;
	}
	
	return true;
}


bool CDomain::AreInverse(string a1, string a2,vector<sh_arg_str> &sh_args)
{
	int i,j,k;
	int n = initial_inv_count;
	vector<sh_arg_str> *v;
		
	pacts->FindProperAction(a1,i);
	pacts->FindProperAction(a2,j);

	if ((*(inv_matrix+i*n+j)).inv){
          v = (*(inv_matrix+i*n+j)).shared_args;
	  for (vector<sh_arg_str>::iterator it=v->begin();it!=v->end();it++){
	    if (find(sh_args.begin(),sh_args.end(),*it)==sh_args.end()) return false; 
	  }
		
	} else return false;
	
	return true;
}

bool CDomain::ReplacableBy(CAction* a1, CAction* a2, vector<sh_arg_str>& sh_args, repl_matrix_field &ret)
{
	int i,j,k;
	int n=GetActions()->Count();
	CAction *o1 = pacts->FindProperAction(a1->GetActName(),i);
	CAction *o2 = pacts->FindProperAction(a2->GetActName(),j);
	CMacroAction *mo = new CMacroAction(o1,o2,sh_args);
	vector<sh_arg_str> *tmp;

	for(k=0;k<n;k++){
		tmp = new vector<sh_arg_str>();
		if (mo->ReplacableBy((*pacts)[k],*tmp)&&compatible_sh_arg_str(*tmp)){
			ret.a = (*pacts)[k];
			ret.shared_args = tmp;
			ret.repl = true;
			return true;
		}
	}
	
	return false;
}

void CDomain::InitializeReplMatrix(void)
{
	int i,j,k;
	int n=GetActions()->Count();
	CAction *a1,*a2,*a3, *ma;
	vector<sh_arg_str> *sh_args;
	
//	bool(*fn_pt)(char,char) = fncomp;

//	this->repl_matrix = new map<vector<sh_arg_str>,repl_matrix_field,sh_arg_str_eq>[n*n];

/*
	for (i=0;i<n;i++){
		for (j=0;j<n;j++){
			a1 = (*pacts)[i];
			a2 = (*pacts)[j];
			ma = new CMacroAction(a1,a2,*tmp);
			
			for(k=0;k<n;k++){
				a3 = (*pacts)[k];
				sh_args = new vector<sh_arg_str>();
				if (ma->ReplacableBy(a3,*sh_args)){
					(*(repl_matrix+i*n+j)).repl = true;
					(*(repl_matrix+i*n+j)).shared_args=sh_args;
					(*(repl_matrix+i*n+j)).a = a3;
					break;
				} else {
					(*(repl_matrix+i*n+j)).repl = false;
					(*(repl_matrix+i*n+j)).shared_args=NULL;
					(*(repl_matrix+i*n+j)).a=NULL;
				}
			}
		}
	}
*/

}

void CDomain::OutStatic(ostream &s)
{
	int i,j,k;
	short x;
	CAction *op;
	CPredicate *p;
	bool ineff;

	for (i=0;i<this->pacts->Count();i++){
		op=(*this->pacts)[i];
		s << op->GetActName() << ": ";
		for (j=0;j<op->GetPrec()->Count();j++){
			p=(*op->GetPrec())[j];
			if (p->IsStatic()) {
				s << " static  " << p->GetPrologRepresentationVars() << ",";
			}
			if (p->IsEntangled()){
				s << " entangled_init " << p->GetPrologRepresentationVars() << ",";
			}
			x=p->GetInnerEntanglement();
			if (x!=-1){
				s << " previously entangled by " << (*pacts)[x]->GetActName() << " with " << p->GetPrologRepresentationVars() << ",";
			}

		}
		for (j=0;j<op->GetPosEff()->Count();j++){
			p=(*op->GetPosEff())[j];
			if (p->IsEntangled()){
				s << " entangled_goal " << p->GetPrologRepresentationVars() << ",";
			}
			x=p->GetInnerEntanglement();
			if (x!=-1){
				s << " successively entangled by " << (*pacts)[x]->GetActName() << " with " << p->GetPrologRepresentationVars() << ",";
			}

		}
		s << endl;
	}


	
}

void CDomain::OutInv(ostream &s)
{
	int i,j,k;
	int n=GetActions()->Count();
	vector<sh_arg_str> *sa;

	for (i=0;i<n;i++){
		for (j=0;j<n;j++){
			if ((*(inv_matrix+i*n+j)).inv) {
				s << (*pacts)[i]->GetActName() << " inverse " << (*pacts)[j]->GetActName();
				sa = (*(inv_matrix+i*n+j)).shared_args;
				for (k=0;k<sa->size();k++){
					if (k>0) s << ", ";
					s << "[" << (*sa)[k].first << "," << (*sa)[k].second << "]";
				}
				s << endl;
			}
		}
	}
}

void CDomain::OutInc(ostream &s)
{
	int i,j,k;
	int n=initial_pred_count;
	vector<sh_arg_str> *sa;

	for (i=0;i<n;i++){
		for (j=i;j<n;j++){
			if (incompatible_preds[i*n+j].first) {
				s << (*ppreds)[i]->GetName() << " incompatible " << (*ppreds)[j]->GetName();
				sa = incompatible_preds[i*n+j].second;
				for (k=0;k<sa->size();k++){
					if (k>0) s << ", ";
					s << "[" << (*sa)[k].first << "," << (*sa)[k].second << "]";
				}
				s << endl;
			}
		}
	}
}

void CDomain::OutRepl(ostream &s)
{
	int i,j,k;
	int n=GetActions()->Count();
	vector<sh_arg_str> *sa;

	for (i=0;i<n;i++){
		for (j=0;j<n;j++){
			if ((*(repl_matrix+i*n+j)).repl) {
				s << (*pacts)[i]->GetActName() << " and " << (*pacts)[j]->GetActName() << " replacable by " << (*(repl_matrix+i*n+j)).a->GetActName();
				sa = (*(repl_matrix+i*n+j)).shared_args;
				for (k=0;k<sa->size();k++){
					if (k>0) s << ", ";
					s << "[" << (*sa)[k].first << "," << (*sa)[k].second << "]";
				}
				s << endl;
			}
		}
	}
}


void CDomain::GeneratePDDL(ostream& s)
{
	
	s << "; Generated by PTT" << endl;

	s << "(define (domain " << name << ")" << endl;
	
	//may be customized in future
	s << "(:requirements :typing" <</* (action_cost ? " :action-costs" : "") <<*/ ")" << endl;

	//types 
	s << "(:types" << ptypes->ToString() << ")" <<endl; 
	
	//constants (if applicable)
	
	if (constants!=NULL){
	  s << "(:constants" << constants->ToString() << ')' << endl; 
	}

	//predicates
	s << "(:predicates " << ppreds->ToString(true) << ")" << endl;
	
	//action cost (if applicable)

	if (action_cost) s << "(:functions " <<pfunct->ToString(true) << ")" << endl;
	
	//operators (actions)
	s << pacts->ToString();
	
	if (!knowledge_file.is_open())	knowledge_file.open("knowledge.kf",fstream::out|fstream::trunc);
	knowledge_file << ";macros" << endl;
	vector<short> px;
	for (int i=0;i<pacts->Count();i++){
	  if ((*pacts)[i]->isMacro()){
	    vector<pair<string,vector<short> > > *unfold=((CMacroAction*)(*pacts)[i])->Unfold(&px);
	    for(int j=0;j<unfold->size();j++){
		knowledge_file << (*unfold)[j].first;
		for (int k=0;k<(*unfold)[j].second.size();k++){
		  knowledge_file << " $" << (*unfold)[j].second[k];
		}
		knowledge_file << endl;
	    }
	    knowledge_file << endl;
	  }
	}
	s << ")";
}

void CDomain::MergePredTypes(CPredicateList *pl){
  for (int i=0;i<pl->Count();i++){
      for (int j=i+1;j<pl->Count();j++){
	if ((*pl)[i]->GetName()==(*pl)[j]->GetName()){
	    for (int k=0;k<(*pl)[i]->GetPars()->Count();k++){
	      //copy((*(*pl)[j]->GetPars())[k]->plisttypes->begin(),(*(*pl)[j]->GetPars())[k]->plisttypes->end(),(*(*pl)[i]->GetPars())[k]->plisttypes->end());
	      (*(*pl)[i]->GetPars())[k]->plisttypes->insert((*(*pl)[i]->GetPars())[k]->plisttypes->end(),(*(*pl)[j]->GetPars())[k]->plisttypes->begin(),(*(*pl)[j]->GetPars())[k]->plisttypes->end());
	      //sort((*(*pl)[i]->GetPars())[k]->plisttypes->begin(),(*(*pl)[i]->GetPars())[k]->plisttypes->end());
	      (*(*pl)[i]->GetPars())[k]->plisttypes->erase(unique((*(*pl)[i]->GetPars())[k]->plisttypes->begin(),(*(*pl)[i]->GetPars())[k]->plisttypes->end()),(*(*pl)[i]->GetPars())[k]->plisttypes->end());
	    }
	    pl->RemovePredicate(j);
	    j--;
	}
      }
  }
}


void CDomain::ReformulateByEntanglements(CPredicateList* ent_init, CPredicateList* ent_goal)
{
	int i;
	string s;

	MergePredTypes(ent_init);
	MergePredTypes(ent_goal);
	
	if (!knowledge_file.is_open())	knowledge_file.open("knowledge.kf",fstream::out|fstream::trunc);
	
	knowledge_file << ";ent_init" << endl;
	s="stai_";
	for (i=0;i<ent_init->Count();i++){		
		knowledge_file << (*ent_init)[i]->GetName();
		(*ent_init)[i]->SetName(s+(*ent_init)[i]->GetName());
		ppreds->AddRecord((*ent_init)[i]);
		knowledge_file << "->" << (*ent_init)[i]->GetName() << endl;
	}

	knowledge_file << ";ent_goal" << endl;
	s="stag_";
	for (i=0;i<ent_goal->Count();i++){		
		knowledge_file << (*ent_goal)[i]->GetName();
		(*ent_goal)[i]->SetName(s+(*ent_goal)[i]->GetName());
		ppreds->AddRecord((*ent_goal)[i]);
		knowledge_file << "->" << (*ent_goal)[i]->GetName() << endl;
	}
}

void CDomain::ReformulateByInnerEntanglements(CPredicateList* ent_prev, CPredicateList* ent_succ)
{
	int i;
	
	if (!knowledge_file.is_open())	knowledge_file.open("knowledge.kf",fstream::out|fstream::trunc);

	knowledge_file << ";ent_prec" << endl;
	for (i=0;i<ent_prev->Count();i++){		
		ppreds->AddRecord((*ent_prev)[i]);
		knowledge_file << (*ent_prev)[i]->GetName() << endl;
	}
	
	knowledge_file << ";ent_succ" << endl;
	for (i=0;i<ent_succ->Count();i++){		
		ppreds->AddRecord((*ent_succ)[i]);
		knowledge_file << (*ent_succ)[i]->GetName() << endl;
	}

}

void CDomain::ImportMacros(list<mcr*>* macros)
{
    CActionList *new_mcrs = new CActionList();
    CAction *a;
    sh_arg_str *sh;
    vector<sh_arg_str> *sh_args;
    int i;
    for (list<mcr*>::iterator it = macros->begin();it!=macros->end();it++){
        a = NULL;
        for (vector<act*>::iterator it2= (*it)->second.begin();it2!=(*it)->second.end();it2++){
	   if (a == NULL) {a = this->pacts->FindProperAction((*it2)->first,i);continue;}
	   CAction *b = this->pacts->FindProperAction((*it2)->first,i);
	   sh_args = new vector<sh_arg_str>();
	   for (int j=0;j<(*it2)->second.size();j++){
	     if ((*it2)->second[j]>a->GetParams()->Count()) continue; //parameter is not shared
	     sh = new sh_arg_str();
	     sh->first = (*it2)->second[j]-1;
	     sh->second = j;
	     sh_args->push_back(*sh);
	   }
	   a = new CMacroAction(a,b,*sh_args);
	}
	if (!((CMacroAction*)a)->IsUninformative() && new_mcrs->FindProperAction(a->GetActName(),i)==NULL)
	  new_mcrs->AddRecord(a);
    }
    
    for (int j=0;j<new_mcrs->Count();j++) pacts->AddRecord((*new_mcrs)[j]);
}

