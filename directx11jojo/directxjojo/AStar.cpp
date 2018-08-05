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

	// ���̶��..
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
	//// �ڽ�Ʈ ����� ���ؼ�,
	//// ������ ���� m_BestList �ε��������� ��´�.///
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

	while (true)	// Ż������ ? ��尡 �������� �����Ҷ�����...
	{
		int JOJOCX = 20;
		int JOJOCY = 20;
		/*
		if(�����ϴ� Ÿ���̳�?,
		���� �ִ� Ÿ���̳�?,
		�̹������߰ų�,
		���¸���Ʈ�� �߰����ִ� �༮ ����)
		*/
		// ��
		iIndex = pParent2->iIndex - JOJOCX;

		if (pParent2->iIndex >= JOJOCX &&		// �� �����̾ƴ϶��,
			(*pVecTile)[iIndex]->byOption == 0 &&	// ���� �ִ� Ÿ��?
			this->CheckList(iIndex))
		{
			unique_ptr<NODE> pNode = nullptr;
			pNode = move(MakeNode(iIndex, pParent2, pVecTile));
			mOpenList.push_back(move(pNode));
		}
		// ������ ��
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
		// ������
		iIndex = pParent2->iIndex + 1;
		if (pParent2->iIndex % JOJOCX != JOJOCX - 1 &&
			(*pVecTile)[iIndex]->byOption == 0 &&
			CheckList(iIndex))
		{
			unique_ptr<NODE> pNode = nullptr;
			pNode = move(MakeNode(iIndex, pParent2, pVecTile));
			mOpenList.push_back(move(pNode));
		}

		// ������ �Ʒ�
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
		// �Ʒ�
		iIndex = pParent2->iIndex + JOJOCX;
		if (pParent2->iIndex < JOJOCX * JOJOCY - JOJOCX &&
			(*pVecTile)[iIndex]->byOption == 0 &&
			CheckList(iIndex))
		{
			unique_ptr<NODE> pNode = nullptr;
			pNode = move(MakeNode(iIndex, pParent2, pVecTile));
			mOpenList.push_back(move(pNode));
		}
		// ���� �Ʒ�
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

		// ����
		iIndex = pParent2->iIndex - 1;
		if (pParent2->iIndex % (JOJOCX) != 0 &&
			(*pVecTile)[iIndex]->byOption == 0 &&
			CheckList(iIndex))
		{
			unique_ptr<NODE> pNode = nullptr;
			pNode = move(MakeNode(iIndex, pParent2, pVecTile));
			mOpenList.push_back(move(pNode));
		}
		// ���� ��
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

		// Parent�� ���� 8���� ���簡 ���� ����,
		// 8���� ����߿� �� �� �ִ� ��带 OpenList�� �־�ξ���.
		// �� �߿��� �ڽ�Ʈ�� ���� ���� ����. -> �������� ����
		// Sort(�Լ�������);
		mOpenList.sort(Compare);

		list<unique_ptr<NODE>>::iterator iter = mOpenList.begin();
		//pParent->fCost = ((*iter).get())->fCost;
		//pParent->iIndex = ((*iter).get())->iIndex;
		//pParent->pParent = ((*iter).get())->pParent;

		if (mOpenList.empty())
			return;

		pParent2 = (*iter).get();

		// ���� ���� �ڽ�Ʈ�� ���� (*iter)�� CloseList�� ��Ƽ� �����Ѵ�.
		mCloseList.push_back(move(*iter));

		iter = mOpenList.erase(iter);

		// Ż�� !
		if (pParent2->iIndex == miGoalIdx)
		{
			while (true)
			{
				mBestList.push_back(pParent2->iIndex);
				pParent2 = pParent2->pParent;
				
				if (pParent2->iIndex == miStartIdx)
					break;
			}

			// �����̳ʸ� �����´�.(���� Goal -> Start)
			mBestList.reverse();
			
			break;	// MakeRoute while�� Ż�� !
		}
	}
}

