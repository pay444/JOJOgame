#include "stdafx.h"
#include "TurnGrapic.h"


TurnGrapic::TurnGrapic()
{
}


TurnGrapic::TurnGrapic(SpriteBatch * pBatch, SGASpriteSheet * pSheet, SpriteFont * pFont)
	:SGAActor(pBatch, pSheet, pFont), mTGVisible(false)
{

}

TurnGrapic::~TurnGrapic()
{
}

void TurnGrapic::Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible)
{
	Animation anim[] = {
		{ "Ui0", 1,{ { "Ui0", 0.3f } } },
	{ "Ui1", 1,{ { "Ui1", 0.3f } } },
	{ "Ui2", 1,{ { "Ui2", 0.3f } } },
	{ "Ui3", 1,{ { "Ui3", 0.3f } } },
	{ "Ui4", 1,{ { "Ui4", 0.3f } } },
	{ "Ui5", 1,{ { "Ui5", 0.3f } } },
	{ "Ui6", 1,{ { "Ui6", 0.3f } } },
	{ "Ui7", 1,{ { "Ui7", 0.3f } } },
	};

	//보이고 안보이고
	mTGVisible = visible;
	SGAActor::Init(anim, 8, eSortID);
	SetPosition(pos);
	SetAnimation("Ui0");
}

E_SCENE TurnGrapic::Update(float dt)
{
	E_SCENE eResult = SGAActor::Update(dt);

	if (eResult > E_SCENE_NONPASS)
		return eResult;

	return E_SCENE_NONPASS;
}

void TurnGrapic::Draw()
{
	XMFLOAT2 offset = XMFLOAT2(0, 0);
	Color tint = Colors::White;

	offset.x = (int)ScrollMgr::Instance().GetScroll().x;
	offset.y = (int)ScrollMgr::Instance().GetScroll().y;

	auto mouse = Mouse::Get().GetState();

	//이동범위를 보여줄때
	if (mTGVisible)
	{
		SetAnimation("Ui0");
		mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition - offset, tint);
	}

}
