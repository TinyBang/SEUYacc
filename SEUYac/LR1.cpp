#include"LR1.h"
using namespace std;
//vector<string> terminals;
vector<producer>expressions;
vector<int>expUsage;
//bool ifTerminals(string a);
//LrState addpredict(LrState tmpState, int k, int j);




#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<set>
#include<string>
#include<stack>
#include<sstream>
using namespace std;


ifstream ifile;
ofstream ofile;
int line = 0;
int checkState(char c);
#define FSTART -1
#define FEND -2
#define SECOND -3
#define ERROR -4
//操作符
struct op {
	string dest;//左操作符或右操作符
	int level;//优先级，level越大优先级越高
	string name;//操作符名称
};
//产生式
set<string> terminals;//终结符
set<string> unterminals;//非终结符
vector<op> oper;
vector<producer> pro;
vector<string> First(string);
int opLevel = 0;

void readFile()
{
	ifile.open("yacc.y", ios::in);
	ofile.open("yyyacc.cpp", ios::out);
	producer p;
	p.left = "S'";
	p.right.push_back("S");
	pro.push_back(p);
	if (!ifile.good())
	{
		cout << "Open file error!";
		return;
	}
	char c = ifile.get();
	int state = checkState(c);
	if (state != FSTART)
		cout << "File Error!";
	while (!ifile.eof() && state != FEND)
	{
		c = ifile.get();
		if (c == '%') {
			state = checkState(c);
			continue;
		}
		if (c == '\n')
			line++;
		ofile.put(c);
	}//完成yacc.y说明部分的解析

	c = ifile.get();
	string type, meaning;
	while (true)
	{
		c = ifile.get();
		if (c == '%')
		{
			if (checkState(c) == -3)
				break;
			ifile >> type;
			if (type == "token")
			{
				c = ifile.get();
				while (c != '\n')
				{
					ifile >> meaning;
					terminals.insert(meaning);
					c = ifile.get();
				}
			}
			else if (type == "left")
			{
				c = ifile.get();
				while (c != '\n')
				{
					ifile >> meaning;
					op o;
					o.level = opLevel;
					o.dest = type;
					o.name = meaning;
					oper.push_back(o);
					c = ifile.get();
				}
				opLevel++;
			}
		}
	}//说明部分解析完毕

	c = ifile.get();
	string str;
	getline(ifile, str);
	while (true)
	{
		if (str == "%%")
			break;
		if (str == "\n" | str == "")
		{
			getline(ifile, str);
			continue;
		}
		else if (str.find(':') != -1)
		{
			int offset = str.find(':');
			string str1 = str.substr(0, offset);
			string str2 = str.substr(offset + 1, str.size() - 1);
			if (terminals.find(str1) == terminals.end() && unterminals.find(str1) == unterminals.end())
			{
				unterminals.insert(str1);
			}

			producer pr;
			pr.left = str1;
			pr.right.push_back(str2);
			while (true)
			{
				getline(ifile, str);
				if (str.find('|') != -1)
				{
					int offset = str.find('|');
					string str1 = str.substr(offset + 1, str.size() - 1);

					pr.right.push_back(str1);
				}
				else
					break;
			}
			pro.push_back(pr);
		}
		else {
			cout << "file error!" << endl;
		}
	}
	c = ifile.get();
	while (!ifile.eof())
	{
		ofile.put(c);
		c = ifile.get();
	}
}

