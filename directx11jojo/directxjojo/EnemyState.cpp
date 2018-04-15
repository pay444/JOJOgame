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

	//�÷��̾�, ������
	//Player *pPlayer = GunGeon::Blackboard::Instance().GetPlayer();
	//Enemy *pEnemy = (Enemy*)(this->mpFSM->GetOwner());

	//�÷��̾�, ������
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
	//	//����Ʈ �����͸� �����ö� ���� ��Ȯ�� �������
	//	auto c = (*pVecMoveBoxIndex).begin()[i].get();
	//	int a = *(*pVecMoveBoxIndex)[i].get();
	//	
	//}

	
	//Ž���ؼ� �������� �÷��̾� �߰��ϸ� ��������
	//Ž�� (�÷��̾� <-> �� �Ÿ�)
	float dist = Vector2::Distance(pPlayer->GetPosition(), pEnemy->GetPosition());

	//���� ���� �÷��̾� �߰�
	if (dist < pEnemy->GetDetectRange() && !MActorManager::Instance().GetTurn()
		&& pEnemy->GetActionTurn() < 1&& pEnemy->GetActionTime()>0.7f)
	{
		//���� ����
		mpFSM->ChangeState(GunGeon::EnemyState::Enemy_Chase);

		//���� ����ڽ��� ���عڽ� ��ġ�� �ٲ���
		MActorManager::Instance().GetClassMoveBox()->SetPosition(pEnemy->GetPosition());
		MActorManager::Instance().GetClassMoveBox()->SetMoveDis(pEnemy->GetMoveDistance());
		MActorManager::Instance().GetClassMoveBox()->TileScope();
		MActorManager::Instance().GetClassMoveBox()->SetVisible(true);

		//���� ���عڽ� ��ġ�� �ٲ���
		MActorManager::Instance().GetClassAttackBox()->Release();
		MActorManager::Instance().GetClassAttackBox()->SetPosition(pEnemy->GetPosition());
		MActorManager::Instance().GetClassAttackBox()->SetAttackDis(pEnemy->GetAttackDistance());
		MActorManager::Instance().GetClassAttackBox()->AttackScope();
		MActorManager::Instance().GetClassAttackBox()->SetVisible(true);	
		MActorManager::Instance().GetClassAttackBox()->SetCharacter(pEnemy);
		MActorManager::Instance().GetClassAttackBox()->SetAttackDamge
		(MActorManager::Instance().GetClassAttackBox()->GetCharacter()->GetAttack());

		//�������߿��� ���ݹ��� �ȿ� ������ 
		if (MActorManager::Instance().GetClassAttackBox()->AIIntersecRectScope(pPlayer))
		{
			//�浹 �Ǿ��ٸ� ���� ���� ->Attack
			//MActorManager::Instance().GetClassMoveBox()->SetVisible(false);
			mpFSM->ChangeState(GunGeon::EnemyState::Enemy_Attack);
			return;
		}

		//�÷��̾����� 4ĭ�� ������ �Ÿ��� ���� �������� 
		//Ÿ���� ������ ������ �Ǿ������� �ȿ�����
		XMFLOAT2 topPos = pPlayer->GetPosition() - XMFLOAT2(0.0f, 48.0f);
		vecPos.push_back(topPos);
		XMFLOAT2 botumPos = pPlayer->GetPosition() + XMFLOAT2(0.0f, 48.0f);
		vecPos.push_back(botumPos);
		XMFLOAT2 leftPos = pPlayer->GetPosition() - XMFLOAT2(48.0f, 0.0f);
		vecPos.push_back(leftPos);
		XMFLOAT2 rightPos = pPlayer->GetPosition() + XMFLOAT2(48.0f, 0.0f);
		vecPos.push_back(rightPos);

		//0���� �۾����� ���� ���� �ٸ�ĳ���Ͱ� �ö�� ������ ����
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
		
		//���� �÷��̾��� ��ġ dist ������ ���
		for (int i = 0; i < vecPos.size(); ++i)
		{
			float dist = Vector2::Distance(vecPos[i], pEnemy->GetPosition());
			Float2Andflaot faf;
			faf.pos = vecPos[i];
			faf.dist = dist;
			vecFloat2Dis.push_back(faf);
		}
		std::sort(vecFloat2Dis.begin(), vecFloat2Dis.end(), less_than_dist());

		//�÷��̾�� ���� ������ ��ġ�� ����ڽ��� ��ġ�� ���ؼ� �־���
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

		//���ʹ��� AStar����
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
	if (MActorManager::Instance().GetTurn())
	{
		mpFSM->ChangeState(GunGeon::EnemyState::Enemy_Idle);
	}

	// �����Ÿ� �ȿ� ���ƿ��� -> Atack
	//if (dist < pEnemy->GetAttackRange())
	//{
	//	mpFSM->ChangeState(GunGeon::EnemyState::Enemy_Attack);
	//	return;
	//}

	//���� ���� �÷��̾� �߰� ���� ����ϴ� �����϶�
	if (!MActorManager::Instance().GetTurn()
		&& pEnemy->GetActionTurn() == 1 && mfElapsedTime > pEnemy->GetMoveDelay())//dist < pEnemy->GetAttackRange()
	{
		mfElapsedTime = 0.0f;
		//���� ���عڽ� ��ġ�� �ٲ���
		MActorManager::Instance().GetClassAttackBox()->Release();
		MActorManager::Instance().GetClassAttackBox()->SetPosition(pEnemy->GetPosition());
		MActorManager::Instance().GetClassAttackBox()->SetAttackDis(pEnemy->GetAttackDistance());
		MActorManager::Instance().GetClassAttackBox()->AttackScope();
		MActorManager::Instance().GetClassAttackBox()->SetVisible(false);
		MActorManager::Instance().GetClassAttackBox()->SetCharacter(pEnemy);
		MActorManager::Instance().GetClassAttackBox()->SetAttackDamge(MActorManager::Instance().GetClassAttackBox()->GetCharacter()->GetAttack());
		//�÷��̾ ���ݹ����� �����ִ���
		if (MActorManager::Instance().GetClassAttackBox()->AIIntersecRectScope(pPlayer))
		{
			//�浹 �Ǿ��ٸ� ���� ���� ->Attack
			mpFSM->ChangeState(GunGeon::EnemyState::Enemy_Attack);
			return;
		}
		else
		{
			//�浹�� �ȵǾ��ٸ� �ൿ�� ������ ���º��� ->Idle
			pEnemy->SetActionTurn(2);
			MActorManager::Instance().GetClassAttackBox()->SetVisible(false);
			MActorManager::Instance().GetClassMoveBox()->SetVisible(false);
			mpFSM->ChangeState(GunGeon::EnemyState::Enemy_Idle);
		}
		
	}


	// Ÿ�� ��ħ -> Idle
	//if (dist > pEnemy->GetDetectRange())
	//{
	//	mpFSM->ChangeState(GunGeon::EnemyState::Enemy_Idle);
	//}
	// Ÿ�� ��ħ -> Idle
	//if (!MActorManager::Instance().GetClassAttackBox()->IntersecRectScope(pPlayer))
	//{
	//	//�浹 �ȵǾ��ٸ� ���� ���� ->idle
	//	mpFSM->ChangeState(GunGeon::EnemyState::Enemy_Idle);
	//	return;
	//}

	//���� ( Ÿ�� �������� �̵�)
	//Vector2 dir = pPlayer->GetPosition() - pEnemy->GetPosition();
	//dir.Normalize();
	//pEnemy->SetPosition(pEnemy->GetPosition()+dir*pEnemy->GetSpeed() * dt);

	
	// ASTAR �˰��� ���� ���� ( Ÿ�� �������� �̵�)
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
	//�÷��̾�, ������
	Enemy *pEnemy = (Enemy*)(this->mpFSM->GetOwner());
	Player *pPlayer = (Player*)pEnemy->GetTarget();

	mfElapsedTime += dt;
	MActorManager::Instance().SetMBVisible(false);
	//���� �÷��̾� �϶� ���� ���·� ����
	if (MActorManager::Instance().GetTurn())
	{
		mpFSM->ChangeState(GunGeon::EnemyState::Enemy_Idle);
		mfElapsedTime = 0;
	}
	
	if (pPlayer->GetHealth() <= 0)
	{
		//�÷��̾����� �׾����� Ÿ�� ����
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
