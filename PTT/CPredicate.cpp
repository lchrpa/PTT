// CPredicate.cpp: implementation of the CPredicate class.
//
//////////////////////////////////////////////////////////////////////

#include "CPredicate.h"

extern string ToLower(string);
extern string ToUpper(string);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPredicate::CPredicate()

{
	this->neg=false;
	this->d=0;
	this->stat=false;
	this->entangled=false;
	this->inner_entangled=-1;
	this->indirect=false;
	this->params=new CParameter();;
}

CPredicate::CPredicate(string nam,CParameter *pars=NULL)
{
	this->name=nam;
	this->params= (pars!=NULL) ? pars : new CParameter();
	this->neg=false;
	this->d=0;
	this->stat=false;
	this->entangled=false;
	this->inner_entangled=-1;
	this->indirect=false;
}

CPredicate::~CPredicate()
{
	delete this->params;
}

/* Generate combination of types */
void CPredicate::GetAllTypesCombinations(list<string> *pliststr, int argno, string s){
	
	string p=s; 
	string p2; //pomocny retezec
	int i,n;
	ParamRecord *pr;

	if (argno>=this->params->Count()){  //argno reached the limit 
		p+=")";
		pliststr->push_front(p);
	} else {
		if (argno>0) {p+=",";} //comma for non-first types
		pr=(*this->params)[argno];
		n=pr->plisttypes->size();
		for(i=0;i<n;i++){  //go through all types
			p2=p+ToLower((*pr->plisttypes)[i]);
			GetAllTypesCombinations(pliststr,argno+1,p2);
		}
	}
	
}

void CPredicate::MakeAllInstancesRec(vector<deque<string>*>* obj_list, CParameter* part_pars, int arg_no, CPredicateList* ret)
{
	int n = obj_list->size();
	
	if (arg_no >= n){
		ret->AddRecord(new CPredicate(name,part_pars));
	} else {
		deque<string>::iterator it;
		ParamRecord *pr;
		for(it = (*obj_list)[arg_no]->begin();it != (*obj_list)[arg_no]->end(); it++){
			CParameter *tmp_pars = part_pars->Clone();
			pr = new ParamRecord();
			pr->constant = (*it);
			pr->plisttypes = NULL;
			tmp_pars->AddRecord(pr);

			MakeAllInstancesRec(obj_list,tmp_pars,arg_no+1,ret);
		}

	}
}

CPredicateList* CPredicate::MakeAllInstances(CTypes* obejcts, CTypes* types)
{
	vector<deque<string>*> lists;
	CPredicateList *ret = new CPredicateList();
	int i;

	for (i=0;i<params->Count();i++){
		lists.push_back(types->FindAllObjects((*params)[i]->plisttypes,obejcts));
	}

	MakeAllInstancesRec(&lists,new CParameter(),0,ret);
	
	return ret;
}

list<string> CPredicate::GetPrologRepresentationTypes()
{
	list<string> r;
	string name=ToLower(this->name);

	if (this->params==NULL){
		r.push_back(name);
	} else {
	
		//ParamRecord pr;

		name+="(";
		this->GetAllTypesCombinations(&r,0,name);
	}
	return r;
}

void CPredicate::Negate()
{
	this->neg=!this->neg;

}

bool CPredicate::Negative()
{
	return this->neg;
}


string CPredicate::GetPrologRepresentationVars()
{
	string r=ToLower(this->name);
	
	if (this->params==NULL) return r;

	deque<ParamRecord>*	plist=this->params->GetPList();
	int n=plist->size();
	//ParamRecord pr;

	r+="(";
	for (int i=0;i<n;i++){
		if(!(*plist)[i].name.empty())
			r+=ToUpper((*plist)[i].name);
		else
			r+=ToLower((*plist)[i].constant);
		if (i<n-1) {
			r+=",";
		} else {
			r+=")";
		}
	}
	
	return r;
}

CPredicate* CPredicate::Instantiate(CParameter *oldp, CParameter *newp)
{
	CPredicate *ret;
	CParameter *newpredpar;
	deque<string> *cnames=new deque<string>();
	int j;

	if (this->params!=NULL){
		for (int i=0;i<this->params->Count();i++){
			if (!(*this->params)[i]->constant.empty()){ //is a constant
			    cnames->push_back((*this->params)[i]->constant);
			    continue;
			}
		        j=oldp->Find((*this->params)[i]->name);
			cnames->push_back((*newp)[j]->constant);
		
		}
	}
	newpredpar=new CParameter(cnames,true);
	ret=new CPredicate(this->name,newpredpar);
	if (this->Negative()) ret->Negate();
	if (this->IsEntangled()) ret->SetEntanglement(true);
	if (this->IsStatic()) ret->SetStatic(true);
	ret->SetIndirect(this->IsIndirect());

	return ret;
}

