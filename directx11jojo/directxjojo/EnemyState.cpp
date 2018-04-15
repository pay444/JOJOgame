#include "stdafx.h"
#include "EnemyState.h"

struct Float2Andflaot
{
	XMFLOAT2 pos;
	float dist;
};
struct less_than_dist
{
	inline bool operator() (const Float2Andflaot& struct1, const Float2Andflaot& struct2)
	{
		return (struct1.dist < struct2.dist);
	}
};


void IdleState::Enter()
{
	OutputDebugString(L"Enter Idle\n");
}

void IdleState::Execute(float dt)
{

	//플레이어, 소유자
	//Player *pPlayer = GunGeon::Blackboard::Instance().GetPlayer();
	//Enemy *pEnemy = (Enemy*)(this->mpFSM->GetOwner());

	//플레이어, 소유자
	Enemy *pEnemy = (Enemy*)(this->mpFSM->GetOwner());
	Player *pPlayer = (Player*)pEnemy->GetTarget();

	if (pPlayer == NULL || pEnemy == NULL)
		return;

	vector<XMFLOAT2> vecPos;
	vector<Float2Andflaot> vecFloat2Dis;
	vector<Float2Andflaot> vecMoveBoxFloat2Dis;
	vector<unique_ptr<TILE>> *pVecTile = MActorManager::Instance().GetTileInfo();
	vector<unique_ptr<int>> *pVecMoveBoxIndex = MActorManager::Instance().GetClassMoveBox()->GetVecMBScopeIndex();
	

	//for (int i = 0; i < (*pVecMoveBoxIndex).size(); ++i)
	//{
	//	//스마트 포인터를 가져올때 값을 명확히 해줘야함
	//	auto c = (*pVecMoveBoxIndex).begin()[i].get();
	//	int a = *(*pVecMoveBoxIndex)[i].get();
	//	
	//}

	
	//탐지해서 범위내에 플레이어 발견하면 상태전이
	//탐지 (플레이어 <-> 적 거리)
	float dist = Vector2::Distance(pPlayer->GetPosition(), pEnemy->GetPosition());

	//범위 내에 플레이어 발견
	if (dist < pEnemy->GetDetectRange() && !MActorManager::Instance().GetTurn()
		&& pEnemy->GetActionTurn() < 1&& pEnemy->GetActionTime()>0.7f)
	{
		//상태 전이
		mpFSM->ChangeState(GunGeon::EnemyState::Enemy_Chase);

		//적의 무브박스와 어텍박스 위치를 바꿔줌
		MActorManager::Instance().GetClassMoveBox()->SetPosition(pEnemy->GetPosition());
		MActorManager::Instance().GetClassMoveBox()->SetMoveDis(pEnemy->GetMoveDistance());
		MActorManager::Instance().GetClassMoveBox()->TileScope();
		MActorManager::Instance().GetClassMoveBox()->SetVisible(true);

		//적의 어텍박스 위치를 바꿔줌
		MActorManager::Instance().GetClassAttackBox()->Release();
		MActorManager::Instance().GetClassAttackBox()->SetPosition(pEnemy->GetPosition());
		MActorManager::Instance().GetClassAttackBox()->SetAttackDis(pEnemy->GetAttackDistance());
		MActorManager::Instance().GetClassAttackBox()->AttackScope();
		MActorManager::Instance().GetClassAttackBox()->SetVisible(true);	
		MActorManager::Instance().GetClassAttackBox()->SetCharacter(pEnemy);
		MActorManager::Instance().GetClassAttackBox()->SetAttackDamge
		(MActorManager::Instance().GetClassAttackBox()->GetCharacter()->GetAttack());

		//정지도중에도 공격범위 안에 들어오면 
		if (MActorManager::Instance().GetClassAttackBox()->AIIntersecRectScope(pPlayer))
		{
			//충돌 되었다면 상태 변경 ->Attack
			//MActorManager::Instance().GetClassMoveBox()->SetVisible(false);
			mpFSM->ChangeState(GunGeon::EnemyState::Enemy_Attack);
			return;
		}

		//플레이어주위 4칸과 적과의 거리중 가장 가까운곳의 
		//타일을 정해줌 못가게 되어있으면 안움직임
		XMFLOAT2 topPos = pPlayer->GetPosition() - XMFLOAT2(0.0f, 48.0f);
		vecPos.push_back(topPos);
		XMFLOAT2 botumPos = pPlayer->GetPosition() + XMFLOAT2(0.0f, 48.0f);
		vecPos.push_back(botumPos);
		XMFLOAT2 leftPos = pPlayer->GetPosition() - XMFLOAT2(48.0f, 0.0f);
		vecPos.push_back(leftPos);
		XMFLOAT2 rightPos = pPlayer->GetPosition() + XMFLOAT2(48.0f, 0.0f);
		vecPos.push_back(rightPos);

		//0보다 작아지는 값은 제외 다른캐릭터가 올라와 있으면 제외
		auto iter1 = vecPos.begin();
		while (iter1 != vecPos.cend())
		{
			Vector2 vec2 = Vector2(iter1->x,iter1->y);
			
			if (iter1->x < 0 || iter1->y < 0)
			{
				iter1 = vecPos.erase(iter1);
			}
			else if ((*pVecTile)[pEnemy->GetTileIndex(vec2)]->byOption == 1)
			{
				iter1 = vecPos.erase(iter1);
			}
			else 
			{
				iter1++;
			}
		}
		
		//적과 플레이어의 위치 dist 측정및 등록
		for (int i = 0; i < vecPos.size(); ++i)
		{
			float dist = Vector2::Distance(vecPos[i], pEnemy->GetPosition());
			Float2Andflaot faf;
			faf.pos = vecPos[i];
			faf.dist = dist;
			vecFloat2Dis.push_back(faf);
		}
		std::sort(vecFloat2Dis.begin(), vecFloat2Dis.end(), less_than_dist());

		//플레이어와 적의 근접한 위치와 무브박스의 위치를 비교해서 넣어줌
		for (int i = 0; i < (*pVecMoveBoxIndex).size(); ++i)
		{
			float dist = Vector2::Distance(vecFloat2Dis[0].pos,
				(*pVecTile)[*(*pVecMoveBoxIndex)[i].get()]->vPos + XMFLOAT2(24.0f,24.0f));
			Float2Andflaot faf;
			faf.pos = (*pVecTile)[*(*pVecMoveBoxIndex)[i].get()]->vPos + XMFLOAT2(24.0f, 24.0f);
			faf.dist = dist;
			vecMoveBoxFloat2Dis.push_back(faf);
		}
		std::sort(vecMoveBoxFloat2Dis.begin(), vecMoveBoxFloat2Dis.end(), less_than_dist());

		//에너미의 AStar시작
		pEnemy->JoAstar_Start(pEnemy->GetPosition(), vecMoveBoxFloat2Dis[0].pos+XMFLOAT2(24.0f,24.0f));



		//MActorManager::Instance().RePosAndVisiAt();

	}

	
}

