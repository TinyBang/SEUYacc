#pragma once
#ifndef LR1_H
#define LR1_H

#include<vector>
#include<map>
#include<set>
using namespace std;

struct producer {//解析后的产生式
	string left;
	vector<string> right;

};
struct LrItem
{
	string left;
	vector<string> right;
	int dotpos;
	vector<string>predict;
	bool &operator<=(const LrItem &item) {/*和==差不多，但没有要求预测符数量，只要前者的预测符都能在后者找到就算满足*/
		bool tmp = true;
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
						if (!findpre) {
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
	bool &operator==(const LrItem &item) {/*重载等号运算符，判断是否两个状态相等*/
		bool tmp = true;//两个Item是否相同的标志位
		if (item.dotpos == this->dotpos) {//先判断点的位置
			if (item.left == this->left) {//判断产生式左边
				if (item.right == this->right) {//产生式右边
					if (this->predict.size() == item.predict.size())//预测符数量
						for (int i = 0; i < this->predict.size(); i++) {
							bool findpre = false;//预测符标记位，后面很多代码都用到了这种方式
							for (int j = 0; j < item.predict.size(); j++) {
								if (this->predict[i] == item.predict[j]) {
									findpre = true;//如果在前者找到了后者中的预测符，那么标记位置1
								}
							}
							if (!findpre) {
								tmp = false;//如果其中一个标志位没有找到，那么说明这两个Item不相同。
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
	int Number;//State编号
	vector<LrItem> items;
	vector<pair<string, int>>edges;//出边
	vector<string>outedge;//点向后看一位的集合
	bool &operator==(const LrState &state) {
		bool ifequal = true;//两个State是否相同的标记位
		if (this->items.size() == state.items.size()) {//判断Item数，提高代码效率
			for (int i = 0; i < this->items.size(); i++) {/*判断items*/
				bool ifitemssequal = false;//Item相同的标记位
				for (int j = 0; j < state.items.size(); j++) {
					if (this->items[i] == state.items[j]) {
						ifitemssequal = true;//找到Item，标记位置1
						break;
					}
				}
				if (!ifitemssequal) {//如果没找到Item，则两个State不同
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
	vector<LrState> createLR1();//创建LR1
	vector<LrState> AllStates;//保存所有State
	LrState createNewState(int, string);//被转换的State的序号，规则数，边
										//vector<producer>expressions;
	LrState adjust(LrState tmpState);//移动点后，对点后面的非终结符进行扩展
									 //int currentStateNum = 0;
	bool LR1::ifTerminals(string a);//判断是否为终结符
	LrState LR1::addpredict(LrState tmpState, int k, int j);//k是原有的式子的下标
	int ifStateExistant(LrState newState);//判断State是否存在，若不存在，返回-1，如存在，返回那个State的编号
};
#endif // !LR1_H
