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
struct tfidf_node{
	string val;
	double tfidf;
	tfidf_node(string v,double c):val(v),tfidf(c){};
};
void MergeMap(unordered_map<string,pair<long long,long long> >&target,unordered_map<string,long long> &source){
	for(unordered_map<string,long long> ::iterator it=source.begin();it!=source.end();it++){
		if(target.find(it->first)==target.end()){
			target[it->first]=pair<long long,long long>(it->second,1);
		}else{
			target[it->first].first+=it->second;// string total  docs
			target[it->first].second++;
		}
	}
};
void readToMap(string filepath,unordered_map<string,long long> &source){
		ifstream infile;                                                 //定义文件输入流对象
         infile.open(filepath.c_str(),ios::in);               //定义输入流对象，以输入的方式打开文件
         if(!infile){                                                            //判断文件是否正确打开
                   cout<<"Cannot open file!"<<endl;    //如果文件创建或打开不成功，打印输出出错信息
                   return ;                                              //退出程序
         }
         string keyword;
         int times;
        while(!infile.eof()){//treat the whole doc as a sentence , bug! ......................................
			infile>>keyword;
			if(!infile.eof()){
				infile>>times;
			}else{
				break;
			}
			if(keyword.size()==0)	continue;
			source[keyword]=(long long)times;
			cout<<"keyword: "<<keyword<<" times: "<<times<<endl;
   		}
};

bool sort_node(tfidf_node a, tfidf_node b){// from greater to smaller
	return a.tfidf > b.tfidf;
};
vector<tfidf_node> get_tf_idf(unordered_map<string,pair<long long,long long> >& total,unordered_map<string,long long>&source,long long total_size, long long source_size){
	vector< tfidf_node>  tfidf;
	for(unordered_map<string,long long>::iterator it=source.begin();it!=source.end();it++){
		tfidf.push_back(tfidf_node(it->first,1.0*it->second/(1.0*(total[it->first].second*total[it->first].first))));
	}
	sort(tfidf.begin(),tfidf.end(),sort_node);
	return tfidf;
};
void writeTxt(string outFileName, vector<tfidf_node> & A)	{
	cout<<endl<<"writeTxt :size of A-B"<<A.size()<<endl;
	 ofstream outfile;                                             //定义输出文件类对象
     outfile.open(outFileName.c_str(),ios::out);           //以输出
	for(vector<tfidf_node>::iterator it=A.begin();it!=A.end();it++){
		outfile<<it->val<<" "<<it->tfidf<<endl;
		cout<<it->val<<" "<<it->tfidf<<endl;
	}
};
int main(int argc, char *argv[]){
//读取二个共现矩阵文件A和B ，并做差，产生A－B 和B－A二个结果文档
	unordered_map<string,pair<long long,long long> > total;
	vector< unordered_map<string,long long> >  sources(argc-1);
	vector< unordered_map<string,double> >  results(argc-1);

	for(int i=1;i<=argc;i++){
		readToMap(argv[i],sources[i-1]);
		MergeMap(total,sources[i-1]);
	}
	for(int i=1;i<=argc;i++){
		vector<tfidf_node> tfidf_values=get_tf_idf(total,sources[i-1],(long)total.size(),(long)sources[i-1].size());
 		writeTxt(string(argv[i-1])+"_tf_idf.txt",tfidf_values);
	}
	return 0;
}