int checkState(char c)
{
	if (c == '%')
	{
		char cc = ifile.get();
		switch (cc)
		{
		case '{':
			return -1;
		case '}':
			return -2;
		case '%':
			return -3;
		default:
			ifile.unget();
			return -4;
		}
	}
	return 0;
}
vector<string> First(string str)
{
	vector<string> first;
	int value = 0;
M:
	if (terminals.find(str) != terminals.end())
	{
		first.push_back(str);
		value = 1;
	}
	else if (unterminals.find(str) != unterminals.end())
	{
		value = 1;
	R:
		for (int i = 0; i < pro.size(); i++)
		{
			if (pro[i].left == str)
			{
				for (int j = 0; j < pro[i].right.size(); j++)
				{
					string temp = pro[i].right[j];
					int offset = temp.find(" ");
					if (offset != -1)
						temp = temp.substr(0, offset);
					if (terminals.find(temp) != terminals.end())
						first.push_back(temp);
					else if (unterminals.find(temp) != unterminals.end())
					{
						str = pro[i].right[j];
						goto R;
					}
				}
			}
		}
	}
	else if (value == 0)
	{
		char c = str[0];
		stringstream stream;
		stream << c;
		str = stream.str();
		goto M;
	}
	else
	{
		cout << "first error";
	}
	return first;
}

LR1::LR1(producer pro0)
{
	LrState I0;
		I0.Number = 0;
		LrItem tmpItem;
		tmpItem.left = pro0.left;
		for (int i = 0; i < pro0.right[0].size(); i++) {
			string str;
			stringstream stream;
			stream << pro0.right[0][i];
			str = stream.str();
			tmpItem.right.push_back(str);
		}
		//tmpItem.right = pro0.right;
		tmpItem.dotpos = 0;
		tmpItem.predict.push_back("#");
		I0.items.push_back(tmpItem);
	I0=adjust(I0);
	AllStates.push_back(I0);
}
bool ItemExistance(LrState state,LrItem item) {
	for (int i = 0; i < state.items.size(); i++) {
		if (item <= state.items[i]) {
			return true;
		}
	}
	return false;
}
bool ifeleexist(vector<string> tmpvector, string tmpstring) {
	for (int i = 0; i < tmpvector.size(); i++) {
		if (tmpvector[i] == tmpstring) {
			return true;
		}
	}
	return false;
}
LrState LR1::adjust(LrState tmpState)
{
//	bool findSameItem = false;
		for (int i = 0; i < tmpState.items.size(); i++) {
			if (tmpState.items[i].dotpos<tmpState.items[i].right.size()&&!ifTerminals(tmpState.items[i].right[tmpState.items[i].dotpos])) {//如果后面的是非终结
				for (int j = 0; j < pro.size(); j++) {
					if (pro[j].left == tmpState.items[i].right[tmpState.items[i].dotpos] ) {//如果是该非终结符产生的生成式
						for (int counter = 0; counter < pro[j].right.size(); counter++) {
							LrItem tmpItem;
							tmpItem.dotpos = 0;
							tmpItem.left = pro[j].left;
							for (int k = 0; k < pro[j].right[counter].size();k++) {
								if (pro[j].right[counter] == "ID")
								{
									tmpItem.right.push_back(pro[j].right[counter]);
									break;
								}
								if (pro[j].right[counter] == "INTEGER") {
									tmpItem.right.push_back(pro[j].right[counter]);
									break;
								}
								if (pro[j].right[counter][k] != ' ') {
									string str;
									stringstream stream;
									stream << pro[j].right[counter][k];
									str = stream.str();
									tmpItem.right.push_back(str);
								}
							}
							if (tmpState.items[i].dotpos == tmpState.items[i].right.size() - 1&&!ifTerminals( tmpState.items[i].right[tmpState.items[i].dotpos])) {
								for (int k = 0; k < tmpState.items[i].predict.size(); k++) {
									tmpItem.predict.push_back(tmpState.items[i].predict[k]);
								}
							}
							else {
								tmpItem.predict.push_back("#");
							}
							string tmpstring = "";
							for (int k = tmpState.items[i].dotpos+1; k < tmpState.items[i].right.size(); k++) {
								tmpstring += tmpState.items[i].right[k];
							}
							if(tmpstring!="")
							for (int k = 0; k < First(tmpstring).size(); k++) {
								//tmpItem.predict.push_back(firststring[k]);

								if (!ifeleexist(tmpItem.predict, First(tmpstring)[k])) {
									tmpItem.predict.push_back(First(tmpstring)[k]);//加入first到预测符
								}
								
							}
							if (!ItemExistance(tmpState,tmpItem)) {
								tmpState.items.push_back(tmpItem);
							}
						}
					}
				}
				
			}
		}
		cout << "Done!" << endl;
		for (int j = tmpState.items.size() - 1; j >= 0; j--) {/*检查是否已有该产生式的Item*/
					for (int k = 0; k < j; k++) {
						if (tmpState.items[k].left == tmpState.items[j].left&&tmpState.items[k].right == tmpState.items[j].right&&tmpState.items[k].dotpos == tmpState.items[j].dotpos) {//如果已有
							tmpState = addpredict(tmpState, k, j);//如果两者预测符不同，后面的加到已有的。
							tmpState.items.erase(tmpState.items.begin() + j);//去掉新生成的Item
							break;
						}
					}
				}
		for (int i = 0; i < tmpState.items.size(); i++) {/*寻找出边*/
			if (tmpState.items[i].dotpos != tmpState.items[i].right.size()) {//点不在item最后
				bool outedgeexist = false;
				for (int j = 0; j < tmpState.outedge.size(); j++) {
					if (tmpState.items[i].right[tmpState.items[i].dotpos] == tmpState.outedge[j]) {
						outedgeexist = true;
						continue;
					}
				}
				if (!outedgeexist) {
					tmpState.outedge.push_back(tmpState.items[i].right[tmpState.items[i].dotpos]);
				}
			}
		}
	return tmpState;
}

