#include "stdafx.h"
//#include "MActor.h"
#include "MoveBox.h"


MoveBox::MoveBox() :MActor(),
mSeekScope(false)
{
	mAnimations.clear();
}

MoveBox::MoveBox(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont)
	:MActor(pBatch, pSheet, pFont),
	mSeekScope(false),
	mpPlayer(nullptr)
{

}


MoveBox::~MoveBox()
{
}

void MoveBox::Init(E_SORTID eSortID, XMFLOAT2 pos, float limitDis, bool visible)
{
	//MoveBox(pBatch, pSheet, pFont);

	//mpBatch = pBatch;
	//mpSheet = pSheet;
	//mpFont = pFont;

	Animation anim[] = {
		{ "CursorBox", 2,{ { "CursorBox0", 0.3f },
		{ "CursorBox1", 0.3f }, }
		},
	};
	//한계거리
	mLimitDis = limitDis;
	//이동거리
	//mMoveDistance = moveDis;
	//보이고 안보이고
	mVisible = visible;
	MActor::Init(anim, 1, eSortID);
	SetPosition(pos);
	SetAnimation("CursorBox");

}

void MoveBox::Init(SpriteBatch* pBatch, SpriteSheet *pSheet, SpriteFont *pFont, E_SORTID eSortID, XMFLOAT2 pos, float limitDis, bool visible)
{
	MActor();

	//mAnimations.clear();

	mpBatch = pBatch;
	mpSheet = pSheet;
	mpFont = pFont;

	Animation anim[] = {
		{ "CursorBox", 2,{ { "CursorBox0", 0.3f },
		{ "CursorBox1", 0.3f }, }
		},
	};
	//한계거리
	mLimitDis = limitDis;
	//이동거리
	//mMoveDistance = moveDis;
	//보이고 안보이고
	mVisible = visible;
	MActor::Init(anim, 1, eSortID);
	SetPosition(pos);
	SetAnimation("CursorBox");
}

E_SCENE MoveBox::Update(float dt)
{
	MActor::Update(dt);
	//E_SCENE eResult = MActor::Update(dt);

	//if (eResult > E_SCENE_NONPASS)
	//	return eResult;
	//auto state = 

	return E_SCENE_NONPASS;
}

void MoveBox::Draw()
{
	XMFLOAT2 offset = XMFLOAT2(0, 0);
	Color tint = Colors::White;

	offset.x = (int)ScrollMgr::Instance().GetScroll().x;
	offset.y = (int)ScrollMgr::Instance().GetScroll().y;

	vector<unique_ptr<TILE>> *pVecTile = MActorManager::Instance().GetTileInfo();
	//XMFLOAT2 pos1, pos2, pos3;
	//pos1 = XMFLOAT2(24.0f, 24.0f);
	//pos2 = XMFLOAT2(24.0f + 24.0f, 24.0f);
	//pos3 = XMFLOAT2(24.0f + 48.0f, 24.0f);

	//vector<XMFLOAT2> vecpos;
	//vecpos.push_back(pos1);
	//vecpos.push_back(pos2);
	//vecpos.push_back(pos3);

	//if (mVisible)
	//{
	//	TileScope();
	//	mSeekScope = ScopeSeek();
	//	for (int x = 0; x < vecScopePos.size(); ++x)
	//	{
	//		mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + vecScopePos[x] - offset, tint);
	//	}
	//	vecScopePos.clear();
	//}

	if (mVisible)
	{
		if (MFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
		{
			TileScope();
		}
		mSeekScope = ScopeSeek();
		for (int x = 0; x < mspVecScopeIndex.size(); ++x)
		{
			Vector2 vTilePos = Vector2(
				(*pVecTile)[*mspVecScopeIndex[x]]->vPos.x + JOJOTILESX / 2,
				(*pVecTile)[*mspVecScopeIndex[x]]->vPos.y + JOJOTILESY / 2);
			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + vTilePos - offset, tint);
		}
		//타일 위치 값 초기화
		(*pVecTile)[GetTileIndex(mPosition)]->moveNum = 0;
		for (int x = 0; x < mspVecScopeIndex.size(); ++x)
		{
			(*pVecTile)[*mspVecScopeIndex[x]]->moveNum = 0;
		}
	}
	else
	{
		mSeekScope = false;
	}

}

