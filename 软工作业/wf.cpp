#include <io.h>
#include <string>
#include <bits/stdc++.h>
using namespace std;

int letterNumber[26];
int total = 0;
int len = 0;
char buffer[1024+1];

struct alphabeticStruct //定义	结构体 
{
	char name;
	float letterNumber;
	alphabeticStruct(char n = 'a', float lN = 0){
		name = n;
		letterNumber = lN;
	}
};
typedef pair<string, int> PAIR;

alphabeticStruct letterStruct[26];


bool isUpperLetter(char x){
	return 'A' <= x && x <= 'Z';
}
bool isLowerLetter(char x){
	return 'a' <= x && x <= 'z';
}
bool isLetter(char x){
	return isUpperLetter(x) || isLowerLetter(x);
}
int getLetterId(char x){
	if(isUpperLetter(x))
		return x - 'A';
	else
		return x - 'a';
}

void judgeLetter(char str[]){
	for(int i = 0; str[i]; i++)
		if(isLetter(str[i]) ){
			total++;
			letterNumber[getLetterId(str[i])]++;
		}
}
bool cmpLetter(alphabeticStruct a, alphabeticStruct b){
	return (a.letterNumber > b.letterNumber) || (a.letterNumber == b.letterNumber && a.name < b.name);
}



void alphabeticStatistics(char *fileName) //第0步 统计频率 
{
	memset(letterNumber, 0, sizeof(letterNumber) * 26);
	total = 0;
	ifstream inFile;
	inFile.open(fileName);
	ofstream outFile;
	if( !inFile.is_open())
	{
		cout << ":文件打开失败" << endl;
		exit(1);
	}
	while( !inFile.eof())
	{
		inFile.getline(buffer, 1024);
		len = strlen(buffer);
		judgeLetter(buffer);
	}
	inFile.close();
	for(int i = 0; i < 26; i++)
	{ 
		letterStruct[i].name = i +'a';
		letterStruct[i].letterNumber = 1.0 * letterNumber[i] / total * 100;
	}
	sort(letterStruct, letterStruct+26, cmpLetter);
	for(int i = 0; i < 26; i++)
		printf("%2d %c: %.2f\n", i+1, letterStruct[i].name, letterStruct[i].letterNumber);
}

 void wordStatistics(char *fileName) //功能1：统计不重复单词出现的次数
{
	ifstream inFile;
	inFile.open(fileName);
	
	if( !inFile.is_open()){
		cout << fileName << ":文件打开失败" << endl;
		exit(1);
	}
	
	string str;
	map<string,int>wordcount;

	while( !inFile.eof()){
		inFile >> str;
		wordcount[str]++;
	}
	priority_queue<pair<int,string>, vector<pair<int,string> >, less<pair<int,string> > > queueK; //建立队列 
	
	for (map<string,int>::iterator iter=wordcount.begin(); iter!=wordcount.end(); iter++)
		queueK.push(make_pair(iter->second,iter->first));
		
	pair<int,string>tmp;
	
	while (!queueK.empty()){	 
		tmp=queueK.top();
		queueK.pop();
		cout<<tmp.second<<"\t"<<tmp.first<<endl;
	}
}

struct CmpByValue {    
  bool operator()(const PAIR& lhs, const PAIR& rhs) {    
    return lhs.second > rhs.second;    
  }    
};

void wordGroups(char *fileName, int n){
	ifstream inFile;
	inFile.open(fileName);
	if( !inFile.is_open()){
		cout << fileName << ":文件打开失败" << endl;
		exit(1);
	}
	string str;
	string wordgroup;
	map<string,int>strmap;
	int i = 0, j = 0;
	while( !inFile.eof()){
		while(i!=n){
			inFile >> str;
			if((str[0] >= 'a' && str[0] <= 'z') || str[0] >= 'A' && str[0] <= 'Z'){
				if(wordgroup!=""){
					wordgroup += " ";
					wordgroup += str;
				}else
					wordgroup += str;
			}	
			i++;
		}
		i = 0;
		strmap[wordgroup]++;	
		wordgroup = "";
	}
	vector<PAIR> strvec(strmap.begin(), strmap.end());
	sort(strvec.begin(), strvec.end(), CmpByValue());
	for (int i = 0; i != strvec.size(); ++i)   //可在此对按value排完序之后进行操作  
        cout << strvec[i].first << " "<< strvec[i].second << endl;  
}


