#include "stdafx.h"
#include "TurnGrapic.h"


TurnGrapic::TurnGrapic()
{
}


TurnGrapic::TurnGrapic(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont)
	:MActor(pBatch, pSheet, pFont), mTGVisible(false)
{
	mfTEalpsdtime = 0.0f;
	mEndTIme = false;
}

TurnGrapic::~TurnGrapic()
{
}

void TurnGrapic::Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible)
{
	Animation anim[] = {
		{ "pTurn", 1,{ { "pTurn0", 0.3f } } },
	{ "eTurn", 1,{ { "eTurn0", 0.3f } } },

	};

	//���̰� �Ⱥ��̰�
	mTGVisible = visible;
	mTGPos = mPosition;
	MActor::Init(anim, 2, eSortID);
	SetPosition(pos);
	SetAnimation("pTurn");
}

E_SCENE TurnGrapic::Update(float dt)
{
	
	//�����ִ°��� Ȱ��ȭ �Ǿ������
	if (mTGVisible)
	{	
		mfTEalpsdtime += dt;
		mEndTIme = false;
	}

	if (mfTEalpsdtime > 1.0f)
	{
		mTGVisible = false;
		mEndTIme = true;
		mfTEalpsdtime = 0.0f;
	}
	auto key = Keyboard::Get().GetState();

	if (key.G)
	{
		mTGVisible = !mTGVisible;
	}

	E_SCENE eResult = MActor::Update(dt);

	if (eResult > E_SCENE_NONPASS)
		return eResult;

	return E_SCENE_NONPASS;
}

void TurnGrapic::Draw()
{
	XMFLOAT2 offset = XMFLOAT2(0, 0);
	Color tint = Colors::White;

	//offset.x = (int)ScrollMgr::Instance().GetScroll().x;
	//offset.y = (int)ScrollMgr::Instance().GetScroll().y;

	auto mouse = Mouse::Get().GetState();

	//�̵������� �����ٶ�
	if (mTGVisible)
	{
		mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition - offset, tint);
	}

}
