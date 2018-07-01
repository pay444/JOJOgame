#pragma once
class AStar
{
public:
	AStar();
	~AStar();

private:
	list<unique_ptr<NODE>>		mOpenList;	//조사할 대상의 집합
	vector<unique_ptr<NODE>> mvecOpenList;	//벡터의 오픈 리스트
	list<unique_ptr<NODE>>		mCloseList;	//조사한 대상의 집합
	list<int>					mBestList;	//최적의 경로
	int miStartIdx;							//출발인덱스
	int miGoalIdx;							//목적지 인덱스

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

