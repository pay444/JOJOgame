#include "stdafx.h"
#include "ProductionState.h"
#include "EventCharacter.h"

void P_IdleState::Enter()
{
	//MFramework::OnResize();
	misFirst = true;
	mpFSM->GetOwner()->SetAnimation("StandF");
	//EventEditorMgr::Instance().EventCheck();
}

void P_IdleState::Execute(float dt)
{
	if (MFramework::mMouseTracker.rightButton == Mouse::ButtonStateTracker::ButtonState::RELEASED
		&& misFirst)
	{
		mpFSM->ChangeState(JoJoGun::ProductionState::Production_Move);
		misFirst = false;
	}

}

void P_IdleState::Exit()
{
}

void P_MoveState::Enter()
{
	mpFSM->GetOwner()->SetAnimation("WalkF");
	//EventEditorMgr::Instance().EventCheck();

}

void P_MoveState::Execute(float dt)
{
	EventCharacter* pChr = static_cast<EventCharacter*>(mpFSM->GetOwner());
	Vector2 vDest= pChr->GetDestPos();
	Vector2 vMyPos = pChr->GetPosition();
	Vector2 vDir;
	vDir = vDest - vMyPos;
	//vDir.x= vMyPos.x + vDest.x;
	//vDir.y = vMyPos.y - vDest.y;
	vDir.Normalize();

	pChr->SetPosition(pChr->GetPosition() + vDir * pChr->GetMoveSpeed() * dt);

	//ÁöÁ¤ÇÑ °÷¿¡ µµÂøÇÏ¸é ¸ØÃã
	RECT colrc;
	POINT pt;
	pt.x = pChr->GetPosition().x;
	pt.y = pChr->GetPosition().y;
	colrc = RectMakeCenter(pChr->GetDestPos().x, pChr->GetDestPos().y, 10, 10);
	if (PtInRect(&colrc, pt))
	{
		pChr->SetPosition(XMFLOAT2(pt.x, pt.y));
		mpFSM->ChangeState(JoJoGun::ProductionState::Production_Idle);
	}
}

void P_MoveState::Exit()
{
}

void P_TalkState::Enter()
{
	//mpFSM->GetOwner()->SetAnimation("StandF");

	//EventEditorMgr::Instance().EventCheck();

}

void P_TalkState::Execute(float dt)
{
}

void P_TalkState::Exit()
{
}
