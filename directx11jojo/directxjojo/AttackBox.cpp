#include "stdafx.h"
#include "AttackBox.h"


AttackBox::AttackBox()
{
}

AttackBox::AttackBox(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont)
	:MActor(pBatch, pSheet, pFont),
	mAtVisible(false),
	mMultiSkillAtVisible(false)
{
	
}


AttackBox::~AttackBox()
{

}

void AttackBox::Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible)
{

	Animation anim[] = {
		{ "AttackBox", 1,{ { "AttackBox0", 0.3f } }
		},
	{ "SkillBox", 1,{ { "skillArea", 0.3f } }
	},
	};
	mAtVisible = visible;
	MActor::Init(anim, 2, eSortID);
	SetPosition(pos);
	SetAnimation("AttackBox");
}

E_SCENE AttackBox::Update(float dt)
{
	MActor::Update(dt);
	//E_SCENE eResult = MActor::Update(dt);

	//if (eResult > E_SCENE_NONPASS)
	//	return eResult;

	return E_SCENE_NONPASS;
}

void AttackBox::Draw()
{
	XMFLOAT2 offset = XMFLOAT2(0, 0);
	Color tint = Colors::White;
	auto mouse = Mouse::Get().GetState();
	int mouseIndex = 0;
	int posIndex = 0;
	float fScrollx = ScrollMgr::Instance().GetScroll().x;
	float fScrolly = ScrollMgr::Instance().GetScroll().y;

	offset.x = (int)ScrollMgr::Instance().GetScroll().x;
	offset.y = (int)ScrollMgr::Instance().GetScroll().y;
	vector<unique_ptr<TILE>> *pVecTile = MActorManager::Instance().GetTileInfo();

	posIndex = GetTileIndex(mPosition);

	//���ݹ����� �����ٶ�
	if (mAtVisible)
	{
		Vector2 mousePos = Vector2(mouse.x + fScrollx, mouse.y + fScrolly);
		mouseIndex =GetTileIndex(mousePos);

		if (MFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
		{
			AttackScope();
		}
		//mAtSeekScope = AttackScopeSeek();
		SetAnimation("AttackBox");
		for (int x = 0; x < mspVecAtScopeIndex.size(); ++x)
		{
			auto a = *mspVecAtScopeIndex[x].get();
			Vector2 vTilePos = Vector2(
				(*pVecTile)[*mspVecAtScopeIndex[x].get()]->vPos.x + JOJOTILESX / 2,
				(*pVecTile)[*mspVecAtScopeIndex[x].get()]->vPos.y + JOJOTILESY / 2);
			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + vTilePos - offset, tint);
		}
		//SetAnimation("SkillBox");
		//for (size_t i = 0; i < mVecSkillAtScopeIndex.size(); i++)
		//{
		//	Vector2 vTilePos = Vector2(
		//		(*pVecTile)[mVecSkillAtScopeIndex[i]]->vPos.x + JOJOTILESX / 2,
		//		(*pVecTile)[mVecSkillAtScopeIndex[i]]->vPos.y + JOJOTILESY / 2);
		//	mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + vTilePos - offset, tint);
		//}

	}
	else //if(!MActorManager::Instance().GetUICheckArea() && mAtVisible== false && posIndex!=mouseIndex)
	{
		//Ÿ���� ��ġ ���� �ʱ�ȭ
		//(*pVecTile)[GetTileIndex(mPosition)]->AttackNum = 0;
		//for (int x = 0; x < mspVecAtScopeIndex.size(); ++x)
		//{
		//	(*pVecTile)[*mspVecAtScopeIndex[x].get()]->AttackNum = 0;
		//}
		//vecAtScopeIndex.clear();
		//Release();
		//mAtSeekScope = false;
	}

}

void AttackBox::OnHit(MActor * pCollidee)
{
	if (typeid(*pCollidee) == typeid(FotMan))
	{
//		int x = 1;
	}
}

