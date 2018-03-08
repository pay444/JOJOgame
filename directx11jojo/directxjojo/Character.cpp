#include "stdafx.h"
#include "Character.h"


Character::Character()
{
	InitEffect();
	mStartIndex = 0;
}

Character::Character(SpriteBatch * pBatch, SGASpriteSheet * pSheet, SpriteFont * pFont)
	: SGAActor(pBatch, pSheet, pFont),
	mFontVisible(false),
	mColor(Colors::White),
	mColorAllow(false)
{
	InitEffect();
	mStartIndex = 0;
	mVisbleScope = false;
	mActionTurn = 0;
	//Animation anim[] = {
	//	{ "CursorBox", 2,{ { "CursorBox0", 0.3f },
	//	{ "CursorBox1", 0.3f }, }
	//	},
	//};
	//SGAActor::Init(anim, 1);

}

Character::Character(SpriteBatch * pBatch)
{
	InitEffect();
	mStartIndex = 0;

}


Character::~Character()
{
}

void Character::Init()
{
	mpJoAStar = make_unique<AStar>();
	//mpMoveBox = make_unique<MoveBox>();

	auto pTexture = SGAResourceManager::Instance().GetShaderResource(L"Images\\jojo\\jojoSprites.png");
	auto pSheet = SGAResourceManager::Instance().GetSpriteSheet(L"Images\\jojo\\jojoSprites.xml", pTexture);
	//auto pMoveBox = SGAActorManager::Instance().Create<MoveBox>(mpBatch, mpSheet, mpFont);
	//mpMoveBox = SGAActorManager::Instance().Create<MoveBox>(mpBatch, mpSheet, mpFont);
	//mpMoveBox->Init(E_SORTID_FIRST, mPosition, limitDistance,mVisbleScope,mMoveDistance);
}

