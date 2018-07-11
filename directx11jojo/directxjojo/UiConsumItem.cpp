#include "stdafx.h"
#include "ConsumItems.h"
#include "UiConsumItem.h"


UiConsumItem::UiConsumItem()
{
}


UiConsumItem::UiConsumItem(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont)
	:MActor(pBatch, pSheet, pFont),
	mpPlayer(nullptr),
	mUICVisible(false),
	mCancelBtnPos(0, 0),
	mpCurItem(nullptr),
	mAreaVisible(false),
	mFlag(false)
{

}

UiConsumItem::~UiConsumItem()
{
	this->Release();
}

void UiConsumItem::Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible)
{
	Animation anim[] = {
	{ "UiItem", 1,{ { "uiItem", 0.1f } } },
	{ "ToolBack", 1,{ { "toolBack", 0.1f } } },
	{ "Bean", 1,{ { "hpBean", 0.1f } } },
	{ "MpWater", 1,{ { "mpWater", 0.1f } } },
	{ "AttackBox", 1,{ { "AttackBox0", 0.3f } } },
	{ "CancelBtn", 1,{ { "cancelBtn", 0.3f } } },
	};

	//이동거리
	//mMoveDistance = moveDis;
	//보이고 안보이고
	mUICVisible = visible;
	MActor::Init(anim, 6 ,eSortID);
	SetPosition(pos + XMFLOAT2(100.0f, 0.0f));
	SetAnimation("UiItem");
	mCancelBtnPos = mPosition + XMFLOAT2(82, 64);
}

E_SCENE UiConsumItem::Update(float dt)
{
	float fScrollx = ScrollMgr::Instance().GetScroll().x;
	float fScrolly = ScrollMgr::Instance().GetScroll().y;
	MActor::Update(dt);

	//Ui를 보여주는 상태일때
	if (mUICVisible)
	{
		//취소 버튼의 위치 갱신
		mCancelBtnPos = mPosition + XMFLOAT2(82, 64);

		//해당 스킬을 클릭할 시에 스킬이 선택된다.
		if (MFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
		{
			auto mouse = Mouse::Get().GetState();

			POINT mousePos;
			mousePos.x = mouse.x + fScrollx;
			mousePos.y = mouse.y + fScrolly;

		}
	}
	return E_SCENE_NONPASS;
}

void UiConsumItem::Draw()
{
}

void UiConsumItem::Release()
{
	auto iter = mspVecAreaIndex.begin();
	while (iter != mspVecAreaIndex.end())
	{
		iter->reset();
		iter = mspVecAreaIndex.erase(iter);
	}
}

void UiConsumItem::CalArea(int distance)
{
	int JoTileCx = 20;
	int JoTileCy = 20;
	//캐릭터 주위의 네모난 칸만 벡터에 넣어줌
	int tileOnPlayerIndex = GetTileIndex(mpPlayer->GetPosition());
	//벡터안에 플레이어가 서있는 위치를 넣어줌
	mspVecAreaIndex.push_back(make_unique<int>(tileOnPlayerIndex));
	//8방향 확인후 범위 벡터에 넣어줌
	//위
	if ((tileOnPlayerIndex) >= JoTileCx)
	{
		mspVecAreaIndex.push_back(make_unique<int>(tileOnPlayerIndex-JoTileCx));
	}
	//오위
	if ((tileOnPlayerIndex) >= JoTileCx && tileOnPlayerIndex 
		% JoTileCx != JoTileCx - 1)
	{
		mspVecAreaIndex.push_back(
			make_unique<int>(tileOnPlayerIndex - (JoTileCx - 1)));
	}
	//오
	if (tileOnPlayerIndex % JoTileCx != JoTileCx - 1)
	{
		mspVecAreaIndex.push_back(make_unique<int>(tileOnPlayerIndex + 1));
	}
	//오아
	if ((tileOnPlayerIndex) < JoTileCx * JoTileCy - JoTileCx 
		&& tileOnPlayerIndex % JoTileCx != JoTileCx - 1)
	{
		mspVecAreaIndex.push_back(
			make_unique<int>(tileOnPlayerIndex + (JoTileCx + 1)));
	}
	//아래
	if ((tileOnPlayerIndex) < JoTileCx * JoTileCy - JoTileCx)
	{
		mspVecAreaIndex.push_back(make_unique<int>(tileOnPlayerIndex + JoTileCx));
	}
	//왼아
	if ((tileOnPlayerIndex) < JoTileCx * JoTileCy - JoTileCx 
		&& tileOnPlayerIndex % JoTileCx != 0)
	{
		mspVecAreaIndex.push_back(
			make_unique<int>(tileOnPlayerIndex + (JoTileCx + 1)));

	}
	//왼
	if (tileOnPlayerIndex % JoTileCx != 0)
	{
		mspVecAreaIndex.push_back(
			make_unique<int>(tileOnPlayerIndex - 1));
	}
	//왼위
	if ((tileOnPlayerIndex) >= JoTileCx
		&& tileOnPlayerIndex % JoTileCx != 0)
	{
		mspVecAreaIndex.push_back(
			make_unique<int>(tileOnPlayerIndex - (JoTileCx + 1)));
	}


}
