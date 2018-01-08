 // CLearner.cpp: implementation of the CLearner class.
//
//////////////////////////////////////////////////////////////////////

#include "CLearner.h"
#include "common.h"

#include <strings.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern PTT_Data data;
extern void revert_ah_arg_str(vector<sh_arg_str>&);
extern bool sh_arg_str_cmp(sh_arg_str s1, sh_arg_str s2);

CLearner::CLearner(){
	
}

CLearner::~CLearner()
{

	this->ClearEntStr();
	this->ClearInnerEntStr();

}

void CLearner::Initialize(short par)
{
	switch(par){
		case 1: data.pdom->GetActions()->MakeIndirectEffects();
			break;
		default:
			break;
	}
	
	for (int i=0;i<data.train->size();i++)
		(*data.train)[i]->GetPlan()->InstantiateActions(data.pdom->GetActions());
	dependencies.assign(data.train->size(),NULL);
	this->frequency=new int[data.pdom->GetActions()->Count()];
	memset(frequency, 0, data.pdom->GetActions()->Count()*sizeof(int));
}

bool CLearner::CreateDependencies()
{
	bool some_plan_invalid=false;
	CDependency *dep;

	for (int i=0;i<data.train->size();i++){
		//(*data.train)[i]->GetPlan()->InstantiateActions(data.pdom->GetActions());
		//dep=dependencies[i];
		//do {
		//	if (dep!=NULL) {delete dep;}
			dep=new CDependency((*data.train)[i]->GetPlan(),(*data.train)[i]->GetInit(),(*data.train)[i]->GetGoal());
			cout << "Problem: " << (*data.train)[i]->GetName() << " - Length of plan: " << (*data.train)[i]->GetPlan()->Length() << endl; //for debug reasons
			if (!dep->CreateDependencies()) { 
				some_plan_invalid=true;
				cout << "Bad Plan:"  << endl; //for debug reasons
				(*data.train)[i]->GetPlan()->GeneratePDDLPlan(cout);
				//break;
				dependencies[i]=dep;
				continue;
			}
		//optimize training plans
		//} while (!some_plan_invalid&&this->OptimizePlan((*data.train)[i]->GetPlan(),dep));
			

		if (!dep->CreateIndependencies()){
			cout << "Bad Plan(independencies):"  << endl; //for debug reasons
			break;
		}

		if (!dep->FindPairsForAssemblage()){
			cout << "Bad Plan(pairs for assemblage):"  << endl; //for debug reasons
			break;
		}
	//	dep->OutPut(cout); //for debug reasons
	//	cout << "Problem: " << (*data.train)[i]->GetName() << " - Length of plan after optimization: " << (*data.train)[i]->GetPlan()->Length() << endl; //for debug reasons
		//this->dependencies.push_back(dep);
		dependencies[i]=dep;
	}
	
	return !some_plan_invalid;
}


void CLearner::FindInverseActions(CPlan* pi, CDependency* dep, list<int> &unnecessaty_acts)
{
	int i,j,k;
	int n = pi->Length();
	bool innerdep;

	for(i=0;i<n;i++){
		if (find(unnecessaty_acts.begin(),unnecessaty_acts.end(),i+1)!=unnecessaty_acts.end()) continue; //the action is already marked for removel
		for(j=i+1;j<n;j++){
			if (find(unnecessaty_acts.begin(),unnecessaty_acts.end(),j+1)!=unnecessaty_acts.end()) continue; //the action is already marked for removel
			if (data.pdom->AreInverse((*pi)[i],(*pi)[j])){
				innerdep = false;
				for (k=i+1;k<j;k++){
					innerdep = innerdep || dep->IsStrDep(k+1,i+1);
					if (innerdep) {
						break;
					}
				}
				if (!innerdep){
					unnecessaty_acts.push_back(i+1);
					unnecessaty_acts.push_back(j+1);
				}
			}

			
			/* old and inefficient
			if (dep->IsAssembleable(i+1,j+1)){
				if (data.pdom->AreInverse((*pi)[i],(*pi)[j])){
					unnecessaty_acts.push_back(i+1);
					unnecessaty_acts.push_back(j+1);
				}
			}
			*/
		}
	}
}

bool CLearner::MarkUnnecessaryInverseActions(CPlan* pi, CDependency* dep, deque<inv_pair_str>& inverses, list<int>& unnecessary_acts)
{
	deque<inv_pair_str>::iterator it,it2; 
	bool innerdep, linear;
	int k;
	CPredicateList *pl;

	linear=true;

	it = inverses.begin();

	while(it!=inverses.end()){
		if (find(unnecessary_acts.begin(),unnecessary_acts.end(),it->first+1)!=unnecessary_acts.end() ||
			find(unnecessary_acts.begin(),unnecessary_acts.end(),it->second+1)!=unnecessary_acts.end()){it++; continue;}
		innerdep = false;
		
		/*
		if (it->first == 40) {
			k=0;
		}
		*/
		pl = new CPredicateList();
		for (k=it->first+1;k<it->second;k++){
			if (find(unnecessary_acts.begin(),unnecessary_acts.end(),k+1)!=unnecessary_acts.end()){
				(*pl)=(*pl)+(*(*pi)[it->first]->GetPosEff()->Clone()->Intersect((*pi)[k]->GetPosEff()));
			} else {
				innerdep = innerdep || (dep->IsStrDep(k+1,it->first+1) || !pl->Disjunct((*pi)[k]->GetPrec()));
			}
			if (innerdep) break;
		}
		delete pl;
		if (!innerdep){
			unnecessary_acts.push_back(it->first+1);
			unnecessary_acts.push_back(it->second+1);
			//go though unremoved pairs to reveal "linear breakers"
			it2 = it;
			if (it!=inverses.begin()){
				do {
					it2--;
					if (it2->first<it->second && it2->second>it->second && dep->IsStrDep(it->second,it2->first)){
						for (k=it->first+1;k<it2->second;k++){
							if (k!=it->second-1 && dep->IsStrDep(k+1,it2->first+1)) break;
						}
						if (k==it2->second){
							linear = false;	
							cout << "Must be taken once more..." << it->first <<"," << it->second << " - " << it2->first <<"," << it2->second  <<endl; //debug reasons
						}
					}
				} while (linear && it2!=inverses.begin());
			}
			it = inverses.erase(it);
			continue;
		} 
		it++;
	}

	return linear;
}

void CLearner::MarkUnnecessaryNestedInverseActions(CPlan* pi, CDependency* dep, deque<inv_pair_str>& inverses, list<int>& unnecessary_acts)
{
	deque<inv_pair_str>::iterator it,it2; 
	int k;
	bool firstok,secondok;
	list<int> temp;
	CPredicateList *pl;

	firstok = secondok = false;

	for (it = inverses.begin();it!=inverses.end();it++){
		if (find(unnecessary_acts.begin(),unnecessary_acts.end(),it->first+1)!=unnecessary_acts.end() ||
			find(unnecessary_acts.begin(),unnecessary_acts.end(),it->second+1)!=unnecessary_acts.end()){continue;}
		it2 = it+1;
		firstok = secondok = false;
		while(it2!=inverses.end()){
			if (find(unnecessary_acts.begin(),unnecessary_acts.end(),it2->first+1)!=unnecessary_acts.end() ||
			find(unnecessary_acts.begin(),unnecessary_acts.end(),it2->second+1)!=unnecessary_acts.end()){it2++; continue;}
			if (it2->first<it->first && it2->second>it->second){//nested inverses..
				//cout << "Nested inverses to explore..." << it->first <<"," << it->second << " - " << it2->first <<"," << it2->second  <<endl; //debug reasons
				firstok = true;
				pl = (*pi)[it2->first]->GetPosEff()->Clone()->Intersect((*pi)[it->second]->GetPosEff());
				for (k=it2->first+1;k<it2->second;k++){
					if (k!=it->first && k!=it->second && (dep->IsStrDep(k+1,it2->first+1) || (k>it->second && !pl->Disjunct((*pi)[k]->GetPrec())))){
						firstok = false;
						break;
					}
				}
				if (!firstok) {it2++;continue;}
				
				for (k=it->first+1;k<it->second;k++){
					secondok=true;
					if (dep->IsStrDep(k+1,it->first+1)){
						CPredicateList* preds = (*pi)[it->first]->GetPosEff()->Intersect((*pi)[k]->GetPrec());
						if (!(*pi)[it2->first]->GetPrec()->PrecondFulfilled(preds,temp)) {
							secondok = false;
							break;
						}
					}
				}
				if (secondok) break;
			}
			it2++;
		}
		if (firstok && secondok){
			cout << "Will be removed..." << it->first <<"," << it->second << " - " << it2->first <<"," << it2->second  <<endl; //debug reasons
			unnecessary_acts.push_back(it->first+1);
			unnecessary_acts.push_back(it2->first+1);
			unnecessary_acts.push_back(it->second+1);
			unnecessary_acts.push_back(it2->second+1);

		}
	}

}

void CLearner::FindShrinkableActions(CPlan* pi, CDependency* dep, list<int> &unnecessaty_acts, list<MacroReplStr> &replacable_acts)
{
	int i,j,k,l;
	int n = pi->Length();
	CMacroAction *ma;
	vector<sh_arg_str> *sh_args, *tmp_sh_args;
	repl_matrix_field *rf;
	MacroReplStr *rpl;
	list<MacroReplStr>::iterator tmprit;
	bool xx;

	for(i=0;i<n;i++){
		if (find(unnecessaty_acts.begin(),unnecessaty_acts.end(),i+1)!=unnecessaty_acts.end()) continue; //the action is already marked for removel
		for(j=i+1;j<n;j++){
			if (find(unnecessaty_acts.begin(),unnecessaty_acts.end(),j+1)!=unnecessaty_acts.end()) continue; //the action is already marked for removel
			if (dep->IsAssembleable(i+1,j+1)){
				sh_args = new vector<sh_arg_str>();
				(*pi)[i]->DetectSharedArgs((*pi)[j],*sh_args);
				rf = new repl_matrix_field();
				//for debug reasons..
/*				if ((*pi)[i]->GetActName()=="Drive" && (*pi)[j]->GetActName()=="Drive" && sh_args->size()>=2){
					int xxx =0;
				}
*/				
				if (data.pdom->ReplacableBy((*pi)[i],(*pi)[j],*sh_args,*rf)){
					////////////////////////
					//just for "quick fix"
					
					tmprit = replacable_acts.end();
					xx = false;
					while (tmprit != replacable_acts.begin()){
						tmprit--;
						//detect interferences..
						if (i+1<(*tmprit).j && j+1>(*tmprit).j) {
							xx = true;
							break;
						}
					}
					if (xx) continue;
					
					///////////////////////
					
					rpl = new MacroReplStr();
					rpl->i = i+1;
					rpl->j = j+1;
					rpl->move_pred = dep->GetMovePred(i+1,j+1);
					CParameter *tmp_pars = (*pi)[i]->GetParams()->Clone();
					for(k=0;k<(*pi)[j]->GetParams()->Count();k++){
						for(l=0;l<sh_args->size();l++){
							if ((*sh_args)[l].second == k) {
								break;
							}
						}
						if (l==sh_args->size()) {
							tmp_pars->AddRecord((*(*pi)[j]->GetParams())[k]);		
						}
					}
					revert_ah_arg_str(*rf->shared_args);
					sort(rf->shared_args->begin(), rf->shared_args->end(), sh_arg_str_cmp);
					CParameter *tmp_pars2 = new CParameter();
					for (k=0;k<rf->shared_args->size();k++){
						tmp_pars2->AddRecord((*tmp_pars)[(*rf->shared_args)[k].second]);
					}
					rpl->a = rf->a->Instantiate(tmp_pars2);
					replacable_acts.push_back(*rpl);
					unnecessaty_acts.push_back(i+1);
					unnecessaty_acts.push_back(j+1);
					cout << "repl suceeded: " << (*pi)[i]->GetActName() << "&" <<(*pi)[j]->GetActName() << " sh_args " << sh_args->size() << " by " << rf->a->GetActName() <<endl;
				}
			}
		}
	}
}


