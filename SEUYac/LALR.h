#pragma once
#ifndef LALR_H
#include "LR1.h"
class LALR {
public:
	LALR(vector<LrState>);//构造函数
	vector<LrState>AllState;//保存LALR中所有点
	vector<vector<int>>StateSet;//保存LALR中的STATE和LR1中State的对应关系
	vector<LrState> getAllState();//返回AllStates
};

#endif // !LALR_H
