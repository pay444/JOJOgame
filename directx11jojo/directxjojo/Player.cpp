#include "stdafx.h"
#include "Player.h"


Player::Player()
{
}

Player::Player(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont)
	:Character(pBatch, pSheet, pFont),
	mvForward(0.0f, 0.0f)
{
}


Player::~Player()
{
}

void Player::Init(float moveSpeed, XMFLOAT2 startpos, E_SORTID eSortID)
{
	//Animation anim[] = {
	//	{ "DOWN", 2,{ { "jojoBWalk0", 0.3f },
	//	{ "jojoBWalk1", 0.3f }, }
	//	},
	//	{ "LEFT", 2,{ { "jojoLWalk0", 0.3f },
	//	{ "jojoLWalk1", 0.3f }, }
	//	},
	//	{ "RIGHT", 2,{ { "jojoRWalk0", 0.3f },
	//	{ "jojoRWalk1", 0.3f }, }
	//	},
	//	{ "UP", 2,{ { "jojoTWalk0", 0.3f },
	//	{ "jojoTWalk1", 0.3f }, }
	//	},
	//};
	//Animation anim[] = {
	//	{ "DOWN", 4,{ { "Hero01", 0.3f },
	//	{ "Hero02", 0.3f },
	//	{ "Hero03", 0.3f },
	//	{ "Hero02", 0.3f }, }
	//	},
	//	{ "LEFT", 4,{ { "Hero04", 0.3f },
	//	{ "Hero05", 0.3f },
	//	{ "Hero06", 0.3f },
	//	{ "Hero05", 0.3f }, }
	//	},
	//	{ "RIGHT", 4,{ { "Hero07", 0.3f },
	//	{ "Hero08", 0.3f },
	//	{ "Hero09", 0.3f },
	//	{ "Hero08", 0.3f }, }
	//	},
	//	{ "UP", 4,{ { "Hero10", 0.3f },
	//	{ "Hero11", 0.3f },
	//	{ "Hero12", 0.3f },
	//	{ "Hero11", 0.3f }, }
	//	},
	//};
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

	//mpUi = MActorManager::Instance().Create<UI>(mpBatch, mpSheet, mpFont);
	//mpUi->Init(E_SORTID_FIRST, mPosition, mVisbleScope);

}

E_SCENE Player::Update(float dt)
{

	Character::Update(dt);

	//���� �÷��̾��� ���϶� true �϶� �̵�����
	if (MActorManager::Instance().GetTurn())
	{
		UpdateMove(dt);
	}
	

	MoveStateCheck();

	//mpUi->SetVisible(mpMoveBox->GetSeekScope());
	//mpUi->SetPosition(mPosition+XMFLOAT2(100.0f, 0.0f));

	auto state = Keyboard::Get().GetState();

	if (state.R)
	{
		mActionTurn = 0;
	}
	if (state.D2)
		meScene = E_SCENE_LOGO;
	else if (state.D3)
		meScene = E_SCENE_STAGE;
	else
		meScene = E_SCENE_NONPASS;

	return meScene;

}


