#include "stdafx.h"
#include "UiConsumItem.h"


UiConsumItem::UiConsumItem()
{
}


UiConsumItem::UiConsumItem(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont)
	:MActor(pBatch, pSheet, pFont),
	mpPlayer(nullptr),
	mUICVisible(false),
	mCancelBtnPos(0, 0),
	mpCurSkill(nullptr),
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
	};

	//이동거리
	//mMoveDistance = moveDis;
	//보이고 안보이고
	mUICVisible = visible;
	MActor::Init(anim, 4, eSortID);
	SetPosition(pos + XMFLOAT2(100.0f, 0.0f));
	SetAnimation("UiItem");
	mCancelBtnPos = mPosition + XMFLOAT2(82, 64);
}

E_SCENE UiConsumItem::Update(float dt)
{
	MActor::Update(dt);

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

}
