#include "stdafx.h"
#include "AStar.h"
#include "MoveBox.h"

AStar::AStar()
{
}


AStar::~AStar()
{
}

void AStar::AStarStat(const int & iStartidx_, const int & iGoalidx_)
{
	if (iStartidx_ == iGoalidx_)
		return;
	const vector<unique_ptr<TILE>>* pVecTile = MActorManager::Instance().GetTileInfo();

	if (pVecTile == NULL)
		return;

	// 벽이라면..
	if ((*pVecTile)[iGoalidx_]->byDrawID == 1)
		return;

	miStartIdx = iStartidx_;
	miGoalIdx = iGoalidx_;

	this->Release();

	//this->MakeRoute();
	this->VecMakeRoute();
}

void AStar::MakeRoute()
{
	//// 코스트 계산을 통해서,
	//// 최적의 길을 m_BestList 인덱스형으로 담는다.///
	unique_ptr<NODE> pParent(new NODE);
	pParent->iIndex = miStartIdx;
	pParent->pParent = NULL;
	pParent->fCost = 0.0f;
	mCloseList.push_back(move(pParent));

	NODE* pParent2 = mCloseList.back().get();

	NODE* npParent = nullptr;
	const vector<unique_ptr<TILE>>* pVecTile =
		MActorManager::Instance().GetTileInfo();

	if (pVecTile == NULL)
		return;

	int iIndex = 0;

	while (true)	// 탈출조건 ? 노드가 목적지에 도달할때까지...
	{
		int JOJOCX = 20;
		int JOJOCY = 20;
		/*
		if(존재하는 타일이냐?,
		갈수 있는 타일이냐?,
		이미조사했거나,
		오픈리스트에 추가되있는 녀석 제외)
		*/
		// 위
		iIndex = pParent2->iIndex - JOJOCX;

		if (pParent2->iIndex >= JOJOCX &&		// 맨 윗줄이아니라면,
			(*pVecTile)[iIndex]->byOption == 0 &&	// 갈수 있는 타일?
			this->CheckList(iIndex))
		{
			unique_ptr<NODE> pNode = nullptr;
			pNode = move(MakeNode(iIndex, pParent2, pVecTile));
			mOpenList.push_back(move(pNode));
		}
		// 오른쪽 위
		/*iIndex = pParent->iIndex -
		(JOJOCX - 1);
		if (pParent->iIndex >= JOJOCX &&
		pParent->iIndex % (JOJOCX) != JOJOCX - 1 &&
		(*pVecTile)[iIndex]->byOption == 0 &&
		this->CheckList(iIndex))
		{
		pNode = MakeNode(iIndex, pParent, pVecTile);
		m_OpenList.push_back(pNode);
		}*/
		// 오른쪽
		iIndex = pParent2->iIndex + 1;
		if (pParent2->iIndex % JOJOCX != JOJOCX - 1 &&
			(*pVecTile)[iIndex]->byOption == 0 &&
			CheckList(iIndex))
		{
			unique_ptr<NODE> pNode = nullptr;
			pNode = move(MakeNode(iIndex, pParent2, pVecTile));
			mOpenList.push_back(move(pNode));
		}

		// 오른쪽 아래
		/*iIndex = pParent->iIndex +
		(JOJOCX+1);

		if (pParent->iIndex < JOJOCX * JOJOCY - JOJOCX &&
		pParent->iIndex % JOJOCX != JOJOCX - 1 &&
		(*pVecTile)[iIndex]->byOption == 0 &&
		CheckList(iIndex))
		{
		pNode = MakeNode(iIndex, pParent, pVecTile);
		m_OpenList.push_back(pNode);
		}*/
		// 아래
		iIndex = pParent2->iIndex + JOJOCX;
		if (pParent2->iIndex < JOJOCX * JOJOCY - JOJOCX &&
			(*pVecTile)[iIndex]->byOption == 0 &&
			CheckList(iIndex))
		{
			unique_ptr<NODE> pNode = nullptr;
			pNode = move(MakeNode(iIndex, pParent2, pVecTile));
			mOpenList.push_back(move(pNode));
		}
		// 왼쪽 아래
		/*iIndex = pParent->iIndex +
		(JOJOCX - 1);

		if (pParent->iIndex < JOJOCX * JOJOCY - JOJOCX &&
		pParent->iIndex % JOJOCX != 0 &&
		(*pVecTile)[iIndex]->byOption == 0 &&
		CheckList(iIndex))
		{
		pNode = MakeNode(iIndex, pParent, pVecTile);
		m_OpenList.push_back(pNode);
		}*/

		// 왼쪽
		iIndex = pParent2->iIndex - 1;
		if (pParent2->iIndex % (JOJOCX) != 0 &&
			(*pVecTile)[iIndex]->byOption == 0 &&
			CheckList(iIndex))
		{
			unique_ptr<NODE> pNode = nullptr;
			pNode = move(MakeNode(iIndex, pParent2, pVecTile));
			mOpenList.push_back(move(pNode));
		}
		// 왼쪽 위
		/*iIndex = pParent->iIndex -
		(JOJOCX + 1);

		if (pParent->iIndex >= JOJOCX &&
		pParent->iIndex % (JOJOCX) != 0 &&
		(*pVecTile)[iIndex]->byOption == 0 &&
		CheckList(iIndex))
		{
		pNode = MakeNode(iIndex, pParent, pVecTile);
		m_OpenList.push_back(pNode);
		}*/

		// Parent로 부터 8방향 조사가 끝이 났고,
		// 8개의 노드중에 갈 수 있는 노드를 OpenList에 넣어두었다.
		// 이 중에서 코스트가 가장 적을 고른다. -> 오름차순 정렬
		// Sort(함수포인터);
		mOpenList.sort(Compare);

		list<unique_ptr<NODE>>::iterator iter = mOpenList.begin();
		//pParent->fCost = ((*iter).get())->fCost;
		//pParent->iIndex = ((*iter).get())->iIndex;
		//pParent->pParent = ((*iter).get())->pParent;

		if (mOpenList.empty())
			return;

		pParent2 = (*iter).get();

		// 가장 적은 코스트를 가진 (*iter)를 CloseList에 담아서 관리한다.
		mCloseList.push_back(move(*iter));

		iter = mOpenList.erase(iter);

		// 탈출 !
		if (pParent2->iIndex == miGoalIdx)
		{
			while (true)
			{
				mBestList.push_back(pParent2->iIndex);
				pParent2 = pParent2->pParent;
				
				if (pParent2->iIndex == miStartIdx)
					break;
			}

			// 컨테이너를 뒤집는다.(현재 Goal -> Start)
			mBestList.reverse();
			
			break;	// MakeRoute while문 탈출 !
		}
	}
}

