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

	offset.x = (int)ScrollMgr::Instance().GetScroll().x;
	offset.y = (int)ScrollMgr::Instance().GetScroll().y;

	vector<unique_ptr<TILE>> *pVecTile = SGAActorManager::Instance().GetTileInfo();


	//공격범위를 보여줄때
	if (mAtVisible)
	{
		AttackScope();
		mAtSeekScope = AttackScopeSeek();
		for (int x = 0; x < vecAtScopeIndex.size(); ++x)
		{
			Vector2 vTilePos = Vector2(
				(*pVecTile)[vecAtScopeIndex[x]]->vPos.x + JOJOTILESX / 2,
				(*pVecTile)[vecAtScopeIndex[x]]->vPos.y + JOJOTILESY / 2);
			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + vTilePos - offset, tint);
		}
		for (int x = 0; x < vecAtScopeIndex.size(); ++x)
		{
			(*pVecTile)[vecAtScopeIndex[x]]->AttackNum = 0;
		}
		vecAtScopeIndex.clear();
	}
	else
	{
		mAtSeekScope = false;
	}

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
	vecAtScopeIndex.push_back(GetTileIndex(mPosition));
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
							vecAtScopeIndex.push_back((i - 1) * (20) + j);
						}
						if (i + 1 < 20 && (*pVecTile)[(i + 1) * (20) + j]->AttackNum < mDis)
						{
							(*pVecTile)[(i + 1) * (20) + j]->AttackNum = mDis - 1;
							vecAtScopeIndex.push_back((i + 1) * (20) + j);
						}

						if (j + 1 < 20 && (*pVecTile)[i * (20) + (j + 1)]->AttackNum < mDis)
						{
							(*pVecTile)[i * (20) + (j + 1)]->AttackNum = mDis - 1;
							vecAtScopeIndex.push_back(i * (20) + (j + 1));
						}

						if (j - 1 >= 0 && (*pVecTile)[i * (20) + (j - 1)]->AttackNum < mDis)
						{
							(*pVecTile)[i * (20) + (j - 1)]->AttackNum = mDis - 1;
							vecAtScopeIndex.push_back(i * (20) + (j - 1));
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

	for (int i = 0; i < vecAtScopeIndex.size(); ++i)
	{
		Vector2 vec2ScopePos = (*pVecTile)[vecAtScopeIndex[i]]->vPos + XMFLOAT2(JOJOTILESX / 2, JOJOTILESY / 2);
		if (vec2ScopePos.x == vecMousePos.x && vec2ScopePos.y == vecMousePos.y)
			return true;
	}

	return false;
}
