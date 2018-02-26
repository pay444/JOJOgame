#include "stdafx.h"
#include "Character.h"


Character::Character()
{
	InitEffect();
	mStartIndex = 0;
}

Character::Character(SpriteBatch * pBatch, SGASpriteSheet * pSheet, SpriteFont * pFont)
	: SGAActor(pBatch, pSheet, pFont)
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

	auto pTexture = SGAResourceManager::Instance().GetShaderResource(L"Images\\jojoWalk\\jojoWalk.png");
	auto pSheet = SGAResourceManager::Instance().GetSpriteSheet(L"Images\\jojoWalk\\jojoWalk.xml", pTexture);
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

		//if ((*spVecTile)[mouseIndex]->underObject == 1)
		//{
		//	
		//	pMoveBox->Init(E_SORTID_FIRST,mPosition,limitDistance);
		//	mVisbleScope = !mVisbleScope;
		//	//pMoveBox->SetDestroyed();
		//}
		//else
		//{
		//	pMoveBox->SetDestroyed();
		//}
		if (mouseIndex == posIndex)
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
			//SGAActorManager::Instance().SetMBVisible(mVisbleScope);
			//mpMoveBox->SetVisible(mVisbleScope);	
		}

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
	SetAnimation(mAnimName2);

	mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition - offset, tint);
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
		((Character*)pCollidee)->SetActionTurn(2);
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
	//SGAActorManager::Instance().SetUiCheck(false);
	if (this->mHealth <= 0)
		this->SetDestroyed();
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
	//아래 변경
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
	}
	mVisbleScope = false;
	//mpMoveBox->SetVisible(mVisbleScope);

	return true;
	//return false;
}