void AttackBox::Release()
{
	vector<unique_ptr<TILE>> *pVecTile = MActorManager::Instance().GetTileInfo();

	//Ÿ���� ��ġ ���� �ʱ�ȭ
	(*pVecTile)[GetTileIndex(mPosition)]->AttackNum = 0;
	for (int x = 0; x < mspVecAtScopeIndex.size(); ++x)
	{
		(*pVecTile)[*mspVecAtScopeIndex[x].get()]->AttackNum = 0;
	}


	auto iter = mspVecAtScopeIndex.begin();
	while (iter != mspVecAtScopeIndex.end())
	{
		iter->reset();
		iter = mspVecAtScopeIndex.erase(iter);
	}

	mspVecAtScopeIndex.clear();

	//mVecSkillAtScopeIndex.clear();
}

bool AttackBox::UIntersecRectScope(MActor * pActor)
{

	RECT src = GetBoundScope();
	RECT trg = pActor->GetBound();
	RECT intersect;
	return	::IntersectRect(&intersect, &src, &trg);
}

bool AttackBox::AIIntersecRectScope(MActor * pActor)
{
	for (int i = 0; i < mspVecAtScopeIndex.size(); ++i)
	{
		RECT src = GetAtBoundScope(i);
		RECT trg = pActor->GetBound();
		trg.left += 10;
		trg.right -= 10;
		trg.top += 10;
		trg.bottom -= 10;
		RECT intersect;

		if (::IntersectRect(&intersect, &src, &trg) == true)
		{
			return true;
		}
	}

	return false;
}

