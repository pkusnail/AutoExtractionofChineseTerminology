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
struct TF_IDF_INFO{
	long long all;
	long long domains;
	TF_IDF_INFO(long long a=1L,long long d=1L):all(a),domains(d){};
};
long long  readToMap(string filepath,unordered_map<string,long long> &source,unordered_map<string,TF_IDF_INFO >& total){
		long long num_of_words_in_source=0;
		ifstream infile;                                                 //定义文件输入流对象
         infile.open(filepath.c_str(),ios::in);               //定义输入流对象，以输入的方式打开文件
         if(!infile){                                                            //判断文件是否正确打开
                   cout<<"Cannot open file!"<<endl;    //如果文件创建或打开不成功，打印输出出错信息
                   return num_of_words_in_source;                                              //退出程序
         }
         string keyword;
         int times;
        while(!infile.eof()){//treat the whole doc as a sentence , bug! ......................................
			infile>>keyword;
			if(!infile.eof()){
				infile>>times;
				num_of_words_in_source+=times;
			}else{
				break;
			}
			if(keyword.size()==0)	continue;
			cout<<"keyword: "<<keyword<<" times: "<<times<<" 		";
			source[keyword]=(long long)times; 
			cout<<filepath<<endl;

			if(total.find(keyword)==total.end()){
				total[keyword]=TF_IDF_INFO(times,1);
			}else{
				total[keyword].all+=times;// string total  docs
				total[keyword].domains++;
			}

   		}
   		return num_of_words_in_source;
};

static bool sort_node(tfidf_node a, tfidf_node b){// from greater to smaller
	return a.tfidf > b.tfidf;
};

vector<tfidf_node> get_tf_idf(unordered_map<string,TF_IDF_INFO >& total,unordered_map<string,long long>&source,long long total_size, long long source_size){
	vector< tfidf_node>  tfidf;
	double weight_equal_size_factor=total_size/(source_size*1.0);////????????
	for(unordered_map<string,long long>::iterator it=source.begin();it!=source.end();it++){
		tfidf.push_back(tfidf_node(it->first,weight_equal_size_factor*(double)1.0*it->second/(1.0*(total[it->first].all*(total[it->first].domains+3)))));
	}
	sort(tfidf.begin(),tfidf.end(),sort_node);
	return tfidf;
};
void write2Txt(string outFileName, vector<tfidf_node> & A)	{
	cout<<endl<<"write2Txt :size of    "<<A.size()<<endl;
	 ofstream outfile;                                             //定义输出文件类对象
     outfile.open(outFileName.c_str(),ios::out);           //以输出
	for(vector<tfidf_node>::iterator it=A.begin();it!=A.end();it++){
		if(it->val.empty()) continue;
		outfile<<it->val<<" "<<it->tfidf<<endl;
		cout<<it->val<<" "<<it->tfidf<<endl;
	}
};
//cmd  TF_file1 TF_file2 TF_file3 TF_file4
 int main(int argc, char *argv[]){ 
	unordered_map<string,TF_IDF_INFO > total;
	vector< unordered_map<string,long long> >  sources(argc-1);
	vector<long long> num_of_words_in_sources(argc-1,0);
	long long num_of_all_words=0;

	for(int i=0;i<argc-1;i++){
		num_of_words_in_sources[i]=readToMap(argv[i+1],sources[i],total);
		num_of_all_words+=num_of_words_in_sources[i];
		cout<<argv[i+1]<<"readToMap completed!  "<<endl;
		cout<<"Statistics: num_of_all_words "<<num_of_all_words<<" num_of_words_in_sources["<< i<< "] "<<num_of_words_in_sources[i]<<endl;
	}
	for(int i=0;i<argc-1;i++){
		vector<tfidf_node> tfidf_values=get_tf_idf(total,sources[i],num_of_all_words,num_of_words_in_sources[i]);
 		write2Txt(string(argv[i+1])+"_tf_idf.txt",tfidf_values);
	}
	return 0;
}
