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
		{ "Bean", 1,{ { "Bean", 0.3f } } },
	};

	//이동거리
	//mMoveDistance = moveDis;
	//보이고 안보이고
	//mUISVisible = true;
	MActor::Init(anim, 1, eSortID);
	SetPosition(pos + XMFLOAT2(100.0f, 0.0f));
	SetAnimation("Bean");
	mMpPlus = 5;
	mCountItem = 3;
}

E_SCENE MpWater::Update(float dt)
{
	return E_SCENE_NONPASS;
}

void MpWater::Draw()
{
}