void AttackBox::AttackScope()
{

		float fScrollx = ScrollMgr::Instance().GetScroll().x;
		float fScrolly = ScrollMgr::Instance().GetScroll().y;
		XMFLOAT2 f2Scroll = XMFLOAT2(ScrollMgr::Instance().GetScroll().x, ScrollMgr::Instance().GetScroll().y);
		auto mouse = Mouse::Get().GetState();
		XMFLOAT2 mMousePos = XMFLOAT2(mouse.x + fScrollx, mouse.y + fScrolly);

		vector<unique_ptr<TILE>> *pVecTile = MActorManager::Instance().GetTileInfo();

		//ĳ������ ��ġŸ�Ͽ� ���ݰŸ��� ����
		int posIndex = GetTileIndex(mPosition);
		int joTileCx = 20;
		int joTileCy = 20;
		(*pVecTile)[posIndex]->AttackNum = mAttackDistance;
		//���������Ұ� ���ٸ� ����
		if (mAttackDistance <= 0)
			return;
		//mspVecAtScopeIndex.push_back(make_unique<int>(posIndex));
		stack<int> stk;
		stk.push(posIndex);
		//ť��� ������ �ߺ������� �߻���

		//queue<int> que;
		//que.push(posIndex);
		//while (!que.empty())
		//{
		//	int top = que.front();
		//	que.pop();
		//	//��
		//	if (top >= joTileCx)
		//	{
		//		//������ Ÿ���� ������ ���� ������ ���ٸ� ���� �߰� ���ش�
		//		//���࿡ Ÿ�Ͼȿ� �������� �ƴϰ� ���� Ÿ�ϰ� ������ Ȯ��� Ÿ����
		//		//�������� ���۴ٸ� �� ����
		//		if ((*pVecTile)[top]->AttackNum != 0
		//			&& (*pVecTile)[top - joTileCx]->AttackNum == 0)
		//		{
		//			//���� �̹� ���� �ִµ� �ٲ�� �Ұ�� ���� �߰������� �ʴ´�
		//			//if ((*pVecTile)[top - joTileCx]->AttackNum == 0)
		//			{
		//				mspVecAtScopeIndex.push_back(make_unique<int>((top - joTileCx)));
		//			}
		//			(*pVecTile)[top - joTileCx]->AttackNum =
		//				(*pVecTile)[top]->AttackNum - 1;
		//			que.push(top - joTileCx);
		//		}
		//	}
		//	//�Ʒ�
		//	if (top < joTileCx*joTileCy - joTileCx)
		//	{
		//		//������ Ÿ���� ������ ���� ������ ���ٸ� ���� �߰� ���ش�
		//		if ((*pVecTile)[top]->AttackNum != 0
		//			&& (*pVecTile)[top + joTileCx]->AttackNum == 0)
		//		{
		//			//if ((*pVecTile)[top + joTileCx]->AttackNum == 0)
		//			{
		//				mspVecAtScopeIndex.push_back(make_unique<int>((top + joTileCx)));
		//			}
		//			(*pVecTile)[top + joTileCx]->AttackNum =
		//				(*pVecTile)[top]->AttackNum - 1;
		//			que.push((top)+joTileCx);
		//		}
		//	}
		//	//����
		//	if (top % joTileCx != 0)
		//	{
		//		//������ Ÿ���� ������ ���� ������ ���ٸ� ���� �߰� ���ش�
		//		if ((*pVecTile)[top]->AttackNum != 0
		//			&& (*pVecTile)[top - 1]->AttackNum == 0)
		//		{
		//			//if ((*pVecTile)[top - 1]->AttackNum == 0)
		//			{
		//				mspVecAtScopeIndex.push_back(make_unique<int>(top - 1));
		//			}
		//			(*pVecTile)[top - 1]->AttackNum =
		//				(*pVecTile)[top]->AttackNum - 1;
		//			que.push(top - 1);
		//		}
		//	}
		//	//������
		//	if (top % joTileCx != joTileCx - 1)
		//	{
		//		//������ Ÿ���� ������ ���� ������ ���ٸ� ���� �߰� ���ش�
		//		if ((*pVecTile)[top]->AttackNum != 0
		//			&& (*pVecTile)[top + 1]->AttackNum == 0)
		//		{
		//		//	if ((*pVecTile)[top + 1]->AttackNum == 0)
		//			{
		//				mspVecAtScopeIndex.push_back(make_unique<int>(top + 1));
		//			}
		//			(*pVecTile)[top + 1]->AttackNum =
		//				(*pVecTile)[top]->AttackNum - 1;
		//			que.push(top + 1);
		//		}
		//	}
		//}

		//���ù��
		while (!stk.empty())
		{
			int top = stk.top();
			stk.pop();
			//��
			if (top >= joTileCx)
			{
				//������ Ÿ���� ������ ���� ������ ���ٸ� ���� �߰� ���ش�
				//���࿡ Ÿ�Ͼȿ� �������� �ƴϰ� ���� Ÿ�ϰ� ������ Ȯ��� Ÿ����
				//�������� ���۴ٸ� �� ����
				if ((*pVecTile)[top]->AttackNum != 1
					&& (*pVecTile)[top - joTileCx]->AttackNum
					< (*pVecTile)[top]->AttackNum - 1)
				{
					//���� �̹� ���� �ִµ� �ٲ�� �Ұ�� ���� �߰������� �ʴ´�
					if ((*pVecTile)[top - joTileCx]->AttackNum == 0)
					{
						mspVecAtScopeIndex.push_back(make_unique<int>((top - joTileCx)));
					}
					(*pVecTile)[top - joTileCx]->AttackNum =
						(*pVecTile)[top]->AttackNum - 1;
					stk.push(top - joTileCx);
				}
			}
			//�Ʒ�
			if (top < joTileCx*joTileCy - joTileCx)
			{
				//������ Ÿ���� ������ ���� ������ ���ٸ� ���� �߰� ���ش�
				if ((*pVecTile)[top]->AttackNum != 1
					&& (*pVecTile)[top + joTileCx]->AttackNum 
					< (*pVecTile)[top]->AttackNum-1)
				{
					if ((*pVecTile)[top + joTileCx]->AttackNum == 0)
					{
						mspVecAtScopeIndex.push_back(make_unique<int>((top + joTileCx)));
					}
					(*pVecTile)[top + joTileCx]->AttackNum =
						(*pVecTile)[top]->AttackNum - 1;
					stk.push((top)+joTileCx);
				}
			}
			//����
			if (top % joTileCx != 0)
			{
				//������ Ÿ���� ������ ���� ������ ���ٸ� ���� �߰� ���ش�
				if ((*pVecTile)[top]->AttackNum != 1
					&& (*pVecTile)[top - 1]->AttackNum 
					< (*pVecTile)[top]->AttackNum - 1)
				{
					if ((*pVecTile)[top - 1]->AttackNum == 0)
					{
						mspVecAtScopeIndex.push_back(make_unique<int>(top - 1));
					}
					(*pVecTile)[top - 1]->AttackNum =
						(*pVecTile)[top]->AttackNum - 1;
					stk.push(top - 1);
				}
			}
			//������
			if (top % joTileCx != joTileCx - 1)
			{
				//������ Ÿ���� ������ ���� ������ ���ٸ� ���� �߰� ���ش�
				if ((*pVecTile)[top]->AttackNum != 1
					&& (*pVecTile)[top + 1]->AttackNum 
					< (*pVecTile)[top]->AttackNum - 1)
				{
					if ((*pVecTile)[top + 1]->AttackNum == 0)
					{
						mspVecAtScopeIndex.push_back(make_unique<int>(top + 1));
					}
					(*pVecTile)[top + 1]->AttackNum =
						(*pVecTile)[top]->AttackNum - 1;
					stk.push(top + 1);
				}
			}
		}

		//vecAtScopeIndex.push_back(GetTileIndex(mPosition));
		//vecAtScopeIndex2.push_back(make_unique<int>(GetTileIndex(mPosition)));
		//for (int mDis = mAttackDistance; mDis > 0; --mDis)
		//{
		//	for (int i = 0; i < 20; ++i)
		//	{
		//		for (int j = 0; j < 20; ++j)
		//		{
		//			if ((*pVecTile)[i * (20) + j]->AttackNum == mDis)
		//			{
		//				if ((*pVecTile)[i * (20) + j]->AttackNum > 0)
		//				{
		//					if (i - 1 >= 0 && (*pVecTile)[(i - 1) * (20) + j]->AttackNum < mDis)
		//					{
		//						(*pVecTile)[(i - 1) * (20) + j]->AttackNum = mDis - 1;
		//						//vecAtScopeIndex.push_back((i - 1) * (20) + j);
		//						mspVecAtScopeIndex.push_back(make_unique<int>((i - 1) * (20) + j));
		//					}
		//					if (i + 1 < 20 && (*pVecTile)[(i + 1) * (20) + j]->AttackNum < mDis)
		//					{
		//						(*pVecTile)[(i + 1) * (20) + j]->AttackNum = mDis - 1;
		//						//vecAtScopeIndex.push_back((i + 1) * (20) + j);
		//						mspVecAtScopeIndex.push_back(make_unique<int>((i + 1) * (20) + j));
		//						
		//					}
		//					if (j + 1 < 20 && (*pVecTile)[i * (20) + (j + 1)]->AttackNum < mDis)
		//					{
		//						(*pVecTile)[i * (20) + (j + 1)]->AttackNum = mDis - 1;
		//						//vecAtScopeIndex.push_back(i * (20) + (j + 1));
		//						mspVecAtScopeIndex.push_back(make_unique<int>(i * (20) + (j + 1)));
		//					}
		//					if (j - 1 >= 0 && (*pVecTile)[i * (20) + (j - 1)]->AttackNum < mDis)
		//					{
		//						(*pVecTile)[i * (20) + (j - 1)]->AttackNum = mDis - 1;
		//						//vecAtScopeIndex.push_back(i * (20) + (j - 1));
		//						mspVecAtScopeIndex.push_back(make_unique<int>(i * (20) + (j - 1)));
		//					}
		//				}
		//			}
		//		}
		//	}
		//}

		//Ÿ���� ��ġ ���� �ʱ�ȭ
		(*pVecTile)[GetTileIndex(mPosition)]->AttackNum = 0;
		for (int x = 0; x < mspVecAtScopeIndex.size(); ++x)
		{
			(*pVecTile)[*mspVecAtScopeIndex[x].get()]->AttackNum = 0;
		}
}

