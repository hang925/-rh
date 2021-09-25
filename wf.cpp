#include <io.h>
#include <string>
#include <bits/stdc++.h>
using namespace std;

int letterNumber[26];
int total = 0;
int len = 0;
char buffer[1024+1];

struct alphabeticStruct //����	�ṹ�� 
{
	char name;
	float letterNumber;
	alphabeticStruct(char n = 'a', float lN = 0){
		name = n;
		letterNumber = lN;
	}
};

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



void alphabeticStatistics(char *fileName) //��0�� ͳ��Ƶ�� 
{
	memset(letterNumber, 0, sizeof(letterNumber) * 26);
	total = 0;
	ifstream inFile;
	inFile.open(fileName);
	ofstream outFile;
	if( !inFile.is_open())
	{
		cout << ":�ļ���ʧ��" << endl;
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

 void wordStatistics(char *fileName) //����1��ͳ�Ʋ��ظ����ʳ��ֵĴ���
{
	ifstream inFile;
	inFile.open(fileName);
	
	if( !inFile.is_open()){
		cout << fileName << ":�ļ���ʧ��" << endl;
		exit(1);
	}
	
	string str;
	map<string,int>wordcount;

	while( !inFile.eof()){
		inFile >> str;
		wordcount[str]++;
	}
	priority_queue<pair<int,string>, vector<pair<int,string> >, greater<pair<int,string> > > queueK; //�������� 
	
	for (map<string,int>::iterator iter=wordcount.begin(); iter!=wordcount.end(); iter++)
		queueK.push(make_pair(iter->second,iter->first));
		
	pair<int,string>tmp;
	
	while (!queueK.empty()){	 
		tmp=queueK.top();
		queueK.pop();
		cout<<tmp.second<<"\t"<<tmp.first<<endl;
	}
}





int main(int argc, char *argv[])
{
	for(int i = 0; i < argc; i++)
		cout<<"argv["<<i<<"]"<<argv[i]<<" ";
	cout<<endl;
	char currentFilePath[256];
	getcwd(currentFilePath, sizeof(currentFilePath));
	cout << "��ǰ����·���ǣ� " << currentFilePath << endl;
	if(argc == 1){ 
		cout << "���������������иó���" << endl;
		return 0;
	}
	else if(argc == 2)
		return 0;
	else if(argc == 3){//ͳ����ĸ��Ƶ��
		if(!strcmp(argv[1], "-c"))
			alphabeticStatistics(argv[2]);
		else if(!strcmp(argv[1], "-f"))
			wordStatistics(argv[2]);
	}
			 
	
} 

