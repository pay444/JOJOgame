#include "stdafx.h"
#include "AttackBox.h"


AttackBox::AttackBox()
{
}

AttackBox::AttackBox(SpriteBatch * pBatch, SGASpriteSheet * pSheet, SpriteFont * pFont)
	:SGAActor(pBatch, pSheet, pFont),
	mAtVisible(false)
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
	};
	mAtVisible = visible;
	SGAActor::Init(anim, 1, eSortID);
	SetPosition(pos);
	SetAnimation("AttackBox");
}

E_SCENE AttackBox::Update(float dt)
{
	E_SCENE eResult = SGAActor::Update(dt);

	if (eResult > E_SCENE_NONPASS)
		return eResult;

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
	vector<unique_ptr<TILE>> *pVecTile = SGAActorManager::Instance().GetTileInfo();

	posIndex = GetTileIndex(mPosition);

	//공격범위를 보여줄때
	if (mAtVisible)
	{
		Vector2 mousePos = Vector2(mouse.x + fScrollx, mouse.y + fScrolly);
		mouseIndex =GetTileIndex(mousePos);

		if (SGAFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
		{
			AttackScope();
		}
		//mAtSeekScope = AttackScopeSeek();
		for (int x = 0; x < mspVecAtScopeIndex.size(); ++x)
		{
			auto a = *mspVecAtScopeIndex[x].get();
			Vector2 vTilePos = Vector2(
				(*pVecTile)[*mspVecAtScopeIndex[x].get()]->vPos.x + JOJOTILESX / 2,
				(*pVecTile)[*mspVecAtScopeIndex[x].get()]->vPos.y + JOJOTILESY / 2);
			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + vTilePos - offset, tint);
		}
		//타일의 위치 측정 초기화
		(*pVecTile)[GetTileIndex(mPosition)]->AttackNum = 0;
		for (int x = 0; x < mspVecAtScopeIndex.size(); ++x)
		{
			(*pVecTile)[*mspVecAtScopeIndex[x].get()]->AttackNum = 0;
		}
	}
	else //if(!SGAActorManager::Instance().GetUICheckArea() && mAtVisible== false && posIndex!=mouseIndex)
	{
		//vecAtScopeIndex.clear();
		//Release();
		//mAtSeekScope = false;
	}

}

void AttackBox::OnHit(SGAActor * pCollidee)
{
	if (typeid(*pCollidee) == typeid(FotMan))
	{
//		int x = 1;
	}
}

void AttackBox::Release()
{
	mVecAtScopeIndex.clear();
	auto iter = mspVecAtScopeIndex.begin();
	while (iter != mspVecAtScopeIndex.end())
	{
		iter->reset();
		iter = mspVecAtScopeIndex.erase(iter);
	}
}

bool AttackBox::UIntersecRectScope(SGAActor * pActor)
{

	RECT src = GetBoundScope();
	RECT trg = pActor->GetBound();
	RECT intersect;
	return	::IntersectRect(&intersect, &src, &trg);
}

bool AttackBox::AIIntersecRectScope(SGAActor * pActor)
{
	for (int i = 0; i < mspVecAtScopeIndex.size(); ++i)
	{
		RECT src = GetAtBoundScope(i);
		RECT trg = pActor->GetBound();
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
		auto mouse = Mouse::Get().GetState();
		XMFLOAT2 mMousePos = XMFLOAT2(mouse.x + fScrollx, mouse.y + fScrolly);

		vector<unique_ptr<TILE>> *pVecTile = SGAActorManager::Instance().GetTileInfo();

		//케릭터의 위치타일에 공격거리를 넣음
		(*pVecTile)[GetTileIndex(mPosition)]->AttackNum = mAttackDistance;
		//vecAtScopeIndex.push_back(GetTileIndex(mPosition));
		//vecAtScopeIndex2.push_back(make_unique<int>(GetTileIndex(mPosition)));
		for (int mDis = mAttackDistance; mDis > 0; --mDis)
		{
			for (int i = 0; i < 20; ++i)
			{
				for (int j = 0; j < 20; ++j)
				{
					if ((*pVecTile)[i * (20) + j]->AttackNum == mDis)
					{
						if ((*pVecTile)[i * (20) + j]->AttackNum > 0)
						{
							if (i - 1 >= 0 && (*pVecTile)[(i - 1) * (20) + j]->AttackNum < mDis)
							{
								(*pVecTile)[(i - 1) * (20) + j]->AttackNum = mDis - 1;
								//vecAtScopeIndex.push_back((i - 1) * (20) + j);
								mspVecAtScopeIndex.push_back(make_unique<int>((i - 1) * (20) + j));
							}
							if (i + 1 < 20 && (*pVecTile)[(i + 1) * (20) + j]->AttackNum < mDis)
							{
								(*pVecTile)[(i + 1) * (20) + j]->AttackNum = mDis - 1;
								//vecAtScopeIndex.push_back((i + 1) * (20) + j);
								mspVecAtScopeIndex.push_back(make_unique<int>((i + 1) * (20) + j));
								
							}

							if (j + 1 < 20 && (*pVecTile)[i * (20) + (j + 1)]->AttackNum < mDis)
							{
								(*pVecTile)[i * (20) + (j + 1)]->AttackNum = mDis - 1;
								//vecAtScopeIndex.push_back(i * (20) + (j + 1));
								mspVecAtScopeIndex.push_back(make_unique<int>(i * (20) + (j + 1)));
							}

							if (j - 1 >= 0 && (*pVecTile)[i * (20) + (j - 1)]->AttackNum < mDis)
							{
								(*pVecTile)[i * (20) + (j - 1)]->AttackNum = mDis - 1;
								//vecAtScopeIndex.push_back(i * (20) + (j - 1));
								mspVecAtScopeIndex.push_back(make_unique<int>(i * (20) + (j - 1)));
							}


						}
					}
				}
			}
		}

	
}

bool AttackBox::AttackScopeSeek()
{
	float fScrollx = ScrollMgr::Instance().GetScroll().x;
	float fScrolly = ScrollMgr::Instance().GetScroll().y;
	auto mouse = Mouse::Get().GetState();
	vector<unique_ptr<TILE>> *pVecTile = SGAActorManager::Instance().GetTileInfo();

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

RECT AttackBox::GetBoundScope()
{

	RECT rct;
	ZeroMemory(&rct, sizeof(RECT));
	float fScrollx = ScrollMgr::Instance().GetScroll().x;
	float fScrolly = ScrollMgr::Instance().GetScroll().y;
	vector<unique_ptr<TILE>> *pVecTile = SGAActorManager::Instance().GetTileInfo();
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
	vector<unique_ptr<TILE>> *pVecTile = SGAActorManager::Instance().GetTileInfo();

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
