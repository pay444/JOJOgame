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
	//Player *pPlayer = JoJoGun::Blackboard::Instance().GetPlayer();
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
		&& pEnemy->GetActionTurn() < 1)//&& pEnemy->GetActionTime()>0.7f)
	{
		//상태 전이
		mpFSM->ChangeState(JoJoGun::EnemyState::Enemy_Chase);

		auto moveBox = MActorManager::Instance().GetClassMoveBox();
		//적의 무브박스와 어텍박스 위치를 바꿔줌
		moveBox->SetPosition(pEnemy->GetPosition());
		moveBox->SetMoveDis(pEnemy->GetMoveDistance());
		moveBox->SetCharacter(pEnemy);
		moveBox->TileScope();
		moveBox->SetVisible(true);

		auto attackBox = MActorManager::Instance().GetClassAttackBox();
		//적의 어텍박스 위치를 바꿔줌
		attackBox->Release();
		attackBox->SetPosition(pEnemy->GetPosition());
		attackBox->SetAttackDis(pEnemy->GetAttackDistance());
		attackBox->AttackScope();
		attackBox->SetVisible(true);	
		attackBox->SetCharacter(pEnemy);
		attackBox->SetAttackDamge(attackBox->GetCharacter()->GetAttack());
		//정지도중에도 공격범위 안에 들어오면 
		if (attackBox->AIIntersecRectScope(pPlayer))
		{
			//충돌 되었다면 상태 변경 ->Attack
			//MActorManager::Instance().GetClassMoveBox()->SetVisible(false);
			mpFSM->ChangeState(JoJoGun::EnemyState::Enemy_Attack);
			return;
		}

		//플레이어주위 4칸과 적과의 거리중 가장 가까운곳의 
		//타일을 정해줌 못가게 되어있으면 안움직임
		vecPos.push_back(pPlayer->GetPosition());
		//XMFLOAT2 topPos = pPlayer->GetPosition() - XMFLOAT2(0.0f, 48.0f);
		//vecPos.push_back(topPos);
		//XMFLOAT2 botumPos = pPlayer->GetPosition() + XMFLOAT2(0.0f, 48.0f);
		//vecPos.push_back(botumPos);
		//XMFLOAT2 leftPos = pPlayer->GetPosition() - XMFLOAT2(48.0f, 0.0f);
		//vecPos.push_back(leftPos);
		//XMFLOAT2 rightPos = pPlayer->GetPosition() + XMFLOAT2(48.0f, 0.0f);
		//vecPos.push_back(rightPos);

		//0보다 작아지는 값은 제외 다른캐릭터가 올라와 있으면 제외
		//auto iter1 = vecPos.begin();
		//while (iter1 != vecPos.cend())
		//{
		//	Vector2 vec2 = Vector2(iter1->x,iter1->y);
		//	
		//	//화면 끝에 다다랐을때 예외처리
		//	auto a = pEnemy->GetTileIndex(vec2);
		//	
		//	if (iter1->x < 0 || iter1->y < 0)
		//	{
		//		iter1 = vecPos.erase(iter1);
		//	}
		//	else if (pEnemy->GetTileIndex(vec2) > 0 &&(*pVecTile)[pEnemy->GetTileIndex(vec2)]->byOption == 1)
		//	{
		//		iter1 = vecPos.erase(iter1);
		//	}
		//	else 
		//	{
		//		iter1++;
		//	}
		//}
		
		//적과 플레이어의 위치 dist 측정및 등록
		for (int i = 0; i < vecPos.size(); ++i)
		{
			float dist = Vector2::Distance(vecPos[i], pEnemy->GetPosition());
			Float2Andflaot faf;
			faf.pos = vecPos[i];
			faf.dist = dist;
			vecFloat2Dis.push_back(faf);
		}
		//std::sort(vecFloat2Dis.begin(), vecFloat2Dis.end(), less_than_dist());

		//플레이어와 적의 근접한 위치와 무브박스의 위치를 비교해서 넣어줌
		vector<int> vecindex;
		for (int i = 0; i < (*pVecMoveBoxIndex).size(); ++i)
		{
			float dist = Vector2::Distance(vecFloat2Dis[0].pos,
				(*pVecTile)[*(*pVecMoveBoxIndex)[i].get()]->vPos + XMFLOAT2(24.0f, 24.0f));//
			Float2Andflaot faf;
			faf.pos = (*pVecTile)[*(*pVecMoveBoxIndex)[i].get()]->vPos + XMFLOAT2(24.0f, 24.0f);//
			faf.dist = dist;
			int index = *(*pVecMoveBoxIndex)[i].get();
			//단 이동범위에 어떤 캐릭터가 올라와있으면 안됨
			if ((*pVecTile)[*(*pVecMoveBoxIndex)[i].get()]->underObject == 0)
			{
				vecindex.push_back(index);
				vecMoveBoxFloat2Dis.push_back(faf);
			}
		}
 		std::sort(vecMoveBoxFloat2Dis.begin(), vecMoveBoxFloat2Dis.end(), less_than_dist());

		//에너미의 AStar시작
		pEnemy->JoAstar_Start(pEnemy->GetPosition(), vecMoveBoxFloat2Dis[0].pos);//+XMFLOAT2(24.0f,24.0f)



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
	//Player *pPlayer = JoJoGun::Blackboard::Instance().GetPlayer();
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
		mpFSM->ChangeState(JoJoGun::EnemyState::Enemy_Idle);
	}

	// 사정거리 안에 돌아오면 -> Atack
	//if (dist < pEnemy->GetAttackRange())
	//{
	//	mpFSM->ChangeState(JoJoGun::EnemyState::Enemy_Attack);
	//	return;
	//}

	//범위 내에 플레이어 발견 턴이 허용하는 상태일때
	if (!MActorManager::Instance().GetTurn()
		&& pEnemy->GetActionTurn() == 1 && mfElapsedTime > pEnemy->GetMoveDelay())//dist < pEnemy->GetAttackRange()
	{
		mfElapsedTime = 0.0f;
		//적의 어텍박스 위치를 바꿔줌
		auto attackBox = MActorManager::Instance().GetClassAttackBox();
		attackBox->SetPosition(pEnemy->GetPosition());
		attackBox->SetAttackDis(pEnemy->GetAttackDistance());
		attackBox->AttackScope();
		attackBox->SetVisible(false);
		attackBox->SetCharacter(pEnemy);
		attackBox->SetAttackDamge(attackBox->GetCharacter()->GetAttack());
		//플레이어가 공격범위에 들어와있는지
		if (attackBox->AIIntersecRectScope(pPlayer))
		{
			//충돌 되었다면 상태 변경 ->Attack
			mpFSM->ChangeState(JoJoGun::EnemyState::Enemy_Attack);
			return;
		}
		else
		{
			//충돌이 안되었다면 행동턴 종료후 상태변경 ->Idle
			pEnemy->SetActionTurn(2);
			Color col = Colors::Gray;
			pEnemy->SetColor(col);
			attackBox->SetVisible(false);
			MActorManager::Instance().GetClassMoveBox()->SetVisible(false);
			mpFSM->ChangeState(JoJoGun::EnemyState::Enemy_Idle);
		}
		
	}


	// 타겟 놓침 -> Idle
	//if (dist > pEnemy->GetDetectRange())
	//{
	//	mpFSM->ChangeState(JoJoGun::EnemyState::Enemy_Idle);
	//}
	// 타겟 놓침 -> Idle
	//if (!MActorManager::Instance().GetClassAttackBox()->IntersecRectScope(pPlayer))
	//{
	//	//충돌 안되었다면 상태 변경 ->idle
	//	mpFSM->ChangeState(JoJoGun::EnemyState::Enemy_Idle);
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
	//OutputDebugString(L"------------Exit Chase\n");
}