void AttackBox::AttackCubeScope(bool isChracterPospush,vector<unique_ptr<int>> &vecAtScopeIndex)
{
	vector<unique_ptr<TILE>> *pVecTile = MActorManager::Instance().GetTileInfo();
	int JoTileCx = 20;
	int JoTileCy = 20;
	//ĳ���� ������ �׸� ĭ�� ���Ϳ� �־���
	int tileOnPlayerIndex = GetTileIndex(mpCharacter->GetPosition());
	//ĳ���� ��ġ ���Կ���
	if (isChracterPospush)
	{
		//���;ȿ� �÷��̾ ���ִ� ��ġ�� �־���
		vecAtScopeIndex.push_back(make_unique<int>(tileOnPlayerIndex));
	}
	//8���� Ȯ���� ���� ���Ϳ� �־���
	//��
	if ((tileOnPlayerIndex) >= JoTileCx)
	{
		vecAtScopeIndex.push_back(make_unique<int>(tileOnPlayerIndex - JoTileCx));
	}
	//����
	if ((tileOnPlayerIndex) >= JoTileCx && tileOnPlayerIndex
		% JoTileCx != JoTileCx - 1)
	{
		vecAtScopeIndex.push_back(
			make_unique<int>(tileOnPlayerIndex - (JoTileCx - 1)));
	}
	//��
	if (tileOnPlayerIndex % JoTileCx != JoTileCx - 1)
	{
		vecAtScopeIndex.push_back(make_unique<int>(tileOnPlayerIndex + 1));
	}
	//����
	if ((tileOnPlayerIndex) < JoTileCx * JoTileCy - JoTileCx
		&& tileOnPlayerIndex % JoTileCx != JoTileCx - 1)
	{
		vecAtScopeIndex.push_back(
			make_unique<int>(tileOnPlayerIndex + (JoTileCx + 1)));
	}
	//�Ʒ�
	if ((tileOnPlayerIndex) < JoTileCx * JoTileCy - JoTileCx)
	{
		vecAtScopeIndex.push_back(make_unique<int>(tileOnPlayerIndex + JoTileCx));
	}
	//�޾�
	if ((tileOnPlayerIndex) < JoTileCx * JoTileCy - JoTileCx
		&& tileOnPlayerIndex % JoTileCx != 0)
	{
		vecAtScopeIndex.push_back(
			make_unique<int>(tileOnPlayerIndex + (JoTileCx - 1)));

	}
	//��
	if (tileOnPlayerIndex % JoTileCx != 0)
	{
		vecAtScopeIndex.push_back(
			make_unique<int>(tileOnPlayerIndex - 1));
	}
	//����
	if ((tileOnPlayerIndex) >= JoTileCx
		&& tileOnPlayerIndex % JoTileCx != 0)
	{
		vecAtScopeIndex.push_back(
			make_unique<int>(tileOnPlayerIndex - (JoTileCx + 1)));
	}

	//Ÿ���� ��ġ ���� �ʱ�ȭ
	(*pVecTile)[GetTileIndex(mPosition)]->AttackNum = 0;
	for (int x = 0; x < vecAtScopeIndex.size(); ++x)
	{
		(*pVecTile)[*vecAtScopeIndex[x].get()]->AttackNum = 0;
	}
}

