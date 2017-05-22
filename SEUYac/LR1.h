#pragma once
#ifndef LR1_H
#define LR1_H

#include<vector>
#include<map>
#include<set>
using namespace std;

struct producer {
	string left;
	vector<string> right;

};
struct LrItem
{
	string left;
	vector<string> right;
	int dotpos;
	vector<string>predict;
	bool &operator<=(const LrItem &item) {
		bool tmp=true;
		if (item.dotpos == this->dotpos) {
			if (item.left == this->left) {
				if (item.right == this->right) {
					for (int i = 0; i < this->predict.size(); i++) {
						bool findpre = false;
						for (int j = 0; j < item.predict.size(); j++) {
							if (this->predict[i] == item.predict[j]) {
								findpre = true;
							}
						}
						if (!findpre){
							tmp = false;
						}
					}
				}
				else
					tmp = false;
			}
			else
				tmp = false;
		}
		else
			tmp = false;
		return tmp;
	}
	bool &operator==(const LrItem &item) {
		bool tmp = true;
		if (item.dotpos == this->dotpos) {
			if (item.left == this->left) {
				if (item.right == this->right) {
					if (this->predict.size() == item.predict.size())
						for (int i = 0; i < this->predict.size(); i++) {
							bool findpre = false;
							for (int j = 0; j < item.predict.size(); j++) {
								if (this->predict[i] == item.predict[j]) {
									findpre = true;
								}
							}
							if (!findpre) {
								tmp = false;
							}
						}
					else
						tmp = false;
				}
				else
					tmp = false;
			}
			else
				tmp = false;
		}
		else
			tmp = false;
		return tmp;
	}

};

struct LrState
{
	int Number;
	vector<LrItem> items;
	vector<pair<string, int>>edges;
	vector<string>outedge;
	bool &operator==(const LrState &state) {
		bool ifequal = true;
		/*if (this->edges.size() == state.edges.size())
		{
			for (int i = 0; i < this->edges.size(); i++) {/*判断出边
				bool ifedgesequal = false;
				for (int j = 0; j < state.edges.size(); j++) {
					if (this->edges[i] == state.edges[j]) {
						ifedgesequal = true;
						break;
					}
				}
				if (!ifedgesequal) {
					ifequal = false;
					break;
				}
			}
		}
		else
		{
			ifequal = false;
		}*/
		if (this->items.size() == state.items.size()) {
			for (int i = 0; i < this->items.size(); i++) {/*判断items*/
				bool ifitemssequal = false;
				for (int j = 0; j < state.items.size(); j++) {
					if (this->items[i] == state.items[j]) {
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
};
class LR1
{
public:
	LR1(producer);
	void createLR1();
	vector<LrState> AllStates;
	LrState createNewState(int,string);//被转换的State的序号，规则数，边
	vector<producer>expressions;
	LrState adjust(LrState tmpState);
	int currentStateNum = 0;
	bool LR1::ifTerminals(string a);
	LrState LR1::addpredict(LrState tmpState, int k, int j);//k是原有的式子的下标
	int ifStateExistant(LrState newState);//判断State是否存在，若不存在，返回-1，如存在，返回那个State的编号
};
#endif // !LR1_H
