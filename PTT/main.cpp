#include <stdio.h>

#include "common.h"
#include "CProblem.h"
#include "CLearner.h"

#include <time.h>
#include <strings.h>

extern FILE *yyin, *yyout;
extern int yylex(void);
extern int yyparse(void);

extern string LTrim(string);
extern string ToLower(string);
extern deque<string>* ExplodeBySpace(string,const char* c=" \t");
extern bool CompareProbNames(CProblem*, CProblem*);

CDomain *dom; //must remain global due to bison 

CProblem *prob;  //must remain global due to bison

CPlan pl;

PTT_Data data; //global structure

void init(){
 
  	//initial settings
	data.pdom=NULL;
	data.pprob=new deque<CProblem*>();
	data.train=new deque<CProblem*>();
	data.plannercmd=new list<string>();
	data.flawratio = 0.0f;
	data.oplimit = 0;
	data.inent_filter=false;
	data.par_a=data.par_b= 1.0f;
	data.par_c=5;
	data.optimization=data.goal=data.inv=data.grinv=data.repl = data.entanglements = data.init = data.prec = data.succ = false;
	data.macroentanglements=data.macros=data.macrosx=0;
	data.stats=false;

  
}

act* toAct(deque<string> *str_list){
 
  act *ret=new act();
  int x;
  
  ret->first=str_list->front();
  str_list->pop_front();
  while (!str_list->empty()){
      string s = str_list->front();
      sscanf(s.data(),"$%i",&x);
      ret->second.push_back(x);
      str_list->pop_front();
  }
  
  return ret;
}

list<mcr*> mcr_stuff;

void parseKF(string fname){
 
  fstream f;
  string s;
  bool ismcr = false;
  deque<string> *temp_strings;
  act *tmp_act;
  mcr *mcr_tmp;
  
   f.open(fname.data(),fstream::in);
	
   while (!f.eof()){
	getline(f,s);
	s=LTrim(s);
	
	if(s.find(";macro",0)==0){	
	    ismcr=true;
	    continue;
	}
	
	if(s[0]==';'){
	    continue;
	}
	
	if (!ismcr) continue;
	
	//process macros
	bool first=true;
	while (!f.eof()&&!s.empty()){
	  temp_strings=ExplodeBySpace(s);
	  tmp_act=toAct(temp_strings);
	  if (first){
	    mcr_tmp=new mcr();
	    mcr_tmp->first=tmp_act;
	    first=false;
	  } else {
	    mcr_tmp->second.push_back(tmp_act);
	  }
	  getline(f,s);
	  s=LTrim(s);
	}
	if (!first) mcr_stuff.push_back(mcr_tmp);
	
  }
  f.close();
}