E_SCENE Character::Update(float dt)
{

	float fScrollx = ScrollMgr::Instance().GetScroll().x;
	float fScrolly = ScrollMgr::Instance().GetScroll().y;
	const vector<unique_ptr<TILE>>* spVecTile = SGAActorManager::Instance().GetTileInfo();

	//마우스 버튼을 눌렀을때 그 타일위에 어떤 오브젝트가 있는지 확인
	if (SGAFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	{
		auto mouse = Mouse::Get().GetState();

		Vector2 mousePos = Vector2(mouse.x + fScrollx, mouse.y + fScrolly);
		int mouseIndex = GetTileIndex(mousePos);
		int posIndex = GetTileIndex(mPosition);

		int uiPlayerPosIndex = -1;
		if (SGAActorManager::Instance().GetClassUi()->GetPlayer() != nullptr)
		{
			uiPlayerPosIndex = GetTileIndex(SGAActorManager::Instance().GetClassUi()->GetPlayer()->GetPosition());
		}
		if (mouseIndex == posIndex || posIndex == uiPlayerPosIndex)
		{
			mVisbleScope = true;
			//SGAActorManager::Instance().SetMBVisible(mVisbleScope);
			//SGAActorManager::Instance().SetMBVisible(mVisbleScope);
			//mpMoveBox->SetVisible(mVisbleScope);
			//mpMoveBox->SetPosition(mPosition);
		}
		else
		{
			mVisbleScope = false;
			//SGAActorManager::Instance().SetClickCount(0)
			//SGAActorManager::Instance().SetMBVisible(mVisbleScope);
			//mpMoveBox->SetVisible(mVisbleScope);	
		}

	}
	//auto state = Keyboard::Get().GetState();

	//if (state.F)
	//{
	//	this->SetAnimation("DEAD");
	//}

	//죽음 시간측정후 없어지도록
	if (this->mHealth <= 0)
	{
		if (mfActionElapsedTime > 2.2f)
		{
			this->SetDestroyed();
			mfActionElapsedTime = 0.0f;
			return E_SCENE_NONPASS;
		}
	}
	else if (mfActionElapsedTime > 2.8f)
	{
		SetAnimation(mAnimName2);
		mfActionElapsedTime = 0.0f;
	}
	E_SCENE eResult = SGAActor::Update(dt);

	mspShake->Update(dt);

	mspTint->Update(dt);

	if (eResult > E_SCENE_NONPASS)
		return eResult;

	return E_SCENE_NONPASS;
}

void Character::Draw()
{
	XMFLOAT2 offset = XMFLOAT2(0, 0);
	Color tint = Colors::White;
	
	//offset = Vector2(0.0f, 0.0f);
	//float speed = 48;

	//auto state = Keyboard::Get().GetState();

	////타일이동
	//if (state.D1)
	//	offset.y = speed;

	if (mspShake->IsActive())
	{
		offset = mspShake->GetValue();
	}
	if (mspTint->IsActive())
	{
		tint = this->mspTint->GetValue();
	}


	if (mpBatch == NULL || mpSheet == NULL)
		return;
	if (mpSpriteFrame == NULL)
		return;

	offset.x += (int)ScrollMgr::Instance().GetScroll().x;
	offset.y += (int)ScrollMgr::Instance().GetScroll().y;



	//공격 모션후 색깔 딜레이주기
	if (mfActionElapsedTime > 0.8f)
	{
		SetAnimation(mAnimName2);
		if (mColorAllow && mActionTurn >= 2)
		{
			mColorAllow = false;
			mColor = Colors::Gray;
			//tint = mColor;
			mfActionElapsedTime = 0.0f;
		}
		else if(mActionTurn < 2)
		{
			mColor = Colors::White;
			//mfActionElapsedTime = 0.0f;
		}
		
	}

	if (mActionTurn >= 2)
	{
		tint = mColor;
	}


	mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition - offset, tint);

	//wchar_t wch[128];

	//swprintf_s(wch, L"%d", mfActionElapsedTime);
	//mpFont->DrawString(mpBatch, wch, XMFLOAT2(mPosition + XMFLOAT2(0.0f, 25.0f)), DirectX::Colors::Black, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.5f, 0.5f));

	//if (mVisbleScope)
	//{
	//	XMFLOAT2 a = { 100, 100 };
	//	//char * abc;
	//	mAnimName = "CursorBox";
	//	this->SetAnimation(mAnimName);

	//	//Character::Draw();
	//	mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition - offset + a, tint);
	//}

}

void Character::OnHit(SGAActor * pCollidee)
{
	if (this->GetCamp() != pCollidee->GetCamp())
	{
		DoDamage(pCollidee);
	}
}

void Character::OnHit(SGAActor * pCollider, SGAActor * pCollidee)
{
	if (this->GetCamp() != pCollidee->GetCamp())
	{

		//때린놈의 액션턴을 바꿔준다.
		((Character*)pCollidee)->SetActionTurn(2);
		//때린놈의 색깔을 바꿔준다.
		((Character*)pCollidee)->SetColorAllow(true);
		
		//때린놈의 애니메이션 상태 변경
		const vector<unique_ptr<TILE>>* pVecTile = SGAActorManager::Instance().GetTileInfo();
		int attackerIndex = GetTileIndex(pCollidee->GetPosition());
		int defenderIndex = GetTileIndex(mPosition);
		//아래 애니매이션 변경
		if (mPosition.y >
			(*pVecTile)[attackerIndex]->vPos.y + JOJOTILESY) //+ JOJOTILESY / 2 + JOJOTILESY / 2)
		{
			pCollidee->SetAnimation("DATTACK");
			((Character*)pCollidee)->SetAniName("DOWN");
		}
		//위 애니매이션 변경
		else if (mPosition.y <
			(*pVecTile)[attackerIndex]->vPos.y)// + JOJOTILESY / 2 - JOJOTILESY / 2)
		{
			pCollidee->SetAnimation("UATTACK");
			((Character*)pCollidee)->SetAniName("UP");
		}

		//오른쪽과 왼쪽 변경
		else if ((mPosition.x <
			(*pVecTile)[attackerIndex]->vPos.x))
			//&&(mPosition.x != (*pVecTile)[iDestinationIdx]->vPos.x))
		{
			pCollidee->SetAnimation("LATTACK");
			((Character*)pCollidee)->SetAniName("LEFT");
		}
		else if (mPosition.x >
			(*pVecTile)[attackerIndex]->vPos.x + JOJOTILESX)
		{
			pCollidee->SetAnimation("RATTACK");
			((Character*)pCollidee)->SetAniName("RIGHT");
		}
		//때린놈 행동 시간 초기화
		pCollidee->SetActionTime(0.0f);

		//mActionTurn++;
		DoDamage(pCollider);
	}
}