//optimize single plan by the given dependency relation
bool CLearner::OptimizePlan(CPlan *pi, CDependency *dep,ostream &s)
{
	list<int> unnecessary_acts;
	deque<inv_pair_str> inverses;
	list<MacroReplStr> replacable_acts;
	int counter=0,i;
	int size=0;

	if (!dep->CreateDependenciesWithOptimization(data.pdom,inverses)) { 
		cout << "Bad Plan:"  << endl; //for debug reasons
		s << "Bad Plan:"  << endl; //for debug reasons
		return false;
	}
 if (data.goal){
	dep->GoalNotDependent(unnecessary_acts);
	size = unnecessary_acts.size();
	s << size << ";";
 }
	//dep->InversePairs(unnecessary_acts);
	//FindInverseActions(pi,dep,unnecessary_acts); bool linear=false;

 if (data.inv){
	bool linear = MarkUnnecessaryInverseActions(pi,dep,inverses,unnecessary_acts);

	size = unnecessary_acts.size()-size;
	s << size << ";";
	if (!unnecessary_acts.empty()){
		unnecessary_acts.unique();
		unnecessary_acts.sort();
		pi->ModifyPlan(unnecessary_acts,replacable_acts);
		if (!linear) return true;

		unnecessary_acts.clear();
		inverses.clear();
		dep->Clear();
		//dep->CreateDependencies();
		if (!dep->CreateDependenciesWithOptimization(data.pdom,inverses)) { 
			cout << "Bad Plan:"  << endl; //for debug reasons
			s << "Bad Plan:"  << endl; //for debug reasons
			return false;
		}
	}
	size =0;
 }
	
 if (data.grinv){
	bool repeat = false;
   do {
	MarkUnnecessaryNestedInverseActions(pi,dep,inverses,unnecessary_acts);
	size+=unnecessary_acts.size();	
	//s << unnecessary_acts.size() << ";";
	repeat = false;
	if (!unnecessary_acts.empty()){
		unnecessary_acts.unique();
		unnecessary_acts.sort();
		pi->ModifyPlan(unnecessary_acts,replacable_acts);
	
		unnecessary_acts.clear();
		inverses.clear();
		dep->Clear();
		//dep->CreateDependencies();
		if (!dep->CreateDependenciesWithOptimization(data.pdom,inverses)) { 
			cout << "Bad Plan:"  << endl; //for debug reasons
			s << "Bad Plan:"  << endl; //for debug reasons
			return false;
		}
		repeat = true;
	} 
   } while (repeat);
	s << size << ";";
	size=0;
 }
//	return false;
 if (data.repl){
	
	if(!unnecessary_acts.empty()){
		pi->ModifyPlan(unnecessary_acts,replacable_acts);
		unnecessary_acts.clear();
		if (!dep->CreateDependenciesWithOptimization(data.pdom,inverses)) { 
			cout << "Bad Plan:"  << endl; //for debug reasons
			s << "Bad Plan:"  << endl; //for debug reasons
			return false;
		}
	}

	dep->CreateIndependencies();
	dep->FindPairsForAssemblage();

	FindShrinkableActions(pi,dep,unnecessary_acts,replacable_acts);
	unnecessary_acts.unique();
	unnecessary_acts.sort();
	
	size=replacable_acts.size();
	s << size << ";";

	return pi->ModifyPlan(unnecessary_acts,replacable_acts);

 }
	//return false; 

	/*
	while(!unnecessary_acts.empty()){
		i=unnecessary_acts.front();
		unnecessary_acts.pop_front();
		pi->RemoveAction(i-1-counter);
		counter++;
	}
	*/

	//return counter>0;
	return false;
}

bool CLearner::OptimizePlans(void)
{
	CDependency *dep,*dep2;
	list<int> unnecessary_acts; //debug..
	fstream f; //output..

	f.open((data.pdom->GetName()+"-opt.data").data(),fstream::out|fstream::trunc);

	data.pdom->MakeInvMatrix();

	data.pdom->OutInv(cout);

	for(int i=0;i<data.train->size();i++){
		dep = (dependencies)[i];
		if (dep!=NULL) delete dep;
		dep = new CDependency((*data.train)[i]->GetPlan(),(*data.train)[i]->GetInit(),(*data.train)[i]->GetGoal());
		//dep2 = new CDependency((*data.train)[i]->GetPlan(),(*data.train)[i]->GetInit(),(*data.train)[i]->GetGoal()); //debug reasons
		
		//debug..
		/*
		dep->CreateDependencies();
		dep2->CreateDependenciesWithOptimization(data.pdom,unnecessary_acts);
		dep->OutPut(cout);
		dep2->OutPut(cout);
		getchar();
		*/
		dependencies[i]=dep;
		
		cout << "Problem: " << (*data.train)[i]->GetName() << " - Length of plan: " << (*data.train)[i]->GetPlan()->Length() <<  /*" ver: " << dep->Equal(dep2) <<*/ endl; //for debug reasons
		
		f << (*data.train)[i]->GetName() << ";" << (*data.train)[i]->GetPlan()->Length() << ";";

		if (!OptimizePlan((*data.train)[i]->GetPlan(),dep,f)){
			f << (*data.train)[i]->GetPlan()->Length() << endl;
			cout << "Problem: " << (*data.train)[i]->GetName() << " - Length of OPTIMIZED plan: " << (*data.train)[i]->GetPlan()->Length() <<  /*" ver: " << dep->Equal(dep2) <<*/ endl; //for debug reasons	
			continue;
		}
		f <<endl;
		i--;
	}

	f.close();

	return true;
}

//Can be called only after CreateDependencies()
void CLearner::CreateMatrixOfCandidates()
{
	int i,j,m,a,b;
	int n=data.pdom->GetActions()->Count();
	CPlan *tr_plan;
	field_of_matrix *cell;
	pair_str pair;
	CAction *a1,*a2;
	
	this->matrix=new field_of_matrix[n*n];
	for (i=0;i<n*n;i++){//initialize matrix
		(matrix+i)->counter=0;
		(matrix+i)->shared_args=new vector<sh_arg_str>();
		(matrix+i)->pairs=new list<pair_str>();
	}
	this->frequency=new int[n];
	for (i=0;i<n;i++) {*(this->frequency+i)=0;}
	this->totalplanslength=0;
	
	for (int k=0;k<data.train->size();k++){ //for each problem
		m=(*data.train)[k]->GetPlan()->Length();
		totalplanslength+=m; //increase total plans lenght
		for (i=0;i<n*n;i++){//initialize unused acts
			(matrix+i)->unused_acts=new unsigned char[m];
			for(j=0;j<m;j++){
				*((matrix+i)->unused_acts+j)=1;
			}
		}
		
		tr_plan=(*data.train)[k]->GetPlan();
		for (i=1;i<=tr_plan->Length();i++){
			a1=data.pdom->GetActions()->FindProperAction((*tr_plan)[i-1]->GetActName(),a);
			(*(this->frequency+a))++;
			for (j=i+1;j<=tr_plan->Length();j++){
				if (this->dependencies[k]->IsAssembleable(i,j)){
					a2=data.pdom->GetActions()->FindProperAction((*tr_plan)[j-1]->GetActName(),b);
					cell=this->matrix+n*a+b;
					if (*(cell->unused_acts+i-1)==1&&*(cell->unused_acts+j-1)==1){ //no duplication of actions in the same field
						vector<sh_arg_str> *temp=new vector<sh_arg_str>(*cell->shared_args);
					        (*tr_plan)[i-1]->DetectSharedArgs((*tr_plan)[j-1],*cell->shared_args);
						if (cell->shared_args->empty()) {cell->shared_args=temp;continue;}
					        *(cell->unused_acts+i-1)=*(cell->unused_acts+j-1)=0;
						cell->counter++;
						pair.prob=k;
						pair.first=i;
						pair.second=j;
						cell->pairs->push_back(pair);
											}
				}
			}
		}
		
	}

	this->OutputMatrix(cout); //for debug reasons
	
}

void CLearner::OutputMatrix(ostream &s)
{
	CActionList *acts=data.pdom->GetActions();
	int n=acts->Count();
	field_of_matrix *cell;

	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			cell=this->matrix+n*i+j;
			if (cell->counter>0){
				s << (*acts)[i]->GetActName() << "(" << *(this->frequency+i) << "x)->" << (*acts)[j]->GetActName() << "(" << *(this->frequency+j) << "x)" << " - " << cell->counter << "x  - ";
				for (int k=0;k<cell->shared_args->size();k++){
					if (k>0) s << ", ";
					s << "[" << (*cell->shared_args)[k].first << "," << (*cell->shared_args)[k].second << "]";
				}
				s << endl;
			}
		}
	}

}

// Learrns macros according to the KER paper
void CLearner::LearnMacros(void)
{
	field_of_matrix *cell;
	int si,sj;
	double maxA,x;
	list<int> unn;
	list<MacroReplStr> repl_acts;
	MacroReplStr rst;

	while (true){
	CActionList *acts=data.pdom->GetActions();
	int n=acts->Count();
	si=sj=-1;
	maxA=0.0;
	CreateDependencies();
	CreateMatrixOfCandidates();
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			cell=this->matrix+n*i+j;
			if (cell->counter>0){
				//no of params
				if (((*acts)[i]->GetParams()->Count()+(*acts)[j]->GetParams()->Count()-cell->shared_args->size())>PAR_C) continue;
				if ((double)cell->counter/totalplanslength<PAR_B) continue;
				x=(double)cell->counter/((*(this->frequency+i)) > (*(this->frequency+j)) ? (*(this->frequency+j)) : (*(this->frequency+i)));
				if (x <PAR_A) continue;
				if (x>maxA){si=i;sj=j;maxA=x;}
			}
		}
	}
	if (si==-1){ //no macro has been found
		int x=0;
		for(int i=0;i<n;i++){
			if (*(frequency+i+x)==0) {acts->Remove(i);i--;n--;x++;}
		}
		return;
	} else {
		CMacroAction *ma = new CMacroAction((*acts)[si],(*acts)[sj],*(matrix+n*si+sj)->shared_args);
		cout << "selected: " << (*acts)[si]->GetActName() << ", " << (*acts)[sj]->GetActName() << endl; //debug reasons
		cout << ma->ToString() << endl; //debug reasons
		acts->AddRecord(ma);
		//update plans
		for (int k=0;k<data.train->size();k++){
			repl_acts.clear();
			unn.clear();
			list<pair_str>::iterator it;
			for(it=((matrix+n*si+sj)->pairs->begin());it!=((matrix+n*si+sj)->pairs->end());it++){
				if (it->prob!=k) continue;
				rst.i=it->first;
				rst.j=it->second;
				rst.a=new CMacroAction((*(*data.train)[k]->GetPlan())[rst.i-1],(*(*data.train)[k]->GetPlan())[rst.j-1],*(matrix+n*si+sj)->shared_args);
				rst.move_pred=dependencies[k]->GetMovePred(rst.i,rst.j);
				repl_acts.push_back(rst);
				unn.push_back(rst.i);
				unn.push_back(rst.j);
			}
			(*data.train)[k]->GetPlan()->ModifyPlan(unn,repl_acts,(*data.train)[k]->GetInit(),(*data.train)[k]->GetGoal());
		}
	}
	}
}

