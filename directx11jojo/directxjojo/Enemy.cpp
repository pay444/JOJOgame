#include "stdafx.h"
#include "Enemy.h"


Enemy::Enemy()
{
}

Enemy::Enemy(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont)
	: Character(pBatch, pSheet, pFont),
	mpTarget(nullptr),
	mFontVisible(false),
	mCode(0),
	mActionBool(false)
{
}


Enemy::~Enemy()
{
}

void Enemy::Init(float moveSpeed, XMFLOAT2 startpos, E_SORTID eSortID)
{

	Character::Init();

	//MActor::Init(anim, 4, eSortID);
	mAnimName2 = "DOWN";
	SetAnimation(mAnimName2);
	SetPosition(startpos);
	mfMoveSpeed = moveSpeed;
	//mpJoAStar = make_unique<AStar>();

	//�ʱ� ��ġ�� ������ ���� ���ֱ�
	vector<unique_ptr<TILE>> * pVecTile = MActorManager::Instance().GetTileInfo();
	int istartIndex = GetTileIndex(mPosition);
	(*pVecTile)[istartIndex]->underObject = 1;
	(*pVecTile)[istartIndex]->byOption = 1;
	mPosition = XMFLOAT2((*pVecTile)[istartIndex]->vPos.x + JOJOTILESX / 2,
		(*pVecTile)[istartIndex]->vPos.y + JOJOTILESY / 2);

	mspFSM = make_unique<SGAFSM>();
	mspFSM->SetOwner(this);
	mspFSM->AddState<IdleState>(GunGeon::EnemyState::Enemy_Idle);
	mspFSM->AddState<ChaseState>(GunGeon::EnemyState::Enemy_Chase);
	mspFSM->AddState<AttackState>(GunGeon::EnemyState::Enemy_Attack);
	mspFSM->ChangeState(GunGeon::EnemyState::Enemy_Idle);

}

E_SCENE Enemy::Update(float dt)
{
	Character::Update(dt);
	

	//�ı��Ǿ����ÿ� vectorindex �� ��ϵǾ��ִ� ������ ������
	if (mbDestroyed)
	{
		auto vecEnemyIndex = MActorManager::Instance().GetVecEnemyIndex();
		auto iter = vecEnemyIndex->begin();
		while (iter != vecEnemyIndex->cend())
		{
			if (*(*iter).get() == mCode)
			{
				mActionTurn = 2;
				iter->reset();
				iter = vecEnemyIndex->erase(iter);
				MActorManager::Instance().SetEnemyControllCount(0);
				return E_SCENE_NONPASS;
			}
			else
			{
				++iter;
			}
		}
	}
	//���� ���϶��� �����
	if (mActionBool&&mHealth>0)
	{
		mspFSM->Update(dt);
	}

	
	//���� ���� �������� Manager ���� �ٸ��� ������¶�� �˷���
	if (mActionTurn >= 2 && mActionBool)
	{
		mColor = Colors::Gray;
		mActionBool = false;
		auto vecEnemyIndex = MActorManager::Instance().GetVecEnemyIndex();
		int temp = MActorManager::Instance().GetEnemyControllCount();
		MActorManager::Instance().SetEnemyControllCount(++temp);
		if (MActorManager::Instance().GetEnemyControllCount() >=
			MActorManager::Instance().GetEnemyCount())
		{
			MActorManager::Instance().SetEnemyControllCount(0);
		}
		//int intCode = *(*vecEnemyIndex)[MActorManager::Instance().GetEnemyControllCount()].get();
		mspFSM->ChangeState(GunGeon::EnemyState::Enemy_Idle);
		//MActorManager::Instance().SetEnemyControllCount(intCode);

	}


	auto state = Keyboard::Get().GetState();

	if (state.R)
	{
		mActionTurn = 0;
	}

	//if (state.D2)
	//	meScene = E_SCENE_LOGO;
	//else if (state.D3)
	//	meScene = E_SCENE_STAGE;
	//else
	//	meScene = E_SCENE_NONPASS;

	//return meScene;

	return E_SCENE_NONPASS;
}

void Enemy::Draw()
{
	Character::Draw();

	auto state = Keyboard::Get().GetState();

	if (state.Q)
	{
		mFontVisible = !mFontVisible;
	}
	if (mFontVisible)
	{
		//AI ���� ���
		wchar_t wch[128];
		
		switch (mspFSM->GetStateID())
		{
		case 0:
			swprintf_s(wch,L"Enemy_Idle");
			break;
		case 1:
			swprintf_s(wch,L"Enemy_Chase");
			break;
		case 2:
			swprintf_s(wch,L"Enemy_Attack");
			break;
		default:
			swprintf_s(wch,L"No_Enemy_StateID");
			break;
		}
		//swprintf_s(wch, L"%d", mspFSM->GetStateID());
		mpFont->DrawString(mpBatch, wch, XMFLOAT2(mPosition + XMFLOAT2(0.0f, 25.0f)), DirectX::Colors::Black, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.5f, 0.5f));
	}
}
