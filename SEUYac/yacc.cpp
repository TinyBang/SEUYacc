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
struct producer {
	string left;
	vector<string> right;
};
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
		if (str=="%%")
			break;	
		if (str == "\n"| str == "")
		{
			getline(ifile, str);
			continue;
		}			
		else if (str.find(':') != -1)
		{
			int offset = str.find(':');
			string str1 = str.substr(0, offset);
			string str2 = str.substr(offset+1, str.size() - 1);
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
					string str1 = str.substr(offset+1, str.size() - 1);
					
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
	while (!ifile.eof() )
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
						temp = temp.substr( 0, offset);
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
	else if(value==0)
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
	cout << "Done!" << endl;
	return first;
}


int main()
{
	readFile();
	First("(T");
}