void Character::DoDamage(SGAActor * pAttacker)
{
	//AttackBox* pCharacter = (AttackBox*)pAttacker;
	this->mHealth -= ((AttackBox*)pAttacker)->GetAttack();

	mspShake->Start(0.1f, 5);
	mspTint->Start(0.1f, (Color)Colors::Wheat, (Color)Colors::Red);

	this->SetAnimation("HIT");
	mfActionElapsedTime = 0.0f;
	//SGAActorManager::Instance().SetUiCheck(false);
	if (this->mHealth <= 0)
	{
		mAnimName2 = "DEAD";
		this->SetAnimation(mAnimName2);
		mfActionElapsedTime = 0.0f;
		//this->SetDestroyed();
		
	}
		
}

void Character::MoveStateCheck()
{
	list<int>* pBestList = mpJoAStar->GetBestList();

	if (pBestList->empty())	// 비어있다면..
		return;

	const vector<unique_ptr<TILE>>* pVecTile = SGAActorManager::Instance().GetTileInfo();

	int iDestinationIdx = pBestList->front();

	//int iDestinationIdx1 = pBestList->begin();
	//위 애니매이션 변경
	if (mPosition.y >
		(*pVecTile)[iDestinationIdx]->vPos.y + JOJOTILESY) //+ JOJOTILESY / 2 + JOJOTILESY / 2)
	{
		mAnimName2 = "UP";
		SetAnimation(mAnimName2);
	}
	//아래 애니매이션 변경
	else if (mPosition.y <
		(*pVecTile)[iDestinationIdx]->vPos.y)// + JOJOTILESY / 2 - JOJOTILESY / 2)
	{
		mAnimName2 = "DOWN";
		SetAnimation(mAnimName2);
	}

	//왼쪽과 오른쪽 변경
	else if ((mPosition.x <
		(*pVecTile)[iDestinationIdx]->vPos.x))
		//&&(mPosition.x != (*pVecTile)[iDestinationIdx]->vPos.x))
	{
		mAnimName2 = "RIGHT";
		SetAnimation(mAnimName2);
	}
	else if (mPosition.x >
		(*pVecTile)[iDestinationIdx]->vPos.x + JOJOTILESX)
	{
		mAnimName2 = "LEFT";
		SetAnimation(mAnimName2);
	}

	//(*pVecTile)[iDestinationIdx]->vPos.x = 0;
	//(*pVecTile)[iDestinationIdx]->vPos.x = 0;
}

void Character::InitEffect()
{
	mspShake = make_unique<EffectShake>();
	mspTint = make_unique<EffectTint>();
}

void Character::JoAstar_Start(const Vector2 & vDestPos, const Vector2 & vSorcePos)
{

	const vector<unique_ptr<TILE>>* pVecTile = SGAActorManager::Instance().GetTileInfo();

	int iStartIdx = GetTileIndex(vDestPos);
	mStartIndex = iStartIdx;
	//(*pVecTile)[iStartIdx]->underObject = 0;

	if (iStartIdx < 0)
		return;

	int iGoalIdx = GetTileIndex(vSorcePos);

	if (iGoalIdx < 0)
		return;


	// 길찾기 시작 !
	mpJoAStar->AStarStat(iStartIdx, iGoalIdx);

}

