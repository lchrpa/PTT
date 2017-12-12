// CDependency.cpp: implementation of the CDependency class.
//
//////////////////////////////////////////////////////////////////////

#include "CDependency.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDependency::CDependency(CPlan *pi,CPredicateList *init,CPredicateList *goal)
{
	int n;
	
	this->plan=pi;
	this->init=init;
	this->goal=goal;

	n=this->plan->Length();

	
	this->dep_field=(unsigned char*)malloc((n+2)*(n+1)/2);
	for (int i=0;i<(n+2)*(n+1)/2;i++) {*(dep_field+i)=0;}
	
}

CDependency::~CDependency()
{
	delete this->dep_field;
}

void CDependency::Clear(void)
{
	int n=this->plan->Length();
	this->dep_field=(unsigned char*)realloc(this->dep_field,(n+2)*(n+1)/2);
	for (int i=0;i<(n+2)*(n+1)/2;i++) {*(dep_field+i)=0;}
}

int CDependency::position(int j, int i)
{
    if (j<=i) return -1;
	return (j*(j-1)/2)+i;
}


//computes relations of straight dependency and dependency (returns true while plan is valid)
bool CDependency::CreateDependencies()
{
	CPredicateList *curstate;
	list<int> attrs;
	int d,i,j;
	
	curstate=this->init->Clone();
	states.push_back(curstate->Clone());

	//go through plan
	for (i=0;i<this->plan->Length();i++){
		if (!curstate->PrecondFulfilled((*this->plan)[i]->GetPrec(),attrs)) {return false;} //plan is invalid
		while(!attrs.empty()){
			d=attrs.front();
			*(dep_field+this->position(i+1,d))=ACT_STRDEP|ACT_DEP;
			attrs.pop_front();
			for(j=0;j<d;j++){  //search for dependencies
				if (this->IsDep(d,j)) {*(dep_field+this->position(i+1,j))=ACT_DEP;}
			}
		}
		curstate->ApplyEffects((*this->plan)[i]->GetEff(),i+1);
		states.push_back(curstate->Clone());

	}
	//for goal
	if (!curstate->PrecondFulfilled(this->goal,attrs)) {return false;}
			while(!attrs.empty()){
			d=attrs.front();
			*(dep_field+this->position(i+1,d))=ACT_STRDEP|ACT_DEP;
			attrs.pop_front();
			for(j=0;j<d;j++){  //search for dependencies
				if (this->IsDep(d,j)) {*(dep_field+this->position(i+1,j))|=ACT_DEP;}
			}
		}

	return true;
}


//computes relations of independency (must be run after CrateDependencies())
bool CDependency::CreateIndependencies()
{
	int i,j;
	//go through plan
	for (i=1;i<=this->plan->Length();i++){
		for (j=i+1;j<=this->plan->Length();j++){
			if (!this->IsDep(j,i) &&
			    (*this->plan)[i-1]->GetPrec()->Disjunct((*this->plan)[j-1]->GetNegEff()) &&
				(*this->plan)[i-1]->GetNegEff()->Disjunct((*this->plan)[j-1]->GetPosEff()))
			{
				*(dep_field+this->position(j,i))=ACT_INDEP;	
			}

		}
	}

	return true;
}


//must be run after CreateIndependencies()
bool CDependency::FindPairsForAssemblage()
{
	return FindPairsForAssemblage(true);
}

bool CDependency::FindPairsForAssemblage(bool inc_dep)
{
	int i,j;
	deque<int> *mv_pred;

	//initialize move_pred matrix
	int n=this->plan->Length();
	move_pred_matrix = new vector<deque<int>*>(((n+2)*(n+1)/2),(deque<int>*)NULL);

	//go through plan
	for (i=1;i<=n;i++){
		for (j=i+1;j<=n;j++){
			if (!inc_dep||this->IsStrDep(j,i)){
				mv_pred = new deque<int>();
				if (this->CanBeAdjacent(i,j,*mv_pred)) {
					*(dep_field+this->position(j,i))|=ACT_ASSEMBLEABLE; 
					(*move_pred_matrix)[position(j,i)]=mv_pred;
				}	
			}

		}
	}

	return true;

}


//just for debug or tuning reasons
void CDependency::OutPut(ostream &str)
{
	int i,j;
	
	if (dep_field==NULL) {
		str << "No dependencies allocated" << endl;
		return;
	}

	str << "Straight dependencies:" << endl << endl;
	
	for (i=0;i<=this->plan->Length()+1;i++){
		for (j=0;j<i;j++){
			if (this->IsStrDep(i,j)) {
				str << j << ", " <<i <<endl;
			}
		}	
	}

	str << "Dependencies:" << endl << endl;
	
	for (i=0;i<=this->plan->Length()+1;i++){
		for (j=0;j<i;j++){
			if (this->IsDep(i,j)) {
				str << j << ", " <<i <<endl;
			}
		}	
	}

		str << "Independencies:" << endl << endl;
	
	for (i=0;i<=this->plan->Length()+1;i++){
		for (j=0;j<i;j++){
			if (this->IsIndep(i,j)) {
				str << j << ", " <<i <<endl;
			}
		}	
	}

			str << "Assembleable:" << endl << endl;
	
	for (i=0;i<=this->plan->Length()+1;i++){
		for (j=0;j<i;j++){
			if (this->IsAssembleable(j,i)) {
				str << j << ", " <<i <<endl;
			}
		}	
	}
}