bool AttackBox::SkillScope(vector<int> &vecAtSkillScopeIndex, int activeArea, int camp)
{
	//�������� �ʱ�ȭ
	vecAtSkillScopeIndex.clear();

	vector<unique_ptr<TILE>> *pVecTile = MActorManager::Instance().GetTileInfo();
	int JoTileCx = 20;
	int JoTileCy = 20;
	
	XMFLOAT2 f2Scrol = XMFLOAT2(ScrollMgr::Instance().GetScroll().x
		, ScrollMgr::Instance().GetScroll().y);
	auto mouse = Mouse::Get().GetState();
	//���� Ŭ���� ������ �ش� ������ ������ ĳ���� �ε��� ��ġ�� ���Ϳ� ����Ѵ�.
	int tileOnPlayerIndex = GetTileIndex(Vector2(mouse.x + f2Scrol.x, mouse.y + f2Scrol.y));
	vecAtSkillScopeIndex.push_back(tileOnPlayerIndex);
	//�������� ���������� �ٸ��������� ���� �ش� �ϴ� �����̴ٸ�
	switch (activeArea)
	{
	//����
	case 1:
	{
		//4���� Ȯ���� ���� ���Ϳ� �־���
		//��
		if ((tileOnPlayerIndex) >= JoTileCx)
		{
			//��Ÿ�Ͽ� ���� �ö�� �ִٸ麤�Ϳ� �־���
			if ((*pVecTile)[tileOnPlayerIndex - JoTileCx]->underObject == camp)
			{
				vecAtSkillScopeIndex.push_back((tileOnPlayerIndex - JoTileCx));
			}
		}
		//��
		if (tileOnPlayerIndex % JoTileCx != JoTileCx - 1)
		{
			if ((*pVecTile)[(tileOnPlayerIndex + 1)]->underObject == camp)
			{
				vecAtSkillScopeIndex.push_back((tileOnPlayerIndex + 1));
			}

		}
		//�Ʒ�
		if ((tileOnPlayerIndex) < JoTileCx * JoTileCy - JoTileCx)
		{
			if ((*pVecTile)[(tileOnPlayerIndex + JoTileCx)]->underObject == camp)
			{
				vecAtSkillScopeIndex.push_back((tileOnPlayerIndex + JoTileCx));
			}
		}
		//��
		if (tileOnPlayerIndex % JoTileCx != 0)
		{
			if ((*pVecTile)[(tileOnPlayerIndex - 1)]->underObject == camp)
			{
				vecAtSkillScopeIndex.push_back(
					(tileOnPlayerIndex - 1));
			}
		}

	}
	break;
	//������
	case 2:
	{
		//8���� Ȯ���� ���� ���Ϳ� �־���
		//��
		if ((tileOnPlayerIndex) >= JoTileCx)
		{
			//��Ÿ�Ͽ� ���� �ö�� �ִٸ麤�Ϳ� �־���
			if ((*pVecTile)[tileOnPlayerIndex - JoTileCx]->underObject == camp)
			{
				vecAtSkillScopeIndex.push_back((tileOnPlayerIndex - JoTileCx));
			}
		}
		//����
		if ((tileOnPlayerIndex) >= JoTileCx && tileOnPlayerIndex
			% JoTileCx != JoTileCx - 1)
		{
			if ((*pVecTile)[(tileOnPlayerIndex - (JoTileCx - 1))]->underObject == camp)
			{
				vecAtSkillScopeIndex.push_back(
					(tileOnPlayerIndex - (JoTileCx - 1)));
			}
		}
		//��
		if (tileOnPlayerIndex % JoTileCx != JoTileCx - 1)
		{
			if ((*pVecTile)[(tileOnPlayerIndex + 1)]->underObject == camp)
			{
				vecAtSkillScopeIndex.push_back((tileOnPlayerIndex + 1));
			}

		}
		//����
		if ((tileOnPlayerIndex) < JoTileCx * JoTileCy - JoTileCx
			&& tileOnPlayerIndex % JoTileCx != JoTileCx - 1)
		{
			if ((*pVecTile)[(tileOnPlayerIndex + (JoTileCx + 1))]->underObject == camp)
			{
				vecAtSkillScopeIndex.push_back(
					(tileOnPlayerIndex + (JoTileCx + 1)));
			}
		}
		//�Ʒ�
		if ((tileOnPlayerIndex) < JoTileCx * JoTileCy - JoTileCx)
		{
			if ((*pVecTile)[(tileOnPlayerIndex + JoTileCx)]->underObject == camp)
			{
				vecAtSkillScopeIndex.push_back((tileOnPlayerIndex + JoTileCx));
			}
		}
		//�޾�
		if ((tileOnPlayerIndex) < JoTileCx * JoTileCy - JoTileCx
			&& tileOnPlayerIndex % JoTileCx != 0)
		{
			if ((*pVecTile)[(tileOnPlayerIndex + (JoTileCx - 1))]->underObject == camp)
			{
				vecAtSkillScopeIndex.push_back(
					(tileOnPlayerIndex + (JoTileCx - 1)));
			}
			
		}
		//��
		if (tileOnPlayerIndex % JoTileCx != 0)
		{
			if ((*pVecTile)[(tileOnPlayerIndex - 1)]->underObject == camp)
			{
				vecAtSkillScopeIndex.push_back(
					(tileOnPlayerIndex - 1));
			}
		}
		//����
		if ((tileOnPlayerIndex) >= JoTileCx
			&& tileOnPlayerIndex % JoTileCx != 0)
		{
			if ((*pVecTile)[(tileOnPlayerIndex - (JoTileCx + 1))]->underObject == camp)
			{
				vecAtSkillScopeIndex.push_back(
					(tileOnPlayerIndex - (JoTileCx + 1)));

			}
		}

	}
	break;
	//��ü����
	case 4:
	{
		//Ÿ�������� �ϴ� �ҷ��ͼ� �ش� �༮�� ���Ϳ� �־��� �� ��ü ���� �� ��üȸ��
		for (size_t i = 0; i < (*pVecTile).size(); i++)
		{
			//�ش� Ÿ�Ͽ� ���̳� �츮���� �ö� �ִٸ� ���Ϳ� �־���
			if ((*pVecTile)[i]->underObject == camp)
			{
				vecAtSkillScopeIndex.push_back(i);
			}
		}
	}
	break;
	default:
		break;
	}

	//Ÿ���� ��ġ ���� �ʱ�ȭ
	//(*pVecTile)[GetTileIndex(mPosition)]->AttackNum = 0;
	//for (int x = 0; x < vecAtSkillScopeIndex.size(); ++x)
	//{
	//	(*pVecTile)[vecAtSkillScopeIndex[x]]->AttackNum = 0;
	//}
	return false;
}