void AStar::VecMakeRoute()
{
	//// �ڽ�Ʈ ����� ���ؼ�,
	//// ������ ���� m_BestList �ε��������� ��´�.///
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

	while (true)	// Ż������ ? ��尡 �������� �����Ҷ�����...
	{
		int JOJOCX = 20;
		int JOJOCY = 20;
		/*
		if(�����ϴ� Ÿ���̳�?,
		���� �ִ� Ÿ���̳�?,
		�̹������߰ų�,
		���¸���Ʈ�� �߰����ִ� �༮ ����)
		*/
		// ��
		iIndex = pParent2->iIndex - JOJOCX;
		auto moveBox = MActorManager::Instance().GetClassMoveBox();
		auto pCharacter = moveBox->GetCharacter();
		if (pParent2->iIndex >= JOJOCX &&		// �� �����̾ƴ϶��,
			((*pVecTile)[iIndex]->byOption == 0 || 
			(*pVecTile)[iIndex]->underObject == pCharacter->GetCamp())&&	// ���� �ִ� Ÿ��?
			this->CheckList(iIndex))
		{
			unique_ptr<NODE> pNode = nullptr;
			pNode = move(MakeNode(iIndex, pParent2, pVecTile));

			HeapInsert(&mvecOpenList, &pNode);
			//mOpenList.push_back(move(pNode));
		}
		// ������ ��
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
		// ������
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

		// ������ �Ʒ�
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
		// �Ʒ�
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
		// ���� �Ʒ�
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

		// ����
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
		// ���� ��
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

		// Parent�� ���� 8���� ���簡 ���� ����,
		// 8���� ����߿� �� �� �ִ� ��带 OpenList�� �־�ξ���.
		// �� �߿��� �ڽ�Ʈ�� ���� ���� ����. -> �������� ����
		// Sort(�Լ�������);
		//mOpenList.sort(Compare);

		//Heap������ �̿��� ���
		vector<unique_ptr<NODE>>::iterator iter = mvecOpenList.begin();
		//auto a = ; == 27;

		//list<unique_ptr<NODE>>::iterator iter = mOpenList.begin();
		//pParent->fCost = ((*iter).get())->fCost;
		//pParent->iIndex = ((*iter).get())->iIndex;
		//pParent->pParent = ((*iter).get())->pParent;

		if (mvecOpenList.empty())
			return;

		pParent2 = (*iter).get();

		// ���� ���� �ڽ�Ʈ�� ���� (*iter)�� CloseList�� ��Ƽ� �����Ѵ�.
		mCloseList.push_back(move(*iter));

		mvecOpenList[0] = move(mvecOpenList.back());
		mvecOpenList.pop_back();

		HeapMinSort(&mvecOpenList);

		// Ż�� !
		if (pParent2->iIndex == miGoalIdx)
		{
			while (true)
			{
				mBestList.push_back(pParent2->iIndex);
				pParent2 = pParent2->pParent;

				if (pParent2->iIndex == miStartIdx)
					break;
			}

			// �����̳ʸ� �����´�.(���� Goal -> Start)
			mBestList.reverse();

			break;	// MakeRoute while�� Ż�� !
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

	// �θ� �������� �Ÿ�(���)
	Vector2 vDir = (*pVecTile)[index_]->vPos -
		(*pVecTile)[pParent_->iIndex]->vPos;
	pNode->gCost = pParent_->gCost + vDir.Length();

	//XMFLOAT2 float2 = {XMVectorGetX(vDir),XMVectorGetY(vDir)};
	
	//float fPCost = XMVector2Length(&vDir);
	float fPCost = pNode->gCost; //sqrt(pow(float2.x, 2) + pow(float2.y, 2));

	// ����(������) �������� �Ÿ�(���)
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
			//�����ͺ��� ���θ������ �ڽ�Ʈ�� ������ ����
			if ((*iter)->fCost > (*tileNode)->fCost)
			{
				iter = mvecOpenList.erase(iter);
				mvecOpenList.insert(iter, move(*tileNode));

				return;
			}
			//�ƴϸ� ����
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
		//�θ��� �ε��� ����
		int parentIndex = (pushIndex - 1)*0.5f;

		if ((*vecOpen)[parentIndex]->fCost <= (*vecOpen)[pushIndex]->fCost)
		{
			break;
		}
		//�θ𺸴� ���� �ε����� �� ũ�� �ٲ�
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
		//���ʰ� ������ �� �ؼ� ���������� ����
		int RchildIndex = (minIndex * 2) + 2;
		int LchildIndex = (minIndex * 2) + 1;
		//������ ���� �ε��� ���� ũ�� break;
		if (LchildIndex >= (*vecOpen).size())//RchildIndex >= mvecOpenList.size() ||
		{
			break;
		}
		//�� ���� ������
		if (RchildIndex < (*vecOpen).size() &&
			LchildIndex < (*vecOpen).size() && (*vecOpen)[RchildIndex]->fCost < (*vecOpen)[LchildIndex]->fCost)
		{
			//������ �𺸴� �ؿ����� �۴ٸ�
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
			//������ �𺸴� �ؿ����� �۴ٸ�
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
			//������ �𺸴� �ؿ����� �۴ٸ�
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