//learns entanglements from training plans
void CLearner::LearnEntanglements()
{
	int i,j,k,l,m,n;
	ent_str es;
	vector<ent_str> *ent_vec;
	CPredicateList *pred_list;
	CProblem *prob;
	CAction *act;
	vector<string> init_skip,goal_skip,tmp;
	
	this->ClearEntStr();
	
	for (i=0;i<data.train->size();i++){
	  prob=(*data.train)[i];
	  prob->DetermineAllInstancesPredicates();
	  if (i==0) {
	    init_skip=prob->allinstpreds_init;
	    goal_skip=prob->allinstpreds_goal;
	  } else {
	    tmp.resize(init_skip.size());
	    set_intersection(init_skip.begin(),init_skip.end(),prob->allinstpreds_init.begin(),prob->allinstpreds_init.end(),tmp.begin());
	    init_skip=tmp;
	    tmp.clear();
	    tmp.resize(goal_skip.size());
	    set_intersection(goal_skip.begin(),goal_skip.end(),prob->allinstpreds_goal.begin(),prob->allinstpreds_goal.end(),tmp.begin());
	    goal_skip=tmp;
	    tmp.clear();
	  }
	}
	n=data.pdom->GetActions()->Count();
	//init ent_data structure
	for (i=0;i<n;i++){
		ent_vec=new vector<ent_str>();
		//preconditions
		pred_list=(*data.pdom->GetActions())[i]->GetPrec();
		for (j=0;j<pred_list->Count();j++){
			es.skip=(*pred_list)[j]->IsStatic()||(*pred_list)[j]->IsEntangled()||find(init_skip.begin(),init_skip.end(),(*pred_list)[j]->GetName())!=init_skip.end();
			es.fail=es.total=0;
			ent_vec->push_back(es);
		}
		
		//positive effects (are stored in ent_vec just behind preconditions)
		pred_list=(*data.pdom->GetActions())[i]->GetPosEff();
		for (j=0;j<pred_list->Count();j++){
			es.skip=(*pred_list)[j]->IsEntangled()||find(goal_skip.begin(),goal_skip.end(),(*pred_list)[j]->GetName())!=goal_skip.end();
			es.fail=es.total=0;
			ent_vec->push_back(es);
		}
		
		this->ent_data.push_back(ent_vec);
	}

	//go through the training plans and check entanglement conditions
	for (i=0;i<data.train->size();i++){
		prob=(*data.train)[i];
		for (j=0;j<prob->GetPlan()->Length();j++){
			act=(*prob->GetPlan())[j];
			data.pdom->GetActions()->FindProperAction(act->GetActName(),m);
			(*(frequency+m))++;
			//process preconditions
			for (k=0;k<act->GetPrec()->Count();k++){
				if (!(*this->ent_data[m])[k].skip){
					(*this->ent_data[m])[k].total++;
					if (prob->GetInit()->Find((*act->GetPrec())[k])==-1) {
						(*this->ent_data[m])[k].fail++;
					}
				}

			}
			//process pos effects (remember that pos effects are stored in ent_data just behind preconds
			for (l=0;l<act->GetPosEff()->Count();l++){
				if (!(*this->ent_data[m])[k+l].skip){
					(*this->ent_data[m])[k+l].total++;
					if (prob->GetGoal()->Find((*act->GetPosEff())[l])==-1) {
						(*this->ent_data[m])[k+l].fail++;
					}
				}

			}
		}

	}
	
	//assign entanglements
	for (i=0;i<n;i++){
		act=(*data.pdom->GetActions())[i];
		for (k=0;k<act->GetPrec()->Count();k++){
				if (!(*this->ent_data[i])[k].skip &&
					 (*this->ent_data[i])[k].total>0 &&
					 ((double)(*this->ent_data[i])[k].fail)/(*this->ent_data[i])[k].total<=FLAW_RATIO  //might be replaced by a predefined constant
					){
						(*act->GetPrec())[k]->SetEntanglement(true);
					} else {
						//(*act->GetPrec())[k]->SetEntanglement(false);
					}
		}

			//process pos effects (remember that pos effects are stored in ent_data just behind preconds
		for (l=0;l<act->GetPosEff()->Count();l++){
				if (!(*this->ent_data[i])[k+l].skip &&
					 (*this->ent_data[i])[k+l].total>0 &&
					 ((double)(*this->ent_data[i])[k+l].fail)/(*this->ent_data[i])[k+l].total<=FLAW_RATIO  //might be replaced by a predefined constant
					){
						(*act->GetPosEff())[l]->SetEntanglement(true);
					} else {
					//	(*act->GetPosEff())[l]->SetEntanglement(false);
					}
		}
	}

}

void CLearner::ClearEntStr()
{
	while (!this->ent_data.empty()){
		delete this->ent_data.back();
		this->ent_data.pop_back();
	}
}



void CLearner::LearnInnerEntanglements(void)
{
	int i,j,k,l,m,m2,n;
	CDependency *dep;
	CPlan *pi;
	CPredicateList *pred_list;
	vector<in_ent_str> *in_ent_vec;
	in_ent_str es;

	this->ClearInnerEntStr();
	
	n=data.pdom->GetActions()->Count();
	//init inner_ent_data structure
	for (i=0;i<n;i++){
		in_ent_vec=new vector<in_ent_str>();
		//preconditions
		pred_list=(*data.pdom->GetActions())[i]->GetPrec();
		for (j=0;j<pred_list->Count();j++){
			es.total=0;
			es.op_counts = (int*) calloc(n,sizeof(int));
			in_ent_vec->push_back(es);
		}
		
		//positive effects (are stored in ent_vec just behind preconditions)
		pred_list=(*data.pdom->GetActions())[i]->GetPosEff();
		for (j=0;j<pred_list->Count();j++){
			es.total=0;
			es.op_counts = (int*) calloc(n,sizeof(int));
			in_ent_vec->push_back(es);
		}
		
		this->inner_ent_data.push_back(in_ent_vec);
	}

	//go through the training plans and check entanglement conditions
	for (i=0;i<data.train->size();i++){
		pi = (*data.train)[i]->GetPlan();
		dep = new CDependency(pi,(*data.train)[i]->GetInit(),(*data.train)[i]->GetGoal());
		if (!dep->CreateDependencies()){ //getting also previous entanglements
			cout << "Bad Plan:"  << endl; //for debug reasons
			break;
		}
		//setting successive entanglements
		/*
		for (j=pi->Length()-1;j>=0;j--){			
			for (k=0;k<(*pi)[j]->GetPrec()->Count();k++){
				CPredicate* tmp = (*(*pi)[j]->GetPrec())[k];
				if (tmp->GetInnerEntanglement()>0){
					CPredicateList *tmplist = (*pi)[tmp->GetInnerEntanglement()-1]->GetPosEff();
					int pos = tmplist->Find(tmp);
					(*tmplist)[pos]->SetInnerEntanglement(j+1);
				}
			}
		}
		*/
		//counting inner entanglements
		for (j=0;j<pi->Length();j++){
			
			data.pdom->GetActions()->FindProperAction((*pi)[j]->GetActName(),m);
			
			for (k=0;k<(*pi)[j]->GetPrec()->Count();k++){
				(*this->inner_ent_data[m])[k].total++;
				CPredicate *tmp_pred = (*(*pi)[j]->GetPrec())[k];
				if (tmp_pred->GetInnerEntanglement()>0){
					//previous ents
					data.pdom->GetActions()->FindProperAction((*pi)[tmp_pred->GetInnerEntanglement()-1]->GetActName(),m2);
					(* (int*)((*this->inner_ent_data[m])[k].op_counts+m2))++;
					//sucessive ents
					CPredicateList *tmplist = (*pi)[tmp_pred->GetInnerEntanglement()-1]->GetPosEff();
					int pos = tmplist->Find(tmp_pred);
					(* (int*)((*this->inner_ent_data[m2])[(*pi)[tmp_pred->GetInnerEntanglement()-1]->GetPrec()->Count()+pos].op_counts+m))++;
				}
			}

			for (l=0;l<(*pi)[j]->GetPosEff()->Count();l++){
				(*this->inner_ent_data[m])[k+l].total++;
			}
			
			//succesive entanglements
			/*
			for (l=0;l<(*pi)[j]->GetPosEff()->Count();l++){
				(*this->inner_ent_data[m])[k+l].total++;
				CPredicate *tmp_pred = (*(*pi)[j]->GetPosEff())[l];
				if (tmp_pred->GetInnerEntanglement()>0){
					data.pdom->GetActions()->FindProperAction((*pi)[tmp_pred->GetInnerEntanglement()-1]->GetActName(),m2);
					(* (int*)((*this->inner_ent_data[m])[k+l].op_counts+m2))++;
				}
			}
			*/
		}
	}

	//assign inner entanglements
	for (i=0;i<n;i++){
		CAction* act=(*data.pdom->GetActions())[i];
		//prev ents
		for (k=0;k<act->GetPrec()->Count();k++){
			if ((*this->inner_ent_data[i])[k].total>OP_LIMIT){
				for (j=0;j<n;j++){
					//cout << "prev:" << act->GetActName() << "," << j << ": " << (* (int*)((*this->inner_ent_data[i])[k].op_counts+j)) <<endl;
					if (((double)(* (int*)((*this->inner_ent_data[i])[k].op_counts+j)))/(*this->inner_ent_data[i])[k].total>=1.0-FLAW_RATIO)  //might be replaced by a predefined constant
					{
						(*act->GetPrec())[k]->SetInnerEntanglement(j);
					}
				}
			}
		}

		//succ ents
		for (l=0;l<act->GetPosEff()->Count();l++){
			if ((*this->inner_ent_data[i])[k+l].total>OP_LIMIT){
				int no = 0;
				int ent = -1;
				for (j=0;j<n;j++){
					//cout << "succ:" << act->GetActName() << "," << j << ": " << (* (int*)((*this->inner_ent_data[i])[k+l].op_counts+j))<<endl;
					double ratio = ((double)(* (int*)((*this->inner_ent_data[i])[k+l].op_counts+j)))/(*this->inner_ent_data[i])[k+l].total;
					
					if (ratio>=1.0-FLAW_RATIO)  //might be replaced by a predefined constant
					{
						//(*act->GetPosEff())[l]->SetInnerEntanglement(j);
						ent = j;
					} 
					if (ratio<=FLAW_RATIO){
						no++;
					}
				}
				if (ent!=-1 && no==n-1) (*act->GetPosEff())[l]->SetInnerEntanglement(ent);
			}
		}
	}
}

void CLearner::ClearInnerEntStr(void)
{
	while (!this->inner_ent_data.empty()){
		//delete (*this->inner_ent_data.back()).op_counts;
		delete this->inner_ent_data.back();
		this->inner_ent_data.pop_back();
	}
}

//must be called after LearnEntanglements()
void CLearner::ApplyEntanglements(bool mcronly,bool actupd)
{
	CActionList *acts = data.pdom->GetActions();
	int i,j;
	CPredicateList *ent_init, *ent_goal;
	CPredicateList *tmplist;
	string s;

	ent_init = new CPredicateList();
	ent_goal = new CPredicateList();

	for (i=0;i<acts->Count();i++){
	  if (mcronly&&!(*acts)[i]->isMacro()) continue;
	  if (data.init){	
		//ent_init
		tmplist = (*acts)[i]->GetPrec();
		s="stai_";
		for (j=0;j<tmplist->Count();j++){
			if ((*tmplist)[j]->IsEntangled() && !(*tmplist)[j]->IsStatic()){
				if (actupd){
			           CPredicate *tmp = (*tmplist)[j]->Clone();
				   tmp->SetName(s+tmp->GetName());
				   tmp->SetStatic(true);
				   (*acts)[i]->GetPrec()->AddRecord(tmp);
				}
				if (!ent_init->Subsume((*tmplist)[j], data.pdom->GetTypes())){
					ent_init->AddRecord((*tmplist)[j]->Clone());
				}
			}
		}
	  }
	  if (data.goal){
		//ent_goal
		tmplist = (*acts)[i]->GetPosEff();
		s="stag_";
		for (j=0;j<tmplist->Count();j++){
			if ((*tmplist)[j]->IsEntangled()){ 
				if (actupd){
			            CPredicate *tmp = (*tmplist)[j]->Clone();
				    tmp->SetName(s+tmp->GetName());
				    tmp->SetStatic(true);
				    (*acts)[i]->GetPrec()->AddRecord(tmp);
				}
				if (!ent_goal->Subsume((*tmplist)[j], data.pdom->GetTypes())){
					ent_goal->AddRecord((*tmplist)[j]->Clone());
				}
			}
		}
	  }
	}

	//reformulate problems
	for (i=0;i<data.pprob->size();i++){
		(*data.pprob)[i]->AssignTypes();
		(*data.pprob)[i]->ReformulateByEntangelments(ent_init,ent_goal,data.pdom->GetTypes());
	}

	//reformulate domain
	data.pdom->ReformulateByEntanglements(ent_init,ent_goal);

}