//is i straightly dependent on j ?
bool CDependency::IsStrDep(int i, int j)
{
	unsigned char n=*(dep_field+this->position(i,j));
	
	return n&ACT_STRDEP;
}

//is i dependent on j ?
bool CDependency::IsDep(int i, int j)
{
	unsigned char n=*(dep_field+this->position(i,j));
	
	return n&ACT_DEP;
}


bool CDependency::IsIndep(int i, int j)
{
	unsigned char n=*(dep_field+this->position(i,j));
	
	return n&ACT_INDEP;
}

bool CDependency::IsAssembleable(int i, int j)
{
	unsigned char n=*(dep_field+this->position(j,i));
	
	return n&ACT_ASSEMBLEABLE;
}

deque<int>* CDependency::GetMovePred(int i,int j){
	return (*move_pred_matrix)[position(j,i)];
}

void CDependency::GoalNotDependent(list<int> &l)
{
	int i,n;
	n=this->plan->Length();
	
	for (i=1;i<=n;i++){
		if (!IsDep(n+1,i)){
			l.push_back(i);
		}
	
	}
}

void CDependency::InversePairs(list<int> &l)
{
	int i,j,k,n;
	n=this->plan->Length();
	bool other_str_dep;
	CPredicateList *curstate;
	list<int> test;

	for (i=2;i<n;i++){
		if (find(l.begin(),l.end(),i)==l.end()) //i cannot be in l (action marked for removal)
		for (j=1;j<i;j++){
			if (find(l.begin(),l.end(),j)==l.end()&&IsStrDep(i,j)) {  //if not straight dependency is in place here, then goal is not dependent on j
				other_str_dep=false;
				for (k=j+1;k<=n+1;k++){ //testing if other (than i-th) action is straightly dependent on j
					if (k!=i&&IsStrDep(k,j)) {
						other_str_dep=true;
						break;
					}
				}
				if (!other_str_dep){  
					curstate=this->states[j-1]->Clone();
					//apply actions
					if (curstate->PrecondFulfilled((*this->plan)[j-1]->GetPrec(),test)){
						curstate->ApplyEffects((*this->plan)[j-1]->GetEff(),0);
						if (curstate->PrecondFulfilled((*this->plan)[i-1]->GetPrec(),test)){
							curstate->ApplyEffects((*this->plan)[i-1]->GetEff(),0);
							if (curstate->PrecondFulfilled(this->states[j-1],test)){ //actions i and j are inverse and can be removed
								l.push_back(j);
								l.push_back(i);
							}
						}
					}
					delete curstate;
				}
			}

		}
	}
}



bool CDependency::CanBeAdjacent(int i, int j, deque<int> &move_pred)
{
	deque<int> intermediate_acts;
	int k,l;
	bool chg=false, testall;

	if (i>=j) return false; //notation reasons (assume i<j)
	if (i==j-1) return true; //actions are adjacent

	for (k=i+1;k<j;k++) intermediate_acts.push_back(k); 
	
	do {
		chg=false;
		//first case (can be made repeatedly)
		while (!intermediate_acts.empty()&&this->IsIndep(intermediate_acts.front(),i)) {
			chg=true;
			move_pred.push_back(intermediate_acts.front());
			intermediate_acts.pop_front();			
		}
		//second case (can be made repeatedly)
		while (!intermediate_acts.empty()&&this->IsIndep(j,intermediate_acts.back())) {
			chg=true;
			intermediate_acts.pop_back();
		}
		//third case
		k=-1;
		for (l=intermediate_acts.size()-1;l>=0;l--){
			if (!IsIndep(intermediate_acts[l],i)){
				k=l;
				break;
			}
		}
		if (k!=-1){
			testall=true;
			for(l=k+1;l<intermediate_acts.size();l++){ //testing all intermadiate actions left between intermediate_acts[k] and j whether they are independent on intermediate_acts[k]
				if (!IsIndep(intermediate_acts[l],intermediate_acts[k])){
					testall=false;
					break;
				}
			}
			if (testall&&IsIndep(j,intermediate_acts[k])){
				chg=true;
				intermediate_acts.erase(intermediate_acts.begin()+k);
			}
		}
		//fourth case
		k=-1;
		for (l=0;l<intermediate_acts.size();l++){
			if (!IsIndep(j,intermediate_acts[l])){
				k=l;
				break;
			}
		}
		if (k!=-1){
			testall=true;
			for(l=k-1;l>=0;l--){ //testing all intermadiate actions left between i and intermediate_acts[k] whether they are independent on intermediate_acts[k]
				if (!IsIndep(intermediate_acts[k],intermediate_acts[l])){
					testall=false;
					break;
				}
			}
			if (testall&&IsIndep(intermediate_acts[k],i)){
				chg=true;
				move_pred.push_back(intermediate_acts[k]);
				intermediate_acts.erase(intermediate_acts.begin()+k);
			}
		}

	} while (chg);

	return intermediate_acts.empty();

}


