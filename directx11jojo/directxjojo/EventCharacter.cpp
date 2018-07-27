#include "stdafx.h"
#include "EventCharacter.h"


EventCharacter::EventCharacter()
{
}


EventCharacter::EventCharacter(SpriteBatch *pBatch, SpriteSheet *pSheet, SpriteFont * pFont)
	:MActor(pBatch,pSheet,pFont)
{

}

EventCharacter::~EventCharacter()
{
}

void EventCharacter::Init(float moveSpeed, XMFLOAT2 startpos, E_SORTID eSortID)
{

	SetPosition(startpos);
	//SetAnimation("EventMap0");
	mMouseposVible = false;
	mHorizonVerticall = 0;
	meSortID = eSortID;
	mfMoveSpeed = moveSpeed;

	mspFSM = make_unique<MFSM>();
	mspFSM->SetOwner(this);
	mspFSM->AddState<P_IdleState>(JoJoGun::ProductionState::Production_Idle);
	mspFSM->AddState<P_MoveState>(JoJoGun::ProductionState::Production_Move);
	mspFSM->AddState<P_TalkState>(JoJoGun::ProductionState::Production_Talk);
	mspFSM->ChangeState(JoJoGun::ProductionState::Production_Idle);
}

E_SCENE EventCharacter::Update(float dt)
{
	MActor::Update(dt);

	mspFSM->Update(dt);

	if (MFramework::mKeyboardTracker.IsKeyPressed(Keyboard::D0))
	{
		mMouseposVible = !mMouseposVible;
	}
	return E_SCENE_NONPASS;
}

void EventCharacter::Draw()
{
	XMFLOAT2 offset = XMFLOAT2(0, 0);
	Color tint = Colors::White;

	switch (mHorizonVerticall)
	{
	case 0:
		mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition, tint
			, 0, 1.0f, DirectX::SpriteEffects_None, 0.0f);
		break;
	case 1:
		mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition, tint
			, 0, 1.0f, DirectX::SpriteEffects_FlipHorizontally, 0.0f);
		break;
	case 2:
		mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition, tint
			, 0, 1.0f, DirectX::SpriteEffects_FlipVertically, 0.0f);
		break;

	}

	if (mMouseposVible)
	{
		wchar_t wch[64];
		auto mouse = Mouse::Get().GetState();

		//swprintf_s(wch, L"%d", index);
		swprintf_s(wch, L"x = %d y = %d", mouse.x, mouse.y);//mTileInfo[index]->underObject
		mpFont->DrawString(mpBatch, wch, XMFLOAT2(mouse.x, mouse.y), DirectX::Colors::White, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.5f, 0.5f));
	}


}