// must be called after LearnInnerEntanglements()
void CLearner::ApplyInnerEntanglements(void)
{
	CActionList *acts = data.pdom->GetActions();
	int i,j,n;
	short x;
	CPredicateList *ent_prev, *ent_succ;
	CPredicateList *tmplist;
	string s,s2;
	bool other,both,good;
	CPredicate *toent=NULL;

	ent_prev = new CPredicateList();
	ent_succ = new CPredicateList();

		//ent_prev
	 if (data.prec)
	   for (i=0;i<acts->Count();i++){
		tmplist = (*acts)[i]->GetPrec();
		n = tmplist->Count();	
		for (j=0;j<n;j++){
			x = (*tmplist)[j]->GetInnerEntanglement();
			
			//check whether other operator might provide the predicate
			other = false;
			good = false;
			if (x!=-1)
			for (int l=0;l<acts->Count();l++){
				if (l!=x && (*acts)[l]->GetPosEff()->Subsume((*tmplist)[j],data.pdom->GetTypes())){
					other = true;
					if ((*acts)[x]->GetParams()->Count()<=(*acts)[l]->GetParams()->Count()) good = true;
					//break;
				}
			}
			
			if (x!=-1 && other){
								
				CPredicate *tmp = (*tmplist)[j];
				CPredicate* tmp2 = (*acts)[x]->GetPosEff()->SubsumeP(tmp,data.pdom->GetTypes());
				
				both = data.succ && tmp2->GetInnerEntanglement()==i;
			   if (!data.inent_filter || good || both){
				s = (*acts)[i]->GetActName();
				s += "_";
				s += (*acts)[x]->GetActName();
				s += both ? "_both_" : "_prev_";

				toent=NULL;
				
				if (!both){
				  for (int l=0;l<acts->Count();l++){
					if (l!=x && l!=i){
						//CPredicate* tmpp = (*acts)[l]->GetNegEff()->SubsumeP((*tmplist)[j],data.pdom->GetTypes());
						CPredicate* tmpp = (*acts)[l]->GetPosEff()->SubsumeP((*tmplist)[j],data.pdom->GetTypes());
						if (tmpp != NULL){
							toent = (toent==NULL || toent->Subsume(tmpp,data.pdom->GetTypes(),true)) ? tmpp : toent;
							tmpp = tmpp->Clone();
							tmpp->SetName(s+tmpp->GetName());
							if (!tmpp->Negative()) tmpp->Negate();
							(*acts)[l]->GetEff()->AddRecord(tmpp);
						}
					}
				  }

				  tmp=tmp->Clone();
				  tmp2=tmp2->Clone();
				  tmp->SetInnerEntanglement(-1);
				  tmp2->SetInnerEntanglement(-1);
				
				  s2 = tmp->GetName();
				  tmp->SetName(s+tmp->GetName());
				  if (tmp->Negative()) tmp->Negate();

				  tmp2->SetName(tmp->GetName());
				  if (tmp2->Negative()) tmp2->Negate();

				  (*acts)[i]->GetPrec()->AddRecord(tmp->Clone());	
				  (*acts)[x]->GetEff()->AddRecord(tmp2);
				  //tmp->Negate();
				  //(*acts)[i]->GetEff()->AddRecord(tmp);
				}
				
				toent = (toent==NULL || toent->Subsume(tmp,data.pdom->GetTypes(),true)) ? tmp : toent;
				toent = (toent==NULL || toent->Subsume(tmp2,data.pdom->GetTypes(),true)) ? tmp2 : toent;
				
				toent=data.pdom->GetPredicates()->SubsumeP(toent,data.pdom->GetTypes())->Clone();
				toent->SetName(both ? s+tmp->GetName() : tmp->GetName());
				if (toent->Negative()) toent->Negate();
				toent->SetInnerEntanglement(-1);
				
				ent_prev->AddRecord(toent);
				
				if (both){
				  for (int k=0;k<acts->Count();k++){
					CPredicate* tmpp = (*acts)[k]->GetPosEff()->SubsumeP(tmp,data.pdom->GetTypes());
					if (k!=x && tmpp!=NULL){
						toent = (toent==NULL || toent->Subsume(tmpp,data.pdom->GetTypes(),true)) ? tmpp : toent;
						tmpp = tmpp->Clone();
						tmpp->SetName(s+tmpp->GetName());
						tmpp->SetInnerEntanglement(-1);
						if (!tmpp->Negative()) tmpp->Negate();
						//if (!tmp3->Negative()) tmp3->Negate();
						(*acts)[k]->GetEff()->AddRecord(tmpp);
					}
				  }
				  CPredicate *tmp3 =  (*acts)[i]->GetNegEff()->SubsumeP(tmp,data.pdom->GetTypes());
				  if (tmp3!=NULL) tmp3->SetName(s+tmp->GetName());
				  CPredicate *tmp4 = tmp2->Clone();
				  tmp4->Negate();
				  (*acts)[x]->GetEff()->AddRecord(tmp4);
				  tmp->SetName(s+tmp->GetName());
				  tmp2->SetName(s+tmp2->GetName());
				  tmp->SetInnerEntanglement(-1);
				  tmp2->SetInnerEntanglement(-1);
				 
				}
			   }
			}
		}
	 }
	 if (data.succ)  
	   for (i=0;i<acts->Count();i++){
	 //ent_succ
		tmplist = (*acts)[i]->GetPosEff();
		n = tmplist->Count();	
		for (j=0;j<n;j++){
			x = (*tmplist)[j]->GetInnerEntanglement();
			
			//check whether other operator might provide the predicate
			other = false;
			good = false;
			if (x!=-1)
			for (int l=0;l<acts->Count();l++){
				if (l!=x && (*acts)[l]->GetPrec()->Subsume((*tmplist)[j],data.pdom->GetTypes())){
					other = true;
					if ((*acts)[x]->GetParams()->Count()<=(*acts)[l]->GetParams()->Count()) good = true;
					//break;
				}
			}
			
			if (x!=-1 && other && (!data.inent_filter || good)){
				s = (*acts)[i]->GetActName();
				s += "_";
				s += (*acts)[x]->GetActName();
				s += "_succ_";

				toent=NULL;

				CPredicate *tmp = (*tmplist)[j]->Clone();
				CPredicate* tmp2 = (*acts)[x]->GetPrec()->SubsumeP(tmp,data.pdom->GetTypes());
				// a bit of hacking...
				if (tmp2==NULL) continue; else tmp2=tmp2->Clone();
				tmp->SetInnerEntanglement(-1);
				tmp2->SetInnerEntanglement(-1);
				
				for (int k=0;k<acts->Count();k++){
					CPredicate* tmp3 = (*acts)[k]->GetPrec()->SubsumeP(tmp,data.pdom->GetTypes());
					if (k!=x && tmp3!=NULL){
						toent = (toent==NULL || toent->Subsume(tmp3,data.pdom->GetTypes(),true)) ? tmp3 : toent;
						tmp3 = tmp3->Clone();
						tmp3->SetName(s+tmp3->GetName());
						tmp3->SetInnerEntanglement(-1);
						if (tmp3->Negative()) tmp3->Negate();
						//if (!tmp3->Negative()) tmp3->Negate();
						(*acts)[k]->GetPrec()->AddRecord(tmp3);
					}
					CPredicate* tmp4 = (*acts)[k]->GetPosEff()->SubsumeP(tmp,data.pdom->GetTypes());
					if (k!=i && tmp4!=NULL){
						toent = (toent==NULL || toent->Subsume(tmp4,data.pdom->GetTypes(),true)) ? tmp4 : toent;
						tmp4 = tmp4->Clone();
						tmp4->SetName(s+tmp4->GetName());
						tmp4->SetInnerEntanglement(-1);
						if (tmp4->Negative()) tmp4->Negate();
						//if (!tmp3->Negative()) tmp3->Negate();
						(*acts)[k]->GetEff()->AddRecord(tmp4);
					}
				}

				toent = (toent==NULL || toent->Subsume(tmp,data.pdom->GetTypes(),true)) ? tmp : toent;
				toent = (toent==NULL || toent->Subsume(tmp2,data.pdom->GetTypes(),true)) ? tmp2 : toent;
				
				toent=data.pdom->GetPredicates()->SubsumeP(toent,data.pdom->GetTypes())->Clone();
				toent->SetName(s+tmp->GetName());
				if (toent->Negative()) toent->Negate();
				toent->SetInnerEntanglement(-1);

				tmp2->SetName(s+tmp->GetName());
				if (tmp2->Negative()) tmp2->Negate();
				//if (!tmp2->Negative()) tmp2->Negate();
				(*acts)[x]->GetEff()->AddRecord(tmp2);

				tmp->SetName(s+tmp->GetName());
			
				ent_succ->AddRecord(toent);

				if (!tmp->Negative()) tmp->Negate();
				//if (tmp->Negative()) tmp->Negate();

				(*acts)[i]->GetEff()->AddRecord(tmp);


			}
		}
	 
	}

	//reformulate problems
	if (data.succ)
	for (i=0;i<data.pprob->size();i++){
		(*data.pprob)[i]->AssignTypes();
		(*data.pprob)[i]->ReformulateByInnerEntanglements(ent_succ,data.pdom->GetTypes());
	}

	//reformulate domain
	data.pdom->ReformulateByInnerEntanglements(ent_prev,ent_succ);

}

//#define cutline(l,n) memmove((considered+l*n),(considered+(l+1)*n),sizeof(short)*(n-l-1)*n);for(int zzz=l;zzz<n-1;zzz++) memmove((considered+zzz*n+l),(considered+zzz*n+l+1),sizeof(short)*(n-l-1));
#define cutline(l,n) memmove(&considered[(l)*(n)],&considered[((l)+1)*(n)],sizeof(short)*((n)-l-1)*(n));for(int zzz=0;zzz<(n)-1;zzz++) memmove(&considered[zzz*((n)-1)+(l)],&considered[zzz*(n)+(l)+1],sizeof(short)*((n)-1));

void CLearner::GenerateMacrosFromInnerEntanglements(short *carry)
{
	CActionList *acts = data.pdom->GetActions();
	int nact = acts->Count();
	int i,j,k,n,x,q;
	CPredicateList *tmplist;
	CPredicate *tmppred;
	bool firsttime = carry==NULL;
	short* considered = firsttime ? new short[nact*nact] : carry;
	vector<vector<sh_arg_str> > sh_args(nact*nact);
	bool macroadded = false;
	
	if (firsttime) memset(considered,NOT_CONSIDERED,sizeof(short)*nact*nact);

	for(i=0;i<nact;i++){
		//debug
		//std::cout << (*acts)[i]->ToString() /*ma->GetActName()*/  << std::endl;
		//ent_prec
		tmplist = (*acts)[i]->GetPrec();
		n = tmplist->Count();	
		for (j=0;j<n;j++){
			if (i==j) continue;
			x = (*tmplist)[j]->GetInnerEntanglement();
			if (x!=-1){
				if (!firsttime && !(considered[nact*x+i]&PREC_CONSIDERED)) continue;
				tmppred = (*acts)[x]->GetPosEff()->SubsumeP((*tmplist)[j],data.pdom->GetTypes());
				if (!firsttime || (*acts)[i]->GetNegEff()->Find(tmppred)!=-1 || IsOneToOne((*acts)[x],(*acts)[i])){
					tmppred->Equal((*tmplist)[j],(*acts)[x]->GetParams(),(*acts)[i]->GetParams(),sh_args[nact*x+i]);
					considered[nact*x+i] |= PREC_CONSIDERED;
				}
			} 
			
		}
		
		//ent_succ
		tmplist = (*acts)[i]->GetPosEff();
		n = tmplist->Count();	
		for (j=0;j<n;j++){
			if (i==j) continue;
			x = (*tmplist)[j]->GetInnerEntanglement();
			if (x!=-1){
				if (!firsttime && !(considered[nact*i+x]&SUCC_CONSIDERED)) continue;
				tmppred = (*acts)[x]->GetPrec()->SubsumeP((*tmplist)[j],data.pdom->GetTypes());
				if (!firsttime ||(*acts)[x]->GetNegEff()->Find(tmppred)!=-1 || IsOneToOne((*acts)[i],(*acts)[x])){
					(*tmplist)[j]->Equal(tmppred,(*acts)[i]->GetParams(),(*acts)[x]->GetParams(),sh_args[nact*i+x]);
					considered[nact*i+x] |= SUCC_CONSIDERED;
				}
			} 	
			
		}

	}
	
	//debug reasons
	for (i=0;i<nact;i++)
		for (j=0;j<nact;j++){
			if (considered[nact*i+j]>0){
				std::cout << (considered[nact*i+j]&PREC_CONSIDERED ? "P" : " ") << (considered[nact*i+j]&SUCC_CONSIDERED ? "S" : " ") << "  " << (*acts)[i]->GetActName() << " + " << (*acts)[j]->GetActName();
				for (k=0;k<(sh_args[nact*i+j]).size();k++){
					if (k>0) std::cout << ", ";
					std::cout << "[" << sh_args[nact*i+j][k].first << "," << sh_args[nact*i+j][k].second << "]";
				}
				std::cout << endl;
			}
		}
	
	for (i=0;i<nact;i++){
	 for (j=0;j<nact;j++){
		 if (considered[nact*i+j]&(PREC_CONSIDERED|SUCC_CONSIDERED)){
			CMacroAction *ma = new CMacroAction((*acts)[i],(*acts)[j],sh_args[nact*i+j]);
			//for debug reasons
			std::cout << "Suggested Macro: " << /*ma->ToString()*/ ma->GetActName() << std::endl;

			if (ma->VerifyValidity(acts,data.pdom->GetTypes(),this)){
				//for debug reasons
				std::cout << "Considered Macro: " << ma->ToString() /*ma->GetActName()*/  << std::endl;
				vector<int> toremove;
				if (considered[nact*i+j]&PREC_CONSIDERED) toremove.push_back(j);
				if (considered[nact*i+j]&SUCC_CONSIDERED) toremove.push_back(i);
				if (toremove.size()==1){
					x = toremove.front()==i ? j: i;
					tmplist = (considered[nact*i+j]&SUCC_CONSIDERED) ? (*acts)[x]->GetPrec() : (*acts)[x]->GetPosEff();
					for (k=0;k<tmplist->Count();k++) if ((*tmplist)[k]->GetInnerEntanglement() == toremove.front()) (*tmplist)[k]->SetInnerEntanglement(-1);
					tmplist = (considered[nact*i+j]&SUCC_CONSIDERED) ? ma->GetPrec() : ma->GetPosEff();
					for (k=0;k<tmplist->Count();k++) if ((*tmplist)[k]->GetInnerEntanglement() == toremove.front() || (*tmplist)[k]->GetInnerEntanglement() == x) (*tmplist)[k]->SetInnerEntanglement(-1);
					acts->Replace(ma,toremove.front());
					//considered[nact*i+j]=0;
					//remove entanglements between the non-replaced operator and others since they might be compromised
					for (int z=0;z<nact;z++) {
						considered[nact*x+z]=0;
						considered[nact*z+x]=0;
					}
				} else {
					//more difficult...
					tmplist = ma->GetPrec(); 
					for (k=0;k<tmplist->Count();k++) if ((*tmplist)[k]->GetInnerEntanglement() == toremove[0] || (*tmplist)[k]->GetInnerEntanglement() == toremove[1]) (*tmplist)[k]->SetInnerEntanglement(-1);
					tmplist = ma->GetPosEff();
					for (k=0;k<tmplist->Count();k++) if ((*tmplist)[k]->GetInnerEntanglement() == toremove[0] || (*tmplist)[k]->GetInnerEntanglement() == toremove[1]) (*tmplist)[k]->SetInnerEntanglement(-1);
					sort(toremove.begin(),toremove.end());
					
					acts->Replace(ma,toremove[0]);
					acts->Remove(toremove[1]);
					nact--;
					//update inner entanglements
					for (q=0;q<nact;q++){
						//if (q==toremove[0] || q==toremove[1]) continue;
						tmplist = (*acts)[q]->GetPrec();
						for (k=0;k<tmplist->Count();k++) {
							if ((*tmplist)[k]->GetInnerEntanglement() == toremove[1]) (*tmplist)[k]->SetInnerEntanglement(toremove[0]);
							if ((*tmplist)[k]->GetInnerEntanglement() > toremove[1]) (*tmplist)[k]->SetInnerEntanglement((*tmplist)[k]->GetInnerEntanglement()-1);
						}
						tmplist = (*acts)[q]->GetPosEff();
						for (k=0;k<tmplist->Count();k++) {
							if ((*tmplist)[k]->GetInnerEntanglement() == toremove[1]) (*tmplist)[k]->SetInnerEntanglement(toremove[0]);
							if ((*tmplist)[k]->GetInnerEntanglement() > toremove[1]) (*tmplist)[k]->SetInnerEntanglement((*tmplist)[k]->GetInnerEntanglement()-1);
						}
					}
					considered[(nact+1)*i+j]=0;
					for (k=0;k<nact+1;k++) considered[(nact+1)*toremove[0]+k]|=considered[(nact+1)*toremove[1]+k];
					for (k=0;k<nact+1;k++) considered[(nact+1)*k+toremove[0]]|=considered[(nact+1)*k+toremove[1]];
					cutline(toremove[1],nact+1);
					
					
				}
				//acts->AddRecord(ma);
					
					//debug reasons
	/*
				for (int ii=0;ii<nact;ii++)
		for (int jj=0;jj<nact;jj++){
			if (considered[(nact)*ii+jj]>0){
				std::cout << (considered[(nact)*ii+jj]&PREC_CONSIDERED ? "P" : " ") << (considered[(nact)*ii+jj]&SUCC_CONSIDERED ? "S" : " ") << "  " <<  ii << " + " <<  jj;
				std::cout << endl;
			}
		}
*/
				macroadded=true;break;
				//if (macroadded) break;

			}
		 }
	 }
	 if (macroadded) break;
	/* 
	 if (macroadded) {
	     macroadded=false;
		 i=j=-1;
		 continue;}
	*/
	}
	if (macroadded) GenerateMacrosFromInnerEntanglements(considered);
}


