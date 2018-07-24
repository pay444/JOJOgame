#include "stdafx.h"
#include "MpWater.h"


MpWater::MpWater()
{
}


MpWater::MpWater(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont)
	:ConsumItems(pBatch,pSheet,pFont)
{

}

MpWater::~MpWater()
{
}

void MpWater::Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible)
{
	Animation anim[] = {
		{ "Water", 1,{ { "Water", 0.3f } } },
	};

	//이동거리
	//mMoveDistance = moveDis;
	//보이고 안보이고
	//mUISVisible = true;
	MActor::Init(anim, 1, eSortID);
	SetPosition(pos + XMFLOAT2(100.0f, 0.0f));
	SetAnimation("Water");
	mPlus = 5;
	mStock = 3;
	mName = L"신비물";
	mEfficacy = L"MP회복";
}

E_SCENE MpWater::Update(float dt)
{
	return E_SCENE_NONPASS;
}

void MpWater::Draw()
{
	XMFLOAT2 offset = XMFLOAT2(0, 0);
	Color tint = Colors::White;
	offset.x = (int)ScrollMgr::Instance().GetScroll().x;
	offset.y = (int)ScrollMgr::Instance().GetScroll().y;

	mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition- offset, tint);

}
