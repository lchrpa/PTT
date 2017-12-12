#ifndef CPARAMETER
#define CPARAMETER

#include <string>
#include <strings.h>
#include <ctype.h>
#include  <deque>
#include <vector>
#include <list>

#include "CTypes.h"

//#include "CDomain.h"

using namespace std;

struct ParamRecord{
	string	name;
	deque<string>*  plisttypes;
	string constant;

	bool operator==(ParamRecord &pr){
		return (name.empty()&&strcasecmp(constant.data(),pr.constant.data())==0)||
			(constant.empty()&&strcasecmp(name.data(),pr.name.data())==0);
	}
};

struct sh_arg_str {
	int first,second;

	bool operator==(sh_arg_str s){
		return (this->first == s.first && this->second == s.second);
	}
	
};


struct par_repl_str {
	ParamRecord *first;
	ParamRecord *second;
};

class CParameter{
private:
	deque<ParamRecord>* params;

public:
	void DetectShared(CParameter*,vector<sh_arg_str>&);
	bool IsCorresponding(CParameter *p, vector<sh_arg_str> &sh);
	CParameter* Clone();
	int Find(string);
	unsigned int Count();
	CParameter();
	CParameter(deque<string>*,bool constant=false);
	~CParameter();
	void AddRecord(ParamRecord*);
	void AddRecord(ParamRecord*,string);
	deque<ParamRecord>*  GetPList();
	CParameter& operator+=(CParameter&);
	ParamRecord* operator[](unsigned int);
	bool operator==(CParameter&);
	void ReplacePars(list<par_repl_str>& par_repl);
	string ToString(bool types);
	void AssignTypes(CTypes* types);
};


#endif