void IdleState::Exit()
{
	OutputDebugString(L"-----------Exit Idle\n");
}

void ChaseState::Enter()
{
	OutputDebugString(L"Enter Chase\n");
	//mfElapsedTime = 0.0f;
}

void ChaseState::Execute(float dt)
{
	mfElapsedTime += dt;
	//플레이어, 소유자
	//Player *pPlayer = GunGeon::Blackboard::Instance().GetPlayer();
	//Enemy *pEnemy = (Enemy*)(this->mpFSM->GetOwner());

	//플레이어, 소유자
	Enemy *pEnemy = (Enemy*)(this->mpFSM->GetOwner());
	Player *pPlayer = (Player*)pEnemy->GetTarget();

	float fScrollx = ScrollMgr::Instance().GetScroll().x;
	float fScrolly = ScrollMgr::Instance().GetScroll().y;

	if (pPlayer == NULL || pEnemy == NULL)
		return;

	//거리//탐지
	float dist = Vector2::Distance(pEnemy->GetPosition(), pPlayer->GetPosition());

	//턴이 플레이어 일때 정지 상태로 변경
	if (MActorManager::Instance().GetTurn())
	{
		mpFSM->ChangeState(GunGeon::EnemyState::Enemy_Idle);
	}

	// 사정거리 안에 돌아오면 -> Atack
	//if (dist < pEnemy->GetAttackRange())
	//{
	//	mpFSM->ChangeState(GunGeon::EnemyState::Enemy_Attack);
	//	return;
	//}

	//범위 내에 플레이어 발견 턴이 허용하는 상태일때
	if (!MActorManager::Instance().GetTurn()
		&& pEnemy->GetActionTurn() == 1 && mfElapsedTime > pEnemy->GetMoveDelay())//dist < pEnemy->GetAttackRange()
	{
		mfElapsedTime = 0.0f;
		//적의 어텍박스 위치를 바꿔줌
		MActorManager::Instance().GetClassAttackBox()->Release();
		MActorManager::Instance().GetClassAttackBox()->SetPosition(pEnemy->GetPosition());
		MActorManager::Instance().GetClassAttackBox()->SetAttackDis(pEnemy->GetAttackDistance());
		MActorManager::Instance().GetClassAttackBox()->AttackScope();
		MActorManager::Instance().GetClassAttackBox()->SetVisible(false);
		MActorManager::Instance().GetClassAttackBox()->SetCharacter(pEnemy);
		MActorManager::Instance().GetClassAttackBox()->SetAttackDamge(MActorManager::Instance().GetClassAttackBox()->GetCharacter()->GetAttack());
		//플레이어가 공격범위에 들어와있는지
		if (MActorManager::Instance().GetClassAttackBox()->AIIntersecRectScope(pPlayer))
		{
			//충돌 되었다면 상태 변경 ->Attack
			mpFSM->ChangeState(GunGeon::EnemyState::Enemy_Attack);
			return;
		}
		else
		{
			//충돌이 안되었다면 행동턴 종료후 상태변경 ->Idle
			pEnemy->SetActionTurn(2);
			MActorManager::Instance().GetClassAttackBox()->SetVisible(false);
			MActorManager::Instance().GetClassMoveBox()->SetVisible(false);
			mpFSM->ChangeState(GunGeon::EnemyState::Enemy_Idle);
		}
		
	}


	// 타겟 놓침 -> Idle
	//if (dist > pEnemy->GetDetectRange())
	//{
	//	mpFSM->ChangeState(GunGeon::EnemyState::Enemy_Idle);
	//}
	// 타겟 놓침 -> Idle
	//if (!MActorManager::Instance().GetClassAttackBox()->IntersecRectScope(pPlayer))
	//{
	//	//충돌 안되었다면 상태 변경 ->idle
	//	mpFSM->ChangeState(GunGeon::EnemyState::Enemy_Idle);
	//	return;
	//}

	//추적 ( 타겟 방향으로 이동)
	//Vector2 dir = pPlayer->GetPosition() - pEnemy->GetPosition();
	//dir.Normalize();
	//pEnemy->SetPosition(pEnemy->GetPosition()+dir*pEnemy->GetSpeed() * dt);

	
	// ASTAR 알고리즘 으로 추적 ( 타겟 방향으로 이동)
	if (!MActorManager::Instance().GetTurn() &&pEnemy->GetActionTurn() < 1)
	{
		pEnemy->JoAStar_Move(dt);

		pEnemy->MoveStateCheck();
	}
	
}

