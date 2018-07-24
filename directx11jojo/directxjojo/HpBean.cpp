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
		{ "Bean", 1,{ { "Bean", 0.3f } } },
	};

	//이동거리
	//mMoveDistance = moveDis;
	//보이고 안보이고
	//mUISVisible = true;
	MActor::Init(anim, 1, eSortID);
	SetPosition(pos + XMFLOAT2(100.0f, 0.0f));
	SetAnimation("Bean");
	mPlus = 5;
	mStock = 3;
	mName = L"콩";
	mEfficacy = L"HP회복";
}

E_SCENE HpBean::Update(float dt)
{
	return E_SCENE_NONPASS;
}

void HpBean::Draw()
{
	XMFLOAT2 offset = XMFLOAT2(0, 0);
	Color tint = Colors::White;
	offset.x = (int)ScrollMgr::Instance().GetScroll().x;
	offset.y = (int)ScrollMgr::Instance().GetScroll().y;

	mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition - offset, tint);
}
