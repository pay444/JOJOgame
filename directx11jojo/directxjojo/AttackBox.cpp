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

	//공격범위를 보여줄때
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
		//타일의 위치 측정 초기화
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

	//타일의 위치 측정 초기화
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

		//캐릭터의 위치타일에 공격거리를 넣음
		int posIndex = GetTileIndex(mPosition);
		int joTileCx = 20;
		int joTileCy = 20;
		(*pVecTile)[posIndex]->AttackNum = mAttackDistance;
		//범위측정할게 없다면 리턴
		if (mAttackDistance <= 0)
			return;
		//mspVecAtScopeIndex.push_back(make_unique<int>(posIndex));
		stack<int> stk;
		stk.push(posIndex);
		//큐방식 하지만 중복문제는 발생함

		//queue<int> que;
		//que.push(posIndex);
		//while (!que.empty())
		//{
		//	int top = que.front();
		//	que.pop();
		//	//위
		//	if (top >= joTileCx)
		//	{
		//		//방향의 타일이 그전에 받은 영향이 없다면 값을 추가 해준다
		//		//만약에 타일안에 범위값이 아니고 현재 타일과 범위가 확장될 타일의
		//		//범위값이 더작다면 값 적용
		//		if ((*pVecTile)[top]->AttackNum != 0
		//			&& (*pVecTile)[top - joTileCx]->AttackNum == 0)
		//		{
		//			//만약 이미 값이 있는데 바꿔야 할경우 값을 추가하지는 않는다
		//			//if ((*pVecTile)[top - joTileCx]->AttackNum == 0)
		//			{
		//				mspVecAtScopeIndex.push_back(make_unique<int>((top - joTileCx)));
		//			}
		//			(*pVecTile)[top - joTileCx]->AttackNum =
		//				(*pVecTile)[top]->AttackNum - 1;
		//			que.push(top - joTileCx);
		//		}
		//	}
		//	//아래
		//	if (top < joTileCx*joTileCy - joTileCx)
		//	{
		//		//방향의 타일이 그전에 받은 영향이 없다면 값을 추가 해준다
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
		//	//왼쪽
		//	if (top % joTileCx != 0)
		//	{
		//		//방향의 타일이 그전에 받은 영향이 없다면 값을 추가 해준다
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
		//	//오른쪽
		//	if (top % joTileCx != joTileCx - 1)
		//	{
		//		//방향의 타일이 그전에 받은 영향이 없다면 값을 추가 해준다
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

		//스택방식
		while (!stk.empty())
		{
			int top = stk.top();
			stk.pop();
			//위
			if (top >= joTileCx)
			{
				//방향의 타일이 그전에 받은 영향이 없다면 값을 추가 해준다
				//만약에 타일안에 범위값이 아니고 현재 타일과 범위가 확장될 타일의
				//범위값이 더작다면 값 적용
				if ((*pVecTile)[top]->AttackNum != 1
					&& (*pVecTile)[top - joTileCx]->AttackNum
					< (*pVecTile)[top]->AttackNum - 1)
				{
					//만약 이미 값이 있는데 바꿔야 할경우 값을 추가하지는 않는다
					if ((*pVecTile)[top - joTileCx]->AttackNum == 0)
					{
						mspVecAtScopeIndex.push_back(make_unique<int>((top - joTileCx)));
					}
					(*pVecTile)[top - joTileCx]->AttackNum =
						(*pVecTile)[top]->AttackNum - 1;
					stk.push(top - joTileCx);
				}
			}
			//아래
			if (top < joTileCx*joTileCy - joTileCx)
			{
				//방향의 타일이 그전에 받은 영향이 없다면 값을 추가 해준다
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
			//왼쪽
			if (top % joTileCx != 0)
			{
				//방향의 타일이 그전에 받은 영향이 없다면 값을 추가 해준다
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
			//오른쪽
			if (top % joTileCx != joTileCx - 1)
			{
				//방향의 타일이 그전에 받은 영향이 없다면 값을 추가 해준다
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

		//타일의 위치 측정 초기화
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
	//캐릭터 주위의 네모난 칸만 벡터에 넣어줌
	int tileOnPlayerIndex = GetTileIndex(mpCharacter->GetPosition());
	//캐릭터 위치 포함여부
	if (isChracterPospush)
	{
		//벡터안에 플레이어가 서있는 위치를 넣어줌
		vecAtScopeIndex.push_back(make_unique<int>(tileOnPlayerIndex));
	}
	//8방향 확인후 범위 벡터에 넣어줌
	//위
	if ((tileOnPlayerIndex) >= JoTileCx)
	{
		vecAtScopeIndex.push_back(make_unique<int>(tileOnPlayerIndex - JoTileCx));
	}
	//오위
	if ((tileOnPlayerIndex) >= JoTileCx && tileOnPlayerIndex
		% JoTileCx != JoTileCx - 1)
	{
		vecAtScopeIndex.push_back(
			make_unique<int>(tileOnPlayerIndex - (JoTileCx - 1)));
	}
	//오
	if (tileOnPlayerIndex % JoTileCx != JoTileCx - 1)
	{
		vecAtScopeIndex.push_back(make_unique<int>(tileOnPlayerIndex + 1));
	}
	//오아
	if ((tileOnPlayerIndex) < JoTileCx * JoTileCy - JoTileCx
		&& tileOnPlayerIndex % JoTileCx != JoTileCx - 1)
	{
		vecAtScopeIndex.push_back(
			make_unique<int>(tileOnPlayerIndex + (JoTileCx + 1)));
	}
	//아래
	if ((tileOnPlayerIndex) < JoTileCx * JoTileCy - JoTileCx)
	{
		vecAtScopeIndex.push_back(make_unique<int>(tileOnPlayerIndex + JoTileCx));
	}
	//왼아
	if ((tileOnPlayerIndex) < JoTileCx * JoTileCy - JoTileCx
		&& tileOnPlayerIndex % JoTileCx != 0)
	{
		vecAtScopeIndex.push_back(
			make_unique<int>(tileOnPlayerIndex + (JoTileCx - 1)));

	}
	//왼
	if (tileOnPlayerIndex % JoTileCx != 0)
	{
		vecAtScopeIndex.push_back(
			make_unique<int>(tileOnPlayerIndex - 1));
	}
	//왼위
	if ((tileOnPlayerIndex) >= JoTileCx
		&& tileOnPlayerIndex % JoTileCx != 0)
	{
		vecAtScopeIndex.push_back(
			make_unique<int>(tileOnPlayerIndex - (JoTileCx + 1)));
	}

	//타일의 위치 측정 초기화
	(*pVecTile)[GetTileIndex(mPosition)]->AttackNum = 0;
	for (int x = 0; x < vecAtScopeIndex.size(); ++x)
	{
		(*pVecTile)[*vecAtScopeIndex[x].get()]->AttackNum = 0;
	}
}

bool AttackBox::SkillScope(vector<int> &vecAtSkillScopeIndex, int activeArea, int camp)
{
	//측정전에 초기화
	vecAtSkillScopeIndex.clear();

	vector<unique_ptr<TILE>> *pVecTile = MActorManager::Instance().GetTileInfo();
	int JoTileCx = 20;
	int JoTileCy = 20;
	
	XMFLOAT2 f2Scrol = XMFLOAT2(ScrollMgr::Instance().GetScroll().x
		, ScrollMgr::Instance().GetScroll().y);
	auto mouse = Mouse::Get().GetState();
	//내가 클릭한 주위의 해당 범위에 들어오는 캐릭터 인덱스 위치만 벡터에 기록한다.
	int tileOnPlayerIndex = GetTileIndex(Vector2(mouse.x + f2Scrol.x, mouse.y + f2Scrol.y));
	vecAtSkillScopeIndex.push_back(tileOnPlayerIndex);
	//십자인지 마방진인지 다른것인지에 따른 해당 하는 로직이다름
	switch (activeArea)
	{
	//십자
	case 1:
	{
		//4방향 확인후 범위 벡터에 넣어줌
		//위
		if ((tileOnPlayerIndex) >= JoTileCx)
		{
			//그타일에 적이 올라와 있다면벡터에 넣어줌
			if ((*pVecTile)[tileOnPlayerIndex - JoTileCx]->underObject == camp)
			{
				vecAtSkillScopeIndex.push_back((tileOnPlayerIndex - JoTileCx));
			}
		}
		//오
		if (tileOnPlayerIndex % JoTileCx != JoTileCx - 1)
		{
			if ((*pVecTile)[(tileOnPlayerIndex + 1)]->underObject == camp)
			{
				vecAtSkillScopeIndex.push_back((tileOnPlayerIndex + 1));
			}

		}
		//아래
		if ((tileOnPlayerIndex) < JoTileCx * JoTileCy - JoTileCx)
		{
			if ((*pVecTile)[(tileOnPlayerIndex + JoTileCx)]->underObject == camp)
			{
				vecAtSkillScopeIndex.push_back((tileOnPlayerIndex + JoTileCx));
			}
		}
		//왼
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
	//마방진
	case 2:
	{
		//8방향 확인후 범위 벡터에 넣어줌
		//위
		if ((tileOnPlayerIndex) >= JoTileCx)
		{
			//그타일에 적이 올라와 있다면벡터에 넣어줌
			if ((*pVecTile)[tileOnPlayerIndex - JoTileCx]->underObject == camp)
			{
				vecAtSkillScopeIndex.push_back((tileOnPlayerIndex - JoTileCx));
			}
		}
		//오위
		if ((tileOnPlayerIndex) >= JoTileCx && tileOnPlayerIndex
			% JoTileCx != JoTileCx - 1)
		{
			if ((*pVecTile)[(tileOnPlayerIndex - (JoTileCx - 1))]->underObject == camp)
			{
				vecAtSkillScopeIndex.push_back(
					(tileOnPlayerIndex - (JoTileCx - 1)));
			}
		}
		//오
		if (tileOnPlayerIndex % JoTileCx != JoTileCx - 1)
		{
			if ((*pVecTile)[(tileOnPlayerIndex + 1)]->underObject == camp)
			{
				vecAtSkillScopeIndex.push_back((tileOnPlayerIndex + 1));
			}

		}
		//오아
		if ((tileOnPlayerIndex) < JoTileCx * JoTileCy - JoTileCx
			&& tileOnPlayerIndex % JoTileCx != JoTileCx - 1)
		{
			if ((*pVecTile)[(tileOnPlayerIndex + (JoTileCx + 1))]->underObject == camp)
			{
				vecAtSkillScopeIndex.push_back(
					(tileOnPlayerIndex + (JoTileCx + 1)));
			}
		}
		//아래
		if ((tileOnPlayerIndex) < JoTileCx * JoTileCy - JoTileCx)
		{
			if ((*pVecTile)[(tileOnPlayerIndex + JoTileCx)]->underObject == camp)
			{
				vecAtSkillScopeIndex.push_back((tileOnPlayerIndex + JoTileCx));
			}
		}
		//왼아
		if ((tileOnPlayerIndex) < JoTileCx * JoTileCy - JoTileCx
			&& tileOnPlayerIndex % JoTileCx != 0)
		{
			if ((*pVecTile)[(tileOnPlayerIndex + (JoTileCx - 1))]->underObject == camp)
			{
				vecAtSkillScopeIndex.push_back(
					(tileOnPlayerIndex + (JoTileCx - 1)));
			}
			
		}
		//왼
		if (tileOnPlayerIndex % JoTileCx != 0)
		{
			if ((*pVecTile)[(tileOnPlayerIndex - 1)]->underObject == camp)
			{
				vecAtSkillScopeIndex.push_back(
					(tileOnPlayerIndex - 1));
			}
		}
		//왼위
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
	//전체범위
	case 4:
	{
		//타일정보를 싹다 불러와서 해당 녀석을 벡터에 넣어줌 즉 전체 공격 및 전체회복
		for (size_t i = 0; i < (*pVecTile).size(); i++)
		{
			//해당 타일에 적이나 우리편이 올라가 있다면 벡터에 넣어줌
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

	//타일의 위치 측정 초기화
	//(*pVecTile)[GetTileIndex(mPosition)]->AttackNum = 0;
	//for (int x = 0; x < vecAtSkillScopeIndex.size(); ++x)
	//{
	//	(*pVecTile)[vecAtSkillScopeIndex[x]]->AttackNum = 0;
	//}
	return false;
}

//void AttackBox::SkillScopeAll(vector<int> &vecAtSkillScopeIndex, int camp)
//{
//	//측정전에 초기화
//	vecAtSkillScopeIndex.clear();
//
//	vector<unique_ptr<TILE>> *pVecTile = MActorManager::Instance().GetTileInfo();
//	int JoTileCx = 20;
//	int JoTileCy = 20;
//
//	//타일정보를 싹다 불러와서 해당 녀석을 벡터에 넣어줌 즉 전체 공격 및 전체회복
//	for (size_t i = 0; i < (*pVecTile).size(); i++)
//	{
//		//해당 타일에 적이나 우리편이 올라가 있다면 벡터에 넣어줌
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