void ChaseState::Exit()
{
	OutputDebugString(L"------------Exit Chase\n");
}

void AttackState::Enter()
{
	OutputDebugString(L"Enter Attack\n");
	//mfElapsedTime = 0.0f;
}

void AttackState::Execute(float dt)
{
	//플레이어, 소유자
	Enemy *pEnemy = (Enemy*)(this->mpFSM->GetOwner());
	Player *pPlayer = (Player*)pEnemy->GetTarget();

	mfElapsedTime += dt;
	MActorManager::Instance().SetMBVisible(false);
	//턴이 플레이어 일때 정지 상태로 변경
	if (MActorManager::Instance().GetTurn())
	{
		mpFSM->ChangeState(GunGeon::EnemyState::Enemy_Idle);
		mfElapsedTime = 0;
	}
	
	if (pPlayer->GetHealth() <= 0)
	{
		//플레이어팀이 죽었을때 타겟 변경
		MActorManager::Instance().CheckEnemyTarget();
		mpFSM->ChangeState(GunGeon::EnemyState::Enemy_Idle);
	}

	if (pEnemy->GetActionTurn() < 2 && mfElapsedTime > pEnemy->GetAttackDelay())
	{	
		pPlayer->OnHit(MActorManager::Instance().GetClassAttackBox(),
			MActorManager::Instance().GetClassAttackBox()->GetCharacter());
		MActorManager::Instance().SetAtVisible(false);
		MActorManager::Instance().SetMBVisible(false);
		mfElapsedTime = 0.0f;
	}


	//if (mfElapsedTime > pOwner->GetAttackDelay())
	//{
	//	pOwner->SetTarget(pPlayer);
	//	pOwner->Fire();
	//	
	//	mfElapsedTime = 0.0f;
	//}

	//
	//mpFSM->ChangeState(GunGeon::EnemyState::Enemy_Chase);
}

void AttackState::Exit()
{
	OutputDebugString(L"------------Exit Attack\n");
}