void CPredicate::SetAttribute(int a)
{
	this->d=a;
}

int CPredicate::GetAttribute()
{
	return this->d;
}

//test whether (ground) predicates are equal, then returns attribute d otherwise returns -1; for unground returns -1 if differ in parameter names
int CPredicate::EqualGround(CPredicate *p)
{
	if (strcasecmp(this->name.data(),p->name.data())!=0) return -1;
	bool empty1=this->params==NULL||this->params->Count()==0;
	bool empty2=p->params==NULL||p->params->Count()==0;
	if (empty1&&empty2) {return this->d;} //nulary predicates
	if (empty1||empty2) {return -1;} //only one nulary predicate
	if ((*this->params)==(*p->params)) {return this->d;} else {return -1;}
}

//test whether (unground) predicates are equal, then returns shared_args
bool CPredicate::Equal(CPredicate* p, CParameter *a1_pars, CParameter *a2_pars, vector<sh_arg_str>& sh_args,bool inc)
{
	int i,j,k;
	sh_arg_str x;

	if (strcasecmp(this->name.data(),p->name.data())!=0) return false;
	bool empty1=this->params==NULL||this->params->Count()==0;
	bool empty2=p->params==NULL||p->params->Count()==0;
	if (empty1&&empty2) return true;
	if (empty1||empty2) return false; 
	if (this->params->Count()!=p->params->Count()) return false;

	for (i=0;i<this->params->Count();i++){
		j=-1;
		do {
			j++;
		} while (strcasecmp((*this->params)[i]->name.data(), (*a1_pars)[j]->name.data())!=0 && j<a1_pars->Count()-1);
		k=-1;
		do {
			k++;
		} while (strcasecmp((*p->params)[i]->name.data(), (*a2_pars)[k]->name.data())!=0 && k<a2_pars->Count()-1);
	
		x.first = j;
		x.second = k;
		if (inc) {if (find(sh_args.begin(),sh_args.end(),x)==sh_args.end()) sh_args.push_back(x);} else
		{
		  vector<sh_arg_str>::iterator it=find(sh_args.begin(),sh_args.end(),x);
		  if (it!=sh_args.end()) sh_args.erase(it);
		}
		
	}

	//unique(sh_args.begin(),sh_args.end());

	return true;
}

string CPredicate::GetName()
{
	return this->name;
}

void CPredicate::SetName(string name)
{
	this->name = name;
}


CParameter* CPredicate::GetPars()
{
  	return this->params;
}


CPredicate* CPredicate::Clone()
{
	CPredicate *ret=new CPredicate(this->GetName(),this->GetPars()!=NULL ? this->GetPars()->Clone() : NULL);

	if (this->Negative()) ret->Negate();
	ret->SetAttribute(this->d);
	ret->SetStatic(this->stat);
	ret->SetEntanglement(this->entangled);
	ret->SetInnerEntanglement(this->inner_entangled);
	ret->SetIndirect(this->indirect);

	return ret;
}

void CPredicate::SetStatic(bool st)
{
	this->stat=st;
	this->entangled= st ? true : this->entangled;  //every static predicate is entangled
}

bool CPredicate::IsStatic()
{
	return this->stat;
}

void CPredicate::SetIndirect(bool st)
{
	this->indirect=st;
}

bool CPredicate::IsIndirect()
{
	return this->indirect;
}

//checks if p subsumes this (or backwards) - does not work for grounded predicates
bool CPredicate::Subsume(CPredicate *p,CTypes *types,bool oneway)
{
	int i,n,k,l;
	deque<string> *x1,*x2;
	bool ss,ss2;

	if (strcasecmp(this->GetName().data(),p->GetName().data())!=0) {return false;}

	n=this->GetPars()->Count();
	if (n!=p->GetPars()->Count()) {return false;}

	ss=true;
	for (i=0;i<n;i++){
		x1=(*this->GetPars())[i]->plisttypes;
		x2=(*p->GetPars())[i]->plisttypes;
		for (k=0;k<x1->size();k++){
			for (l=0;l<x2->size();l++){
				ss2=types->InFamilyLine((*x1)[k],(*x2)[l],oneway);
				if (ss2) break;
			}
			if (ss2) break;
		}
		if (!ss2) return false;
	}
	return true;

}