void MoveBox::Release()
{
	vector<unique_ptr<TILE>> *pVecTile = MActorManager::Instance().GetTileInfo();

	mVecScopeIndex.clear();
	//타일 위치 값 초기화
	(*pVecTile)[GetTileIndex(mPosition)]->moveNum = 0;
	for (int x = 0; x < mspVecScopeIndex.size(); ++x)
	{
		(*pVecTile)[*mspVecScopeIndex[x]]->moveNum = 0;
	}
	auto iter = mspVecScopeIndex.begin();
	while (iter != mspVecScopeIndex.end())
	{
		iter->reset();
		iter = mspVecScopeIndex.erase(iter);
	}
}

void MoveBox::TileScope()
{
	//if (MFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	//{
		float fScrollx = ScrollMgr::Instance().GetScroll().x;
		float fScrolly = ScrollMgr::Instance().GetScroll().y;
		auto mouse = Mouse::Get().GetState();
		XMFLOAT2 mMousePos = XMFLOAT2(mouse.x + fScrollx, mouse.y + fScrolly);

		vector<unique_ptr<TILE>> *pVecTile = MActorManager::Instance().GetTileInfo();

		//int iIndex = 0;

		//int i = 1;
		//float Distance = 0;
		//Vector2 vPos = mPosition;
		//Vector2 vTilePos = XMFLOAT2(0.0f, 0.0f);
		RECT rectScope;
		rectScope.left = mPosition.x - mLimitDis;
		rectScope.right = mPosition.x + mLimitDis;
		rectScope.top = mPosition.y - mLimitDis;
		rectScope.bottom = mPosition.y + mLimitDis;

		//XMFLOAT2 fiindex = XMFLOAT2(mPosition.x + mLimitDis, mPosition + mLimitDis);
		//int iindex = GetTileIndex(mPosition.x + mLimitDis);
		XMFLOAT2 sLeftt = XMFLOAT2(rectScope.left, rectScope.top);
		XMFLOAT2 sRightt = XMFLOAT2(rectScope.right, rectScope.top);
		XMFLOAT2 sLeftb = XMFLOAT2(rectScope.left, rectScope.bottom);
		XMFLOAT2 sRightb = XMFLOAT2(rectScope.right, rectScope.bottom);

		int sLTIndex = GetTileIndex(sLeftt);
		int sRTIndex = GetTileIndex(sRightt);
		int sLBIndex = GetTileIndex(sLeftb);
		int sRBIndex = GetTileIndex(sRightb);

		//if (sLTIndex < 0)
		//	sLTIndex = 0;
		//if (sRTIndex > 400)
		//	sRTIndex = 400;
		//if (sLBIndex < 0)
		//	sLBIndex = 0;
		//if (sRBIndex > 400)
		//	sRBIndex = 400;
		//for (int i = 0; i < sLBIndex- sLTIndex; ++i)
		//{
		//	for (int j = 0; j < sRBIndex - sLBIndex; ++j)
		//	{
		//		if (i * (20) + j+(sLTIndex) >= 400)
		//			break;
		//		//if ((*pVecTile)[i * (sLTIndex) + j]->byOption == 1)
		//		//	continue;
		//		Vector2 vTilePos = Vector2((*pVecTile)[i * (20) + j+(sLTIndex)]->vPos.x + JOJOTILESX / 2, (*pVecTile)[i * (20) + j+(sLTIndex)]->vPos.y + JOJOTILESY / 2);
		//		Vector2 vPos = mPosition;

		//		float Distance = Vector2::Distance(vPos, vTilePos);

		//		if (Distance <= mLimitDis)
		//		{
		//			vecScopePos.push_back(vTilePos);
		//		}

		//	}
		//}
		//케릭터의 위치타일에 이동거리를 넣음
		(*pVecTile)[GetTileIndex(mPosition)]->moveNum = mMoveDistance;
		//vecScopeIndex.push_back(GetTileIndex(mPosition));
		for (int mDis = mMoveDistance; mDis > 0; --mDis)
		{
			for (int i = 0; i < 20; ++i)
			{
				for (int j = 0; j < 20; ++j)
				{
					if ((*pVecTile)[i * (20) + j]->moveNum == mDis)
					{
						if ((*pVecTile)[i * (20) + j]->moveNum > 0)
						{
							if (i - 1 >= 0 && (*pVecTile)[(i - 1) * (20) + j]->moveNum < mDis && (*pVecTile)[(i - 1) * (20) + j]->byOption != 1)
							{
								(*pVecTile)[(i - 1) * (20) + j]->moveNum = mDis - 1;
								mspVecScopeIndex.push_back(make_unique<int>((i - 1) * (20) + j));
							}
							if (i + 1 < 20 && (*pVecTile)[(i + 1) * (20) + j]->moveNum < mDis && (*pVecTile)[(i + 1) * 20 + j]->byOption != 1)
							{
								(*pVecTile)[(i + 1) * (20) + j]->moveNum = mDis - 1;
								mspVecScopeIndex.push_back(make_unique<int>((i + 1) * (20) + j));
							}

							if (j + 1 < 20 && (*pVecTile)[i * (20) + (j + 1)]->moveNum < mDis && (*pVecTile)[i * 20 + (j + 1)]->byOption != 1)
							{
								(*pVecTile)[i * (20) + (j + 1)]->moveNum = mDis - 1;
								mspVecScopeIndex.push_back(make_unique<int>(i * (20) + (j + 1)));
							}

							if (j - 1 >= 0 && (*pVecTile)[i * (20) + (j - 1)]->moveNum < mDis && (*pVecTile)[i * 20 + (j - 1)]->byOption != 1)
							{
								(*pVecTile)[i * (20) + (j - 1)]->moveNum = mDis - 1;
								mspVecScopeIndex.push_back(make_unique<int>(i * (20) + (j - 1)));
							}


						}
					}
				}
			}
		}

	//}
	//절대값 다이아 몬드 모양으로 하는방법
	/*
	for (int i = sLTIndex; i < sRBIndex; i+=11)
	{
	for (int j = 1; j < 10; ++j)
	{
	if (i > 400 || i < 0)
	{
	i = 0;
	}
	//if ((*pVecTile)[i * (sLTIndex)+j]->byOption == 1)
	//	continue;

	Vector2 vTilePos = Vector2((*pVecTile)[i]->vPos.x + JOJOTILESX / 2, (*pVecTile)[i]->vPos.y + JOJOTILESY / 2);
	Vector2 vPos = mPosition;

	//float Distance = Vector2::Distance(vPos, vTilePos);

	//if (Distance <= mLimitDis)
	//{
	//	vecScopePos.push_back(vTilePos);
	//}

	float x = vPos.x - vTilePos.x;
	float y = vPos.y - vTilePos.y;

	if (abs(x) + abs(y) <= abs(mLimitDis))
	{
	vecScopePos.push_back(vTilePos);
	}
	i += 1;
	}
	}
	*/

	/*
	while (true)	// 탈출조건 ? 노드가 목적지에 도달할때까지...
	{
	int JOJOCX = 20;
	int JOJOCY = 20;


	//if(존재하는 타일이냐?,
	//갈수 있는 타일이냐?,
	//이미조사했거나,
	//오픈리스트에 추가되있는 녀석 제외)

	// 위
	iIndex = GetTileIndex(mPosition) - JOJOCX*i;

	vTilePos = Vector2((*pVecTile)[iIndex]->vPos.x + JOJOTILESX / 2, (*pVecTile)[iIndex]->vPos.y + JOJOTILESY / 2);


	Distance = Vector2::Distance(vPos, vTilePos);

	if (iIndex >= JOJOCX && Distance <= mLimitDis)
	{
	vecScopePos.push_back(vTilePos);
	}

	// 오른쪽 위
	iIndex = GetTileIndex(mPosition) -
	(JOJOCX - 1)*i;
	vTilePos = Vector2((*pVecTile)[iIndex]->vPos.x + JOJOTILESX / 2, (*pVecTile)[iIndex]->vPos.y + JOJOTILESY / 2);


	Distance = Vector2::Distance(vPos, vTilePos);

	if (iIndex >= JOJOCX &&
	iIndex % (JOJOCX) != JOJOCX - 1 &&
	Distance <= mLimitDis)
	{
	vecScopePos.push_back(vTilePos);
	}
	// 오른쪽
	iIndex = GetTileIndex(mPosition) + 1 * i;
	vTilePos = Vector2((*pVecTile)[iIndex]->vPos.x + JOJOTILESX / 2, (*pVecTile)[iIndex]->vPos.y + JOJOTILESY / 2);

	Distance = Vector2::Distance(vPos, vTilePos);

	if (iIndex % JOJOCX != JOJOCX - 1 &&
	Distance <= mLimitDis)
	{
	vecScopePos.push_back(vTilePos);
	}

	// 오른쪽 아래
	iIndex = GetTileIndex(mPosition) +
	(JOJOCX + 1) * i;

	vTilePos = Vector2((*pVecTile)[iIndex]->vPos.x + JOJOTILESX / 2, (*pVecTile)[iIndex]->vPos.y + JOJOTILESY / 2);


	Distance = Vector2::Distance(vPos, vTilePos);

	if (iIndex < JOJOCX * JOJOCY &&
	iIndex % JOJOCX != JOJOCX - 1 &&
	Distance <= mLimitDis)
	{
	vecScopePos.push_back(vTilePos);
	}
	// 아래
	iIndex = GetTileIndex(mPosition) + JOJOCX*i;

	vTilePos = Vector2((*pVecTile)[iIndex]->vPos.x + JOJOTILESX / 2, (*pVecTile)[iIndex]->vPos.y + JOJOTILESY / 2);


	float Distance = Vector2::Distance(vPos, vTilePos);

	if (iIndex < JOJOCX * JOJOCY - JOJOCX &&
	Distance <= mLimitDis)
	{
	vecScopePos.push_back(vTilePos);
	}

	// 왼쪽 아래
	iIndex = GetTileIndex(mPosition) +
	(JOJOCX - 1)*i;

	vTilePos = Vector2((*pVecTile)[iIndex]->vPos.x + JOJOTILESX / 2, (*pVecTile)[iIndex]->vPos.y + JOJOTILESY / 2);


	Distance = Vector2::Distance(vPos, vTilePos);

	if (iIndex < JOJOCX * JOJOCY - JOJOCX &&
	iIndex % JOJOCX != 0 &&
	Distance <= mLimitDis)
	{
	vecScopePos.push_back(vTilePos);
	}

	// 왼쪽
	iIndex = GetTileIndex(mPosition) - 1 * i;

	vTilePos = Vector2((*pVecTile)[iIndex]->vPos.x + JOJOTILESX / 2, (*pVecTile)[iIndex]->vPos.y + JOJOTILESY / 2);


	Distance = Vector2::Distance(vPos, vTilePos);

	if (iIndex % (JOJOCX) != 0 && Distance <= mLimitDis)
	{
	vecScopePos.push_back(vTilePos);
	}
	// 왼쪽 위
	iIndex = GetTileIndex(mPosition) -
	(JOJOCX + 1)*i;

	vTilePos = Vector2((*pVecTile)[iIndex]->vPos.x + JOJOTILESX / 2, (*pVecTile)[iIndex]->vPos.y + JOJOTILESY / 2);


	Distance = Vector2::Distance(vPos, vTilePos);

	if (iIndex >= JOJOCX &&
	iIndex % (JOJOCX) != 0 &&
	Distance <= mLimitDis)
	{
	vecScopePos.push_back(vTilePos);
	}

	// Parent로 부터 8방향 조사가 끝이 났고,
	// 8개의 노드중에 갈 수 있는 노드를 OpenList에 넣어두었다.
	// 이 중에서 코스트가 가장 적을 고른다. -> 오름차순 정렬
	// Sort(함수포인터);
	//sort(vecScopePos.begin(),vecScopePos.end());

	i++;
	if (i >= JOJOCX)
	i = 1;

	if (vecScopePos.empty())
	return;

	// 탈출 !
	if (Distance >= mLimitDis)
	{
	break;
	}
	}
	*/
}

