#include"ParseTable.h"
#include"LR1.h"

ParseTable::ParseTable(vector<LrState> allState)
{
	for (int i = 0; i < allState.size(); i++)
	{
		State state(i);
		for (int j = 0; j < allState[i].edges.size(); j++)
		{
			TranAction tran;
			if (terminals.find(allState[i].edges[j].first) != terminals.end())
			{
				tran.action = shift;
				tran.index = allState[i].edges[j].second;
				tran.sample =allState[i].edges[j].first;
			}
			else if (unterminals.find(allState[i].edges[j].first) != unterminals.end())
			{
				tran.action = gotoState;
				tran.index = allState[i].edges[j].second;
				tran.sample = allState[i].edges[j].first;
			}
		}
		


	}

}