bool CLearner::IsSameAchiever(CAction* first, CAction* second, CPredicate* p)
{
	
	//CPredicateList *preds = new CPredicateList();
	int i,j,k,q;
	int cnt = 0;
	int err = 0;
	CProblem* prob;
	CAction *act;
	CPredicate *pred,*p2,*p3;
	vector<sh_arg_str> sh_args;

	p2 = first->GetPrec()->SubsumeP(p,data.pdom->GetTypes());
	string ghg = p2->ToString(true);
	string dfd = first->ToString();
	int pos = first->GetPrec()->Find(p2);
	p3 = second->GetPrec()->SubsumeP(p,data.pdom->GetTypes());
	p2->Equal(p3,first->GetParams(),second->GetParams(),sh_args);
	
	//must be fixed !!
	for (i=0;i<data.train->size();i++){
		prob=(*data.train)[i];
		for (j=0;j<prob->GetPlan()->Length();j++){
			act=(*prob->GetPlan())[j];
			if (strcasecmp(act->GetActName().data(),first->GetActName().data())==0){
				pred=(*act->GetPrec())[pos]->Clone();
				cnt++;
				for(k=j+1;k<prob->GetPlan()->Length();k++){
					if (strcasecmp((*prob->GetPlan())[k]->GetActName().data(),second->GetActName().data())==0){
						if (!act->GetParams()->IsCorresponding((*prob->GetPlan())[k]->GetParams(),sh_args)) continue;
						//it's wrong... 
						CPredicateList *tmplist = (*prob->GetPlan())[k]->GetPrec();
						//for (q=0;q<tmplist->Count();q++){
						int pos2 = tmplist->Find(pred);
						if (pos2>=0 && (*tmplist)[pos2]->GetInnerEntanglement()==pred->GetInnerEntanglement()) break;
						//}
						//if (q<tmplist->Count()) break;
					}
					if ((*prob->GetPlan())[k]->GetNegEff()->Find(pred)!=-1 || (*prob->GetPlan())[k]->GetPosEff()->Find(pred)!=-1) {err++;break;}
				}
				
			} 
			/*
			else 
			
			if (strcasecmp(act->GetActName().data(),second->GetActName().data())==0){
				if (!act->GetPrec()->Disjunct(preds)) (*preds)=(*preds)-(*act->GetPrec());
			} else {
				if (!act->GetNegEff()->Disjunct(preds)) {
					(*preds)=(*preds)-(*act->GetNegEff());
					err++;
				}
			}
			*/
		}
	}
	
	return cnt==0 || (double)err/cnt<=FLAW_RATIO;
}

bool CLearner::IsOneToOne(CAction* first, CAction* second)
{
	int i,j,k,x;
	set<int> l1,l2;
	CProblem* prob;
	CAction *act;
	
	for (i=0;i<data.train->size();i++){
		prob=(*data.train)[i];
		for (j=0;j<prob->GetPlan()->Length();j++){
			act=(*prob->GetPlan())[j];
			if (strcasecmp(act->GetActName().data(),first->GetActName().data())==0) l1.insert(j+1);
			if (strcasecmp(act->GetActName().data(),second->GetActName().data())==0) {
				CPredicateList *tmplist = (*prob->GetPlan())[j]->GetPrec();
					for (k=0;k<tmplist->Count();k++){
						x = (*tmplist)[k]->GetInnerEntanglement();
						if (l1.find(x)!=l1.end()) {
							if (l2.find(x)!=l2.end()) return false;
							l2.insert(x);
						}
					}
			}
		}
		l1.clear();
		l2.clear();
	}
	return true;
}

void CLearner::OutputEnt(std::ostream& s)
{
	CActionList *acts=data.pdom->GetActions();
	vector<vector<ent_str>*>::iterator it;
	vector<ent_str>::iterator it2;
	string act_name;
	int i,j,k;

	//s << "entanglements by init" <<endl;
	//s << "entanglements by goal" <<endl;
	i=0;
	for (it=ent_data.begin();it!=ent_data.end();it++){
		act_name=(*acts)[i]->GetActName();
		k=(*acts)[i]->GetPrec()->Count();
		j=0;
		for(it2=(*it)->begin();it2!=(*it)->end();it2++){
			if (!(*it2).skip){
				s << ((j<k) ? "init":"goal") << ";" << act_name << ";" << ((j<k) ? (*(*acts)[i]->GetPrec())[j]->GetName() : (*(*acts)[i]->GetPosEff())[j-k]->GetName()) << ";" << ((*it2).total - (*it2).fail) << ";" << (*it2).total << endl;
			}
			j++;
		}
		
		i++;
	}

}

void CLearner::OutputIntEnt(std::ostream& s)
{
		CActionList *acts=data.pdom->GetActions();
	vector<vector<in_ent_str>*>::iterator it;
	vector<in_ent_str>::iterator it2;
	string act_name;
	int i,j,k,l;

	//s << "entanglements by init" <<endl;
	//s << "entanglements by goal" <<endl;
	i=0;
	for (it=inner_ent_data.begin();it!=inner_ent_data.end();it++){
		act_name=(*acts)[i]->GetActName();
		k=(*acts)[i]->GetPrec()->Count();
		j=0;
		for(it2=(*it)->begin();it2!=(*it)->end();it2++){
			
			CPredicate *tmppred = (j<k) ? (*(*acts)[i]->GetPrec())[j] : (*(*acts)[i]->GetPosEff())[j-k];
			for (l=0;l<acts->Count();l++){
				if (j<k && !(*acts)[l]->GetPosEff()->Subsume(tmppred,data.pdom->GetTypes())) continue;
				if (!(*acts)[l]->GetPrec()->Subsume(tmppred,data.pdom->GetTypes())) continue;
				s << ((j<k) ? "prec":"succ") << ";" << act_name << ";" << tmppred->GetName() << ";" << (*acts)[l]->GetActName() << ";" << ((int) (*((*it2).op_counts+l))) << ";" << (*it2).total << endl;
			}
			j++;
		}
		
		i++;
	}
}



