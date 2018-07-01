#pragma once
class AStar
{
public:
	AStar();
	~AStar();

private:
	list<unique_ptr<NODE>>		mOpenList;	//������ ����� ����
	vector<unique_ptr<NODE>> mvecOpenList;	//������ ���� ����Ʈ
	list<unique_ptr<NODE>>		mCloseList;	//������ ����� ����
	list<int>					mBestList;	//������ ���
	int miStartIdx;							//����ε���
	int miGoalIdx;							//������ �ε���

public:
	list<int>* GetBestList() { return &mBestList; }
public:
	static bool Compare(unique_ptr<NODE>& pDestNode, unique_ptr<NODE>& pSourceNode)
	{
		return pDestNode->fCost < pSourceNode->fCost;
	}
public:
	void AStarStat(const int& iStartidx_, const int& iGoalidx_);
	void MakeRoute();
	void VecMakeRoute();
	bool CheckList(const int& index_);
	unique_ptr<NODE> MakeNode(int index_, NODE* pParent_,
		const vector<unique_ptr<TILE>>* pVecTile);
	void HeapInsert(vector<unique_ptr<NODE>>* vecOpen, unique_ptr<NODE>* tileNode);
	void HeapMinSort(vector<unique_ptr<NODE>>* vecOpen);
	void Release();
};

