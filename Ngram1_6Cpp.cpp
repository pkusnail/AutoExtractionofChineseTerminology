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

class  wordQueue{
private:
	deque<string> deq;
	int capacity;
	unordered_set<string> stopWords;
public:
	wordQueue(int cap=6):capacity(cap){};
	wordQueue(unordered_set<string> sw,int cap=6):capacity(cap),stopWords(sw){};
	void push(string lastWord){
		if(deq.size()==capacity){
			deq.pop_front();
		}
		//if(lastWord=="</"||lastWord==">"||major> <major>  < ><degree> degree>  </degree> </classification )  return ;
		if(stopWords.find(lastWord)==stopWords.end())
			deq.push_back(lastWord);

	};
	void output(unordered_map<string,long long>& rec){
		string str="";
		for(int i=0;i<(int)deq.size();i++){
			str+=deq[i];
			rec[str]++;
		}
	};
	void push(string word,unordered_map<string,long long>& rec){
		if(stopWords.find(word)==stopWords.end()){
			push(word);
			output(rec);
		}else{
			while(deq.size()){
				deq.pop_front();
				output(rec);
			}
		}
	}
};
 
struct node{
	string val;
	long long count;
	node(string v,long long c):val(v),count(c){};
};
bool sort_node(node a, node b){// from greater to smaller
	return a.count > b.count;
};
void writeTxt(string outFileName, int critical_val, unordered_map<string,long long>& A)	{
	cout<<"writeTxt :size of A-B"<<A.size()<<endl;
	 ofstream outfile;                                             //定义输出文件类对象
     outfile.open(outFileName.c_str(),ios::out);           //以输出
     vector<node> outcome;
	for( unordered_map<string,long long> ::iterator it=A.begin();it!=A.end();it++){
	//	string out=it->first+" "+to_string(it->second);
		if(it->second>critical_val)
			outcome.push_back(node(it->first,it->second));
		
	}
	sort(outcome.begin(),outcome.end(),sort_node);
	for( vector<node> ::iterator it=outcome.begin();it!=outcome.end();it++){
		outfile<<it->val<<" "<<it->count<<endl;
		cout<<it->val<<" "<<it->count<<endl;
	}
};
void writeTxt(string outFileName,unordered_map<string,long long>& A){
	writeTxt( outFileName, 5,  A);
};

int main(int argc, char *argv[]){

	string stopWordFile="/home/alejandroseaah/Desktop/Thesis_VIP_BASE/语料 corp/基础学科/合并结果/shopB/word2vec  41/stopWord.txt";    
    
	ifstream stopwordfile;                                                 //定义文件输入流对象
    stopwordfile.open(stopWordFile.c_str(),ios::in);               //定义输入流对象，以输入的方式打开文件
    if(!stopwordfile){                                                            //判断文件是否正确打开
                   cout<<"Cannot open stopwordfile file!"<<endl;    //如果文件创建或打开不成功，打印输出出错信息
                   return 0;                                              //退出程序
    }
	unordered_set<string> stopword_set;
	  string sw;
     while(!stopwordfile.eof()){
     	stopwordfile>>sw;
     	if(stopword_set.find(sw)==stopword_set.end())
     		stopword_set.insert(sw);
     }

    wordQueue worker(stopword_set,5);


	for(int i=0;i<argc-1;i++){
		
			cout<<"Procesing "<<argv[i+1]<<" completed!  "<<endl;

			string inFileName=argv[i+1];//"/home/alejandroseaah/Desktop/Thesis_VIP_BASE/语料 corp/基础学科/合并结果/shopB/word2vec  41/seg_domain_s/seg_domain_11_nobels.txt";
			
			ifstream infile;                                                 //定义文件输入流对象
		    infile.open(inFileName.c_str(),ios::in);               //定义输入流对象，以输入的方式打开文件
		    if(!infile){                                                            //判断文件是否正确打开
		                   cout<<"Cannot open file!"<<endl;    //如果文件创建或打开不成功，打印输出出错信息
		                   return 0;                                              //退出程序
		         }
		     

		 	unordered_map<string,long long> Ngram;
		    string oneWord;
		    while(!infile.eof()){//treat the whole doc as a sentence , bug! ......................................
				infile>>oneWord;	
				cout<<oneWord<<" ";
				worker.push(oneWord,Ngram);
				
				//in.push_back(oneWord);
		   }
		   	worker.push("<",Ngram);
			writeTxt("TF_"+inFileName,Ngram);
		}
	return 0;
}