void CLearner::LearnMacrosFromCA(void)
{
	CActionList* acts=data.pdom->GetActions();
	int n=acts->Count();
	field_of_matrix *max=NULL;
	int si,sj,inst,inst2;
	CMacroAction *ma;
	list<int> unn;
	list<MacroReplStr> repl_acts;
	MacroReplStr rst;
	map<int,int> instances;
	vector<int> com_cnt;
	vector<int> com_cnt_ent;
	int match;
	
	for(int zz=0;zz<data.pdom->GetActions()->Count();zz++) {
		(*data.pdom->GetActions())[zz]->GenerateStatGraph();
		(*data.pdom->GetActions())[zz]->GenerateComponents();
		com_cnt.push_back((*data.pdom->GetActions())[zz]->ComponentCount());
		cout << (*data.pdom->GetActions())[zz]->GetActName() << " - comp.no: " << com_cnt[zz] <<endl;
	}
	LearnEntanglements();
	//OutputEnt(cout);
	
	/*deque<CProblem*>::iterator it; 
	for(it=data.train->begin();it!=data.train->end();it++){
		(*it)->BuildStaticGraph();
		(*it)->BuildAbstractComponents();
		//(*it)->OutputAC(cout);//debug reasons
		//cout << "No of Lift instances: " << (*acts)[1]->MakeAllInstances(data.pdom->GetTypes(),*it,true)->Count() << endl;
	}*
	CProblem::GetGlobalAC(data.train);
	*/
	for(int zz=0;zz<data.pdom->GetActions()->Count();zz++) {
		(*data.pdom->GetActions())[zz]->GenerateStatGraph();
		(*data.pdom->GetActions())[zz]->GenerateComponents();
		com_cnt_ent.push_back((*data.pdom->GetActions())[zz]->ComponentCount());
		cout << (*data.pdom->GetActions())[zz]->GetActName() << " - comp.no with ents: " << com_cnt_ent[zz] <<endl;
	}
	int max_macros=4; //hard...
	while (true){
	CreateDependencies();
	CreateMatrixOfCandidates();
	cout << "Total plan length: " << totalplanslength << endl;
	//return; //debug reasons
	bool accepted=true;
	do {
		accepted=true;
		max=NULL;
		n=acts->Count();
		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				field_of_matrix *cell=this->matrix+n*i+j;
				cell->score = cell->counter==0 ? 0 : ((*acts)[i]->HasMeaningfulInitEnt() && (*acts)[j]->HasMeaningfulGoalEnt() ? 10000 : 0) + ((*acts)[i]->HasMeaningfulInitEnt() || (*acts)[j]->HasMeaningfulGoalEnt() ? 1000 : 0) +cell->counter;
				if (max==NULL || cell->score>max->score) {max=cell;si=i;sj=j;}
			}
		}
		if (max->counter==0) break;
		if (si==sj) {max->counter=0;accepted=false;} //cycle..
		ma = new CMacroAction((*acts)[si],(*acts)[sj],*(max->shared_args),data.pdom->GetTypes());
		if (ma->IsUninformative()) {max->counter=0;accepted=false;} //uninformative
		if (ma->IsRepetitive()) {max->counter=0;accepted=false;} //repetitive
		//TODO: locality rule
		//if (!CProblem::CheckLocalityRule(ma,data.pdom->GetTypes())) {max->counter=0;accepted=false;} //locality rule failed
		ma->GenerateStatGraph();
		ma->GenerateComponents();
		match=ma->StatGraphMatch();
	//	bool conn=ma->StatGraphConnect();

		if (/*match==0 && !conn && */(ma->ComponentCount()>com_cnt[si] && ma->ComponentCount()>com_cnt[sj])) {max->counter=0;accepted=false;} //component rule failed
		cout << "Suggested macro: "<< ma->GetActName() << ", score:" << max->score << ", componenets: " << ma->ComponentCount() << endl;
		/*
		inst=inst2=0;
		bool first = instances.find(si)==instances.end();
		bool second = instances.find(sj)==instances.end();
		for(it=data.train->begin();it!=data.train->end();it++){
			CActionList *tmpacts = ma->MakeAllInstances(data.pdom->GetTypes(),*it,true);
			inst+=tmpacts->Count();
			delete tmpacts;
			//inst2+=(*acts)[si]->MakeAllInstances(data.pdom->GetTypes(),*it,false)->Count();
			//inst2+=(*acts)[sj]->MakeAllInstances(data.pdom->GetTypes(),*it,false)->Count();
			if (first) instances[si]+=(*acts)[si]->MakeAllInstances(data.pdom->GetTypes(),*it,false)->Count();
			if (second) instances[si]+=(*acts)[sj]->MakeAllInstances(data.pdom->GetTypes(),*it,false)->Count();
		}
		if (2*inst>instances[si]+instances[sj]) {max->counter=0;accepted=false;} //too many instances...*/
	} while (!accepted);
	if (max_macros==0 || max->counter==0){
		//no macro has been generated
		//TODO: do some finalizing...
		set<int> removal;
		for(int i=n-1;i>=0;i--){
			if (!(*acts)[i]->isMacro()){
			  if (data.macroentanglements==1) break;
			  if (*(frequency+i)< data.oplimit) acts->Remove(i);
			  continue;
			}
			
			((CMacroAction*)(*acts)[i])->FindActionPositions(acts,si,sj);
			if (removal.find(i)!=removal.end()) {acts->Remove(i);(*(frequency+si))+=(*(frequency+i));(*(frequency+sj))+=(*(frequency+i));continue;}
			if (*(frequency+i)==0) {acts->Remove(i);continue;}
			if (com_cnt[i]>com_cnt[si] || com_cnt[i]>com_cnt[sj]){acts->Remove(i);continue;}
			if (com_cnt[i]<com_cnt[si]) {
				removal.insert(si);
			} else if ((*acts)[si]->isMacro() && *(frequency+i)<(*(frequency+si))) {
				acts->Remove(i);(*(frequency+si))+=(*(frequency+i));(*(frequency+sj))+=(*(frequency+i));continue;
			} else {
				removal.insert(si);
			}
			if (com_cnt[i]<com_cnt[sj]) {
				removal.insert(sj);
			} else if ((*acts)[sj]->isMacro() && *(frequency+i)<(*(frequency+sj))) {
				acts->Remove(i);(*(frequency+si))+=(*(frequency+i));(*(frequency+sj))+=(*(frequency+i));continue;
			} else {
				removal.insert(sj);
			}
		}
		ApplyEntanglements(data.macroentanglements==1);
		return;
	}
	//cout << "Macro: "<< ma->GetActName() << " - pairs: " << max->counter << " - possible instances: " << inst << " primitive instances: " << instances[si]+instances[sj] << endl;
	cout << "Macro: "<< ma->GetActName() << " - match: " << match << ", componenets: " << ma->ComponentCount() << endl;
	acts->AddRecord(ma);
	com_cnt.push_back(ma->ComponentCount());
//	instances[n]=inst;
	//update plans
	for (int k=0;k<data.train->size();k++){
		repl_acts.clear();
		unn.clear();
		list<pair_str>::iterator it2;
		for(it2=(max->pairs->begin());it2!=(max->pairs->end());it2++){
			if (it2->prob!=k) continue;
			rst.i=it2->first;
			rst.j=it2->second;
			rst.a=new CMacroAction((*(*data.train)[k]->GetPlan())[rst.i-1],(*(*data.train)[k]->GetPlan())[rst.j-1],*max->shared_args);
			rst.move_pred=dependencies[k]->GetMovePred(rst.i,rst.j);
			repl_acts.push_back(rst);
			unn.push_back(rst.i);
			unn.push_back(rst.j);
		}
		(*data.train)[k]->GetPlan()->ModifyPlan(unn,repl_acts,(*data.train)[k]->GetInit(),(*data.train)[k]->GetGoal());
	}
	max_macros--;
	}
}

void CLearner::GuessEntanglements()
{
  CAction *act,*act2;
  int n=data.pdom->GetActions()->Count();
  int i,j,k,l;
  
  //assign entanglements
	for (i=0;i<n;i++){
		act=(*data.pdom->GetActions())[i];
		for (k=0;k<act->GetPrec()->Count();k++){
		   CPredicate *tmp = (*act->GetPrec())[k];
		   if (tmp->GetPars()!=NULL && tmp->GetPars()->Count()>=2 && !tmp->IsStatic()){//do not consired static predicates as well as those whose arity is <=2
		      int obj_upper = data.pprob->front()->ObjectsUpperBound(tmp);
		      int pred_no = data.pprob->front()->NumberOfPredicates(tmp,true);
		      
		      if ((double)obj_upper*CCC <= pred_no && pred_no <= obj_upper) tmp->SetEntanglement(true);
		   }
		}

	for (l=0;l<act->GetPosEff()->Count();l++){
		  CPredicate *tmp = (*act->GetPosEff())[l];
		   if (tmp->GetPars()!=NULL && tmp->GetPars()->Count()>=2){//do not consired predicates whose arity is <=2
		      int obj_upper = data.pprob->front()->ObjectsUpperBound(tmp);
		      int pred_no = data.pprob->front()->NumberOfPredicates(tmp,false);
		      
		      if ((double)obj_upper*CCC <= pred_no && pred_no <= obj_upper) tmp->SetEntanglement(true);
		   }	
		}
       }
	
       //handle duplicates
       vector<pair<int,int> > toremove;
       for (i=0;i<n;i++){
		act=(*data.pdom->GetActions())[i];
		for (k=0;k<act->GetPrec()->Count();k++){
		   CPredicate *tmp = (*act->GetPrec())[k];
		   if (!tmp->IsStatic() && tmp->IsEntangled()){//do not consired static predicates as well as those whose arity is <=2
		      for(j=0;j<n;j++){
			if (j==i) continue;
			act2=(*data.pdom->GetActions())[j];
			CPredicate *tmp2=act2->GetPrec()->SubsumeP(tmp,data.pdom->GetTypes());
			if (tmp2!=NULL && !tmp2->IsStatic() && tmp2->IsEntangled()){//possibly "colliding" ents
			  if (!act->GetPosEff()->DisjunctU(act2->GetNegEff(),data.pdom->GetTypes()) || !act2->GetPosEff()->DisjunctU(act->GetNegEff(),data.pdom->GetTypes())){//"colliding" ents
			    bool one,two;
			    one=two=true;
			    //checking potential applicability
			    for (int k1=0;k1<act->GetPrec()->Count();k1++){
			      if (!act2->GetPrec()->Subsume((*act->GetPrec())[k1],data.pdom->GetTypes())){
				int obj_upper = data.pprob->front()->ObjectsUpperBound((*act->GetPrec())[k1]);
				int pred_no = data.pprob->front()->NumberOfPredicates((*act->GetPrec())[k1],true);
				if ((double)obj_upper*CCC > pred_no){
				  one=false;
				  break;
				}
			      }
			    }
			    for (int k1=0;k1<act2->GetPrec()->Count();k1++){
			      if (tmp2->EqualGround((*act2->GetPrec())[k1])) l=k1;
			      if (!act->GetPrec()->Subsume((*act2->GetPrec())[k1],data.pdom->GetTypes())){
				int obj_upper = data.pprob->front()->ObjectsUpperBound((*act2->GetPrec())[k1]);
				int pred_no = data.pprob->front()->NumberOfPredicates((*act2->GetPrec())[k1],true);
				if ((double)obj_upper*CCC > pred_no){
				  two=false;
				  //break;
				}
			      }
			    }
			    if (one == two) {toremove.push_back(pair<int,int>(i,k));toremove.push_back(pair<int,int>(j,l));}
			    else if (one) {toremove.push_back(pair<int,int>(j,l));}
			    else {toremove.push_back(pair<int,int>(i,k));}
			  }
			}
		      }
		   }
		}
       }
		   
       for(vector<pair<int,int> >::iterator it=toremove.begin();it!=toremove.end();it++){
	    (*(*data.pdom->GetActions())[it->first]->GetPrec())[it->second]->SetEntanglement(false);
       }
}

inline bool compSort(CAction* lhs, CAction* rhs){return lhs->ComponentCount()<rhs->ComponentCount() || (lhs->ComponentCount()==rhs->ComponentCount() && ((lhs->HasConnectedEnts() && !rhs->HasConnectedEnts()) || lhs->ActCount()<rhs->ActCount())) ;}