void parse_input(string fname){
	
	fstream f;
	string s;
	CPlan *plan;
	deque<string> *pliststr;

	
	f.open(fname.data(),fstream::in);
	
	while (!f.eof()){
		getline(f,s);
		s=LTrim(s);

		if(s.find("domain=",0)==0){  //it's a domain
			dom=new CDomain();
		//	cout << s.substr(7);
			yyin=fopen(s.substr(7).data(),"r");
			yyparse();
 		//	int zz=yylex();
		//	long zz=ftell(yyin);
			fclose(yyin);
			dom->SetOrigFileName(s.substr(7));
			dom->IdentifyStaticPredicates();
			data.pdom=dom;
			
		}
		if(s.find("training=",0)==0){  //it's a training problem with plan
			prob=new CProblem();
			plan=new CPlan();
			pliststr=ExplodeBySpace(s.substr(9));
			yyin=fopen((*pliststr)[0].data(),"r");
			yyparse();
			fclose(yyin);
			plan->ParsePlanFile((*pliststr)[1]);
			prob->AssignPlan(plan);
			prob->SetOrigFileName((*pliststr)[0]);
			data.train->push_back(prob);
			prob->AssignDomain(data.pdom); //domain must be parsed before
			delete pliststr;
		}
		if(s.find("problem=",0)==0){  //it's a planning problem (without plan)
			prob=new CProblem();
			yyin=fopen(s.substr(8).data(),"r");
			yyparse();
			fclose(yyin);
			prob->SetOrigFileName(s.substr(8));
			data.pprob->push_back(prob);
			prob->AssignDomain(data.pdom); //domain must be parsed before
		}
		if(s.find("plannercmd=",0)==0){  //it's a command for planner (for doing experiments)
			data.plannercmd->push_back(s.substr(11));
		}
		if(s.find("flawratio=",0)==0){ //it's for a flaw ratio
			sscanf(s.data(),"flawratio=%f",&data.flawratio);
		}
		if(s.find("oplimit=",0)==0){ //it's for an operator limit (inner entanglements)
			sscanf(s.data(),"oplimit=%i",&data.oplimit);
			data.inent_filter=true;
		}
		if(s.find("optimization=",0)==0){ //it's for post-planning optimization purposes
			data.optimization=true;
			pliststr=ExplodeBySpace(s.substr(13));
			for (deque<string>::iterator it = pliststr->begin();it<pliststr->end();it++){
				if (strcasecmp(it->data(),"goal")==0) data.goal = true;
				if (strcasecmp(it->data(),"inverse")==0) data.inv = true;
				if (strcasecmp(it->data(),"groupinverse")==0) data.grinv = true;
				if (strcasecmp(it->data(),"replaceable")==0) data.repl = true;
			}
		}
		if(s.find("entanglements=",0)==0){ //it's for entanglements purposes
			data.entanglements=true;
			pliststr=ExplodeBySpace(s.substr(14));
			for (deque<string>::iterator it = pliststr->begin();it<pliststr->end();it++){
				if (strcasecmp(it->data(),"goal")==0) data.goal = true;
				if (strcasecmp(it->data(),"init")==0) data.init = true;
				if (strcasecmp(it->data(),"prec")==0) data.prec = true;
				if (strcasecmp(it->data(),"succ")==0) data.succ = true;
			}
		}
		if(s.find("macroentanglements=",0)==0){ //it's for macroentanglements purposes
			//TODO
			if (s.substr(19).find("outer",0)==0) {data.macroentanglements=1;}
			if (s.substr(19).find("agouter",0)==0) {data.macroentanglements=2;}
			if (s.substr(19).find("inner",0)==0) {data.macroentanglements=3;}
			if (s.substr(19).find("both",0)==0) {data.macroentanglements=4;}
		}
		if(s.find("macros=",0)==0){ //it's for KER macros
			sscanf(s.data(),"macros=%f %f %i",&data.par_a,&data.par_b,&data.par_c);
			data.macros=1;
		}
		if(s.find("macrosx=",0)==0){ //other macro learning techniques
		        sscanf(s.data(),"macrosx=%i",&data.macrosx);
		}
		if(s.find("externmacros=",0)==0){//macros are in the external file
			parseKF(s.substr(13));
			data.macroentanglements=6;
		}
		if(s.find("stats",0)==0){//statistics
                       data.stats=true;
		}

	}
	
	if (data.macrosx!=0) data.macroentanglements=0; //avoiding "double" macro generation

	f.close();
}

void MakeProbsNamesDifferent()
{//make sure that problems have different names
	deque<CProblem*> *plistprobs;
	deque<CProblem*>::iterator it,last;
	string s,s2;
	int i;

	plistprobs=new deque<CProblem*>(data.train->begin(),data.train->end());
	plistprobs->insert(plistprobs->end(),data.pprob->begin(),data.pprob->end());
	sort(plistprobs->begin(),plistprobs->end(),CompareProbNames);
	
	it=plistprobs->begin();
	last=plistprobs->end();
	while(it!=last){
		i=1;
		s=(*it)->GetName();
		if (it<last-1&&s==(*(it+1))->GetName()){
			s2=s;
			s2+=((char) 'a');
			(*it)->SetName(s2);
			while(it<last-i&&s==(*(it+i))->GetName()){
				s2=s;
				s2+=((char) 'a'+i);
				(*(it+i))->SetName(s2);
				i++;
			}
		}
		it+=i;
	}
	delete plistprobs;
		
}