bool MoveBox::ScopeSeek()
{
	float fScrollx = ScrollMgr::Instance().GetScroll().x;
	float fScrolly = ScrollMgr::Instance().GetScroll().y;
	auto mouse = Mouse::Get().GetState();
	vector<unique_ptr<TILE>> *pVecTile = MActorManager::Instance().GetTileInfo();

	XMFLOAT2 mmousePos = XMFLOAT2(mouse.x + fScrollx, mouse.y + fScrolly);

	int mouseIndex = GetTileIndex(mmousePos);
	//Vector2 vecMousePos = (*pVecTile)[mouseIndex]->vPos + XMFLOAT2(JOJOTILESX / 2, JOJOTILESY / 2);
	//for (int i = 0; i < vecScopePos.size(); ++i)
	//{
	//	if ((vecScopePos[i].x == vecMousePos.x) && (vecScopePos[i].y == vecMousePos.y))
	//		return true;
	//}

	//끝에 다다랏을때 인덱스 특저오류 예외처리
	if (mouseIndex < 0)
		return false;
	Vector2 vecMousePos = (*pVecTile)[mouseIndex]->vPos + XMFLOAT2(JOJOTILESX / 2, JOJOTILESY / 2);

	for (int i = 0; i < mspVecScopeIndex.size(); ++i)
	{
		Vector2 vec2ScopePos = (*pVecTile)[*mspVecScopeIndex[i]]->vPos + XMFLOAT2(JOJOTILESX / 2, JOJOTILESY / 2);
		if (vec2ScopePos.x == vecMousePos.x && vec2ScopePos.y == vecMousePos.y)
			return true;
	}

	return false;
}
