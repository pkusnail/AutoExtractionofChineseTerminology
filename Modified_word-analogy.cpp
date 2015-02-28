//  Copyright 2013 Google Inc. All Rights Reserved.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>

#include<iostream>
#include<vector>
#include<unordered_map>
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

int Iteration_times=1000;
int overall_counter=-1;
bool isFirstTime=true;
struct k2v{
	string key;
	float val;
	k2v(){};
	k2v(string k,float v ):key(k),val(v){};
};
static bool DescendingRanking(k2v a, k2v b){
	return a.val>b.val;
};
	string seedName="";
	vector<string> should_be_queried;
	unordered_map<string ,float> Terms2BeQueried;//所有迭代的最终答案
        unordered_map<string ,float> TermsAlreadyQueried;//已经作过查询的所有的第三个词
	//vector<k2v> FinalRanking;//所有迭代的最终答案最后的得分排序
bool check_contain(string& word,char c){
	for(int i=0;i<word.size();i++){
		if(word[i]==c)
			return true;
	}
	return false;
};
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


void writeTxt(string outFileName, unordered_map<string,float>& AllAns){
	std::vector<k2v> output;
	for(unordered_map<string,float>::iterator it=AllAns.begin();it!=AllAns.end();it++){
		output.push_back(k2v(it->first,it->second));
	}
	sort(output.begin(),output.end(),DescendingRanking);
	ofstream outfile;                                     //定义输出文件类对象
     	outfile.open(outFileName.c_str(),ios::app);           //以追加
     for(int i =0;i<output.size();i++){
     	outfile<<output[i].key<<" "<<output[i].val<<endl;
     }
};
struct tf_and_idf
{
	double tf_idf;
	double tf;
	tf_and_idf(){};
	tf_and_idf(double idf, double t):tf_idf(idf),tf(t){};
};
void read_Realterm_File(unordered_map<string,tf_and_idf> &RealTermInfo,string Realterm_file){
	cout<<"Reading Realterm file : "<<Realterm_file<<endl;
	ifstream infile;                                                 //定义文件输入流对象
	infile.open(Realterm_file.c_str(),ios::in);               //定义输入流对象，以输入的方式打开文件
	if(!infile){                                                            //判断文件是否正确打开
                   cout<<"Cannot open Realterm_file File!"<<endl;    //如果文件创建或打开不成功，打印输出出错信息
                                                             //退出程序
	} 
	string key="";
	double tfidf;
	double tf;
	while(!infile.eof()){
		infile>>key;
		if(!infile.eof())infile>>tfidf;
		if(!infile.eof())infile>>tf;
		if(tfidf<0.5 || tf < 20 ) continue; // 过滤
		RealTermInfo[key]=tf_and_idf(tfidf,tf);
	} 
};



const long long max_size = 2000;         // max length of strings
const long long N = 100;                  // number of closest words that will be shown
const long long max_w = 50;              // max length of vocabulary entries