LrState LR1::addpredict(LrState tmpState,int k,int j) {//k是原有的式子的下标
	int a = tmpState.items[j].predict.size();
	int b= tmpState.items[k].predict.size();
	bool findpre=false;
	for (int newpre = 0; newpre <a; newpre++) {
		for (int oldpre = 0; oldpre < b; oldpre++) {
			if (tmpState.items[k].predict[oldpre] == tmpState.items[j].predict[newpre]) {
				findpre = true;
				break;
			}
			else
				continue;
		}
		if (!findpre) {//没找到的话，加入到已有的式子的预测符中
			tmpState.items[k].predict.push_back(tmpState.items[j].predict[newpre]);
		}
		else
		{
			findpre = false;
		}
	}
	return tmpState;
}
LrState LR1::createNewState(int transferform, string edge) {
	LrState tmpState;
	tmpState = AllStates[transferform];
	tmpState.edges.clear();
	for (int i = tmpState.items.size() - 1; i >= 0; i--) {/*去掉不是这个路径的非终结符*/
		if (tmpState.items[i].dotpos < tmpState.items[i].right.size()) {
			if (tmpState.items[i].right[tmpState.items[i].dotpos] != edge) {
				tmpState.items.erase(tmpState.items.begin() + i);
			}
			else
			{
				tmpState.items[i].dotpos++;
			}
		}
		else
		{
			tmpState.items.erase(tmpState.items.begin() + i);
			continue;
		}
	}
	tmpState.outedge.clear();
	tmpState = adjust(tmpState);
	return tmpState;
}
void LR1::createLR1() {
	for (int i = 0; i < AllStates.size(); i++) {
		for (int j = 0; j < AllStates[i].outedge.size(); j++) {
			LrState tmpState;
			tmpState = createNewState(i,AllStates[i].outedge[j]);
			if (ifStateExistant(tmpState)==-1) {
				tmpState.Number = AllStates.size();
				AllStates[i].edges.push_back(pair<string, int>(AllStates[i].outedge[j], tmpState.Number));
				AllStates.push_back(tmpState);
			}
			else {
				AllStates[i].edges.push_back(pair<string, int>(AllStates[i].outedge[j], ifStateExistant(tmpState)));
			}
		}
	}
}
int LR1::ifStateExistant(LrState newState) {
	for (int i = 0; i < AllStates.size(); i++) {
		if (newState == AllStates[i]) {
			return i;
			break;
		}
	}
	return -1;
}
bool LR1::ifTerminals(string a)
{
	if (terminals.find("a") == terminals.end())
		return false;
	else 
		return true;
}




int main()
{
	readFile();
	First("(T");
	LR1 newlr1(pro[0]);
	newlr1.createLR1();
}