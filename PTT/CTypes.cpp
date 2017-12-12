#include "CTypes.h"


extern string ToLower(string);

CTypes::CTypes(){
	this->types=new deque<TypesRecord>();
	this->map_type_name=new map<string,TypesNode*>();
}

CTypes::~CTypes(){
	delete(this->types);
	delete(this->map_type_name);
}

void CTypes::AddRecord(TypesRecord *r){
	
	this->types->push_back(*r);

}

list<string>* CTypes::GetPrologRepresentation()
{
	list<string>  *ret=new list<string>();
	unsigned int  n=this->types->size();
	unsigned int  i,j,n1;
	string		  s;
	TypesRecord   r;

	for(i=0;i<n;i++){
		r=(TypesRecord)(*this->types)[i];
		n1=r.plistchilds->size();
		for(j=0;j<n1;j++){
			s="type_inherits(";
			s+=ToLower((*r.plistchilds)[j]);
			s+=",";
			s+=ToLower(r.parent);
			s+=").";
			ret->push_back(s);
		}
	}

	return ret;
}

list<string>* CTypes::GetPrologRepresentationForObject(string probname)
{
	list<string>  *ret=new list<string>();
	unsigned int  n=this->types->size();
	unsigned int  i,j,n1;
	string		  s;
	TypesRecord   r;

	for(i=0;i<n;i++){
		r=(TypesRecord)(*this->types)[i];
		n1=r.plistchilds->size();
		for(j=0;j<n1;j++){
			s="object(";
			s+=ToLower(probname);
			s+=",";
			s+=ToLower((*r.plistchilds)[j]);
			s+=",";
			s+=ToLower(r.parent);
			s+=").";
			ret->push_back(s);
		}
	}

	return ret;
}

//returns types node (if it does not exist then create it)  -  cannot be called if mapping set as false
TypesNode* CTypes::GetNode(string s)
{
	string type=ToLower(s);
	map<string,TypesNode*>::iterator it;
	TypesNode *tn;

	it=this->map_type_name->find(type);
	if (it!=this->map_type_name->end()) { 
		return it->second;
	} else {
		tn=new TypesNode;
		tn->name=s;
		tn->parent=NULL;
		(*this->map_type_name)[type]=tn;
		return tn;
	}
}

void CTypes::MakeTree()
{
	int i,j;
	TypesRecord *r;
	TypesNode *pred,*succ;

	for (i=0;i<this->types->size();i++){
		r=&(*this->types)[i];
		pred=this->GetNode(r->parent);
		for (j=0;j<r->plistchilds->size();j++){
			succ=this->GetNode((*r->plistchilds)[j]);
			succ->parent=pred;
			pred->listchilds.push_back(succ); 
		}
	}

}

//inspects whether the types are in family line (i.e., predecessor, successor)
bool CTypes::InFamilyLine(string one, string two, bool oneway)
{
	TypesNode *n1,*n2,*test;
	
	n1=this->GetNode(one);
	n2=this->GetNode(two);

	if (n1==n2) return true;

	test=n1->parent;
	while (test!=NULL){
		if (test==n2) {
			return true;
		} else {
			test=test->parent;
		}
	}

	if (oneway) return false;

	test=n2->parent;
	while (test!=NULL){
		if (test==n1) {
			return true;
		} else {
			test=test->parent;
		}
	}

	return false;
}

string CTypes::ToString(void)
{
	string ret;
	deque<TypesRecord>::iterator it = types->begin();

	while(it!=types->end()){
		for (int i=0;i<(*it).plistchilds->size();i++){
			ret+=" ";
			ret+=(*(*it).plistchilds)[i];
		}
		if ((*it).parent!="xobject"){
		  ret+=" - ";
		  ret+=(*it).parent;
		}
		ret+="\n";
		it++;
	}
	
	return ret;
}

string CTypes::FindType(string obj_name)
{
		
	deque<TypesRecord>::iterator it = types->begin();
	
	while (it!=types->end()){
		if (find((*it).plistchilds->begin(),(*it).plistchilds->end(),obj_name) != (*it).plistchilds->end()){
			return (*it).parent;
		}
		it++;
	}

	return "";
}

deque<string>* CTypes::FindAllObjects(deque<string> *type, CTypes* objects)
{
	deque<TypesRecord>::iterator it;
	deque<string>::iterator sit;
	deque<string>* ret = new deque<string>();
	
	for (it = objects->types->begin();it != objects->types->end();it++){
		for (sit = type->begin();sit!=type->end();sit++){
			if (InFamilyLine((*it).parent,*sit)){
				ret->insert(ret->end(),(*it).plistchilds->begin(),(*it).plistchilds->end());
			}
		}
	}
	
	return ret;
}

void CTypes::FindAllLeafTypes(deque< string >* in, deque< string >* out)
{
  deque<string>::iterator it;
  for (it=in->begin();it!=in->end();it++){
     if (GetNode(*it)->listchilds.empty()) out->push_back(*it);
     else {
       deque<string> *tmp=new deque<string>();
       for (deque<TypesNode*>::iterator sit=GetNode(*it)->listchilds.begin();sit!=GetNode(*it)->listchilds.end();sit++){
	 tmp->push_back((*sit)->name);
       }
       FindAllLeafTypes(tmp,out);
     }
  }
  
}


int CTypes::Count(void)
{
	return types->size();
}

CTypes* CTypes::Clone()
{
       CTypes *ret = new CTypes();
       ret->types=new deque<TypesRecord>(types->begin(),types->end());
       return ret;
}

void CTypes::Merge(CTypes *t)
{
   deque<TypesRecord>::iterator it,it2;
   for (it=t->types->begin();it!=t->types->end();it++){
      for (it2=types->begin();it2!=types->end();it2++){
	 if (it->parent == it2->parent){
	   for (deque<string>::iterator its = it->plistchilds->begin();its!=it->plistchilds->end();its++){
	      if (find(it2->plistchilds->begin(),it2->plistchilds->end(),*its)==it2->plistchilds->end()) it2->plistchilds->push_back(*its);
	   }
	   break;
	 }
      }
      if (it2==types->end()) types->push_back(*it);
   }
}