bool CPredicate::IsEntangled()
{
	//string s=this->GetPrologRepresentationVars();
	return this->entangled;
}

void CPredicate::SetEntanglement(bool ent)
{	
	this->entangled=ent;
}

void CPredicate::SetInnerEntanglement(short op_no)
{	
	this->inner_entangled=op_no;
}

short CPredicate::GetInnerEntanglement()
{
	return this->inner_entangled;
}

void CPredicate::ReplacePars(list<par_repl_str>& par_repl)
{
	params->ReplacePars(par_repl);
}

string CPredicate::ToString(bool types)
{
	string ret;

	if (Negative()) {
		ret+="(not (";
	} else {
		ret+="(";
	}
	ret+=name;
	ret+=params!=NULL ? params->ToString(types): "";
	ret+=")";
	if (Negative()) ret+=")";
	return ret;
}

string CPredicate::ToStringSpc(){
  string ret;
  ret+=name;
  //ret+=" ";
  if (params==NULL) return ret;
  for(int i=0;i<params->Count();i++){
    ret+=" ";
    ret+=(*(*params)[i]->plisttypes)[0];
  }
  ret=ToLower(ret);
  return ret;
  
}

//check whether a predicate is an instance of p
bool CPredicate::InstanceOf(CPredicate* p, CTypes *t)
{
	int i;
	string t1,t2;
	deque<string>::iterator it;
	bool b;
	
	if (strcasecmp(this->name.data(),p->name.data())!=0) return false;
	bool empty1=this->params==NULL||this->params->Count()==0;
	bool empty2=p->params==NULL||p->params->Count()==0;
	if (empty1&&empty2) return true;
	if (empty1||empty2) return false; 
	if (this->params->Count()!=p->params->Count()) return false;

	/* Debug reasons
	if (strcasecmp(this->name.data(),"on")==0){
		int x = 1; 
	}

	*/
	for (i=0;i<params->Count();i++){
		t1 = (*(*params)[i]->plisttypes)[0];
		b=false;
		for (it = (*p->params)[i]->plisttypes->begin();it!=(*p->params)[i]->plisttypes->end();it++){
			t2 = (*it);
			if (t->InFamilyLine(t2,t1)){
				b=true;
				break;
			}
		}
		if (!b) return false;
	}
	
	return true;
}

//////////////////////////////////////////////////////////////////////
// CPredicateList Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPredicateList::CPredicateList()
{
	this->ppredlist=new deque<CPredicate*>();
}

CPredicateList::~CPredicateList()
{
	for (deque<CPredicate*>::iterator it=ppredlist->begin();it!=ppredlist->end();it++) delete *it;
        delete this->ppredlist;
}

void CPredicateList::AddRecord(CPredicate *ppred)
{
	if (ppred!=NULL) ppredlist->push_back(ppred);
}

CPredicate* CPredicateList::operator[](unsigned int i)
{
	return (*this->ppredlist)[i];
}

list<string>* CPredicateList::GetPrologRepresentation()
{
	list<string>  *ret=new list<string>();
	list<string>  liststr;
	int n=this->ppredlist->size();
	string s,p;

	for (int i=0;i<n;i++){
		p="predicate(";
		liststr=(*this->ppredlist)[i]->GetPrologRepresentationTypes();	
		while (!liststr.empty()){
			s=p;
			s+=liststr.front();
			liststr.pop_front();
			s+=").";
			ret->push_front(s);
		}
	}
	
	return ret;
}



int CPredicateList::Count()
{
	return this->ppredlist->size();
}



CPredicateList* CPredicateList::Instantiate(CParameter *oldp, CParameter *newp)
{
	CPredicateList *ret=new CPredicateList();
	CPredicate *pred;

	for (int i=0;i<this->Count();i++){
		pred=(*this)[i]->Instantiate(oldp,newp);
		ret->AddRecord(pred);  //it is important to keep the predicates order
	}
	
	return ret;
}