void AStar::VecMakeRoute()
{
	//// 코스트 계산을 통해서,
	//// 최적의 길을 m_BestList 인덱스형으로 담는다.///
	const vector<unique_ptr<TILE>>* pVecTile = MActorManager::Instance().GetTileInfo();

	unique_ptr<NODE> pParent(new NODE);
	pParent->iIndex = miStartIdx;
	pParent->pParent = NULL;
	pParent->fCost = 0.0f;
	pParent->gCost = 0.0f;
	Vector2 vDir;
	vDir = (*pVecTile)[miStartIdx]->vPos - (*pVecTile)[miGoalIdx]->vPos;
	pParent->hCost = vDir.Length();
	mCloseList.push_back(move(pParent));

	NODE* pParent2 = mCloseList.back().get();

	NODE* npParent = nullptr;

	if (pVecTile == NULL)
		return;

	int iIndex = 0;

	while (true)	// 탈출조건 ? 노드가 목적지에 도달할때까지...
	{
		int JOJOCX = 20;
		int JOJOCY = 20;
		/*
		if(존재하는 타일이냐?,
		갈수 있는 타일이냐?,
		이미조사했거나,
		오픈리스트에 추가되있는 녀석 제외)
		*/
		// 위
		iIndex = pParent2->iIndex - JOJOCX;
		auto moveBox = MActorManager::Instance().GetClassMoveBox();
		auto pCharacter = moveBox->GetCharacter();
		if (pParent2->iIndex >= JOJOCX &&		// 맨 윗줄이아니라면,
			((*pVecTile)[iIndex]->byOption == 0 || 
			(*pVecTile)[iIndex]->underObject == pCharacter->GetCamp())&&	// 갈수 있는 타일?
			this->CheckList(iIndex))
		{
			unique_ptr<NODE> pNode = nullptr;
			pNode = move(MakeNode(iIndex, pParent2, pVecTile));

			HeapInsert(&mvecOpenList, &pNode);
			//mOpenList.push_back(move(pNode));
		}
		// 오른쪽 위
		/*iIndex = pParent->iIndex -
		(JOJOCX - 1);
		if (pParent->iIndex >= JOJOCX &&
		pParent->iIndex % (JOJOCX) != JOJOCX - 1 &&
		(*pVecTile)[iIndex]->byOption == 0 &&
		this->CheckList(iIndex))
		{
		pNode = MakeNode(iIndex, pParent, pVecTile);
		m_OpenList.push_back(pNode);
		}*/
		// 오른쪽
		iIndex = pParent2->iIndex + 1;
		if (pParent2->iIndex % JOJOCX != JOJOCX - 1 &&
			((*pVecTile)[iIndex]->byOption == 0 ||
			(*pVecTile)[iIndex]->underObject == pCharacter->GetCamp()) &&
			CheckList(iIndex))
		{
			unique_ptr<NODE> pNode = nullptr;
			pNode = move(MakeNode(iIndex, pParent2, pVecTile));
			HeapInsert(&mvecOpenList, &pNode);
			//mOpenList.push_back(move(pNode));
		}

		// 오른쪽 아래
		/*iIndex = pParent->iIndex +
		(JOJOCX+1);

		if (pParent->iIndex < JOJOCX * JOJOCY - JOJOCX &&
		pParent->iIndex % JOJOCX != JOJOCX - 1 &&
		(*pVecTile)[iIndex]->byOption == 0 &&
		CheckList(iIndex))
		{
		pNode = MakeNode(iIndex, pParent, pVecTile);
		m_OpenList.push_back(pNode);
		}*/
		// 아래
		iIndex = pParent2->iIndex + JOJOCX;
		if (pParent2->iIndex < JOJOCX * JOJOCY - JOJOCX &&
			((*pVecTile)[iIndex]->byOption == 0 ||
			(*pVecTile)[iIndex]->underObject == pCharacter->GetCamp()) &&
			CheckList(iIndex))
		{
			unique_ptr<NODE> pNode = nullptr;
			pNode = move(MakeNode(iIndex, pParent2, pVecTile));
			HeapInsert(&mvecOpenList, &pNode);
			//mOpenList.push_back(move(pNode));
		}
		// 왼쪽 아래
		/*iIndex = pParent->iIndex +
		(JOJOCX - 1);

		if (pParent->iIndex < JOJOCX * JOJOCY - JOJOCX &&
		pParent->iIndex % JOJOCX != 0 &&
		(*pVecTile)[iIndex]->byOption == 0 &&
		CheckList(iIndex))
		{
		pNode = MakeNode(iIndex, pParent, pVecTile);
		m_OpenList.push_back(pNode);
		}*/

		// 왼쪽
		iIndex = pParent2->iIndex - 1;
		if (pParent2->iIndex % (JOJOCX) != 0 &&
			((*pVecTile)[iIndex]->byOption == 0 ||
			(*pVecTile)[iIndex]->underObject == pCharacter->GetCamp()) &&
			CheckList(iIndex))
		{
			unique_ptr<NODE> pNode = nullptr;
			pNode = move(MakeNode(iIndex, pParent2, pVecTile));
			HeapInsert(&mvecOpenList, &pNode);
			//mOpenList.push_back(move(pNode));
		}
		// 왼쪽 위
		/*iIndex = pParent->iIndex -
		(JOJOCX + 1);

		if (pParent->iIndex >= JOJOCX &&
		pParent->iIndex % (JOJOCX) != 0 &&
		(*pVecTile)[iIndex]->byOption == 0 &&
		CheckList(iIndex))
		{
		pNode = MakeNode(iIndex, pParent, pVecTile);
		m_OpenList.push_back(pNode);
		}*/

		// Parent로 부터 8방향 조사가 끝이 났고,
		// 8개의 노드중에 갈 수 있는 노드를 OpenList에 넣어두었다.
		// 이 중에서 코스트가 가장 적을 고른다. -> 오름차순 정렬
		// Sort(함수포인터);
		//mOpenList.sort(Compare);

		//Heap정렬을 이용한 방법
		vector<unique_ptr<NODE>>::iterator iter = mvecOpenList.begin();
		//auto a = ; == 27;

		//list<unique_ptr<NODE>>::iterator iter = mOpenList.begin();
		//pParent->fCost = ((*iter).get())->fCost;
		//pParent->iIndex = ((*iter).get())->iIndex;
		//pParent->pParent = ((*iter).get())->pParent;

		if (mvecOpenList.empty())
			return;

		pParent2 = (*iter).get();

		// 가장 적은 코스트를 가진 (*iter)를 CloseList에 담아서 관리한다.
		mCloseList.push_back(move(*iter));

		mvecOpenList[0] = move(mvecOpenList.back());
		mvecOpenList.pop_back();

		HeapMinSort(&mvecOpenList);

		// 탈출 !
		if (pParent2->iIndex == miGoalIdx)
		{
			while (true)
			{
				mBestList.push_back(pParent2->iIndex);
				pParent2 = pParent2->pParent;

				if (pParent2->iIndex == miStartIdx)
					break;
			}

			// 컨테이너를 뒤집는다.(현재 Goal -> Start)
			mBestList.reverse();

			break;	// MakeRoute while문 탈출 !
		}
	}
}