//void AttackBox::SkillScopeAll(vector<int> &vecAtSkillScopeIndex, int camp)
//{
//	//�������� �ʱ�ȭ
//	vecAtSkillScopeIndex.clear();
//
//	vector<unique_ptr<TILE>> *pVecTile = MActorManager::Instance().GetTileInfo();
//	int JoTileCx = 20;
//	int JoTileCy = 20;
//
//	//Ÿ�������� �ϴ� �ҷ��ͼ� �ش� �༮�� ���Ϳ� �־��� �� ��ü ���� �� ��üȸ��
//	for (size_t i = 0; i < (*pVecTile).size(); i++)
//	{
//		//�ش� Ÿ�Ͽ� ���̳� �츮���� �ö� �ִٸ� ���Ϳ� �־���
//		if ((*pVecTile)[i]->underObject == camp)
//		{
//			vecAtSkillScopeIndex.push_back(i);
//		}
//	}
//}

bool AttackBox::AttackScopeSeek()
{
	float fScrollx = ScrollMgr::Instance().GetScroll().x;
	float fScrolly = ScrollMgr::Instance().GetScroll().y;
	auto mouse = Mouse::Get().GetState();
	vector<unique_ptr<TILE>> *pVecTile = MActorManager::Instance().GetTileInfo();

	XMFLOAT2 mmousePos = XMFLOAT2(mouse.x + fScrollx, mouse.y + fScrolly);

	int mouseIndex = GetTileIndex(mmousePos);

	Vector2 vecMousePos = (*pVecTile)[mouseIndex]->vPos + XMFLOAT2(JOJOTILESX / 2, JOJOTILESY / 2);

	for (int i = 0; i < mspVecAtScopeIndex.size(); ++i)
	{
		Vector2 vec2ScopePos = (*pVecTile)[*mspVecAtScopeIndex[i]]->vPos + XMFLOAT2(JOJOTILESX / 2, JOJOTILESY / 2);
		if (vec2ScopePos.x == vecMousePos.x && vec2ScopePos.y == vecMousePos.y)
			return true;
	}

	return false;
}