void GeneratePrologFiles(){
	int i,n1,n2;
	fstream f;
	string fname;
	
	if (data.pdom==NULL) {
		cout << "No domain defined" << endl;
		return;
	}

	fname="headers-"+data.pdom->GetName();
	f.open(fname.data(),fstream::out|fstream::trunc);
	
	data.pdom->GeneratePrologFile();
	cout << data.pdom->GetName() << endl << endl;
	f << "/*" << data.pdom->GetName() <<"*/" << endl;
	f << ":-include(" << ToLower(data.pdom->GetName()) <<")." <<endl<<endl;
	
	n1=data.train->size();
	for(i=0;i<n1;i++){
		(*data.train)[i]->GeneratePrologFile();
		cout << (*data.train)[i]->GetName() << endl;
		f << "problem(" << (*data.train)[i]->GetName() << ")." <<endl;
	}
	cout << endl;
	n2=data.pprob->size();
	for(i=0;i<n2;i++){
		(*data.pprob)[i]->GeneratePrologFile();
		cout << (*data.pprob)[i]->GetName() << endl;
		f << "out_problem(" << (*data.pprob)[i]->GetName() << ")." <<endl;
	}
	f.close();

}

void GenerateBatchFilesForPlanners(){
	string s,p,name;
	fstream f;
	int i,n;

	while(!data.plannercmd->empty()){
		s=data.plannercmd->front();
		name=s.substr(2,s.find(" ")-2);
		name+='-';
		name+=data.pdom->GetName();
		
		f.open(name.data(),fstream::out|fstream::trunc);
		
		n=data.pprob->size();
		for(i=0;i<n;i++){
			//at first - the original problem
			p=s;
			if (p.find("%d")!= -1) {p.replace(p.find("%d"),2,data.pdom->GetOrigFileName());}
			p.replace(p.find("%p"),2,(*data.pprob)[i]->GetOrigFileName());
			if (p.find("%o")!= -1) {p.replace(p.find("%o"),2,(*data.pprob)[i]->GetName()+"-orig.out");}
			f << p <<endl;
			//then - the updated problem
			p=s;
			//if (p.find("%d")!= -1) {p.replace(p.find("%d"),2,(*data.pprob)[i]->GetName()+"-dom.pddl");}
			if (p.find("%d")!= -1) {p.replace(p.find("%d"),2,ToLower(data.pdom->GetName())+"-upddom.pddl");}
			p.replace(p.find("%p"),2,(*data.pprob)[i]->GetName()+".pddl");
			if (p.find("%o")!= -1) {p.replace(p.find("%o"),2,(*data.pprob)[i]->GetName()+"-updated.out");}
			f << p <<endl;
			//then - the second updated problem
			p=s;
			//if (p.find("%d")!= -1) {p.replace(p.find("%d"),2,(*data.pprob)[i]->GetName()+"-dom.pddl");}
			if (p.find("%d")!= -1) {p.replace(p.find("%d"),2,ToLower(data.pdom->GetName())+"-2-upddom.pddl");}
			p.replace(p.find("%p"),2,(*data.pprob)[i]->GetName()+"-2.pddl");
			if (p.find("%o")!= -1) {p.replace(p.find("%o"),2,(*data.pprob)[i]->GetName()+"-updated-2.out");}
			f << p <<endl <<endl;

		}
		f.close();
	
		data.plannercmd->pop_front();
	}

}

void PlansToPDDL(){
	
	deque<CProblem*>::iterator it;
	fstream f;
	string s;

	for (it = data.train->begin();it != data.train->end();it++){
		s = (*it)->GetOrigFileName();
		s += ".plan";
		f.open(s.data(),fstream::out|fstream::trunc);
		(*it)->GetPlan()->GeneratePDDLPlan(f);
		f.close();
	}
}