bool AStar::CheckList(const int & index_)
{
	for (list<unique_ptr<NODE>>::iterator iter = mOpenList.begin();
		iter != mOpenList.end(); ++iter)
	{
		if ((*iter)->iIndex == index_)
			return false;
	}

	for (list<unique_ptr<NODE>>::iterator iter = mCloseList.begin();
		iter != mCloseList.end(); ++iter)
	{
		if ((*iter)->iIndex == index_)
			return false;
	}

	return true;
	//return false;
}

unique_ptr<NODE> AStar::MakeNode(int index_, NODE* pParent_, const vector<unique_ptr<TILE>>* pVecTile)
{
	unique_ptr<NODE> pNode(new NODE);
	pNode->iIndex = index_;
	pNode->pParent = pParent_;
	//pNode = pParent_;

	// 부모 노드까지의 거리(비용)
	Vector2 vDir = (*pVecTile)[index_]->vPos -
		(*pVecTile)[pParent_->iIndex]->vPos;
	pNode->gCost = pParent_->gCost + vDir.Length();

	//XMFLOAT2 float2 = {XMVectorGetX(vDir),XMVectorGetY(vDir)};
	
	//float fPCost = XMVector2Length(&vDir);
	float fPCost = pNode->gCost; //sqrt(pow(float2.x, 2) + pow(float2.y, 2));

	// 도착(목적지) 노드까지의 거리(비용)
	vDir = (*pVecTile)[index_]->vPos -
		(*pVecTile)[miGoalIdx]->vPos;
	
	pNode->hCost = vDir.Length();

	//float2 = { XMVectorGetX(vDir),XMVectorGetY(vDir) };

	//float fGCost = XMVector2Length(&vDir);
	float fGCost = pNode->hCost; //sqrt(pow(float2.x, 2) + pow(float2.y, 2));
	pNode->fCost = fPCost + fGCost;

	return pNode;
	//return nullptr;
}

