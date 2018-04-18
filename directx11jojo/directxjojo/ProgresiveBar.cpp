#include "stdafx.h"
#include "ProgresiveBar.h"


ProgresiveBar::ProgresiveBar() 
{
}


ProgresiveBar::ProgresiveBar(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont)
	:MActor(pBatch,pSheet,pFont),mPBVisible(false)
{
	mfTEalpsdtime = 0.0f;
	mEndTIme = false;
}

ProgresiveBar::~ProgresiveBar()
{
}

void ProgresiveBar::Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible)
{
	Animation anim[] = {
		{ "PBHp", 1,{ { "HpBar0", 0.3f } } },
		{ "PBMp", 1,{ { "MpBar0", 0.3f } } },
	{ "PBSp", 1,{ { "SpBar0", 0.3f } } },
	{ "HpIcon", 1,{ { "HpIcon0", 0.3f } } },
	{ "MpIcon", 1,{ { "MpIcon0", 0.3f } } },
	{ "SpIcon", 1,{ { "SpIcon0", 0.3f } } },
	};

	//보이고 안보이고
	mPBVisible = visible;
	MActor::Init(anim, 6, eSortID);
	SetPosition(pos);
	SetAnimation("SpIcon");
}

E_SCENE ProgresiveBar::Update(float dt)
{
	mEndTIme = false;
	//보여주는것이 활성화 되었을경우
	if (mPBVisible)
	{
		mfTEalpsdtime += dt;

	}

	if (mfTEalpsdtime > 1.0f)
	{
		mPBVisible = false;
		mEndTIme = true;
		mfTEalpsdtime = 0.0f;
	}
	auto key = Keyboard::Get().GetState();
	
	if (key.F)
	{
		mPBVisible = !mPBVisible;
	}

	E_SCENE eResult = MActor::Update(dt);

	if (eResult > E_SCENE_NONPASS)
		return eResult;

	return E_SCENE_NONPASS;
}

void ProgresiveBar::Draw()
{
	XMFLOAT2 offset = XMFLOAT2(0, 0);
	Color tint = Colors::White;

	//offset.x = (int)ScrollMgr::Instance().GetScroll().x;
	//offset.y = (int)ScrollMgr::Instance().GetScroll().y;

	auto mouse = Mouse::Get().GetState();

	//이동범위를 보여줄때
	if (mPBVisible)
	{
		mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition - offset, tint);
	}
}
