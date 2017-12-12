#include "CParameter.h"

bool operator==(const sh_arg_str& lhs,const sh_arg_str& rhs){
    return (lhs.first==rhs.first && lhs.second==rhs.second);
}

CParameter::CParameter(){
	this->params=new deque<ParamRecord>();
}

//creation of parameters from list of variables (without types)
CParameter::CParameter(deque<string> *vars,bool constant){
	ParamRecord tmp;
	//int i,n;
	
	this->params=new deque<ParamRecord>();
	//n=vars->size();
	tmp.plisttypes=NULL;
	while(!vars->empty()){
		string tmpstr=vars->front();
	        if (!constant&&tmpstr[0]=='?') tmp.name=tmpstr.substr(1); else tmp.constant=tmpstr;
		this->params->push_back(tmp);
		vars->pop_front();
	}

}

CParameter::~CParameter(){
	delete(this->params);
}

void CParameter::AddRecord(ParamRecord *r){
	this->params->push_back(*r);

}

void CParameter::AddRecord(ParamRecord *r, string suffix){
	if (!r->name.empty()) r->name+=suffix;
	this->params->push_back(*r);

}

/* TODO get rid of it... */
deque<ParamRecord>* CParameter::GetPList(){
	return this->params;
}

CParameter& CParameter::operator+=(CParameter &cpar){
	
	CParameter &p=*this;
	deque<ParamRecord> *prlist=cpar.GetPList();
	ParamRecord pr;
	while(!prlist->empty()){
		pr=prlist->front();
		p.AddRecord(&pr);
		prlist->pop_front();
	}
	return p;
}

unsigned int CParameter::Count()
{
	if (this->params->empty()) {return 0;} else {return this->params->size();}
}

ParamRecord* CParameter::operator[](unsigned int i)
{
	return &(*this->params)[i];
}



int CParameter::Find(string name)
{
	for (int i=0;i<this->Count();i++){
		if ((*this)[i]->name==name) return i;
	}

	return -1;
}

//must have the same variable name or be grounded
bool CParameter::operator==(CParameter &p)
{
	if (this->Count()!=p.Count()) return false;
	for (int i=0;i<this->Count();i++){
		if ((*this)[i]->constant.empty()){
			if (strcasecmp((*this)[i]->name.data(),p[i]->name.data())!=0) return false;
		} else {
			if (strcasecmp((*this)[i]->constant.data(),p[i]->constant.data())!=0) return false;
		}
	}
	return true;
}

CParameter* CParameter::Clone()
{
	CParameter *ret=new CParameter();
	ParamRecord *pr;

	for(int i=0;i<this->Count();i++){
		pr=new ParamRecord;
		pr->constant=(*this->params)[i].constant;
		pr->name=(*this->params)[i].name;
		if ((*this->params)[i].plisttypes!=NULL) pr->plisttypes=new deque<string>(*(*this->params)[i].plisttypes); else pr->plisttypes=NULL;
		ret->AddRecord(pr);
	}

	return ret;

}

//tests if ground arguments correspond (w.r.t. sh)
bool CParameter::IsCorresponding(CParameter *p, vector<sh_arg_str> &sh)
{
	int n = sh.size();

	for (int i=0;i<n;i++){
		//if (((*this)[sh[i].first]->constant.empty()||strcasecmp((*this)[sh[i].first]->constant.data(),(*p)[sh[i].second]->constant.data())!=0)) {
                 if (strcasecmp((*this)[sh[i].first]->name.data(),(*p)[sh[i].second]->name.data())!=0||strcasecmp((*this)[sh[i].first]->constant.data(),(*p)[sh[i].second]->constant.data())!=0) {
		  return false;
		}
	}

	return true;

}
//detects shared arguments and return to sh (if sh initially empty) - otherwise test whether arguments in sh are shared (if not then remove from sh)
void CParameter::DetectShared(CParameter *p, vector<sh_arg_str> &sh)
{
	int i,j,k;
	sh_arg_str x;
	
	if (sh.empty()){
		for (i=0;i<this->Count();i++){
			for (j=0;j<p->Count();j++){
				if ((!(*this)[i]->constant.empty()&&strcasecmp((*this)[i]->constant.data(),(*p)[j]->constant.data())==0) ||
				(!(*this)[i]->name.empty()&&strcasecmp((*this)[i]->name.data(),(*p)[j]->name.data())==0) ) {
					x.first=i;
					x.second=j;
					sh.push_back(x);
				}
			}
		}

	} else {
		for (k=0;k<sh.size();k++){
			x=sh[k];
			i=x.first;
			j=x.second;
			if (((*this)[i]->constant.empty()||strcasecmp((*this)[i]->constant.data(),(*p)[j]->constant.data())!=0) &&
				((*this)[i]->name.empty()||strcasecmp((*this)[i]->name.data(),(*p)[j]->name.data())!=0) ) {
				sh.erase(sh.begin()+k);
				k--;
			}
		}
		
	}

}

void CParameter::ReplacePars(list<par_repl_str>& par_repl)
{
	deque<ParamRecord>::iterator it = params->begin();
	list<par_repl_str>::iterator pr_it = par_repl.begin();

	while (it != params->end()){
		for (pr_it = par_repl.begin();pr_it != par_repl.end();pr_it++){
			if ((*it)==*(*pr_it).first){
				(*it) = *(*pr_it).second;
				break;
			}
		}
		it++;
	}
}

string CParameter::ToString(bool types)
{
	string ret;
	deque<ParamRecord>::iterator it = params->begin();
	while (it!=params->end()){
		ret+=" ";
		if ((*it).constant.empty()){
			ret+="?";
			ret+=(*it).name;
		} else {
			ret+=(*it).constant;
		}
		if (types && !(*it).plisttypes->empty()) {
			ret+=" - ";
			if ((*it).plisttypes->size()==1){
				ret+=(*(*it).plisttypes)[0];
			} else {
				ret+="(either";
				for (int i=0;i<(*it).plisttypes->size();i++){
					ret+=" ";
					ret+=(*(*it).plisttypes)[i];
				}
				ret+=")";
			}
		}
		it++;
	}

	return ret;
}

void CParameter::AssignTypes(CTypes* types)
{
	deque<ParamRecord>::iterator it;
	string t;

	for (it = params->begin();it!=params->end();it++){
		t = types->FindType((*it).constant);
		(*it).plisttypes = new deque<string>();
		(*it).plisttypes->push_front(t);
	}
}