void AStar::HeapInsert(vector<unique_ptr<NODE>>* vecOpen, unique_ptr<NODE>* tileNode)
{
	for (vector<unique_ptr<NODE>>::iterator iter = mvecOpenList.begin();
		iter != mvecOpenList.end(); ++iter)
	{
		if ((*iter)->iIndex == (*tileNode)->iIndex)
		{
			//기존것보다 새로만든것이 코스트가 작으면 갱신
			if ((*iter)->fCost > (*tileNode)->fCost)
			{
				iter = mvecOpenList.erase(iter);
				mvecOpenList.insert(iter, move(*tileNode));

				return;
			}
			//아니면 리턴
			else
			{
				return;
			}

		}

	}
	(*vecOpen).push_back(move(*tileNode));
	int pushIndex = (*vecOpen).size() - 1;
	while ((1))
	{
		//부모의 인덱스 연산
		int parentIndex = (pushIndex - 1)*0.5f;

		if ((*vecOpen)[parentIndex]->fCost <= (*vecOpen)[pushIndex]->fCost)
		{
			break;
		}
		//부모보다 현재 인덱스가 더 크면 바꿈
		if ((*vecOpen)[parentIndex]->fCost > (*vecOpen)[pushIndex]->fCost)
		{
			auto temp = move((*vecOpen)[parentIndex]);
			(*vecOpen)[parentIndex] = move((*vecOpen)[pushIndex]);
			(*vecOpen)[pushIndex] = move(temp);
			pushIndex = parentIndex;
		}
	}
}

