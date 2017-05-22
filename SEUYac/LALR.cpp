#include"LALR.h"


bool ifLRStateEqual(LrState state1, LrState state2);
vector<LrState>addedge(vector<LrState> states, int i, vector<pair<string, int>>edges) {
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
vector<LrState>addpre(vector<LrState> states, int i, LrState state) {
	for (int j = 0; j < state.items.size(); j++) {
		for (int k = 0; k < state.items[i].predict.size(); k++) {

		}
	}
	return states;
}
LALR::LALR(vector<LrState>LR1States) {
	for (int i = 0; i < LR1States.size(); i++) {
		vector<int>element;
		element.push_back(i);
		AllState.push_back(LR1States[i]);
		for (int j = i+1; j < LR1States.size(); j++) {
			if (ifLRStateEqual(LR1States[i] , LR1States[j])) {
				element.push_back(j);
				AllState=addedge(AllState,i, LR1States[j].edges);
				AllState=addpre(AllState, i, LR1States[j]);
				LR1States.erase(LR1States.begin() + j);
			}
		}
		StateSet.push_back(element);
	}
	for (int i = 0; i < AllState.size(); i++) {
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
}
bool ifLrItemEqual(LrItem item1, LrItem item2) {
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
bool ifLRStateEqual(LrState state1, LrState state2) {
	bool ifequal = true;
	if (state1.items.size() == state2.items.size()) {
		for (int i = 0; i < state1.items.size(); i++) {/*ÅÐ¶Ïitems*/
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