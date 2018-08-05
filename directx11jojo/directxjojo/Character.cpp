#include "stdafx.h"
#include "Character.h"


Character::Character()
{
	InitEffect();
	mStartIndex = 0;
}

Character::Character(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont)
	: MActor(pBatch, pSheet, pFont),
	mFontVisible(false),
	mColor(Colors::White),
	mColorAllow(false),
	mbCountAction(false)
{
	InitEffect();
	mStartIndex = 0;
	mVisbleScope = false;
	mActionTurn = 0;
	mMotion = false;
	mbSkillActionFlag = false;
	mbMotionFlag = false;
	mfSkillActionElpTime = 0.0f;
	mfDelayMotionTime = 0.8;
	//Animation anim[] = {
	//	{ "CursorBox", 2,{ { "CursorBox0", 0.3f },
	//	{ "CursorBox1", 0.3f }, }
	//	},
	//};
	//MActor::Init(anim, 1);

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

	auto pTexture = ResourceManager::Instance().GetShaderResource(L"Images\\jojo\\jojoSprites.png");
	auto pSheet = ResourceManager::Instance().GetSpriteSheet(L"Images\\jojo\\jojoSprites.xml", pTexture);
	//auto pMoveBox = MActorManager::Instance().Create<MoveBox>(mpBatch, mpSheet, mpFont);
	//mpMoveBox = MActorManager::Instance().Create<MoveBox>(mpBatch, mpSheet, mpFont);
	//mpMoveBox->Init(E_SORTID_FIRST, mPosition, limitDistance,mVisbleScope,mMoveDistance);
}

