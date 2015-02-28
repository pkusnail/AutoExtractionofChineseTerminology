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
     	outfile<<output[i].key<<endl;
     }
};

void worker(unordered_map<string,int> &patterns,string posed_domain_corpa_txt){
	string domainName=posed_domain_corpa_txt;
	string vectorsFilePath=posed_domain_corpa_txt;
	ifstream vectorsFile;                                                 //定义文件输入流对象
	vectorsFile.open(vectorsFilePath.c_str(),ios::in);               //定义输入流对象，以输入的方式打开文件
	if(!vectorsFile){                                                            //判断文件是否正确打开
                   cout<<"Cannot open vectorsFile file!"<<endl;    //如果文件创建或打开不成功，打印输出出错信息
                                                             //退出程序
	}
	string line;
	long long  counter=0;
	string after_merge="";
	string candidates="";
	unordered_map<string,int> rightWords;
	unordered_map<string,int> badWords;
	while(!vectorsFile.eof()){
		counter++;
	     	vectorsFile >> line;
		cout<<line<<endl;
		vector<string> f1=split(line,"=");//=x=农业=n=生物学=n=  
		string pattern="";
		string content="";
		for(int i=2;i<f1.size();i+=2){
			pattern+=f1[i];
			content+=f1[i-1];
		}
		if(patterns.find(pattern)!=patterns.end()){
			rightWords[content]++;
		}else{
			badWords[content]++;
		}	
	}  
		writeTxt(domainName+"_Filtered_good.txt",rightWords);
		writeTxt(domainName+"_Filtered_bad.txt",badWords);
};
unordered_map<string,int> readPatternFile(string patternFilePath){
	ifstream patternFile;                                                 //定义文件输入流对象
	patternFile.open(patternFilePath.c_str(),ios::in);               //定义输入流对象，以输入的方式打开文件
	if(!patternFile){                                                            //判断文件是否正确打开
                   cout<<"Cannot open pattern File!"<<endl;    //如果文件创建或打开不成功，打印输出出错信息
                                                             //退出程序
	}
	unordered_map<string,int> patterns;
	string one="";
	while(!patternFile.eof()){
		patternFile>>one;
		patterns[one]++;
	}
	return patterns;
};
unordered_map<string,int> readConfigurationFile(string filepath){
	ifstream inFile;
	inFile.open(filepath.c_str(),ios::in);
	if(!inFile){ cout<<"Can not open"<<filepath<<endl;}
	unordered_map<string,int> content;
	string one="";
	while(!inFile.eof()){
		inFile>>one;
		content[one]++;
	}
	return content;
};
//
//最后对候选术语进行逐字检查的程序，
//输入是：Not_Start_with  Should_Not_Contain Not_End_With CandidatesFile 输出是最后的结果 ＊_RealTerms.txt
// ./f*v7.out Not_Start_With.txt  Should_Not_Contain.txt Not_End_With.txt Should_Not_Be.txt First_Priority.txt POS*candidates.txt_Filtered_good.txt

int main(int argc, char**argv){	//read Not_Start_with
	if(argc<7){
		cout<<" Parameter Error! "<<endl;
		for(int i =0;i<argc;i++) cout<<argv[i]<<endl;
	}
	unordered_map<string,int> NotStartWith=readConfigurationFile(string(argv[1]));
	//read Not_End_With
	unordered_map<string,int> ShouldNotContain=readConfigurationFile(string(argv[2]));
	
	//read Should_Not_Contain
	unordered_map<string,int> NotEndWith =readConfigurationFile(string(argv[3]));
	
	//read FirstPriority
	unordered_map<string,int> ShouldNotBe=readConfigurationFile(string(argv[4]));
	
	//read candidates
	unordered_map<string,int> FirstPriority=readConfigurationFile(string(argv[5]));
	
	//read candidates
	unordered_map<string,int> Candidates=readConfigurationFile(string(argv[6]));
	
	

	//check process
	unordered_map<string,int> RealTerms;
	for(unordered_map<string,int>::iterator it=Candidates.begin();it!=Candidates.end();it++){
		string candy=it->first;
		if(FirstPriority.find(candy)!=FirstPriority.end()){
			RealTerms[candy]++;
			continue;
		}

		if(ShouldNotBe.find(candy)!=ShouldNotBe.end()){
			cout<<candy<<" Error:ShouldNotBe "<<candy<<endl;
			continue;
		}

		int len=candy.size();
		bool valid=true;
		for(int length=1;length<=len;length++){//Not_Start_With
			if(FirstPriority.find(candy.substr(0,length))!=FirstPriority.end()) continue;

			if(NotStartWith.find(candy.substr(0,length))!=NotStartWith.end())  {
					cout<<candy<<" Error:NotStartWith "<<candy.substr(0,length)<<endl;
					valid=false;
				}
		}
		for(int length=1;valid&&length<=len;length++){//Not_End_With
			if(FirstPriority.find(candy.substr(len-length,length))!=FirstPriority.end()) continue;
			
			if(NotEndWith.find(candy.substr(len-length,length))!=NotEndWith.end())  {
					cout<<candy<<" Error:NotEndWith "<<candy.substr(len-length,length)<<endl;
					valid=false;
				}
		}
		for(int endIdx=len-2;valid&&endIdx>=0;endIdx--){// Should_Not_Contain
			for(int startIdx=1;startIdx<=endIdx;startIdx++){
				if(ShouldNotContain.find(candy.substr(startIdx,endIdx-startIdx+1))!=ShouldNotContain.end()) {
						cout<<candy<<" Error:ShouldNotContain "<<candy.substr(startIdx,endIdx-startIdx+1)<<endl;
						valid=false;
					}
				}
		}
		if(valid){
			RealTerms[candy]++;
		}
	}
	writeTxt(string(argv[6])+"_RealTerms.txt",RealTerms);
	return 0;
}








