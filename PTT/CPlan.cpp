// CPlan.cpp: implementation of the CPlan class.
//
//////////////////////////////////////////////////////////////////////

#include "CPlan.h"

#include "CDependency.h"

extern string ToLower(string);
extern string ToUpper(string);
extern string LTrim(string);
extern deque<string>* ExplodeBySpace(string,const char* c=" \t");

bool MacroReplSort(const MacroReplStr &first, const MacroReplStr &second){
  return first.move_pred->size()<second.move_pred->size();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlan::CPlan()
{
    this->plan=new deque<PlanRecord>();
	this->act_plan=NULL;
}

CPlan::~CPlan()
{
	delete this->plan;
}

void CPlan::ParsePlanFile(string name)
{
	fstream f;
	string str;
	int i=1;
	unsigned int s1,s2;
	deque<string> *args,*args2; //arguments gathered from a single step of the plan (action name and its arguments)
	PlanRecord  pr;

	f.open(name.data(),fstream::in);

	while(!f.eof()){
		getline(f,str);
		str=LTrim(str);
		if (!str.empty()&&str[0]!=';'){
			if (str.find("STEP")==0){ //for MP
				s1 = str.find(":");
				if (s1==  -1) continue;
				s1+=2;
				str=str.substr(s1,str.length()-s1);
				args=ExplodeBySpace(str);
				for (deque<string>::iterator it=args->begin();it!=args->end();it++){
					s2=(*it).find("(");
					if (s2== -1){ 
						pr.actname=*it; 
						pr.params=new CParameter();  //no action arguments
						this->plan->push_back(pr);
						break;
					}
					pr.actname=(*it).substr(0,s2);
					(*it)=(*it).substr(s2+1,(*it).find(")")-s2-1);
					pr.params=new CParameter(ExplodeBySpace((*it),","),1);  //assign action arguments
					this->plan->push_back(pr);
				}
				continue;
			}
			s1=str.find("(");
			s2=str.find(")");
			//if (s1 ==  -1){ //for FF like-outputs
			if (s1 == -1){
				s1 = str.find(":");
				if (s1==  -1) continue;
				s1+=1;
				s2 = str.length();
			}
			str=str.substr(s1+1,s2-s1-1);
			args=ExplodeBySpace(str);
		//	for_each(args->begin(),args->end(),ToLower);
			pr.actname=args->front();   //assing action name
			args->pop_front();
			pr.pos=i;	//assign position
			pr.params=new CParameter(args,1);  //assign action arguments
			this->plan->push_back(pr);
			i++;
		}
	}

	f.close();
}

list<string>* CPlan::GetPrologRepresentation(string prob)
{
	list<string> *ret=new list<string>();
	int i,j,n,n2,k;
	string s;
	PlanRecord pr;
	char a[256];

	/*
	n=this->plan->size();
	for(i=0;i<n;i++){
		pr=(*this->plan)[i];
		s="plan_giv(";
		s+=prob;
		s+=",";
		s+=itoa(pr.pos,a,10);
		s+=",";
		s+=ToLower(pr.actname);
		s+=",[";
		n2=pr.params->Count();  //arguments
		for(j=0;j<n2;j++){
			s+=ToLower((*pr.params)[j]->constant);
			if (j<n2-1) {s+=",";} else {s+="]";}
		}
		s+=").";
		ret->push_back(s);
	}
	*/
	return ret;
}

void CPlan::InstantiateActions(CActionList *acts)
{
	CAction *a,*b;
	int k=0;

	this->act_plan=new vector<CAction*>();
	for (int i=0;i<this->plan->size();i++){
		a=acts->FindProperAction((*this->plan)[i].actname,k); //find proper action (operator)
		//cout << "Finding: " << (*this->plan)[i].actname << " - found:" << a->GetActName() << endl;
		b=a->Instantiate((*this->plan)[i].params);
		this->act_plan->push_back(b);
	}

}

int CPlan::Length()
{
	return this->act_plan->size();
}

CAction* CPlan::operator [](unsigned int n){
	
	return (*this->act_plan)[n];

}

void CPlan::RemoveAction(int i)
{
	this->plan->erase(this->plan->begin()+i);
	this->act_plan->erase(this->act_plan->begin()+i);
}

bool CPlan::ModifyPlan(list<int>& unnecessary_acts, list<MacroReplStr>& repl_acts, CPredicateList *init, CPredicateList *goal)
{
	if (unnecessary_acts.empty()&&repl_acts.empty()) return false;
	
	int i;
	list<PlanModifer> plan_modifiers;
	
	list<int>::iterator it;
	deque<int>::iterator it2;
	list<MacroReplStr>::iterator rit;
	list<PlanModifer>::iterator tmpit;

	PlanModifer pm;
	int n=act_plan->size();


	vector<list<PlanModifer>::iterator> iters;
	int positions[n];
	int positions_rev[n];

	for (i=0;i<n;i++){
		pm.a = (*act_plan)[i];
		pm.pos = i+1;
		pm.mod = 0;
		plan_modifiers.push_back(pm);
		iters.push_back(--plan_modifiers.end());
		positions[i]=i;
		positions_rev[i]=i;
	}
	
	for (it=unnecessary_acts.begin();it!=unnecessary_acts.end();it++){
		(*iters[(*it)-1]).mod |= ACT_REMOVE; 
		unnecessary_acts.erase(it);it--;
	}

	//repl_acts.sort(MacroReplSort);
	for (rit=repl_acts.begin();rit!=repl_acts.end();rit++){
		/* replaced by a new code
		for (it2=(*rit).move_pred->begin();it2!=(*rit).move_pred->end();it2++){
			tmpit=plan_modifiers.insert(iters[(*rit).i-1],*iters[(*it2)-1]);
			(*iters[(*it2)-1]).mod |= ACT_REMOVE;
			(*tmpit).mod |= ACT_BEING_REPLACED;
			iters[(*it2)-1]=tmpit;
		}
		pm.a = (*rit).a;
		pm.mod = ACT_NEW;
		tmpit=plan_modifiers.insert(iters[(*rit).i-1],pm);
		iters[(*rit).i-1]=tmpit;
		*/
		bool issue = (*rit).j-(*rit).i < positions_rev[(*rit).j-1] - positions_rev[(*rit).i-1];
		if (!issue)
		  for (int q=positions_rev[(*rit).i-1];q<positions_rev[(*rit).j-1];q++){
		    if (positions[q]>(*rit).j-1) {issue=true;break;}
		  }
		
		if (issue){
		   //cout <<"Is that happenig ???" << (*rit).i << " " << (*rit).j <<endl; 
		   for (int xx=0;xx<n;xx++){if (positions[positions_rev[xx]]!=xx) cout << "Inconsistent: " <<xx<<endl;}
		   iters[(*rit).i-1]->mod=0;
		   iters[(*rit).j-1]->mod=0;
		   continue;
		}
		
		iters[(*rit).i-1]->mod=ACT_NEW;
		iters[(*rit).i-1]->a=(*rit).a;
		
		
		for (it2=(*rit).move_pred->begin();it2!=(*rit).move_pred->end();it2++){
		    int k=positions_rev[(*it2)-1];
		    int l=positions_rev[(*rit).i-1];
		    for (int z=k;z>l;z--){
		        positions[z]=positions[z-1];
			positions_rev[positions[z-1]]++;
		    }
		    if (k>l){
		      positions[l]=(*it2)-1;
		      positions_rev[(*it2)-1]=l;
		    }
		    
		}
		rit=repl_acts.erase(rit);rit--;
	}

	//rebuilding actplan
	//delete act_plan;
	act_plan=new vector<CAction*>();

	/*
	while (!plan_modifiers.empty()){
		pm = plan_modifiers.front();
		if ((pm.mod&ACT_REMOVE)==0){
			act_plan->push_back(pm.a);
		}
		plan_modifiers.pop_front();
	}
	*/
	
	for (int i=0;i<n;i++){
	    if ((iters[positions[i]]->mod&ACT_REMOVE)==0){
			act_plan->push_back(iters[positions[i]]->a);
		} else {
		  for (int j=i+1;j<n;j++) {positions_rev[positions[j]]--;}
		}
	}
	if (!repl_acts.empty()&&init!=NULL&&goal!=NULL){
	     
	    CDependency *tmpdep=new CDependency(this,init,goal);
	    bool correct = tmpdep->CreateDependencies();
	    tmpdep->CreateIndependencies();
	    tmpdep->FindPairsForAssemblage();
	   // GeneratePDDLPlan(cout); //debug
	    for (rit=repl_acts.begin();rit!=repl_acts.end();rit++){
	       //cout << "Before: " << (*act_plan)[rit->i-1]->ToString() << " and " << (*act_plan)[rit->j-1]->ToString() << endl;
	       rit->i=positions_rev[rit->i-1]+1;
	       rit->j=positions_rev[rit->j-1]+1;
	       if (tmpdep->IsAssembleable(rit->i,rit->j)){
		  unnecessary_acts.push_back(rit->i);
		  unnecessary_acts.push_back(rit->j);
		  rit->move_pred=tmpdep->GetMovePred(rit->i,rit->j);
	       } else {
		 rit=repl_acts.erase(rit);rit--;
	       }
	     // cout << "After: " << (*act_plan)[rit->i-1]->GetActName() << " and " << (*act_plan)[rit->j-1]->GetActName() << endl;
	    }
	   ModifyPlan(unnecessary_acts,repl_acts,init,goal);
	}
	
	return true;
}

void CPlan::GeneratePDDLPlan(ostream& s)
{
	s << "; generated by PTT" << endl;

	vector<CAction*>::iterator it;
	int i=0,j,n;

	for (it=act_plan->begin();it!=act_plan->end();it++){
		s << i <<": (" << (*it)->GetActName();
		n=(*it)->GetParams()->Count(); 
		for (j=0;j<n;j++){
			s << " " << (*(*it)->GetParams())[j]->constant;
		}
		s << ") [1]" << endl;
		i++;
	}
}