vector<string> getFileList(string dir)
{
	vector<string> allPath;
	//在目录后边加上"\\*.*"进行第一次搜索
	string dir2 = dir + "\\*.*";

	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(dir2.c_str(), &findData);
	if (handle == -1)//检查是否成功
	{
		cout << "Can't find file in this Path!" << endl;
		return allPath;
	}
	do
	{
		if (findData.attrib & _A_SUBDIR)//是否含有子目录
		{
			//若该目录是"."或".."，则进行下一次循环，否则输出子目录名，并进入下一次搜索
			if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
				continue;
			//在目录后边添加"\\"和搜索到的目录名进行下一次搜索
//			string dirNew = dir + "\\" + findData.name;
//			vector<string> tempPath = getFileList(dirNew);
//			allPath.insert(allPath.end(), tempPath.begin(), tempPath.end());
		}
		else//不是子目录，即是文件
		{
			string filePath = dir + "\\" + findData.name;
			allPath.push_back(filePath);
		}
	}
	while (_findnext(handle, &findData) == 0);
	_findclose(handle);//关闭搜索句柄

//	for(size_t i = 0; i < allPath.size(); i++)
//	{
//		string perPath = allPath.at(i);
//		cout << perPath << endl;
////		wordStatistics((char *)perPath.c_str());
//	}
	return allPath;
}


//遍历指定文件目录，但是会遍历目录下的所有子目录，对目录下每个文件执行wf.exe -f <file>
vector<string> getFileListInMenu(string dir)
{
	vector<string> allPath;
	//在目录后边加上"\\*.*"进行第一次搜索
	string dir2 = dir + "\\*.*";

	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(dir2.c_str(), &findData);
	if (handle == -1)//检查是否成功
	{
		cout << "Can't find file in this Path!" << endl;
		return allPath;
	}
	do
	{
		if (findData.attrib & _A_SUBDIR)//是否含有子目录
		{
			//若该目录是"."或".."，则进行下一次循环，否则输出子目录名，并进入下一次搜索
			if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
				continue;
			//在目录后边添加"\\"和搜索到的目录名进行下一次搜索
			string dirNew = dir + "\\" + findData.name;
			vector<string> tempPath = getFileList(dirNew);
			allPath.insert(allPath.end(), tempPath.begin(), tempPath.end());
		}
		else//不是子目录，即是文件
		{
			string filePath = dir + "\\" + findData.name;
			allPath.push_back(filePath);
		}
	}
	while (_findnext(handle, &findData) == 0);
	_findclose(handle);//关闭搜索
	return allPath;
}

void wordStatisticsTopK(char *fileName, const int &k)
{
	ifstream inFile;
	inFile.open(fileName);

	if( !inFile.is_open())
	{
		cout << fileName << ":文件打开失败" << endl;
		exit(1);
	}
	
	string str;
	map<string,int>wordcount;

	while( !inFile.eof())
	{
		inFile >> str;
		wordcount[str]++;
	}
	cout << fileName << "\t单词种类："<< wordcount.size() << endl;
	//优先队列使用小顶堆，排在前面的数量少，使用">";
	priority_queue<pair<int,string>, vector<pair<int,string> >, greater<pair<int,string> > > queueK;
	for (map<string,int>::iterator iter=wordcount.end(); iter!=wordcount.begin(); iter++){
		queueK.push(make_pair(iter->second,iter->first));
		if(queueK.size() > k)
			queueK.pop();
	}
	pair<int,string>tmp;
	while (!queueK.empty())
	{
		tmp=queueK.top();
		queueK.pop();
		cout<<tmp.second<<"\t"<<tmp.first<<endl;
	}
}


void erasePunct(string &s)
{
	string::iterator it = s.begin();
	while(it != s.end())
	{
		if(ispunct(*it))
			it = s.erase(it);
		else
			++it;
	}
}
void stringToLower(string &s)
{
	for(string::iterator it = s.begin();
	        it != s.end();
	        ++it)
	{
		if(isupper(*it))
			*it = tolower(*it);
	}
}

bool isAllDigit(const std::string &s)
{
	for(string::const_iterator it = s.begin();
	        it != s.end();
	        ++it)
	{
		if(!isdigit(*it))
			return false;
	}
	return true;
}


class WordFrequency
{
	public:
		WordFrequency(const std::string &filename, const std::string &stopFile);//初始化
		void ReadStopFile();
		void ReadWordFile();
		static int vocabularyLenth;
		void sortWordByFrequency();
		void printWordFrequency()const;
	private:
		void addWordToDict(const std::string &word);
		bool isStopWord(const std::string &word)const;

		typedef std::vector<std::pair<std::string, int> >::iterator Wordit;//为了方便
		typedef std::vector<std::pair<std::string, int> >::const_iterator Wordkit;

		std::string filename_;
		std::string stopFile_;

		std::vector<std::string> stopList_;
		std::vector<std::pair<std::string, int> > words_;
};

int WordFrequency::vocabularyLenth = 1;

WordFrequency::WordFrequency(const std::string &filename, const std::string &stopFile)
	:filename_(filename),stopFile_(stopFile)
{ }

void WordFrequency::ReadStopFile()
{
	ifstream in(stopFile_.c_str());
	if( !in )
		throw runtime_error("文件打开失败");
	string word;
	while(in >> word)
	{
		stopList_.push_back(word);
	}
	in.close();
}

