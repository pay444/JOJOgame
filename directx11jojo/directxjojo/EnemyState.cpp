#include "stdafx.h"
#include "EnemyState.h"

void IdleState::Enter()
{
	OutputDebugString(L"Enter Idle\n");
}

void IdleState::Execute(float dt)
{
	//플레이어, 소유자
	Player *pPlayer = GunGeon::Blackboard::Instance().GetPlayer();
	Enemy *pEnemy = (Enemy*)(this->mpFSM->GetOwner());

	if (pPlayer == NULL || pEnemy == NULL)
		return;

	//탐지해서 범위내에 플레이어 발견하면 상태전이
	//탐지 (플레이어 <-> 적 거리)
	float dist = Vector2::Distance(pPlayer->GetPosition(), pEnemy->GetPosition());

	//범위 내에 플레이어 발견
	//if (dist < pEnemy->GetDetectRange())
	//{
	//	//상태 전이
	//	mpFSM->ChangeState(GunGeon::EnemyState::Enemy_Chase);
	//}

	
}

void IdleState::Exit()
{
	OutputDebugString(L"-----------Exit Idle\n");
}

void ChaseState::Enter()
{
	OutputDebugString(L"Enter Chase\n");
}

void ChaseState::Execute(float dt)
{
	//플레이어, 소유자
	Player *pPlayer = GunGeon::Blackboard::Instance().GetPlayer();
	Enemy *pEnemy = (Enemy*)(this->mpFSM->GetOwner());

	if (pPlayer == NULL || pEnemy == NULL)
		return;
	//거리//탐지
	float dist = Vector2::Distance(pEnemy->GetPosition(), pPlayer->GetPosition());
	//// 사정거리 안에 돌아오면 -> Atack
	//if (dist < pEnemy->GetAttackRange())
	//{
	//	mpFSM->ChangeState(GunGeon::EnemyState::Enemy_Attack);
	//	return;
	//}
	//// 타겟 놓침 -> Idle
	//if (dist > pEnemy->GetDetectRange())
	//{
	//	mpFSM->ChangeState(GunGeon::EnemyState::Enemy_Idle);
	//}
	////추적 ( 타겟 방향으로 이동)
	//Vector2 dir = pPlayer->GetPosition() - pEnemy->GetPosition();
	//dir.Normalize();

	//pEnemy->SetPosition(pEnemy->GetPosition()+dir*pEnemy->GetSpeed() * dt);
}

void ChaseState::Exit()
{
	OutputDebugString(L"------------Exit Chase\n");
}

void AttackState::Enter()
{
	OutputDebugString(L"Enter Attack\n");
}

void AttackState::Execute(float dt)
{
	Enemy* pOwner = (Enemy*)(this->mpFSM->GetOwner());
	Player* pPlayer = GunGeon::Blackboard::Instance().GetPlayer();

	mfElapsedTime += dt;

	//if (mfElapsedTime > pOwner->GetAttackDelay())
	//{
	//	pOwner->SetTarget(pPlayer);
	//	pOwner->Fire();
	//	
	//	mfElapsedTime = 0.0f;
	//}

	
	mpFSM->ChangeState(GunGeon::EnemyState::Enemy_Chase);
}

void AttackState::Exit()
{
	OutputDebugString(L"------------Exit Attack\n");
}