void Player::UpdateMove(float dt)
{
	float fScrollx = ScrollMgr::Instance().GetScroll().x;
	float fScrolly = ScrollMgr::Instance().GetScroll().y;


	/////////////////////////////���͸� �̿��� ������ �̵� �����ϱ�
	auto mouse = Mouse::Get().GetState();

	////XMVECTOR v; //v.�������� ���� ���� up���� ��� ������
	////XMFLOAT2 p; // p. �ϸ� x,y�� ����

	//XMFLOAT2 upFloat = XMFLOAT2(0.0f, -1.0f);
	//const XMVECTOR up = XMLoadFloat2(&upFloat);//�������̱� ������ �̷��� �����������
	//XMVECTOR dir = XMLoadFloat2(&XMFLOAT2(mouse.x, mouse.y));
	//dir = XMVector2Normalize(dir - XMLoadFloat2(&mPosition));
	//this->mvForward = XMFLOAT2(XMVectorGetX(dir), XMVectorGetY(dir));	//�̳����� ���� 17����
	//XMStoreFloat2(&mvForward, dir);//�����Ͱ� ���� float2�� vector�� �־��ִ� �Լ�

	//float angle = XMVectorGetX(XMVector2AngleBetweenNormals(up, dir));

	//if (angle < XM_PIDIV4)
	//	SetAnimation("UP");
	//else if (angle < XM_PIDIV4*3.0f)
	//	mPosition.x > mouse.x ? SetAnimation("LEFT") : SetAnimation("RIGHT");
	//else
	//	SetAnimation("DOWN");

	//���콺 Ŭ���� �̵�
	//if (MFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	//{
	//	mvForward = { 0.0f - ScrollMgr::Instance().GetScroll().x, 0.0f - ScrollMgr::Instance().GetScroll().y };
	//	XMVECTOR dir = XMLoadFloat2(&XMFLOAT2(mouse.x + fScrollx, mouse.y + fScrolly));
	//	mmousePos = XMFLOAT2( mouse.x + fScrollx,mouse.y + fScrolly);
	//	dir = XMVector2Normalize(dir - XMLoadFloat2(&mPosition));
	//	this->mvForward = XMFLOAT2(XMVectorGetX(dir), XMVectorGetY(dir));	//�̳����� ���� 17����
	//}

	if (MFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	{
		vector<unique_ptr<TILE>> *pVecTile = MActorManager::Instance().GetTileInfo();
		//mvForward = { 0.0f - ScrollMgr::Instance().GetScroll().x, 0.0f - ScrollMgr::Instance().GetScroll().y };
		//XMVECTOR dir = XMLoadFloat2(&XMFLOAT2(mouse.x + fScrollx, mouse.y + fScrolly));
		mmousePos = XMFLOAT2(mouse.x + fScrollx, mouse.y + fScrolly);
		//mPosition = XMFLOAT2(mPosition.x + fScrollx, mPosition.y + fScrolly);
		int mouseIndex = GetTileIndex(mmousePos);
		if (mouseIndex < 0)
			return;
		Vector2 vecMousePos = (*pVecTile)[mouseIndex]->vPos + XMFLOAT2(JOJOTILESX / 2, JOJOTILESY / 2);
		Vector2 vecPos = mPosition;

		float distance = Vector2::Distance(vecPos, vecMousePos);

		//���콺�� �ɸ��Ͱ� �Ÿ������� �̵� ����
		if ((mVisbleScope))//distance <= limitDistance&&
		{
			//�ش������ �ش��ؾ� �˰��� ����
			if (MActorManager::Instance().GetMBSeekScope() && mActionTurn < 1)//MoveBox::Instance().GetSeekScope()
			{
				//ASTAR �˰��� ��Ʈ ¥�� ����
				JoAstar_Start(mPosition, mmousePos);
			}

		}

		//dir = XMVector2Normalize(dir - XMLoadFloat2(&mPosition));
		//this->mvForward = XMFLOAT2(XMVectorGetX(dir), XMVectorGetY(dir));	
	}

	//�������� �����ϸ� mvForward�� 0���� ������ش�
	//if ((mPosition.x == mvForward.x * this->mfMoveSpeed * dt) && (mPosition.y == mvForward.y * this->mfMoveSpeed * dt))
	//	mvForward = { 0.f,0.f };
	//if ((mPosition.x >= (mmousePos.x-10)) && (mPosition.x <= (mmousePos.x+10)) &&
	//	(mPosition.y >= (mmousePos.y - 10)) && (mPosition.y <= (mmousePos.y + 10))
	//	)
	//	mvForward = { 0.f,0.f };

	//�������� �̵�
	if (mActionTurn < 1)
	{
		JoAStar_Move(dt);
	}
	
	if (MFramework::mMouseTracker.rightButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	{
		//MActorManager::Instance().SetMBVisible(false);
		////MoveBox::Instance()->SetVisible(false);
		//MActorManager::Instance().SetUIVisible(false);
		//MActorManager::Instance().SetAtVisible(false);
		//MActorManager::Instance().SetClickCount(0);
		//mActionTurn = 0;
	}
	list<int>* pBestList = mpJoAStar->GetBestList();

	if (pBestList->size() == 0)
	{
		//mActionTurn++;
		//MActorManager::Instance().GetClassUi()->SetPosition(mPosition + XMFLOAT2(100.0f, 0.0f));
		//MActorManager::Instance().GetClassAttackBox()->SetPosition(mPosition);
		//MActorManager::Instance().SetAtVisible(true);
		//MActorManager::Instance().SetMBVisible(false);
		//MActorManager::Instance().SetUIVisible(true);
	}


	//auto state = Keyboard::Get().GetState();


	//JoAstar_Start(mPosition)

	//this->mPosition = mPosition + (mvForward * this->mfMoveSpeed * dt);

}

