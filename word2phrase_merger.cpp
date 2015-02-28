#include<iostream>
#include<string>
#include<unordered_map>
#include<unordered_set>
#include<algorithm>
#include<vector>
#include <iomanip>
#include <string.h>
#include <sstream>
#include <fstream>
#include <queue>
#include <stack>
#include <deque>
using namespace std; 
string trim(string s){
    if (s.empty()){  
        return s;  
    }  
    s.erase(0,s.find_first_not_of(" "));  
    s.erase(s.find_last_not_of(" ") + 1);  
    return s;  
};
vector<string> split(string& s,string delim) { // 
	vector<string> ret;
    int idx=0;
    while(idx<s.size()){
    	while(idx+delim.size()<s.size()&&s.substr(idx,(int)delim.size())==delim){
    		idx++;
    	}
    	if(idx<s.size()&&s.substr(idx,(int)delim.size())!=delim){
    		    int start=idx;
    		    while(idx<s.size()&&s.substr(idx,(int)delim.size())!=delim){
		    		idx++;
		    	}
		    	ret.push_back(s.substr(start,idx-start));
    	}
    	idx++;
    }
    return ret;
};
vector<string> split(string s){
	 return split(s," "); 
};

void writeTxt(string outFileName, string content){
	ofstream outfile;                                     //定义输出文件类对象
     	outfile.open(outFileName.c_str(),ios::app);           //以追加
	outfile<<content<<endl;
};
struct  k2v
{
	string key;
	int val;
	k2v(string k,int v):key(k),val(v){};
	/* data */
};
static bool getGreater(k2v a, k2v b){
	return a.val > b.val;
};
void writeTxt(string outFileName, unordered_map<string,int>&TF){
	std::vector<k2v> output;
	for(unordered_map<string,int>::iterator it=TF.begin();it!=TF.end();it++){
		output.push_back(k2v(it->first,it->second));
	}
	sort(output.begin(),output.end(),getGreater);
	ofstream outfile;                                     //定义输出文件类对象
     	outfile.open(outFileName.c_str(),ios::app);           //以追加
     for(int i =0;i<output.size();i++){
     	outfile<<output[i].key<<" "<<output[i].val<<endl;
     }
};


int main(int argc, char**argv){
	string domainName=string(argv[1]);
	string vectorsFilePath=argv[1];//"/home/alejandroseaah/Desktop/Thesis_VIP_BASE/VIP_tools/word2vec_41/数学vectors_iter1.txt";    
	ifstream vectorsFile;                                                 //定义文件输入流对象
	vectorsFile.open(vectorsFilePath.c_str(),ios::in);               //定义输入流对象，以输入的方式打开文件
	if(!vectorsFile){                                                            //判断文件是否正确打开
                   cout<<"Cannot open vectorsFile file!"<<endl;    //如果文件创建或打开不成功，打印输出出错信息
                   return 0;                                              //退出程序
	}
	string oneWord;
	long long  counter=0;
	string after_merge="";
	string candidates="";
	unordered_map<string,int> AllWordsTF;
	while(!vectorsFile.eof()){
		counter++;
	     	vectorsFile >> oneWord;
		cout<<oneWord<<endl;
		vector<string>sp= split(oneWord,"_");
		if(sp.size()>1){
			candidates+=sp[0]+sp[1]+'\n';
			after_merge+=sp[0]+sp[1]+" ";
			AllWordsTF[sp[0]+sp[1]]++;
		}else{
			after_merge+=oneWord+" ";
			AllWordsTF[oneWord]++;
		}
		if(counter%1000==0){
			//writeTxt("数学vectors_iter1_candidates.txt",candidates);
			//writeTxt("数学vectors_iter1_Merger.txt",after_merge);
			writeTxt(domainName+"_candidates.txt",candidates);
			writeTxt(domainName+"_Merger.txt",after_merge);			
			after_merge="";
			candidates="";
		}
	}  
			writeTxt(domainName+"_candidates.txt",candidates);
			writeTxt(domainName+"_Merger.txt",after_merge);
			writeTxt(domainName+"_TF.txt",AllWordsTF);
			after_merge="";
			candidates="";
	return 0;
}








