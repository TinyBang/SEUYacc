#ifndef PARSETABLE_H
#define PARSETABLE_H
#include <vector>
#include"LR1.h"
using namespace std;
#define shift 1
#define reduce 2
#define gotoState 3
#define accept 4
#define error 5
//定义几种基本的操作，在此处将这些操作映射成整数

//vector<int> productionSize;//记录文法产生式右端的size
//vector<int> productionReduce;//记录文法产生式左端

struct TranAction {//一个Action的结构，包含起点，动作，终点
	string sample;
	int action;
	int index;
	TranAction() {};
	TranAction(int s, int a, int i)
	{
		sample = s;
		action = a;
		index = i;
	}
};

struct State {//状态结构，包含该状态本身（用数字表示）和该状态对应的转化列表
	int state;
	vector<TranAction> tranlist;
	State(int s) :state(s) {};
	State(int s, vector<TranAction> tlist)
	{
		state = s;
		tranlist = tlist;
	}
};

class ParseTable {
public:
	ParseTable(vector<LrState>) {

	};
	vector<State> pTable;
	//set<string> terminals;
	//set<string> unterminals;
};

#endif