E_SCENE Character::Update(float dt)
{
	mfActionElapsedTime += dt;
	float fScrollx = ScrollMgr::Instance().GetScroll().x;
	float fScrolly = ScrollMgr::Instance().GetScroll().y;
	const vector<unique_ptr<TILE>>* spVecTile = MActorManager::Instance().GetTileInfo();

	//마우스 버튼을 눌렀을때 그 타일위에 어떤 오브젝트가 있는지 확인
	if (MFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	{
		auto mouse = Mouse::Get().GetState();

		Vector2 mousePos = Vector2(mouse.x + fScrollx, mouse.y + fScrolly);
		int mouseIndex = GetTileIndex(mousePos);
		int posIndex = GetTileIndex(mPosition);

		int uiPlayerPosIndex = -1;
		if (MActorManager::Instance().GetClassUi()->GetPlayer() != nullptr)
		{
			uiPlayerPosIndex = GetTileIndex(MActorManager::Instance().GetClassUi()->GetPlayer()->GetPosition());
		}
		if (mouseIndex == posIndex || posIndex == uiPlayerPosIndex)
		{
			mVisbleScope = true;
			//MActorManager::Instance().SetMBVisible(mVisbleScope);
			//MActorManager::Instance().SetMBVisible(mVisbleScope);
			//mpMoveBox->SetVisible(mVisbleScope);
			//mpMoveBox->SetPosition(mPosition);
		}
		else
		{
			mVisbleScope = false;
			//MActorManager::Instance().SetClickCount(0)
			//MActorManager::Instance().SetMBVisible(mVisbleScope);
			//mpMoveBox->SetVisible(mVisbleScope);	
		}

	}
	//auto state = Keyboard::Get().GetState();

	//if (state.F)
	//{
	//	this->SetAnimation("DEAD");
	//}
	//스킬의 플래그 올라가면 시간측정
	if (mbSkillActionFlag)
	{
		mfSkillActionElpTime += dt;
	}
	//스킬의 시간이 커지면 
	if (mfSkillActionElpTime >= 3.0f)
	{

	}
	//죽음 시간측정후 없어지도록
	if (this->mHealth <= 0)
	{

		if (mfActionElapsedTime > 1.2f)
		{
			//죽는 음악
			FMOD_System_PlaySound(MActorManager::Instance().GetFMODSystem(), (*MActorManager::Instance().GetVecFMODSound())[17], 0, 0, &(*MActorManager::Instance().GetVecFMODChannal())[1]);

			this->SetDestroyed();
			mfActionElapsedTime = 0.0f;
			return E_SCENE_NONPASS;
		}
	}
	//else if (mfActionElapsedTime > 2.8f)
	//{
	//	SetAnimation(mAnimName2);
	//	mfActionElapsedTime = 0.0f;
	//}

	//공격 모션후 색깔 딜레이주기
	if (mbMotionFlag && mfActionElapsedTime > mfDelayMotionTime)
	{
		SetAnimation(mAnimName2);
		//때린 음악
		if (mAnimName == "DATTACK")
		{
			mAnimName = mAnimName2;
			//FMOD_System_PlaySound(MActorManager::Instance().GetFMODSystem(), (*MActorManager::Instance().GetVecFMODSound())[12], 0, 0, &(*MActorManager::Instance().GetVecFMODChannal())[1]);
		}
		mbMotionFlag = false;
		if (mColorAllow && mActionTurn >= 2)
		{
			mColorAllow = false;
			mColor = Colors::Gray;
			MActorManager::Instance().GetClassAttackBox()->SetVisible(false);
			//tint = mColor;
			mfActionElapsedTime = 0.0f;


		}
		else if ( mActionTurn < 2)//mActionTurn > 0 &&
		{
			mColor = Colors::White;
			//mfActionElapsedTime = 0.0f;

		}

	}
	//모션 이 끝나고 난후에 턴넘기는 그래픽 출력
	if (mMotion)
	{
		MActorManager::Instance().GetClassTurnGrapic()->SetVisible(true);
		//MActorManager::Instance().GetClassAttackBox()->GetCharacter()->SetMotion(false);
		this->mMotion = false;
	}
	E_SCENE eResult = MActor::Update(dt);

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
	//tint = Color(255, 0, 255, 1.0f);
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


	if (mActionTurn >= 2)
	{
		tint = mColor;
	}


	mpSheet->Draw(mpBatch, *mpSpriteFrame
		, mWorldPos + mPosition - offset, mColor);

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

void Character::OnHit(MActor * pCollidee)
{
	if (((Character*)this)->GetCamp() != ((Character*)pCollidee)->GetCamp())
	{
		DoDamage(pCollidee);
	}
}

void Character::OnHit(MActor * pCollider, MActor * pCollidee, float delayTime)
{
	if (((Character*)this)->GetCamp() != ((Character*)pCollidee)->GetCamp())
	{

		//때린놈의 액션턴을 바꿔준다.
		((Character*)pCollidee)->SetActionTurn(2);
		//때린놈의 색깔을 바꿔준다.
		((Character*)pCollidee)->SetColorAllow(true);
		//때린놈의 어택플래그 를 바꿔준다
		((Character*)pCollidee)->SetMotionFlag(true);

		//때린놈의 어택딜레이시간 을 바꿔준다
		//((Character*)pCollidee)->SetDelayTime(delayTime);
		//맞은 놈의 어택딜레이 시간을 바꿔준다
		mfDelayMotionTime = delayTime;
		//때린놈의 애니메이션 상태 변경
		const vector<unique_ptr<TILE>>* pVecTile = MActorManager::Instance().GetTileInfo();
		int attackerIndex = GetTileIndex(pCollidee->GetPosition());
		int defenderIndex = GetTileIndex(mPosition);

		//아래 애니매이션 변경
		if (mPosition.y >
			(*pVecTile)[attackerIndex]->vPos.y + JOJOTILESY) //+ JOJOTILESY / 2 + JOJOTILESY / 2)
		{
			pCollidee->SetAnimation("DATTACK");
			((Character*)pCollidee)->SetAniName1("DATTACK");
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
 
		//mfActionElapsedTime = 0;
		//mActionTurn++;
		//맞은 효과음
		FMOD_System_PlaySound(MActorManager::Instance().GetFMODSystem(), (*MActorManager::Instance().GetVecFMODSound())[13], 0, 0, &(*MActorManager::Instance().GetVecFMODChannal())[1]);


		DoDamage(pCollider);
	}
}

void Character::OnHitSkillDamege(MActor* pCollider, MActor* pCollidee, float delayTime /*= 0.8f*/)
{
	if (((Character*)this)->GetCamp() != ((Character*)pCollidee)->GetCamp())
	{

		//때린놈의 액션턴을 바꿔준다.
		((Character*)pCollidee)->SetActionTurn(2);
		//때린놈의 색깔을 바꿔준다.
		((Character*)pCollidee)->SetColorAllow(true);
		//때린놈의 어택플래그 를 바꿔준다
		((Character*)pCollidee)->SetMotionFlag(true);

		//때린놈의 어택딜레이시간 을 바꿔준다
		//((Character*)pCollidee)->SetDelayTime(delayTime);
		//맞은 놈의 어택딜레이 시간을 바꿔준다
		mfDelayMotionTime = delayTime;
		//때린놈의 애니메이션 상태 변경
		const vector<unique_ptr<TILE>>* pVecTile = MActorManager::Instance().GetTileInfo();
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

		//mfActionElapsedTime = 0;
		//mActionTurn++;
		

		DoDamage(pCollider);
	}
}

void Character::OnHitHeal(MActor* pCollider, MActor* pCollidee, float delayTime /*= 0.8f*/)
{
	//때린놈의 액션턴을 바꿔준다.
	((Character*)pCollidee)->SetActionTurn(2);
	//때린놈의 색깔을 바꿔준다.
	((Character*)pCollidee)->SetColorAllow(true);
	//때린놈의 어택플래그 를 바꿔준다
	((Character*)pCollidee)->SetMotionFlag(true);

	//때린놈의 어택딜레이시간 을 바꿔준다
	//((Character*)pCollidee)->SetDelayTime(delayTime);
	//맞은 놈의 어택딜레이 시간을 바꿔준다
	mfDelayMotionTime = delayTime;
	//때린놈의 애니메이션 상태 변경
	const vector<unique_ptr<TILE>>* pVecTile = MActorManager::Instance().GetTileInfo();
	int attackerIndex = GetTileIndex(pCollidee->GetPosition());
	int defenderIndex = GetTileIndex(mPosition);

	//아래 애니매이션 변경
	if (mPosition.y >
		(*pVecTile)[attackerIndex]->vPos.y + JOJOTILESY) //+ JOJOTILESY / 2 + JOJOTILESY / 2)
	{
		//pCollidee->SetAnimation("DATTACK");
		((Character*)pCollidee)->SetAniName("DOWN");
	}
	//위 애니매이션 변경
	else if (mPosition.y <
		(*pVecTile)[attackerIndex]->vPos.y)// + JOJOTILESY / 2 - JOJOTILESY / 2)
	{
		//pCollidee->SetAnimation("UATTACK");
		((Character*)pCollidee)->SetAniName("UP");
	}

	//오른쪽과 왼쪽 변경
	else if ((mPosition.x <
		(*pVecTile)[attackerIndex]->vPos.x))
		//&&(mPosition.x != (*pVecTile)[iDestinationIdx]->vPos.x))
	{
		//pCollidee->SetAnimation("LATTACK");
		((Character*)pCollidee)->SetAniName("LEFT");
	}
	else if (mPosition.x >
		(*pVecTile)[attackerIndex]->vPos.x + JOJOTILESX)
	{
		//pCollidee->SetAnimation("RATTACK");
		((Character*)pCollidee)->SetAniName("RIGHT");
	}
	//때린놈 행동 시간 초기화
	pCollidee->SetActionTime(0.0f);

	//mfActionElapsedTime = 0;
	//mActionTurn++;
	//DoDamage(pCollider);
	mfActionElapsedTime = 0.0f;
	mbMotionFlag = true;
}

void Character::DoDamage(MActor * pAttacker)
{
	//AttackBox* pCharacter = (AttackBox*)pAttacker;
	//어텍박스가 데미지를 줌
	this->mHealth -= ((AttackBox*)pAttacker)->GetAttack();
	
	//맞은놈의 Sp증가 한대맞을떄마다 5씩
	float spRatio = 100;//rand() % ((AttackBox*)pAttacker)->GetAttack();
	if (mSp + spRatio >= mMaxSp)
	{
		mSp = mMaxSp;
	}
	else
	{
		mSp += spRatio;
	}

	mspShake->Start(0.1f, 5);
	mspTint->Start(0.1f, (Color)Colors::Wheat, (Color)Colors::Red);

	this->SetAnimation("HIT");
	mAnimName = "HIT";
	//Color cr = Colors::White;
	//mColor = cr;
	mfActionElapsedTime = 0.0f;
	mbMotionFlag = true;

	//MActorManager::Instance().SetUiCheck(false);
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

	const vector<unique_ptr<TILE>>* pVecTile = MActorManager::Instance().GetTileInfo();

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

	const vector<unique_ptr<TILE>>* pVecTile = MActorManager::Instance().GetTileInfo();

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

	//걷는 음악 재생
	if (mClassType == JoJoGun::ClassType::CAVALRY || mClassType == JoJoGun::ClassType::LORD)
	{
		FMOD_System_PlaySound(MActorManager::Instance().GetFMODSystem(), (*MActorManager::Instance().GetVecFMODSound())[11], 0, 0, &(*MActorManager::Instance().GetVecFMODChannal())[1]);
	}
	else
	{
		FMOD_System_PlaySound(MActorManager::Instance().GetFMODSystem(), (*MActorManager::Instance().GetVecFMODSound())[10], 0, 0, &(*MActorManager::Instance().GetVecFMODChannal())[1]);

	}

}

bool Character::JoAStar_Move(float dt)
{
	list<int>* pBestList = mpJoAStar->GetBestList();

	if (pBestList->empty())	// 비어있다면..
		return false;

	const vector<unique_ptr<TILE>>* pVecTile = MActorManager::Instance().GetTileInfo();

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
		//만약 내가 플레이어 라면 이 타일 값 설정
		if (this->GetCamp() == JoJoGun::CampType::PLAYER)
		{
			(*pVecTile)[pBestList->back()]->underObject = 1;
		}
		//만약 내가 적이라면 이 타일 값 설정
		else if (this->GetCamp() == JoJoGun::CampType::ENEMY)
		{
			(*pVecTile)[pBestList->back()]->underObject = 2;

		}
		(*pVecTile)[pBestList->back()]->byOption = 1;
		(*pVecTile)[mStartIndex]->underObject = 0;
		(*pVecTile)[mStartIndex]->byOption = 0;
	}
	else
	{
		////만약 내가 플레이어 라면 이 타일 값 설정
		//if (this->GetCamp() == JoJoGun::CampType::PLAYER)
		//{
		//	(*pVecTile)[pBestList->back()]->underObject = 1;
		//}
		////만약 내가 적이라면 이 타일 값 설정
		//else if (this->GetCamp() == JoJoGun::CampType::ENEMY)
		//{
		//	(*pVecTile)[pBestList->back()]->underObject = 2;
		//}
		//(*pVecTile)[mStartIndex]->byOption = 1;
		//(*pVecTile)[mStartIndex]->underObject = 0;
		////단 지나가다가 나랑 같은 녀석을 지나지않아야 초기화해줌
		//if ((*pVecTile)[iDestinationIdx]->underObject != mCamp)
		//{
		//	(*pVecTile)[iDestinationIdx]->underObject = 0;
		//	(*pVecTile)[iDestinationIdx]->byOption = 0;
		//}
	}

	if (fDistance < 5.f)
	{
		pBestList->pop_front();

	}

	if (pBestList->size() == 0)
	{
		FMOD_Channel_Stop((*MActorManager::Instance().GetVecFMODChannal())[1]);

		mActionTurn++;
		if (GetCamp() == JoJoGun::CampType::PLAYER)
		{
			auto ui = MActorManager::Instance().GetClassUi();
			ui->SetPosition(mPosition + XMFLOAT2(100.0f, 0.0f));
			ui->SetVisible(true);

			auto attackBox = MActorManager::Instance().GetClassAttackBox();
			attackBox->SetPosition(mPosition);
			attackBox->SetVisible(true);

			MActorManager::Instance().SetMBVisible(false);
			//MActorManager::Instance().SetClickCount(0);
		}
		if (GetCamp() == JoJoGun::CampType::ENEMY)
		{
			auto attackBox = MActorManager::Instance().GetClassAttackBox();

			attackBox->SetVisible(false);

			MActorManager::Instance().SetMBVisible(false);

			//mColor = Colors::Gray;
			//mActionTurn = 2;
			MActorManager::Instance().GetClassMoveBox()->Release();
			attackBox->Release();
		}
	}
	mVisbleScope = false;
	//mpMoveBox->SetVisible(mVisbleScope);

	return true;
	//return false;
}
