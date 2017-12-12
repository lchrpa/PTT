#ifndef CTYPES
#define CTYPES

#include <map>
#include <string>
#include <ctype.h>
#include <list>
#include  <deque>
#include <algorithm>

//#include "CDomain.h"
//#include "common.h"

using namespace std;

/*
struct charcomp {
  bool operator() (const char *lhs, const char *rhs) const
  {return stricmp(lhs,rhs)<0;}
};

*/
struct TypesRecord{
	string	parent;
	deque<string>*  plistchilds;
};

struct TypesNode{
	TypesNode	*parent;
	deque<TypesNode*>  listchilds;
	string name;
};

class CTypes{
private:
	TypesNode* GetNode(string);
	deque<TypesRecord>* types;
	map<string,TypesNode*> *map_type_name; //associative field - key: type name, value: node in types tree

public:
	bool InFamilyLine(string,string,bool oneway=false);
	void MakeTree();
	list<string>* GetPrologRepresentation();
	list<string>* GetPrologRepresentationForObject(string);
	CTypes();
	~CTypes();
	void AddRecord(TypesRecord*);
	inline deque<TypesRecord>* GetTypes(){return this->types;}

	string ToString(void);
	string FindType(string obj_name);
	deque<string>* FindAllObjects(deque<string> *type, CTypes* objects);
	void FindAllLeafTypes(deque<string> *in,deque<string> *out);
	int Count(void);
	CTypes* Clone();
	void Merge(CTypes*);
};

#endif

