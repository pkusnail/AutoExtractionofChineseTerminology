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
struct tf_and_idf{
	double tf_idf;
		double tf;
	tf_and_idf(double a, double b):tf_idf(a),tf(b){};
tf_and_idf(){};
};
struct key_tf_and_idf
{
	string key;
	double tf_idf;
	double tf;
key_tf_and_idf(){};
	key_tf_and_idf(string k,double idf, double t):key(k),tf_idf(idf),tf(t){};
};
static bool getGreaterTfidf(key_tf_and_idf a,key_tf_and_idf b){
	return a.tf_idf>b.tf_idf;
};
static bool getGreaterTf(key_tf_and_idf a,key_tf_and_idf b){
	return a.tf>b.tf;
};
void writeTxt(string outFileName, string content){
	ofstream outfile;                                     //定义输出文件类对象
     	outfile.open(outFileName.c_str(),ios::app);           //以追加
	outfile<<content<<endl;
};

void writeTxt_key_tf_and_idf(string outFileName, vector<key_tf_and_idf>& output){
	ofstream outfile;                                     //定义输出文件类对象
     	outfile.open(outFileName.c_str(),ios::app);           //以追加
     for(int i =0;i<output.size();i++){
     	outfile<<output[i].key<<"  "<<output[i].tf_idf<<"  "<<output[i].tf<<endl;
     }
};



unordered_map<string,tf_and_idf> read_TFIDF_Rank_File(string TFIDF_Rank_File_FINAL){
	cout<<"Reading FINAL Rank file : "<< TFIDF_Rank_File_FINAL<<endl;
	ifstream infile;                                                 //定义文件输入流对象
	infile.open(TFIDF_Rank_File_FINAL.c_str(),ios::in);               //定义输入流对象，以输入的方式打开文件
	if(!infile){                                                            //判断文件是否正确打开
                   cout<<"Cannot open infile File!"<<endl;    //如果文件创建或打开不成功，打印输出出错信息
                                                             //退出程序
	}
	unordered_map<string,tf_and_idf> tfidf_info;
	string key="";
	double tf;
	double tfidf;
	while(!infile.eof()){
		infile>>key;  
		if(!infile.eof()){
			infile>>tfidf; 
			if(!infile.eof()){
				infile>>tf;
				tf_and_idf val;
				val.tf_idf=tfidf;
				val.tf=tf;
		cout<<"key: "<<key<<" idf: "<<tfidf<<" tf: "<<tf<<endl;
				tfidf_info[key]=val;
			}
		}
	}
	return tfidf_info;
};

void read_Realterm_File(vector<string> &RealTerms_Maybe,string Realterm_file){
	cout<<"Reading Realterm file : "<<Realterm_file<<endl;
	ifstream infile;                                                 //定义文件输入流对象
	infile.open(Realterm_file.c_str(),ios::in);               //定义输入流对象，以输入的方式打开文件
	if(!infile){                                                            //判断文件是否正确打开
                   cout<<"Cannot open Realterm_file File!"<<endl;    //如果文件创建或打开不成功，打印输出出错信息
                                                             //退出程序
	} 
	string key="";
	while(!infile.eof()){
		infile>>key;
		cout<<"key: "<<key<<endl;
		RealTerms_Maybe.push_back(key);
	} 
};
//./tfidf_query_rank.out  *_FINAL.txt realterm1.txt realterm2.txt realterm3.txt ...
int main(int argc, char**argv){
	
	unordered_map<string,tf_and_idf> TFIDF_Rank_Info=read_TFIDF_Rank_File(string(argv[1]));
	vector<string> RealTerms_Maybe;
	for(int i=2;i<argc;i++){
		read_Realterm_File(RealTerms_Maybe,argv[i]);
	}
	vector<key_tf_and_idf> result;
	for(int i =0;i<RealTerms_Maybe.size();i++){
		if(TFIDF_Rank_Info.find(RealTerms_Maybe[i])!=TFIDF_Rank_Info.end()){
			double tfidf_=TFIDF_Rank_Info[RealTerms_Maybe[i]].tf_idf;
			double tf_=TFIDF_Rank_Info[RealTerms_Maybe[i]].tf;
			key_tf_and_idf ans;
			ans.key=RealTerms_Maybe[i];
			ans.tf_idf=tfidf_;
			ans.tf=tf_;
			result.push_back(ans);
		}else{
			cout<<RealTerms_Maybe[i]<<"  ERROR : Can NOT find RealTerms_Maybe in TFIDF_INFO"<<endl;
		}
	}
	sort(result.begin(),result.end(),getGreaterTfidf);
	writeTxt_key_tf_and_idf(string(argv[2])+"_etc_Realterm_Maybe_Tfidf_Rank.txt",result);
	sort(result.begin(),result.end(),getGreaterTf);
	writeTxt_key_tf_and_idf(string(argv[2])+"_etc_Realterm_Maybe_Tf_Rank.txt",result);
	return 0;
}