int main(int argc, char **argv) {// cmd *.bin *dict iteration_times
	if(argc<3) {cout<<" Parameter Error! argc ： "<<argc<<endl; return 0;}
	if(argc>3){Iteration_times=stoi(argv[3]);}
	unordered_map<string,tf_and_idf> RealTermInfo;
	read_Realterm_File(RealTermInfo,string(argv[2]));

  FILE *f;
  char st1[max_size];
  char bestw[N][max_size];
  char file_name[max_size], st[100][max_size];
  float dist, len, bestd[N], vec[max_size];
  long long words, size, a, b, c, d, cn, bi[100];
  char ch;
  float *M;
  char *vocab;
  if (argc < 2) {
    printf("Usage: ./word-analogy <FILE>\nwhere FILE contains word projections in the BINARY FORMAT\n");
    return 0;
  }
  strcpy(file_name, argv[1]);
  f = fopen(file_name, "rb");
  if (f == NULL) {
    printf("Input file not found\n");
    return -1;
  }
  fscanf(f, "%lld", &words);
  fscanf(f, "%lld", &size);
  vocab = (char *)malloc((long long)words * max_w * sizeof(char));
  M = (float *)malloc((long long)words * (long long)size * sizeof(float));
  if (M == NULL) {
    printf("Cannot allocate memory: %lld MB    %lld  %lld\n", (long long)words * size * sizeof(float) / 1048576, words, size);
    return -1;
  }
  for (b = 0; b < words; b++) {
    a = 0;
    while (1) {
      vocab[b * max_w + a] = fgetc(f);
      if (feof(f) || (vocab[b * max_w + a] == ' ')) break;
      if ((a < max_w) && (vocab[b * max_w + a] != '\n')) a++;
    }
    vocab[b * max_w + a] = 0;
    for (a = 0; a < size; a++) fread(&M[a + b * size], sizeof(float), 1, f);
    len = 0;
    for (a = 0; a < size; a++) len += M[a + b * size] * M[a + b * size];
    len = sqrt(len);
    for (a = 0; a < size; a++) M[a + b * size] /= len;
  }
  fclose(f);

//作关键字输入前的准备应该从这有个大loop
  while (1) {
    for (a = 0; a < N; a++) bestd[a] = 0;
    for (a = 0; a < N; a++) bestw[a][0] = 0;
    printf("Enter three words (EXIT to break): ");
    a = 0;
    while (1) {
      st1[a] = fgetc(stdin);//st1[a]是要查询的那些个关键词的输入
      if ((st1[a] == '\n') || (a >= max_size - 1)) {
        st1[a] = 0;
        break;
      }
      a++;
    }
    if (!strcmp(st1, "EXIT")) break;
    cn = 0;//cn是输入的关键字的数量
    b = 0;
    c = 0;
    while (1) {
      st[cn][b] = st1[c];//st 是对st1的整理
      b++;
      c++;
      st[cn][b] = 0;
      if (st1[c] == 0) break;
      if (st1[c] == ' ') {
        cn++;
        b = 0;
        c++;
      }
    }
    cn++;
    if (cn < 3) {//cn是输入的关键字的数量
      printf("Only %lld words were entered.. three words are needed at the input to perform the calculation\n", cn);
      continue;
    }

//从这个位置开始计算答案，
int Idx_should_be_queried=0;
while(true){//全部迭代开始
overall_counter++;//总迭代计数器
	    if(!isFirstTime){
		string toBeQueried="";
		/*unordered_map<string,float>::iterator it=Terms2BeQueried.begin();
		for(;toBeQueried.size()==0;it++){
		   if(TermsAlreadyQueried.find(it->first)==TermsAlreadyQueried.end()){
			toBeQueried=it->first;
			TermsAlreadyQueried[it->first]=it->second;
		   }
		*/
		for(;Idx_should_be_queried<should_be_queried.size();Idx_should_be_queried++){
			if(TermsAlreadyQueried.find(should_be_queried[Idx_should_be_queried])==TermsAlreadyQueried.end()){
				toBeQueried=should_be_queried[Idx_should_be_queried];
				TermsAlreadyQueried[toBeQueried]=0;
				break;
			}
		}
		if(overall_counter>Iteration_times){
			cout<<"Done! Writing the file : "<<seedName+"_seed_result.txt"<<endl;
			writeTxt(seedName+"_seed_result.txt",Terms2BeQueried);
			break;// / 唯一 结束整个大while的地方
		}
		for(int idx=0;idx<max_size;idx++)st[2][idx]=0;//清零		
		for(int i=0;i<toBeQueried.size();i++)   st[2][i] = toBeQueried[i];//st 是对st1的整理
	    }
if(isFirstTime){
seedName=string(st[2]);
isFirstTime=false;//下次开始自动循环
}
	    for (a = 0; a < cn; a++) {
	      for (b = 0; b < words; b++) if (!strcmp(&vocab[b * max_w], st[a])) break;
	      if (b == words) b = 0;
	      bi[a] = b;
	      printf("\nWord: %s  Position in vocabulary: %lld\n", st[a], bi[a]);
		      if (b == 0) {
				printf("Out of dictionary word!\n");
				break;
		      }
	    }
	    if (b == 0) continue;
	   
	    for (a = 0; a < size; a++) vec[a] = M[a + bi[1] * size] - M[a + bi[0] * size] + M[a + bi[2] * size];
	    len = 0;
	    for (a = 0; a < size; a++) len += vec[a] * vec[a];
	    len = sqrt(len);
	    for (a = 0; a < size; a++) vec[a] /= len;
	    for (a = 0; a < N; a++) bestd[a] = 0;
	    for (a = 0; a < N; a++) bestw[a][0] = 0;
	    for (c = 0; c < words; c++) {
		      if (c == bi[0]) continue;
		      if (c == bi[1]) continue;
		      if (c == bi[2]) continue;
		      a = 0;
		      for (b = 0; b < cn; b++) if (bi[b] == c) a = 1;
		      if (a == 1) continue;
		      dist = 0;
		      for (a = 0; a < size; a++) dist += vec[a] * M[a + c * size];
		      for (a = 0; a < N; a++) {
				if (dist > bestd[a]) {
					  for (d = N - 1; d > a; d--) {
						    bestd[d] = bestd[d - 1];
						    strcpy(bestw[d], bestw[d - 1]);
					  }
					  bestd[a] = dist;
					  strcpy(bestw[a], &vocab[c * max_w]);
					  break;
				}
		      }
	    }
	//从这个地方开始改
	// printf("\n                                              Word              Distance\n------------------------------------------------------------------------\n");
	
	    for (a = 0; a < N; a++){
		// if(bestd[a]<0.5)  printf("%50s\t\t%f\tRemoved!\n", bestw[a], bestd[a]);//显示结果
		// else{ 
			//printf("%50s\t\t%f\n", bestw[a], bestd[a]);//显示结果
			string word=string(bestw[a]);
			word=trim(word);
			if(RealTermInfo.find(word)==RealTermInfo.end()) continue;
			if(bestd[a]>0.5) {//相似度大于0.5才会考虑，否则不加入这个集合内
				cout<<a;
				Terms2BeQueried[bestw[a]]+=bestd[a];//保留结果，小心，这已经变成真正的结果了
				should_be_queried.push_back(bestw[a]);//放到等询问的集合里面
			}
		//}
	    }
	}
 }//big while 结束
  return 0;
}