void CLearner::GuessMacros()
{
   //int n=data.pdom->GetActions()->Count();
   CAction *act,*act2;
   vector<vector<sh_arg_str> > sh_args_vec;
   int zzz;
   int max_comp=0;
   int no_ops;
   double avg_comp=0.0;
   
   data.pdom->MakeInvMatrix();
   data.pdom->IdentifyIncomaptiblePredicates();
   
   //data.pdom->OutInc(cout);//debug..
   
   vector<int> com_cnt;
   vector<int> com_cnt_ent;
   no_ops=data.pdom->GetActions()->Count();
   for(int zz=0;zz<data.pdom->GetActions()->Count();zz++) {
	(*data.pdom->GetActions())[zz]->GenerateStatGraph();
	(*data.pdom->GetActions())[zz]->GenerateComponents();
	if ((*data.pdom->GetActions())[zz]->ComponentCount()>max_comp) max_comp=(*data.pdom->GetActions())[zz]->ComponentCount();
	avg_comp+=(*data.pdom->GetActions())[zz]->ComponentCount();
       com_cnt.push_back((*data.pdom->GetActions())[zz]->ComponentCount());
	//cout << (*data.pdom->GetActions())[zz]->GetActName() << " - comp.no: " << com_cnt[zz] <<endl;
   }
   avg_comp /= (double)no_ops;
   GuessEntanglements();
   /*
   for(int zz=0;zz<data.pdom->GetActions()->Count();zz++) {
	(*data.pdom->GetActions())[zz]->GenerateStatGraph();
	(*data.pdom->GetActions())[zz]->GenerateComponents();
	com_cnt_ent.push_back((*data.pdom->GetActions())[zz]->ComponentCount());
	//cout << (*data.pdom->GetActions())[zz]->GetActName() << " - comp.no with ents: " << com_cnt_ent[zz] <<endl;
   }
   */
   //guessing macros
   for (int i=0;i<data.pdom->GetActions()->Count();i++){
     act=(*data.pdom->GetActions())[i];
     
     if (act->HasMeaningfulGoalEnt()){
       if (act->isMacro() && act->HasConnectedEnts()) continue; 
       for (int j=0;j<data.pdom->GetActions()->Count();j++){
	 if (j==i) continue;
	 if (data.pdom->GetActions()->Count() >= ((2*no_ops > 8) ? 8+no_ops : 3*no_ops)) break;
	 act2=(*data.pdom->GetActions())[j];
	 if (act2->HasConnectedEnts()) continue;
	 sh_args_vec.clear();
	 if (!act2->AchieverFor(act,data.pdom->GetTypes(),sh_args_vec)) continue;
	 for (vector<vector<sh_arg_str> >::iterator it=sh_args_vec.begin();it!=sh_args_vec.end();it++){
	   CMacroAction *ma = new CMacroAction(act2,act,*it);
	   if (data.pdom->GetActions()->FindProperAction(ma->GetActName(),zzz)!=NULL) break;
	   if (ma->TestUsefulness(data.pdom)){
	   // cout << ma->ToString() << endl; //debug 
	    ma->GenerateStatGraph();
	    ma->GenerateComponents();
	    if (ma->ComponentCount()>com_cnt[j] && ma->ComponentCount()>com_cnt[i]) continue;
	    //cout << "accepted - comp.no: " << ma->ComponentCount() << endl;
	    data.pdom->GetActions()->AddRecord(ma);
	    com_cnt.push_back(ma->ComponentCount());
	    break;
	   }
	 }
       }
     } else
     if (act->HasMeaningfulInitEnt()){
       if (act->isMacro() && act->HasConnectedEnts()) continue; 
       for (int j=0;j<data.pdom->GetActions()->Count();j++){
	 if (j==i) continue;
	 if (data.pdom->GetActions()->Count() >= ((2*no_ops > 8) ? 8+no_ops : 3*no_ops)) break;
	 act2=(*data.pdom->GetActions())[j];
	 if (act2->HasConnectedEnts()) continue;
	 sh_args_vec.clear();
	 if (!act->AchieverFor(act2,data.pdom->GetTypes(),sh_args_vec)) continue;
	 for (vector<vector<sh_arg_str> >::iterator it=sh_args_vec.begin();it!=sh_args_vec.end();it++){
	   CMacroAction *ma = new CMacroAction(act,act2,*it);
	   if (data.pdom->GetActions()->FindProperAction(ma->GetActName(),zzz)!=NULL) break;
	   if (ma->TestUsefulness(data.pdom)){
	   // cout << ma->ToString() << endl; //debug 
	    ma->GenerateStatGraph();
	    ma->GenerateComponents();
	    if (ma->ComponentCount()>com_cnt[i] && ma->ComponentCount()>com_cnt[j]) continue;
	   // cout << "accepted - comp.no: " << ma->ComponentCount() << endl;
	    data.pdom->GetActions()->AddRecord(ma);
	    com_cnt.push_back(ma->ComponentCount());
	    break;
	   }
	 }
       }
     }
   }
   
   //filtering
   /*
   int si,sj;
   //cout << "filtering..." <<endl;
   for(int i=data.pdom->GetActions()->Count()-1;i>=0;i--){
	if (!(*data.pdom->GetActions())[i]->isMacro()) break;
	CMacroAction *tmp_act = (CMacroAction*)(*data.pdom->GetActions())[i];
	if (tmp_act->HasConnectedEnts() && com_cnt[i]<=max_comp) continue;
	if (tmp_act->ActCount()>2) {data.pdom->GetActions()->Remove(i);continue;}
	tmp_act->FindActionPositions(data.pdom->GetActions(),si,sj);
	if (com_cnt[i]>=com_cnt[si] || com_cnt[i]>=com_cnt[sj]){data.pdom->GetActions()->Remove(i);continue;}
   }
   */
   

   vector<CAction*> macros;
   for(int i=data.pdom->GetActions()->Count()-1;i>=0;i--){
	if (!(*data.pdom->GetActions())[i]->isMacro()) break;
       macros.push_back((*data.pdom->GetActions())[i]);
   }

   sort(macros.begin(),macros.end(),compSort);

   bool eliminate = false;
   int i=0;
   no_ops = (no_ops>4) ? 4 : no_ops;
   for(vector<CAction*>::iterator it = macros.begin();it!=macros.end();it++){
      if (i>=no_ops || (double) (*it)->ComponentCount()>=avg_comp) eliminate = true;
      if (eliminate){
         int pos;
         data.pdom->GetActions()->FindProperAction((*it)->GetActName(),pos);
         data.pdom->GetActions()->Remove(pos);
     }
     i++;
   }
   
}

void CLearner::EliminateUselessMacros()
{
  CActionList *acts = data.pdom->GetActions();
  
  int max = 0;
  int total =0;
  int avg = 0;
  
  for(int i=0;i<acts->Count();i++){
      max = (*(frequency+i)>max) ? *(frequency+i) : max;
      if ((*acts)[i]->isMacro()) continue;
      total+=*(frequency+i);
      avg++;
  }
  
  avg = total / avg;
  int rem =0;
  
  for(int i=0;i<acts->Count();i++){
      //debug
      cout << (*acts)[i]->GetActName() << ": " << *(frequency+i+rem) <<endl;
      if (!(*acts)[i]->isMacro()) continue;
      if (*(frequency+i+rem)*2<max||*(frequency+i+rem)<data.train->size()||((CMacroAction*)(*acts)[i])->IsUninformative()){
	acts->Remove(i);
	i--;
	rem++;
	continue;
      }
  }
  
}

void CLearner::LearnMacrosFromFlips()
{
  vector<pair<CAction*,int> > sugg_macros;
  flipping_data *rec;
  data.pdom->IdentifyIncomaptiblePredicates();
  data.pdom->GetFlippingData();
  data.pdom->OutFlippingData(cout); //debug reasons
  while( (rec=data.pdom->NextFlippingItem()) != NULL){
      for (deque<CProblem*>::iterator it_prob=data.train->begin();it_prob!=data.train->end();it_prob++){
	  CPlan *plan = (*it_prob)->GetPlan();
	  for (int i=0;i<plan->Length();i++){
	     int s;
	     if (rec->starting->FindProperAction((*plan)[i]->GetActName(),s)!=NULL){
	        CPredicate *ref_pred = (*(*plan)[i]->GetPosEff())[(*plan)[i]->GetPosEff()->FindProperPredicate(rec->p2->GetName())];
		CAction *macro=(*rec->starting)[s];
		if (rec->finishing->FindProperAction((*plan)[i]->GetActName(),s)!=NULL && (*plan)[i]->GetNegEff()->Find(ref_pred)!=-1) continue; //starting and finishing action is the same
		//cout << "Flipped predicate: " << ref_pred->ToString(false) << endl; //debug reasons
		
		CAction *ref_act = (*plan)[i];
		vector<int> in_ma;
		deque<int> out_ma;
		in_ma.push_back(i);
		int j=i+1;
		bool closed = false;
		while (!closed && j<plan->Length()){
		  if (rec->finishing->FindProperAction((*plan)[j]->GetActName(),s)!=NULL && (*plan)[j]->GetNegEff()->Find(ref_pred)!=-1){ //the "closing action is found
		    closed=true;
		  }
		  if (closed || (*plan)[j]->GetPrec()->Find(ref_pred)!=-1 /*|| !ref_act->IndependentWith((*plan)[j])*/){
		    //vector<sh_arg_str> *sh_args=new vector<sh_arg_str>();
		    //ref_act->DetectSharedArgs((*plan)[j],*sh_args);
		    //ref_act = new CMacroAction(ref_act,(*plan)[j],*sh_args);
		    in_ma.push_back(j);
		    
		  } else {
		    out_ma.push_back(j); 
		  }
		  j++;
		}
		if (closed) {
		  bool independent=true;
		  while (independent && !out_ma.empty()) {//upwards
		      vector<int>::iterator it_out=in_ma.begin(); 
		      while (*it_out < out_ma.front() && independent){
		         independent=(*plan)[out_ma.front()]->IndependentWith((*plan)[*it_out]) && !(*plan)[*it_out]->AchieverForGnd((*plan)[out_ma.front()]);
			 it_out++;
		      }
		      if (independent) out_ma.pop_front(); 
		  }
		  independent=true;
		  while (independent && !out_ma.empty()) {//downwards
		      vector<int>::reverse_iterator it_out=in_ma.rbegin(); 
		      while (*it_out > out_ma.back() && independent){
		         independent=(*plan)[out_ma.back()]->IndependentWith((*plan)[*it_out])  && !(*plan)[out_ma.back()]->AchieverForGnd((*plan)[*it_out]);
			 it_out++;
		      }
		      if (independent) out_ma.pop_back(); 
		  }
		  
		  vector<int> fin(in_ma.size()+out_ma.size());
		  
		  merge(in_ma.begin(),in_ma.end(),out_ma.begin(),out_ma.end(),fin.begin());
		  
		  for (vector<int>::iterator it_in=++fin.begin();it_in!=fin.end();it_in++){
		      vector<sh_arg_str> *sh_args=new vector<sh_arg_str>();
		      ref_act->DetectSharedArgs((*plan)[*it_in],*sh_args);
		      ref_act = new CMacroAction(ref_act,(*plan)[*it_in],*sh_args);
		      data.pdom->GetActions()->FindProperAction((*plan)[*it_in]->GetActName(),s);
		      macro= new CMacroAction(macro,(*data.pdom->GetActions())[s],*sh_args);
		  }
		   
		  //cout << "Macro: " << macro->GetActName() << macro->GetParams()->ToString(true) << endl;//debug reasons 
		  
		  for (int k=0;k<ref_act->GetPosEff()->Count();k++){
		     if ((*it_prob)->GetGoal()->Find((*ref_act->GetPosEff())[k])!=-1){
		    //    cout << "goal achiever: " <<  (*ref_act->GetPosEff())[k]->ToString(false) << endl; //debug reasons
		        string st="stag_";
			(*macro->GetPosEff())[k]->SetEntanglement(true);
		        CPredicate *tmp = (*macro->GetPosEff())[k]->Clone();
			tmp->SetName(st+tmp->GetName());
			tmp->SetStatic(true);
			macro->GetPrec()->AddRecord(tmp);
			//if (!ent_goal->Subsume((*tmplist)[j], data.pdom->GetTypes())){
			//	ent_goal->AddRecord((*tmplist)[j]->Clone());
			//}
		     }
		     
		  }
		  
		  bool found=false;
		  vector<pair<CAction*,int> >::iterator mit=sugg_macros.begin();
		  while (!found && mit !=sugg_macros.end()){
		    if (mit->first->Equal(macro)){found=true;mit->second++;}
		    mit++; 
		  }
		  if (!found){
		    sugg_macros.push_back(make_pair<CAction*,int>(macro,1));
		  }
		}
	     }
	  }
	
      }    
  }
  
  for (vector<pair<CAction*,int> >::iterator mit=sugg_macros.begin();mit!=sugg_macros.end();mit++){
    //not enough macros w.r.t number of training plans
    if (mit->second < 2 * data.train->size() || ((CMacroAction*)mit->first)->IsUninformative() || (!mit->first->HasMeaningfulGoalEnt(1) && ((CMacroAction*)mit->first)->HasIntermediateAdditionalArgs()>1)) {mit=sugg_macros.erase(mit);mit--;continue;} 
    //"deconflicting" goal achievers 
    for (vector<pair<CAction*,int> >::iterator mit2=sugg_macros.begin();mit2!=mit;mit2++){
       if (mit->first->GetActName()==mit2->first->GetActName()){
	  if (mit->first->GetPrec()->Count() > mit2->first->GetPrec()->Count()){
	   if (mit->second >= 2 * mit2->second) {mit2=sugg_macros.erase(mit2);mit2--;} else {mit=sugg_macros.erase(mit);mit--;break;}
	    
	  } else {
	   if (mit2->second < 2 * mit->second) {mit2=sugg_macros.erase(mit2);mit2--;} else {mit=sugg_macros.erase(mit);mit--;break;}
	   
	  }
       }
    }
    
  }
 
  for (vector<pair<CAction*,int> >::iterator mit=sugg_macros.begin();mit!=sugg_macros.end();mit++){
    ((CMacroAction*) mit->first)->DetermineInequalityConstraint();
    cout << mit->second << "x  Macro: " << mit->first->GetActName() << mit->first->GetParams()->ToString(true) << endl;//debug reasons
    //cout <<mit->first->GetPrec()->ToString(false) << endl;//debug reasons
    
    data.pdom->GetActions()->AddRecord(mit->first);
  }
  
  ApplyEntanglements(true,false);

}


/////////////////////////////////////////////////////////
// Plan similarity part
////////////////////////////////////////////////////////
void CLearner::MakeAchieverClasses(vector< achievers > *achs_arr , vector< std::vector< int > > *achs_classes)
{
    bool found;
  
  
    for (int i=0;i<data.train->size();i++){
        found = false; 
        for (vector<vector<int> >::iterator it=achs_classes->begin();it!=achs_classes->end();it++){
	    //int q = (*it)[0];
	    found = true;
	    for (int j=0;j<data.pdom->GetActions()->Count();j++){
	      achievers *ai = &achs_arr[i][j];
	      achievers *aq = &achs_arr[(*it)[0]][j];
	    if (ai->size()==aq->size()){
	       for (achievers::iterator it2 = ai->begin(); it2!=ai->end();it2++){
		  for (achievers::iterator it3 = aq->begin(); it3!=aq->end();it3++){
		    found=true;
		    if (*(it2->acts)==*(it3->acts)) break;
		    found=false;
		  }
		  if (!found) break;
	       }
	    } else {
	      found=false;
	    }
	    if (!found) break;
	    }
	    if (found) {it->push_back(i);break;}
        }
        if (!found) {
	  achs_classes->push_back(*(new vector<int>(1,i)));
	  
	}
    
  }
  
  
}