bool Character::JoAStar_Move(float dt)
{
	list<int>* pBestList = mpJoAStar->GetBestList();

	if (pBestList->empty())	// 비어있다면..
		return false;

	const vector<unique_ptr<TILE>>* pVecTile = SGAActorManager::Instance().GetTileInfo();

	int iDestinationIdx = pBestList->front();

	//플레이어와 타일간의 거리 구하기 
	XMVECTOR vDir;
	//XMStoreFloat2(&mPosition, vPosi);
	Vector2 vPosi = mPosition;
	Vector2 test;
	test.x = (*pVecTile)[iDestinationIdx]->vPos.x + JOJOTILESX / 2;
	test.y = (*pVecTile)[iDestinationIdx]->vPos.y + JOJOTILESY / 2;
	vDir = test - vPosi;

	XMFLOAT2 float2 = { XMVectorGetX(vDir),XMVectorGetY(vDir) };

	float fDistance = sqrt(pow(float2.x, 2) + pow(float2.y, 2));
	vDir = XMVector2Normalize(vDir);

	mPosition.x += XMVectorGetX(vDir) * mfMoveSpeed * dt;
	mPosition.y += XMVectorGetY(vDir) * mfMoveSpeed * dt;

	if (((mPosition.x >= (*pVecTile)[iDestinationIdx]->vPos.x + JOJOTILESX / 2 - JOJOTILESX / 16) &&
		(mPosition.x <= (*pVecTile)[iDestinationIdx]->vPos.x + JOJOTILESX / 2 + JOJOTILESX / 16) &&
		(mPosition.y >= (*pVecTile)[iDestinationIdx]->vPos.y + JOJOTILESX / 2 - JOJOTILESY / 16) &&
		(mPosition.y <= (*pVecTile)[iDestinationIdx]->vPos.y + JOJOTILESX / 2 + JOJOTILESY / 16)))
	{
		mPosition.x = (*pVecTile)[iDestinationIdx]->vPos.x + JOJOTILESX / 2;
		mPosition.y = (*pVecTile)[iDestinationIdx]->vPos.y + JOJOTILESY / 2;
	}

	//타일 위에 케릭터가 있는지 없는지
	if (pBestList->size() == 1)
	{
		(*pVecTile)[pBestList->back()]->underObject = 1;
		(*pVecTile)[pBestList->back()]->byOption = 1;
		(*pVecTile)[mStartIndex]->underObject = 0;
		(*pVecTile)[mStartIndex]->byOption = 0;
	}
	else
	{
		(*pVecTile)[mStartIndex]->underObject = 1;
		(*pVecTile)[mStartIndex]->byOption = 1;
		(*pVecTile)[iDestinationIdx]->underObject = 0;
		(*pVecTile)[iDestinationIdx]->byOption = 0;
	}

	if (fDistance < 5.f)
	{
		pBestList->pop_front();
	}

	if (pBestList->size() == 0)
	{
		mActionTurn++;
		if (GetCamp() == GunGeon::CampType::PLAYER)
		{
			SGAActorManager::Instance().GetClassUi()->SetPosition(mPosition + XMFLOAT2(100.0f, 0.0f));
			SGAActorManager::Instance().GetClassUi()->SetVisible(true);

			SGAActorManager::Instance().GetClassAttackBox()->SetPosition(mPosition);
			SGAActorManager::Instance().GetClassAttackBox()->SetVisible(true);

			SGAActorManager::Instance().SetMBVisible(false);
			//SGAActorManager::Instance().SetClickCount(0);
		}
		if (GetCamp() == GunGeon::CampType::MONSTER)
		{
			SGAActorManager::Instance().GetClassAttackBox()->SetVisible(false);

			SGAActorManager::Instance().SetMBVisible(false);

			SGAActorManager::Instance().GetClassMoveBox()->Release();
			SGAActorManager::Instance().GetClassAttackBox()->Release();
		}
	}
	mVisbleScope = false;
	//mpMoveBox->SetVisible(mVisbleScope);

	return true;
	//return false;
}
