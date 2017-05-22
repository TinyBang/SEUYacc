#pragma once
#ifndef LALR_H
#include "LR1.h"
class LALR {
public:
	LALR(vector<LrState>);
	vector<LrState>AllState;
};

#endif // !LALR_H