bool CDependency::CreateDependenciesWithOptimization(CDomain* dom, deque<inv_pair_str> &inverses)
{
	CPredicateList *curstate;
	list<int> attrs;
	int d,i,j;
	inv_pair_str ips;
	
	int n=plan->Length();

	curstate=this->init->Clone();

	//go through plan
	for (i=0;i<n;i++){
		if (!curstate->PrecondFulfilled((*this->plan)[i]->GetPrec(),attrs)) {return false;} //plan is invalid
		while(!attrs.empty()){
			d=attrs.front();
			*(dep_field+this->position(i+1,d))=ACT_STRDEP|ACT_DEP;
			attrs.pop_front();
			
			if (d>0 && dom->AreInverse((*plan)[d-1],(*plan)[i])){
				ips.first = d-1;
				ips.second = i;
				inverses.push_back(ips);
			}
			
			for(j=0;j<d;j++){  //search for dependencies
				if (this->IsDep(d,j)) {
					*(dep_field+this->position(i+1,j))|=ACT_DEP;
					if (j>0 && dom->AreInverse((*plan)[j-1],(*plan)[i])){
						ips.first = j-1;
						ips.second = i;
						inverses.push_back(ips);
					}
				}
			}
			
		}
		curstate->ApplyEffects((*this->plan)[i]->GetEff(),i+1);

	}
	//for goal
	if (!curstate->PrecondFulfilled(this->goal,attrs)) {return false;}
			while(!attrs.empty()){
			d=attrs.front();
			*(dep_field+this->position(i+1,d))=ACT_STRDEP|ACT_DEP;
			attrs.pop_front();
			for(j=0;j<d;j++){  //search for dependencies
				if (this->IsDep(d,j)) {*(dep_field+this->position(i+1,j))|=ACT_DEP;}
			}
		}

	deque<inv_pair_str>::iterator tmpit = unique(inverses.begin(),inverses.end());
	inverses.resize(tmpit - inverses.begin());
	sort(inverses.begin(),inverses.end());

	return true;
	/* DOES NOT WORK PROPERLY !!!
	curstate = goal->Clone();
	for(i=0;i<curstate->Count();i++) (*curstate)[i]->SetAttribute(n+1);
	
	//go through the plan (backwards)
	for(i=n-1;i>=0;i--){
		curstate->BackwardStep((*plan)[i]->GetPosEff(),attrs);
		if (attrs.empty()) unnecessaty_acts.push_front(i+1);
		while(!attrs.empty()){
			d=attrs.front();
			*(dep_field+this->position(d,i+1))=ACT_STRDEP|ACT_DEP;
			attrs.pop_front();
			for(j=n+1;j>d;j--){  //search for dependencies
				if (this->IsDep(j,d)) {*(dep_field+this->position(j,i+1))|=ACT_DEP;}
			}
		}
		curstate->ApplyEffects((*this->plan)[i]->GetPrec(),i+1,false);
	}
	*/
}

//comparison of dependencies - debug reasons..
bool CDependency::Equal(CDependency *d){
	
	int i,j;
	bool b;

	if (plan->Length() != d->plan->Length()) return false;

	for (i=1;i<=plan->Length()+1;i++){
		for (j=1;j<i;j++){
			b = IsStrDep(i,j)==d->IsStrDep(i,j);
			b = b & (IsDep(i,j)==d->IsDep(i,j));
			if (!b) {
				return false;
			}
		}	
	}

	return true;
}

void CDependency::RetrieveAchievers(vector< achievers >& achs, CActionList *acts)
{
   achs.resize(acts->Count());
   
   CPredicateList *curstate;
	list<int> attrs;
	int d,i,j,k;
	vector<int> *temp;
	bool found;
	
	curstate=this->init->Clone();
	
	//go through plan
	for (i=0;i<this->plan->Length();i++){
		if (!curstate->PrecondFulfilled((*this->plan)[i]->GetPrec(),attrs)) {cout << "Plan is invalid" << endl;return;}
		temp = new vector<int>();
		acts->FindProperAction((*this->plan)[i]->GetActName(), k);
		while(!attrs.empty()){
			d=attrs.front();
			attrs.pop_front();
			if (d==0) j=-1; else acts->FindProperAction((*this->plan)[d-1]->GetActName(), j);
			temp->push_back(j);
		}
		curstate->ApplyEffects((*this->plan)[i]->GetEff(),i+1);
		found=false;
		for (achievers::iterator it=achs[k].begin();it!=achs[k].end();it++){
		   if ((*(it->acts))==*temp){
		     it->freq++;
		     found=true;
		     break;
		   }
		}
		if (!found){
		   achiever *a = new achiever();
		   a->acts=temp;
		   a->freq=1;
		   achs[k].push_back(*a);
		}
	}
}
