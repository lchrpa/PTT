#include "common.h"

string ToUpper(string strToConvert)
{//change each element of the string to upper case
   for(unsigned int i=0;i<strToConvert.length();i++)
   {
	   if(strToConvert[i]=='-'){
		  strToConvert[i]='_';
	   } else {
		  strToConvert[i] = toupper(strToConvert[i]);
	   }
   }
   return strToConvert;//return the converted string
};

string ToUpperFirstChar(string strToConvert)
{//change first element of the string to upper case
	if(strToConvert.length()>0){
		strToConvert[0] = toupper(strToConvert[0]);
	}
   return strToConvert;//return the converted string
};

string ToLower(string strToConvert)
{//change each element of the string to lower case
   for(unsigned int i=0;i<strToConvert.length();i++)
   {
       if(strToConvert[i]=='-'){
		  strToConvert[i]='_';
	   } else {
		  strToConvert[i] = tolower(strToConvert[i]);
	   }
   }
   return strToConvert;//return the converted string
};

string LTrim(string s)
{//remove spaces from the beginning
	while(!s.empty()&&(s[0]==' '||s[0]=='\t')){
		s.erase(0,1);
	}
	if (s[s.size()-1]=='\r') s.erase(s.size()-1,1);
	return s;

}

deque<string>* ExplodeBySpace(string s, const char* c=" \t")
{ //split the input string into elementary arguments
	deque<string> *r=new deque<string>();
	string p;
	int found;

	found=s.find_first_of(c);
	while (found!= -1){
		p=s.substr(0,found);
		s.erase(0,found+1);
		r->push_back(p);
		found=s.find_first_of(c);
	}
	r->push_back(s);

	return r;
}

bool CompareProbNames(CProblem *p1, CProblem *p2){
	
	return (p1->GetName()<p2->GetName());

}

bool sh_arg_str_eq(sh_arg_str s1, sh_arg_str s2){
	return s1.first == s2.first && s1.second == s2.second;
}

bool sh_arg_str_cmp(sh_arg_str s1, sh_arg_str s2){
	if (s1.first < s2.first) return true;
	if (s1.first > s2.first) return false;
	return s1.second < s2.second;
}

void revert_ah_arg_str(vector<sh_arg_str> &sh_args){
	vector<sh_arg_str>::iterator it = sh_args.begin();
	int tmp;
	
	while(it!=sh_args.end()){
		tmp =(*it).first;
		(*it).first=(*it).second;
		(*it).second=tmp;
		it++;
	}

}

bool compatible_sh_arg_str(vector<sh_arg_str> &sh_args){
	
	vector<sh_arg_str> copy = sh_args;
	
	revert_ah_arg_str(copy);

	sort(copy.begin(), copy.end(), sh_arg_str_cmp);
	
	vector<sh_arg_str>::iterator it = copy.begin();
	int prev=-1;
	
	while(it!=copy.end()){
		if ((*it).first==prev) return false;
		prev=(*it).first;
		it++;
	}
	return true;
}
