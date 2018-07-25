#include "stdafx.h"
#include "EventCharacter.h"


EventCharacter::EventCharacter()
{
}


EventCharacter::EventCharacter(SpriteBatch *pBatch, SpriteSheet *pSheet, SpriteFont * pFont)
{

}

EventCharacter::~EventCharacter()
{
}

void EventCharacter::Init(float moveSpeed, XMFLOAT2 startpos, E_SORTID eSortID)
{
	SetPosition(pos);
	SetAnimation("EventMap0");
	RECT rc;
	GetWindowRect(SceneMgr::Instance().GetHWND(), &rc);
	MoveWindow(SceneMgr::Instance().GetHWND()
		, rc.left, rc.top, 658, 438, true);

	mspFSM = make_unique<MFSM>();
	mspFSM->SetOwner(this);
	mspFSM->AddState<P_IdleState>(GunGeon::ProductionState::Production_Idle);
	mspFSM->AddState<P_MoveState>(GunGeon::ProductionState::Production_Move);
	mspFSM->AddState<P_TalkState>(GunGeon::ProductionState::Production_Talk);
	mspFSM->ChangeState(GunGeon::ProductionState::Production_Idle);
}

E_SCENE EventCharacter::Update(float dt)
{
	MActor::Update(dt);

	return E_SCENE_NONPASS;
}

void EventCharacter::Draw()
{
	XMFLOAT2 offset = XMFLOAT2(0, 0);
	Color tint = Colors::White;

	mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition, tint
		, 0, 1.0f, DirectX::SpriteEffects_FlipHorizontally, 0.0f);

}