bool CPredicateList::PrecondFulfilled(CPredicateList *prec, list<int> &ds)
{
	int d,j;
	
	for (int i=0;i<prec->Count();i++){
		if ((*prec)[i]->GetName()=="="){
		   bool eq =  (*(*prec)[i]->GetPars())[0]->constant == (*(*prec)[i]->GetPars())[1]->constant;
		   if (eq != (*prec)[i]->Negative()) continue; else return false;
		}
	        for (j=0;j<this->Count();j++){
			d=(*this)[j]->EqualGround((*prec)[i]);
			if (d!=-1) {
				if ((*prec)[i]->Negative()) return false;
			        ds.push_back(d);
				break;
			}
		}
		if (j==this->Count()&&!(*prec)[i]->Negative()) return false;
		(*prec)[i]->SetInnerEntanglement(d);
	}
	//ds.unique();
	return true;
}

bool CPredicateList::BackwardStep(CPredicateList *pos_eff, list<int> &ds)
{
	int d,j;
	
	for (int i=0;i<pos_eff->Count();i++){
		for (j=0;j<this->Count();j++){
			d=(*this)[j]->EqualGround((*pos_eff)[i]);
			if (d!=-1) {
				ds.push_back(d);
				break;
			}
		}
	}
	ds.unique();
	return true;
}

void CPredicateList::ApplyEffects(CPredicateList *eff, int d,bool noneg)
{
	int i,j;
	CPredicate *p;
	CParameter *par;

	for (i=0;i<eff->Count();i++){
		if (noneg&&(*eff)[i]->Negative()){ //negative effect - remove from the state
			for (j=0;j<this->Count();j++){
				if ((*this)[j]->EqualGround((*eff)[i])!=-1){
					this->ppredlist->erase(this->ppredlist->begin()+j);	
					j--;
				}
			}
		} else { //positive effect - add if not in the state
			for (j=0;j<this->Count();j++){ //if the predicate is already presented
				if ((*this)[j]->EqualGround((*eff)[i])!=-1){
					(*this)[j]->SetAttribute(d);
					break;
				}
			}
			if (j==this->Count()&&(*eff)[i]->GetName()!="total-cost"){ // otherwise
				par=new CParameter();
				(*par)+=(*(*eff)[i]->GetPars()->Clone());
			//	string sss=(*eff)[i]->GetName();
				p=new CPredicate((*eff)[i]->GetName(),par);
				p->SetAttribute(d);
				this->AddRecord(p);
			}
		}

	}
}




CPredicateList* CPredicateList::Clone() const
{
	CPredicateList *ret=new CPredicateList();
	deque<CPredicate*>::iterator it;
	for (it=ppredlist->begin();it!=ppredlist->end();it++){
		ret->AddRecord((*it)->Clone());
	}

	return ret;
}

bool CPredicateList::Disjunct(CPredicateList *pl)//for lists of grounded predicates
{
	int i,j;

	for (i=0;i<this->Count();i++){
		for (j=0;j<pl->Count();j++){
			if ((*this)[i]->EqualGround((*pl)[j])!=-1) {return false;} //share a predicate, not disjunct
		}
	}

	return true; //do not share any predicate
}

bool CPredicateList::DisjunctU(CPredicateList *pl, CTypes *t)
{
	int i,j;

	//for (i=0;i<this->Count();i++){
		for (j=0;j<pl->Count();j++){
			if ((this)->Subsume((*pl)[j],t)) {return false;} //share a predicate, not disjunct
		}
	//}

	return true; //do not share any predicate
}

//checks whether p subsumes some predicate from the list (this)
bool CPredicateList::Subsume(CPredicate *p, CTypes *types)
{
	int i;
	for (i=0;i<this->Count();i++){
		if ((*this)[i]->Subsume(p,types)) return true;
	}

	return false;
}

CPredicate* CPredicateList::SubsumeP(CPredicate* p, CTypes* t)
{
	
	int i;
	for (i=0;i<this->Count();i++){
		if ((*this)[i]->Subsume(p,t)) return (*this)[i];
	}
	
	return NULL;
}


int CPredicateList::Find(CPredicate *p)
{
	for (int i=0;i<this->Count();i++){
		if ((*this)[i]->EqualGround(p)!=-1) {return i;} 
		
	}

	return -1;
}

int CPredicateList::FindProperPredicate(string name)
{
      for (int i=0;i<this->Count();i++){
		if ((*this)[i]->GetName()==name) {return i;} 
		
	}

	return -1;
}


