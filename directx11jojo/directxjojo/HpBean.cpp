#include "stdafx.h"
#include "HpBean.h"


HpBean::HpBean()
{
}


HpBean::HpBean(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont)
	:ConsumItems(pBatch, pSheet, pFont)
{

}

HpBean::~HpBean()
{
}

void HpBean::Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible)
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
	mHpPlus = 5;
	mCountItem = 3;
}

E_SCENE HpBean::Update(float dt)
{
	return E_SCENE_NONPASS;
}

void HpBean::Draw()
{
}