void AStar::HeapMinSort(vector<unique_ptr<NODE>>* vecOpen)
{
	int minIndex = 0;
	while (1)
	{
		//왼쪽과 오른쪽 비교 해서 작은쪽으로 간다
		int RchildIndex = (minIndex * 2) + 2;
		int LchildIndex = (minIndex * 2) + 1;
		//사이즈 보다 인덱스 값이 크면 break;
		if (LchildIndex >= (*vecOpen).size())//RchildIndex >= mvecOpenList.size() ||
		{
			break;
		}
		//더 작은 쪽으로
		if (RchildIndex < (*vecOpen).size() &&
			LchildIndex < (*vecOpen).size() && (*vecOpen)[RchildIndex]->fCost < (*vecOpen)[LchildIndex]->fCost)
		{
			//맨위의 놈보다 밑에놈이 작다면
			if ((*vecOpen)[RchildIndex]->fCost < (*vecOpen)[minIndex]->fCost)
			{
				auto temp = move((*vecOpen)[RchildIndex]);
				(*vecOpen)[RchildIndex] = move((*vecOpen)[minIndex]);
				(*vecOpen)[minIndex] = move(temp);
				minIndex = RchildIndex;
			}
			else
			{
				break;
			}
		}
		else if (RchildIndex < (*vecOpen).size() &&
			LchildIndex < (*vecOpen).size() && (*vecOpen)[LchildIndex]->fCost < (*vecOpen)[RchildIndex]->fCost)
		{
			//맨위의 놈보다 밑에놈이 작다면
			if ((*vecOpen)[LchildIndex]->fCost < (*vecOpen)[minIndex]->fCost)
			{
				auto temp = move((*vecOpen)[LchildIndex]);
				(*vecOpen)[LchildIndex] = move((*vecOpen)[minIndex]);
				(*vecOpen)[minIndex] = move(temp);
				minIndex = LchildIndex;
			}
			else
			{
				break;
			}
		}
		else
		{
			//맨위의 놈보다 밑에놈이 작다면
			if ((*vecOpen)[LchildIndex]->fCost < (*vecOpen)[minIndex]->fCost)
			{
				auto temp = move((*vecOpen)[LchildIndex]);
				(*vecOpen)[LchildIndex] = move((*vecOpen)[minIndex]);
				(*vecOpen)[minIndex] = move(temp);
				minIndex = LchildIndex;
			}
			else
			{
				break;
			}
		}
	}
}

void AStar::Release()
{
	for (list<unique_ptr<NODE>>::iterator iter = mCloseList.begin();
		iter != mCloseList.end(); ++iter)
	{
		(*iter).reset();
		//mCloseList.erase(iter);
	}
	for (list<unique_ptr<NODE>>::iterator iter = mOpenList.begin();
		iter != mOpenList.end(); ++iter)
	{
		(*iter).reset();
	}
	for (vector<unique_ptr<NODE>>::iterator iter = mvecOpenList.begin();
		iter != mvecOpenList.end(); ++iter)
	{
		(*iter).reset();
	}
	mvecOpenList.clear();
	mCloseList.clear();
	mOpenList.clear();
	mBestList.clear();
	
}