void AttackState::Enter()
{
	//OutputDebugString(L"Enter Attack\n");
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
		mpFSM->ChangeState(JoJoGun::EnemyState::Enemy_Idle);
		mfElapsedTime = 0;
	}
	
	if (pPlayer->GetHealth() <= 0)
	{
		//플레이어팀이 죽었을때 타겟 변경
		MActorManager::Instance().CheckEnemyTarget();
		mpFSM->ChangeState(JoJoGun::EnemyState::Enemy_Idle);
	}

	if (pEnemy->GetActionTurn() < 2 && mfElapsedTime > pEnemy->GetAttackDelay())
	{	
		auto attackBox = MActorManager::Instance().GetClassAttackBox();
		attackBox->SetVisible(true);
		//떄린놈 음악
		//FMOD_System_PlaySound(MActorManager::Instance().GetFMODSystem(), (*MActorManager::Instance().GetVecFMODSound())[12], 0, 0, &(*MActorManager::Instance().GetVecFMODChannal())[1]);

		pPlayer->OnHit(attackBox,
			attackBox->GetCharacter());

		auto moveBox = MActorManager::Instance().GetClassMoveBox();
		moveBox->Release();
		//MActorManager::Instance().SetAtVisible(false);
		MActorManager::Instance().SetMBVisible(false);
		mfElapsedTime = 0.0f;
		
		//에너미 쪽에서 공격한 녀석을 찾기 위해서 바꿔줌
		attackBox->SetCharacter(pPlayer);
		//반격 하는 플래그 온
		(pEnemy)->SetCountAtFlag(true);

		//협공이 아닌이상 반격을 계속 할수있도록 변수 초기화
		pPlayer->SetisCountAction(false);

		//Color cr = Colors::Gray;
		//// 때린 캐릭터의 색깔이 변했다면 반격한다
		//if (attackBox->GetCharacter()->GetColor() == cr
		//	&& mCountChracter != nullptr
		//	&& ((Character*)mCountChracter)->GetHealth() > 0
		//	&& !((Character*)mCountChracter)->GetisCountAction())
		//{
		//	MActor* pPlayer = attackBox->GetCharacter();

		//	//GetClassAttackBox()->SetCharacter((Character*)mCountChracter);
		//	attackBox->Release();
		//	attackBox->SetPosition(((Character*)mCountChracter)->GetPosition());
		//	attackBox->SetAttackDis(((Character*)mCountChracter)->GetAttackDistance());
		//	attackBox->AttackScope();
		//	if (attackBox->AttackScopeSeekPick(pPlayer->GetPosition()))
		//	{
		//		pPlayer->OnHit(attackBox, mCountChracter);
		//		((Character*)mCountChracter)->SetisCountAction(true);
		//		((Character*)mCountChracter)->SetActionTurn(0);
		//		//mCountChracter = nullptr;
		//	}

		//}


	}


	//if (mfElapsedTime > pOwner->GetAttackDelay())
	//{
	//	pOwner->SetTarget(pPlayer);
	//	pOwner->Fire();
	//	
	//	mfElapsedTime = 0.0f;
	//}

	//
	//mpFSM->ChangeState(JoJoGun::EnemyState::Enemy_Chase);
	//mpFSM->ChangeState(JoJoGun::EnemyState::Enemy_Idle);
}

void AttackState::Exit()
{
	//OutputDebugString(L"------------Exit Attack\n");
}