bool CPredicateList::Equal(CPredicateList* pl, CParameter* a1_pars, CParameter* a2_pars, vector<sh_arg_str>& sh_args, bool subset)
{
	bool *v = NULL;
	int i,j;
	vector<sh_arg_str>::iterator it;

	if (!subset) {
		v = new bool[pl->Count()];
		for (i=0;i<pl->Count();i++) *(v+i) =false;
	}

	for (i=0;i<Count();i++){
		for(j=0;j<pl->Count();j++){
			if (((*ppredlist)[i])->Equal((*pl)[j],a1_pars,a2_pars,sh_args)){
				if (!subset) {*(v+j)=true;}
				break;
			}
		}
		if (j==pl->Count()) return false; //we haven't found a matching predicate in pl
	}

	if (subset) {
		sort(sh_args.begin(), sh_args.end(), sh_arg_str_cmp);
		it=unique(sh_args.begin(), sh_args.end(), sh_arg_str_eq);
		sh_args.resize(it - sh_args.begin());
		return true;
	}

	for (i=0;i<pl->Count();i++) if (!(*(v+i))) return false;
	
	sort(sh_args.begin(), sh_args.end(), sh_arg_str_cmp);
	it=unique(sh_args.begin(), sh_args.end(), sh_arg_str_eq);
	sh_args.resize(it - sh_args.begin());
	return true;
}

void CPredicateList::ReplacePars(list<par_repl_str> &par_repl)
{
	deque<CPredicate*>::iterator it = ppredlist->begin(); 

	while (it!=ppredlist->end()){
		(*it)->ReplacePars(par_repl);
		it++;
	}
}


void CPredicateList::RemovePredicate(int i)
{
	(*ppredlist)[i]=NULL;
	ppredlist->erase(ppredlist->begin()+i);
}

CPredicateList & CPredicateList::operator-(CPredicateList& pl)
{
	CPredicateList *ret = this->Clone();
	int i,j;

	for(i=0;i<pl.Count();i++){
		for (j=0;j<ret->Count();j++){
			if ((*ret)[j]->EqualGround(pl[i])!=-1){
				ret->RemovePredicate(j);j--;
			}
		}
	}

	return *ret;

}

CPredicateList & CPredicateList::operator+(CPredicateList& pl){

	CPredicateList *ret = this->Clone();
	int i,j;
	for(i=0;i<pl.Count();i++){
		for (j=0;j<ret->Count();j++){
			if ((*ret)[j]->EqualGround(pl[i])!=-1) break;
		}
		if (j==ret->Count()) ret->AddRecord(pl[i]);
	}
	return *ret;
}

CPredicateList & CPredicateList::operator*(CPredicateList& pl){

	CPredicateList *ret = new CPredicateList();
	int i,j;
	for(i=0;i<pl.Count();i++){
		for (j=0;j<this->Count();j++){
			if ((*this)[j]->EqualGround(pl[i])!=-1) {
				ret->AddRecord(pl[i]);
				break;
			}
		}
	}
	return *ret;
}


string CPredicateList::ToString(bool types)
{
	string ret;
	deque<CPredicate*>::iterator it = ppredlist->begin();

	while (it!=ppredlist->end()){
		ret+=(*it)->ToString(types);
		if (types) ret+="\n"; //separator (not necessary)
		it++;
	}
	
	return ret;
}


CPredicateList* CPredicateList::Intersect(CPredicateList* pl)
{
	CPredicateList * ret = new CPredicateList();
	
	int i,j;

	for (i=0;i<this->Count();i++){
		for (j=0;j<pl->Count();j++){
			if ((*this)[i]->EqualGround((*pl)[j])!=-1) {ret->AddRecord((*this)[i]->Clone());break;} //share a (ground) predicate
		}
	}
	
	return ret;
}

bool CPredicateList::SubSetOf(CPredicateList* pl)
{
       int i,j;
       for (i=0;i<Count();i++){
		for(j=0;j<pl->Count();j++){
			if (((*ppredlist)[i])->EqualGround((*pl)[j])!=-1){
				break;
			}
		}
		if (j==pl->Count()) return false; //we haven't found a matching predicate in pl
	}
	
	return true;
}

CPredicateList* CPredicateList::ExtractStatic(int pars,bool noequality)
{
  CPredicateList* ret=new CPredicateList(); 
  for (deque<CPredicate*>::iterator it=ppredlist->begin();it!=ppredlist->end();it++){
    if ((*it)->IsStatic()&&(*it)->GetPars()->Count()>=pars&&(!noequality || (*it)->GetName()!="=")  ) ret->AddRecord(*it);
  }
  
  return ret;
    
}

