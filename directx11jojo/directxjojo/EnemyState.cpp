#include "stdafx.h"
#include "EnemyState.h"

void IdleState::Enter()
{
	OutputDebugString(L"Enter Idle\n");
}

void IdleState::Execute(float dt)
{
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
	//�÷��̾�, ������
	//Player *pPlayer = GunGeon::Blackboard::Instance().GetPlayer();
	//Enemy *pEnemy = (Enemy*)(this->mpFSM->GetOwner());

	//�÷��̾�, ������
	Enemy *pEnemy = (Enemy*)(this->mpFSM->GetOwner());
	Player *pPlayer = (Player*)pEnemy->GetTarget();

	if (pPlayer == NULL || pEnemy == NULL)
		return;

	vector<XMFLOAT2> vecPos;
	vector<Float2Andflaot> vecDis;


	//Ž���ؼ� �������� �÷��̾� �߰��ϸ� ��������
	//Ž�� (�÷��̾� <-> �� �Ÿ�)
	float dist = Vector2::Distance(pPlayer->GetPosition(), pEnemy->GetPosition());

	//���� ���� �÷��̾� �߰�
	if (dist < pEnemy->GetDetectRange() && !SGAActorManager::Instance().GetTurn()
		&& pEnemy->GetActionTurn() < 1)
	{
		//���� ����
		mpFSM->ChangeState(GunGeon::EnemyState::Enemy_Chase);

		////�÷��̾����� 4ĭ�� ������ �Ÿ��� ���� �������� 
		////Ÿ���� ������ ������ �Ǿ������� �ȿ�����
		//XMFLOAT2 topPos = pPlayer->GetPosition() - XMFLOAT2(0.0f, 48.0f);
		//vecPos.push_back(topPos);
		//XMFLOAT2 botumPos = pPlayer->GetPosition() + XMFLOAT2(0.0f, 48.0f);
		//vecPos.push_back(botumPos);
		//XMFLOAT2 leftPos = pPlayer->GetPosition() - XMFLOAT2(48.0f, 0.0f);
		//vecPos.push_back(leftPos);
		//XMFLOAT2 rightPos = pPlayer->GetPosition() + XMFLOAT2(48.0f, 0.0f);
		//vecPos.push_back(rightPos);

		////0���� �۾����� ���� ����
		//auto iter1 = vecPos.begin();
		//while (iter1 != vecPos.cend())
		//{
		//	if (iter1->x < 0 || iter1->y < 0)
		//	{
		//		iter1 = vecPos.erase(iter1);
		//	}
		//	else
		//	{
		//		iter1++;
		//	}
		//}

		//for (int i = 0; i < vecPos.size(); ++i)
		//{
		//	float dist = Vector2::Distance(vecPos[i], pEnemy->GetPosition());
		//	Float2Andflaot faf;
		//	faf.pos = vecPos[i];
		//	faf.dist = dist;
		//	vecDis.push_back(faf);
		//}
		//std::sort(vecDis.begin(), vecDis.end(), less_than_dist());

		////���ʹ��� AStar����
		////pEnemy->JoAstar_Start(pEnemy->GetPosition(), vecDis[0].pos);
		SGAActorManager::Instance().GetClassMoveBox()->SetPosition(pEnemy->GetPosition());
		SGAActorManager::Instance().GetClassMoveBox()->SetMoveDis(pEnemy->GetMoveDistance());
		SGAActorManager::Instance().GetClassMoveBox()->TileScope();
		SGAActorManager::Instance().GetClassMoveBox()->SetVisible(true);

		SGAActorManager::Instance().GetClassAttackBox()->SetPosition(pEnemy->GetPosition());
		SGAActorManager::Instance().GetClassAttackBox()->SetAttackDis(pEnemy->GetAttackDistance());
		SGAActorManager::Instance().GetClassAttackBox()->AttackScope();
		SGAActorManager::Instance().GetClassAttackBox()->SetVisible(true);

		//SGAActorManager::Instance().RePosAndVisiAt();

	}

	
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
	//�÷��̾�, ������
	//Player *pPlayer = GunGeon::Blackboard::Instance().GetPlayer();
	//Enemy *pEnemy = (Enemy*)(this->mpFSM->GetOwner());

	//�÷��̾�, ������
	Enemy *pEnemy = (Enemy*)(this->mpFSM->GetOwner());
	Player *pPlayer = (Player*)pEnemy->GetTarget();

	float fScrollx = ScrollMgr::Instance().GetScroll().x;
	float fScrolly = ScrollMgr::Instance().GetScroll().y;

	if (pPlayer == NULL || pEnemy == NULL)
		return;

	//�Ÿ�//Ž��
	float dist = Vector2::Distance(pEnemy->GetPosition(), pPlayer->GetPosition());

	//���� �÷��̾� �϶� ���� ���·� ����
	if (SGAActorManager::Instance().GetTurn())
	{
		mpFSM->ChangeState(GunGeon::EnemyState::Enemy_Idle);
	}

	// �����Ÿ� �ȿ� ���ƿ��� -> Atack
	//if (dist < pEnemy->GetAttackRange())
	//{
	//	mpFSM->ChangeState(GunGeon::EnemyState::Enemy_Attack);
	//	return;
	//}
	// Ÿ�� ��ħ -> Idle
	//if (dist > pEnemy->GetDetectRange())
	//{
	//	mpFSM->ChangeState(GunGeon::EnemyState::Enemy_Idle);
	//}
	//���� ( Ÿ�� �������� �̵�)
	//Vector2 dir = pPlayer->GetPosition() - pEnemy->GetPosition();
	//dir.Normalize();
	//pEnemy->SetPosition(pEnemy->GetPosition()+dir*pEnemy->GetSpeed() * dt);

	
	//ASTAR �˰��� ���� �̵� ����
	if (!SGAActorManager::Instance().GetTurn() &&pEnemy->GetActionTurn() < 1)
	{
		//pEnemy->JoAStar_Move(dt);

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
}

void AttackState::Execute(float dt)
{
	Enemy* pOwner = (Enemy*)(this->mpFSM->GetOwner());
	Player* pPlayer = GunGeon::Blackboard::Instance().GetPlayer();

	mfElapsedTime += dt;

	//���� �÷��̾� �϶� ���� ���·� ����
	if (SGAActorManager::Instance().GetTurn())
	{
		mpFSM->ChangeState(GunGeon::EnemyState::Enemy_Idle);
		mfElapsedTime = 0;
	}

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