void DomainToPDDL(){
	fstream f;
	string s;
	
	s = data.pdom->GetOrigFileName();
	/*
	int pz = s.rfind(".");

	if (pz==-1){
		s+= "_PTT.pddl";
	} else {
		s.replace(pz,1,"_PTT.");
	}
	*/
	s=s+".ref";

	f.open(s.data(),fstream::out|fstream::trunc);
	data.pdom->GeneratePDDL(f);

	f.close();
}

void ProblemsToPDDL(bool train = false){
	fstream f;
	string s;
	deque<CProblem*>::iterator it;

	it = train ? data.train->begin() : data.pprob->begin();

	while (it != (train ? data.train->end() : data.pprob->end())){

		s = (*it)->GetOrigFileName();
		/*
		int pz = s.rfind(".");

		if (pz==-1){
			s+= "_PTT.pddl";
		} else {
			s.replace(pz,1,"_PTT.");
		}
	*/
		s = s+".ref";

		f.open(s.data(),fstream::out|fstream::trunc);
		(*it)->ProblemToPDDL(f,data.pdom->GetName());

		f.close();

		it++;
	}
}

CLearner learn;

int main(int argc, char** argv){


	clock_t begin, parsed, learning, end;
	double diff;
	
	if (argc==1) {printf("Missing arguments...\n");exit(1);}
	
	init();
	
	if (strcasecmp(argv[1],"-q")==0){//1st argument set to "-q" to allow "quick mode"
	   //domain (2nd argument)
	   dom=new CDomain();
	   yyin=fopen(argv[2],"r");
	   yyparse();
 	   fclose(yyin);
	   dom->SetOrigFileName(argv[2]);
	   dom->IdentifyStaticPredicates();
	   data.pdom=dom;
	   cout << "domain processed.." <<endl;
	   //problem (3rd argument)
	   prob=new CProblem();
	   yyin=fopen(argv[3],"r");
	   yyparse();
	   fclose(yyin);
	   prob->SetOrigFileName(argv[3]);
	   data.pprob->push_back(prob);
	   prob->AssignDomain(data.pdom); //domain must be parsed before
	   prob->AssignTypes();
	   cout << "problem processed.." <<endl;
	   
	   data.init=data.goal=true;
	   //learning
	  // learn.GuessEntanglements();
	   learn.GuessMacros();
	   learn.ApplyEntanglements(true);
	   //exporting reformulated domain and problem models
	   DomainToPDDL();
	   ProblemsToPDDL();
	   exit(0);
	}
	
	begin=clock();
/*
	dom=new CDomain();
	prob=new CProblem();

//	printf("%i - %s\n",argc,argv[1]);

	//playing with domains
	yyin = fopen("driverlog.pddl","r");

	yyparse();
	//printf("%s",dom.name);

	//playing with problems
	yyin = fopen("pfile1","r");

	yyparse();

	//playing with plans
	pl.ParsePlanFile("dl1.out");
	prob->AssignPlan(&pl);

	cout << dom->GetName() << prob->GetName() << endl;
	dom->GeneratePrologFile();
	prob->GeneratePrologFile();
*/
	parse_input(argv[1]);
	//MakeProbsNamesDifferent();
	parsed=clock();
	diff=(parsed-begin);
	diff=diff/CLOCKS_PER_SEC;

	printf("Elapsed time for parsing: %.3lf\n", diff);
	
	
	if (data.macroentanglements!=6 && data.macrosx!=10) learn.Initialize();
	
	if (data.stats){
	 learn.CreateStats();
	 exit(0); 
	}
	
	//DomainToPDDL();
	//PlansToPDDL();
	//return 0;

	if (data.optimization){
		learn.OptimizePlans();
		PlansToPDDL();
	} else {
		/*
		learn.CreateDependencies();
		learn.CreateMatrixOfCandidates();
		*/
		if (data.macros){
		  learn.LearnMacros();
		  DomainToPDDL();
		} else {
		if (data.entanglements&&data.macroentanglements==0&&data.macrosx==0){
			if (data.init || data.goal) learn.LearnEntanglements();
			if (data.prec || data.succ) learn.LearnInnerEntanglements();

			//learn.OutputEnt(cout);
			//learn.OutputIntEnt(cout);
			if (data.init || data.goal) learn.ApplyEntanglements();
			if (data.prec || data.succ) learn.ApplyInnerEntanglements();
			//learn.GenerateMacrosFromInnerEntanglements();
			//learn.ApplyEntanglements();

			DomainToPDDL();
			ProblemsToPDDL();
		} else {
			//learn.CreateDependencies();
			//learn.LearnMacros();
		    if (data.macroentanglements==1 || data.macroentanglements==2){
			learn.LearnMacrosFromCA();
			//PlansToPDDL();//debug reasons
			DomainToPDDL();
			ProblemsToPDDL();
		    }
		    if (data.macroentanglements==3){
			learn.LearnInnerEntanglements();
			learn.GenerateMacrosFromInnerEntanglements();
			DomainToPDDL();
		      
		    }
		    if (data.macroentanglements==4){
			learn.LearnEntanglements();
		        learn.LearnInnerEntanglements();
			learn.GenerateMacrosFromInnerEntanglements();
			learn.ApplyEntanglements();
			DomainToPDDL();
			ProblemsToPDDL();
		      
		    }
		    if (data.macroentanglements==6&&data.macrosx==0){
			data.pdom->ImportMacros(&mcr_stuff);
			if (data.init || data.goal) {
			  data.pdom->IdentifyStaticPredicates();
			  learn.Initialize();
			  learn.LearnEntanglements();
			  learn.EliminateUselessMacros();
			   learn.ApplyEntanglements(true);
			}
			DomainToPDDL();
			ProblemsToPDDL();
		    }
		
		}
		if (data.macrosx==1 || data.macrosx==11){ //"flipping" macros
		   //data.pdom->IdentifyIncomaptiblePredicates();
		   //data.pdom->OutIncompatiblePreds(cout);
		   //data.pdom->GetFlippingData();
		   //data.pdom->OutFlippingData(cout);
		  data.init=data.goal=true;
		  learn.LearnMacrosFromFlips(data.macrosx==11);
		  DomainToPDDL();
		  ProblemsToPDDL();
		}
		
		if (data.entanglements&data.macrosx==10){
		  cout << "importing macros"<< endl;
		  data.pdom->ImportMacros(&mcr_stuff);
		  cout << "imported macros"<< endl;
		  dom->IdentifyStaticPredicates();
		  learn.Initialize();
		  //data.init=data.goal=true;
		  cout << "Initialized"<<endl;
		  learn.LearnEntanglements();
		  cout << "Ents learnt"<<endl;
	        //  learn.EliminateUselessMacros();
		  learn.ApplyEntanglements(true);
		  
		}
		
		if (data.macrosx==2){ //"absorbed" macros
		   //data.pdom->IdentifyIncomaptiblePredicates();
		   //data.pdom->OutIncompatiblePreds(cout);
		   //data.pdom->GetFlippingData();
		   //data.pdom->OutFlippingData(cout);
		 // data.pdom->ImportMacros(&mcr_stuff);
		  //data.pdom->IdentifyStaticPredicates();
	          //learn.Initialize();
		  data.init=data.goal=true;
		  data.pdom->ReconstructEntanglements();
		  //cout << "Initialized " << endl;
		  learn.LearnMacrosFromAbsorbtions();
		  DomainToPDDL();
		  ProblemsToPDDL();
		}
		
		}
	}

	//data.pdom->OutStatic(cout); //for debug reasons

	learning=clock();
	diff=(learning-parsed);
	diff=diff/CLOCKS_PER_SEC;
	printf("Elapsed time for learning: %.3f\n", diff);
	/*
	GeneratePrologFiles();
	GenerateBatchFilesForPlanners();
*/


	//getchar();
}