bool AttackBox::AttackScopeSeekPick(XMFLOAT2 pos)
{
	//float fScrollx = ScrollMgr::Instance().GetScroll().x;
	//float fScrolly = ScrollMgr::Instance().GetScroll().y;
	vector<unique_ptr<TILE>> *pVecTile = MActorManager::Instance().GetTileInfo();

	XMFLOAT2 Pos = XMFLOAT2(pos.x, pos.y);

	int posIndex = GetTileIndex(Pos);

	Vector2 vecPos = (*pVecTile)[posIndex]->vPos + XMFLOAT2(JOJOTILESX / 2, JOJOTILESY / 2);

	for (int i = 0; i < mspVecAtScopeIndex.size(); ++i)
	{
		Vector2 vec2ScopePos = (*pVecTile)[*mspVecAtScopeIndex[i]]->vPos + XMFLOAT2(JOJOTILESX / 2, JOJOTILESY / 2);
		if (vec2ScopePos.x == vecPos.x && vec2ScopePos.y == vecPos.y)
			return true;
	}

	return false;
}

RECT AttackBox::GetBoundScope()
{

	RECT rct;
	ZeroMemory(&rct, sizeof(RECT));
	float fScrollx = ScrollMgr::Instance().GetScroll().x;
	float fScrolly = ScrollMgr::Instance().GetScroll().y;
	vector<unique_ptr<TILE>> *pVecTile = MActorManager::Instance().GetTileInfo();
	auto mouse = Mouse::Get().GetState();
	XMFLOAT2 mmousePos = XMFLOAT2(mouse.x + fScrollx, mouse.y + fScrolly);
	int mouseIndex = GetTileIndex(mmousePos);
	Vector2 vec2MousePos = (*pVecTile)[mouseIndex]->vPos + XMFLOAT2(JOJOTILESX / 2, JOJOTILESY / 2);

	if (mAtSeekScope)
	{
		if (mpSpriteFrame != NULL)
		{
			rct = mpSpriteFrame->sourceRect;
			OffsetRect(&rct,
				static_cast<int>(vec2MousePos.x - mpSpriteFrame->pivot.x) - rct.left,
				static_cast<int>(vec2MousePos.y - mpSpriteFrame->pivot.y) - rct.top);
		}
	}
	return rct;
}

RECT AttackBox::GetAtBoundScope(int i)
{
	RECT rct;
	ZeroMemory(&rct, sizeof(RECT));
	float fScrollx = ScrollMgr::Instance().GetScroll().x;
	float fScrolly = ScrollMgr::Instance().GetScroll().y;
	vector<unique_ptr<TILE>> *pVecTile = MActorManager::Instance().GetTileInfo();

	Vector2 vec2ScopePos = (*pVecTile)[*mspVecAtScopeIndex[i]]->vPos + XMFLOAT2(JOJOTILESX / 2, JOJOTILESY / 2);

	if (mpSpriteFrame != NULL)
	{
		rct = mpSpriteFrame->sourceRect;
		OffsetRect(&rct,
			static_cast<int>(vec2ScopePos.x - mpSpriteFrame->pivot.x) - rct.left,
			static_cast<int>(vec2ScopePos.y - mpSpriteFrame->pivot.y) - rct.top);
	}
	return rct;
}
