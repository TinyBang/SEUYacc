#include"LALR.h"

bool ifLrItemEqual(LrItem item1, LrItem item2);
bool ifLRStateEqual(LrState state1, LrState state2);
vector<LrState>addedge(vector<LrState> states, int i, vector<pair<string, int>>edges) {/*出边的集合加入到相同state的出边*/
	for (int j = 0; j< edges.size(); j++) {
		bool ifsameedges = false;
		for (int k = 0; k < states[i].edges.size(); k++) {
			if (states[i].edges[k] == edges[j]) {
				ifsameedges = true;
				break;
			}
		}
		if (!ifsameedges) {
			states[i].edges.push_back(edges[j]);
		}
	}
	return states;
}
vector<LrState>addpre(vector<LrState> states, int i, LrState state) {/*将相同state的预测符合并*/
	for (int j = 0; j < state.items.size(); j++) {
			for (int l = 0; l < states[i].items.size(); l++) {
				if (ifLrItemEqual(states[i].items[l], state.items[j])) {
					for (int k = 0; k < state.items[j].predict.size(); k++) {
						bool ifpredictexist = false;
						for (int m = 0; m < states[i].items[l].predict.size(); m++) {
							if (states[i].items[l].predict[m] == state.items[j].predict[k]) {
								ifpredictexist = true;
								break;
							}
						}
						if (!ifpredictexist) {
							states[i].items[l].predict.push_back(state.items[j].predict[k]);
						}
					}
				}
		}
	}
	return states;
}
LALR::LALR(vector<LrState>LR1States) {
	for (int i = 0; i < LR1States.size(); i++) {
		vector<int>element;//当前LALR边对应的LR1State集合
		element.push_back(LR1States[i].Number);
		AllState.push_back(LR1States[i]);
		for (int j = i+1; j < LR1States.size(); j++) {//遍历其他LR1States
			if (ifLRStateEqual(LR1States[i] , LR1States[j])) {/*如果有State和当前State相同，把它的出边和预测符加入当前state，并且删掉这个state*/
				element.push_back(LR1States[j].Number);
				AllState=addedge(AllState,i, LR1States[j].edges);
				AllState=addpre(AllState, i, LR1States[j]);
				LR1States.erase(LR1States.begin() + j);
			}
		}
		StateSet.push_back(element);//保存当前关系
	}
	for (int i = 0; i < AllState.size(); i++) {/*将出边的目标设置成LALR的编号*/
		for (int j = 0; j < AllState[i].edges.size(); j++) {
			for (int k = 0; k < StateSet.size(); k++) {
				for (int l = 0; l < StateSet[k].size(); l++) {
					if (AllState[i].edges[j].second == StateSet[k][l]) {
						AllState[i].edges[j].second = k;
				}
				}
			}
		}
	}
	for (int i = 0; i < AllState.size(); i++) {/*设置编号*/
		AllState[i].Number = i;
	}
}
bool ifLrItemEqual(LrItem item1, LrItem item2) {/*判断两个item是不是除了预测符都相同*/
	bool tmp = true;
	if (item1.dotpos == item2.dotpos) {
		if (item1.left == item2.left) {
			if (item1.right != item2.right)
				tmp = false;
		}
		else
			tmp = false;
	}
	else
		tmp = false;
	return tmp;
}
bool ifLRStateEqual(LrState state1, LrState state2) {/*判断两个State是否除了预测符都相同*/
	bool ifequal = true;
	if (state1.items.size() == state2.items.size()) {
		for (int i = 0; i < state1.items.size(); i++) {/*判断items*/
			bool ifitemssequal = false;
			for (int j = 0; j < state2.items.size(); j++) {
				if (ifLrItemEqual( state1.items[i] ,state2.items[j]) ){
					ifitemssequal = true;
					break;
				}
			}
			if (!ifitemssequal) {
				ifequal = false;
				break;
			}
		}
	}
	else
	{
		ifequal = false;
	}
	return ifequal;
}
vector<LrState> LALR::getAllState() {
	return AllState;
}