void WordFrequency::ReadWordFile()
{
	ifstream infile(filename_.c_str());
	if( !infile )
		throw runtime_error("文件打开失败");
	string word;
	while(infile >> word)
	{
		erasePunct(word);
		if( isAllDigit(word))
			continue;
		stringToLower(word);
		if( !isStopWord(word))
			addWordToDict(word);
	}
	infile.close();
}

bool WordFrequency::isStopWord(const string &word)const
{
	vector<string>::const_iterator it = stopList_.begin();
	while( it  != stopList_.end())
	{
		if( *it == word)
			break;
		it ++;
	}
	return (it != stopList_.end());
}

void WordFrequency::addWordToDict(const string &word)
{
	Wordit it = words_.begin();
	while( it != words_.end())
	{
		if( it->first == word)
		{
			++ it->second ;
			break;
		}
		it ++;
	}
	if(it == words_.end())
		words_.push_back(make_pair(word, 1)) ;
}

bool cmp(const pair<string, int> &a, const pair<string, int>&b)
{
	return a.second > b.second;
}

void WordFrequency::sortWordByFrequency(){
	sort(words_.begin(), words_.end(), cmp);
}

void WordFrequency::printWordFrequency()const{
	Wordkit it = words_.begin();
	while(it != words_.end())
	{
		printf("words: %s, frequency: %d\n",it->first.c_str(),it->second);
		it ++;
	}
}
	
void dynamicPrototype(char *fileName1,char *fileName2){
	ifstream inFile1;
	inFile1.open(fileName1);
	if( !inFile1.is_open()){
		cout << fileName1 << ":文件打开失败" << endl;
		exit(1);
	}
	string str;
	string s;
	int i = 0; 
	string v;
	map<string,string>strmap;
	while(getline(inFile1,str)){
		i = 0;
		v = "";
		istringstream is(str);
		while(is >> s){    //istringstream 分割字符串
			if(!i)
				v = s;
			else
				strmap[s] = v;
			i++;
		} 				
	}
	ifstream inFile2;
	inFile2.open(fileName2);
	if( !inFile2.is_open()){
		cout << fileName2 << ":文件打开失败" << endl;
		exit(1);
	}
	while( !inFile2.eof()){
		inFile2 >> str;
		map<string, string>::reverse_iterator iter;
    	for(iter = strmap.rbegin(); iter != strmap.rend(); iter++)
    		if(str == iter->first)
    			str=iter->second;
    	cout<<str<<" ";
	}
}
	
	
	

int main(int argc, char *argv[])
{
	for(int i = 0; i < argc; i++)
		cout<<"argv["<<i<<"]"<<argv[i]<<" ";
	cout<<endl;
	char currentFilePath[256];
	getcwd(currentFilePath, sizeof(currentFilePath));
	cout << "当前工作路径是： " << currentFilePath << endl;
	if(argc == 1){ 
		cout << "请在命令行中运行该程序" << endl;
		return 0;
	}
	else if(argc == 2)
		return 0;
	else if(argc == 3){
		if(!strcmp(argv[1], "-c"))//-c统计字母的频率
			alphabeticStatistics(argv[2]);
		else if(!strcmp(argv[1], "-f")) //-f 统计单词出现次数 
			wordStatistics(argv[2]);
		else if(!strcmp(argv[1], "-d")){// -d 指定文件目录，对目录下每一个文件执行 wf.exe -f 的操作。
			vector<string>allPath = getFileList(strcat(currentFilePath, argv[2]));
			for(size_t i = 0; i < allPath.size(); i++){
				string perPath = allPath.at(i);
				wordStatistics( (char *)perPath.c_str() );
			}
		}		
	}		 
	else if(argc == 4){// wf.exe -d -s
		if(!strcmp(argv[1],"-p")){ 
			wordGroups(argv[2],atoi(argv[3])); 	
		} 
		else if(!strcmp(argv[1], "-d")|| !strcmp(argv[2], "-s")){
			
			vector<string>allPath = getFileListInMenu(strcat(currentFilePath, argv[3]));
			for(size_t i = 0; i < allPath.size(); i++){
				string perPath = allPath.at(i);
				wordStatistics((char *)perPath.c_str());
			}
		}
		else if(!strcmp(argv[1], "-v"))
			dynamicPrototype(argv[2],argv[3]);	
		else if(strcmp(argv[1], "-n") == 0 || atoi(argv[2])%1 == 0)//支持 -n 参数，输出出现次数最多的前 n 个单词。	
			wordStatisticsTopK(argv[3], atoi(argv[2]));
	}
	else if(argc == 5)
	{
		//停词表stop world
		if(!strcmp(argv[1], "-x") || !strcmp(argv[3], "-f"))
		{
			WordFrequency wf(argv[4], argv[2]);
			wf.ReadStopFile();
			wf.ReadWordFile();
			wf.sortWordByFrequency();
			wf.printWordFrequency();
		}
	}
	return 0;	
} 