/*
void CLearner::MakeAchieverSubsumedClasses(vector< achievers > *achs_arr , vector< std::vector< int > > *achs_classes)
{
    bool found;
  
  
    for (int i=0;i<data.train->size();i++){
        found = false; 
        for (vector<vector<int> >::iterator it=achs_classes->begin();it!=achs_classes->end();it++){
	    //int q = (*it)[0];
	    found = true;
	    for (int j=0;j<data.pdom->GetActions()->Count();j++){
	      achievers *ai = &achs_arr[i][j];
	      achievers *aq = &achs_arr[(*it)[0]][j];
	    if (ai->size()==aq->size()){
	       for (achievers::iterator it2 = ai->begin(); it2!=ai->end();it2++){
		  for (achievers::iterator it3 = aq->begin(); it3!=aq->end();it3++){
		    found=true;
		   // if (*(it2->acts)==*(it3->acts)) break;
		    for (int k=0;k<it->acts->size();k++){
		      if ((*(it2->acts))[k]==-1 || (*(it3->acts))[k]==(*(it2->acts))[k]){
		       // temp->push_back((*(it2->acts))[k]);
		      } else if ((*(it2->acts))[k]==-1){
		       //  temp->push_back((*(it->acts))[k]);
		      } else {
		        found=false;break;
		      }
		    }
		    found=false;
		  }
		  if (!found) break;
	       }
	    } else {
	      found=false;
	    }
	    if (!found) break;
	    }
	    if (found) {it->push_back(i);break;}
        }
        if (!found) {
	  achs_classes->push_back(*(new vector<int>(1,i)));
	  
	}
    
  }
  
  
}
*/

bool CLearner::CompatibleClasses(vector< achievers > *achs_arr, vector< int >  *class1, vector< int > *class2)
{
     bool found = true; 
     for (int j=0;j<data.pdom->GetActions()->Count();j++){
	  achievers *a1 = &achs_arr[(*class1)[0]][j];
	  achievers *a2 = &achs_arr[(*class2)[0]][j];
	   if (a1->size()==a2->size()){
	       for (achievers::iterator it2 = a1->begin(); it2!=a1->end();it2++){
		  for (achievers::iterator it3 = a2->begin(); it3!=a2->end();it3++){
		    found=true;
		   // if (*(it2->acts)==*(it3->acts)) break;
		    for (int k=0;k<it2->acts->size();k++){
		      if ((*(it2->acts))[k]==-1 || (*(it3->acts))[k]==-1 || (*(it3->acts))[k]==(*(it2->acts))[k]){
		       // temp->push_back((*(it2->acts))[k]);
		       //  temp->push_back((*(it->acts))[k]);
		      } else {
		        found=false;break;
		      }
		    }
		    if (found) break;
		  }
		  if (!found) return false;
	       }
	    } else {
	      found=false;
	    }
	    if (!found) return false;
      }
      return true;
}


void CLearner::FilterAchievers(vector< achievers > *achs_arr , vector< achievers > *achs_filtered_arr )
{
     int total;
     
     for (int i=0;i<data.train->size();i++){
        achs_filtered_arr[i].resize(data.pdom->GetActions()->Count());
        for (int j=0;j<data.pdom->GetActions()->Count();j++){
	  total=0;
          for (achievers::iterator it = achs_arr[i][j].begin(); it!=achs_arr[i][j].end();it++) total+=it->freq;
	  for (achievers::iterator it = achs_arr[i][j].begin(); it!=achs_arr[i][j].end();it++){
	    if (it->freq>=(float)total*data.flawratio){
	       achiever *a = new achiever();
	       a->acts=new vector<int>(*it->acts);
	       a->freq=it->freq; 
	       achs_filtered_arr[i][j].push_back(*a); 
	    }
	  }
	}
     }
  
}

void CLearner::SubsumeAchievers(vector< achievers > *achs_arr , vector< achievers > *achs_subsumed_arr )
{
   bool found; 
   vector<int> *temp;
   for (int i=0;i<data.train->size();i++){
        achs_subsumed_arr[i].resize(data.pdom->GetActions()->Count());
        for (int j=0;j<data.pdom->GetActions()->Count();j++){
           
	   for (achievers::iterator it = achs_arr[i][j].begin(); it!=achs_arr[i][j].end();it++){
	      found=false;
	      //temp = new list<int>();
	      for (achievers::iterator it2 = achs_subsumed_arr[i][j].begin(); it2!=achs_subsumed_arr[i][j].end();it2++){
		found=true;
	        temp = new vector<int>(); 
		for (int k=0;k<it->acts->size();k++){
		   if ((*(it->acts))[k]==-1 || (*(it->acts))[k]==(*(it2->acts))[k]){
		     temp->push_back((*(it2->acts))[k]);
		   } else if ((*(it2->acts))[k]==-1){
		     temp->push_back((*(it->acts))[k]);
		   } else {
		     found=false;break;
		   }
		 }
		 if (found){
		   it2->acts=temp;
		   it2->freq+=it->freq;
		   break;
		 }
	      }
	      if (!found){
		   achiever *a = new achiever();
		   a->acts=new vector<int>(*it->acts);
		   a->freq=it->freq;
		   achs_subsumed_arr[i][j].push_back(*a);
	      }
	   }
	}
    }
  
}

int CLearner::WhichClass(int prob_no, vector< std::vector< int > >* classes)
{
   for (int i=0;i<classes->size();i++){
      if (find((*classes)[i].begin(),(*classes)[i].end(),prob_no)!=(*classes)[i].end()) return i;
   }
   
   return -1;
}

void CLearner::CombineCompatibleClasses(vector< achievers > *achs , vector< std::vector< int > > *achs_class)
{

   for (int j=0;j<achs_class->size();j++){
    for (int j2=j+1;j2<achs_class->size();j2++){
      if (CompatibleClasses(achs,&(*achs_class)[j],&(*achs_class)[j2])){
	 (*achs_class)[j].insert((*achs_class)[j].end(),(*achs_class)[j2].begin(),(*achs_class)[j2].end());
	 achs_class->erase(achs_class->begin()+j2);
	 j2--;
      }
    }
  }
  
}


void CLearner::CreateStats()
{

  CDependency *dep;
  vector<achievers> achs_arr[data.train->size()];
  vector<achievers> achs_filtered_arr[data.train->size()];
  vector<achievers> achs_subsumed_arr[data.train->size()];
   vector<achievers> achs_filtered_subsumed_arr[data.train->size()];
  vector<vector<int> > achs_classes,achs_filtered_classes,achs_subsumed_classes,achs_filtered_subsumed_classes; 
 
  
  for (int i=0;i<data.train->size();i++){
	  achs_arr[i].clear();
	  dep=new CDependency((*data.train)[i]->GetPlan(),(*data.train)[i]->GetInit(),(*data.train)[i]->GetGoal());
	  dep->RetrieveAchievers(achs_arr[i],data.pdom->GetActions());
	  //debug
	  //cout << "Problem: " << i+1 << endl;
	  //OutputAchievers(achs_arr[i],cout);
	  
  }
  
  FilterAchievers(achs_arr,achs_filtered_arr);
  SubsumeAchievers(achs_arr,achs_subsumed_arr);
  SubsumeAchievers(achs_filtered_arr,achs_filtered_subsumed_arr);
  
  MakeAchieverClasses(achs_arr,&achs_classes);
  MakeAchieverClasses(achs_filtered_arr,&achs_filtered_classes);
  MakeAchieverClasses(achs_subsumed_arr,&achs_subsumed_classes);
  MakeAchieverClasses(achs_filtered_subsumed_arr,&achs_filtered_subsumed_classes);
  
  CombineCompatibleClasses(achs_subsumed_arr,&achs_subsumed_classes);
  CombineCompatibleClasses(achs_filtered_subsumed_arr,&achs_filtered_subsumed_classes);
	
  cout << "problem;class;subsumed;subsumed-filetered" << endl;
  for (int i=0;i<data.train->size();i++){
    cout << (*data.train)[i]->GetOrigFileName() << ";" << WhichClass(i,&achs_classes) << ";" << WhichClass(i,&achs_subsumed_classes) << ";" << WhichClass(i,&achs_filtered_subsumed_classes) << endl;
  }

  cout << "Compatible Subsumed classes: ";
  for (int j=0;j<achs_filtered_subsumed_classes.size();j++){
    for (int j2=j+1;j2<achs_filtered_subsumed_classes.size();j2++){
      if (CompatibleClasses(achs_filtered_subsumed_arr,&achs_filtered_subsumed_classes[j],&achs_filtered_subsumed_classes[j2])) cout << j <<"+" <<j2 << ";";
    }
  }
  cout << endl;	
  
  return;
  //debug
  cout << "Normal: No. of problems: " << data.train->size() << ", no. of classes: " << achs_classes.size() <<endl;
  for (int j=0;j<achs_classes.size();j++){
       cout << "Class: " << j <<endl;
       cout << "problems: ";
       for (vector<int>::iterator it2 = achs_classes[j].begin(); it2!=achs_classes[j].end();it2++){
	  cout << " " << (*it2);
       }
       cout << endl;
       OutputAchievers(achs_arr[achs_classes[j][0]],cout);
  }
  cout << endl << endl;
  
  //debug
  /*
  cout << "Filtered: No. of problems: " << data.train->size() << ", no. of classes: " << achs_filtered_classes.size() <<endl;
  for (int j=0;j<achs_filtered_classes.size();j++){
       cout << "Class: " << j <<endl;
       cout << "problems: ";
       for (vector<int>::iterator it2 = achs_filtered_classes[j].begin(); it2!=achs_filtered_classes[j].end();it2++){
	  cout << " " << (*it2);
       }
       cout << endl;
       OutputAchievers(achs_filtered_arr[achs_filtered_classes[j][0]],cout);
  }
  
  cout << endl << endl;
  */
   //debug
  cout << "Subsumed: No. of problems: " << data.train->size() << ", no. of classes: " << achs_subsumed_classes.size() <<endl;
  for (int j=0;j<achs_subsumed_classes.size();j++){
       cout << "Class: " << j <<endl;
       cout << "problems: ";
       for (vector<int>::iterator it2 = achs_subsumed_classes[j].begin(); it2!=achs_subsumed_classes[j].end();it2++){
	  cout << " " << (*it2);
       }
       cout << endl;
       OutputAchievers(achs_subsumed_arr[achs_subsumed_classes[j][0]],cout);
  }
  
    cout << endl << endl;
   //debug
  cout << "Filtered Subsumed: No. of problems: " << data.train->size() << ", no. of classes: " << achs_filtered_subsumed_classes.size() <<endl;
  for (int j=0;j<achs_filtered_subsumed_classes.size();j++){
       cout << "Class: " << j <<endl;
       cout << "problems: ";
       for (vector<int>::iterator it2 = achs_filtered_subsumed_classes[j].begin(); it2!=achs_filtered_subsumed_classes[j].end();it2++){
	  cout << " " << (*it2);
       }
       cout << endl;
       OutputAchievers(achs_filtered_subsumed_arr[achs_filtered_subsumed_classes[j][0]],cout);
  }
  
    cout << endl << endl;
   //debug

    cout << "Compatible Subsumed classes: ";
  for (int j=0;j<achs_filtered_subsumed_classes.size();j++){
    for (int j2=j+1;j2<achs_filtered_subsumed_classes.size();j2++){
      if (CompatibleClasses(achs_filtered_subsumed_arr,&achs_filtered_subsumed_classes[j],&achs_filtered_subsumed_classes[j2])) cout << j <<"+" <<j2 << ";";
    }
  }
  cout << endl;	
}

void CLearner::OutputAchievers(vector<achievers> &achs, ostream &s)
{
  CActionList *acts = data.pdom->GetActions();
  
  for (int i=0;i<achs.size();i++){
    s << "Action: " <<  (*acts)[i]->GetActName() << endl;
    s << "achievers:" << endl;
    for (achievers::iterator it = achs[i].begin();it!=achs[i].end();it++){
       for (vector<int>::iterator it2 = it->acts->begin(); it2!=it->acts->end();it2++){
	  s << " " << ((*it2)==-1 ? "init" : (*acts)[*it2]->GetActName()); 
       }
       s << " - " << it->freq << endl;
    }
    
  }
}