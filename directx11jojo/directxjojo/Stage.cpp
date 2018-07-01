#include "stdafx.h"
#include "Stage.h"


Stage::Stage():
	mScreenWidth(DEFAULT_SCREEN_WIDTH),
	mScreenHeight(DEFAULT_SCREEN_HEIGHT)
{
}


Stage::~Stage()
{
}

HRESULT Stage::Initialize(SpriteBatch *spriteBatch, SpriteFont * spriteFont)
{
	mpWorld = MActorManager::Instance().Create<World>(spriteBatch, spriteFont, mScreenWidth, mScreenHeight);
	mpWorld->Init("Images\\Map\\stage3.tmx", E_SORTID_FOURTH);

	auto pTexture = ResourceManager::Instance().GetShaderResource(L"Images\\jojo\\jojoSprites.png");//(L"Images\\sprites.png");
	auto pSheet = ResourceManager::Instance().GetSpriteSheet(L"Images\\jojo\\jojoSprites.xml", pTexture); //(L"Images\\sprites.xml",pTexture);

	mpUi = MActorManager::Instance().Create<UI>(spriteBatch, pSheet, spriteFont);
	mpUi->Init(E_SORTID_FIRST, XMFLOAT2((mScreenWidth * 0.5f) + 100, mScreenHeight * 0.5f), false);

	mpMoveBox = MActorManager::Instance().Create<MoveBox>(spriteBatch, pSheet, spriteFont);
	mpMoveBox->Init(E_SORTID_THIRD, XMFLOAT2((mScreenWidth * 0.5f) + 100, mScreenHeight * 0.5f),
		200.0f, false);

	mpTurnGrapic = MActorManager::Instance().Create<TurnGrapic>(spriteBatch, pSheet, spriteFont);
	mpTurnGrapic->Init(E_SORTID_FIRST, XMFLOAT2(mScreenWidth * 0.5f,
		mScreenHeight * 0.5f), false);

	mpProgresiveBar = MActorManager::Instance().Create<ProgresiveBar>(spriteBatch, pSheet, spriteFont);
	mpProgresiveBar->Init(E_SORTID_FIRST, XMFLOAT2(mScreenWidth * 0.5f,
		mScreenHeight * 0.5f), false);
	
	mpAttackBox = MActorManager::Instance().Create<AttackBox>(spriteBatch, pSheet, spriteFont);
	mpAttackBox->Init(E_SORTID_THIRD, XMFLOAT2((mScreenWidth * 0.5f) + 100, mScreenHeight * 0.5f), false);
	//AttackBox::Instance().Init(spriteBatch, pSheet, spriteFont, E_SORTID_THIRD, 
	//	XMFLOAT2((mScreenWidth * 0.5f) + 100, mScreenHeight * 0.5f), false);

	mpJojo = MActorManager::Instance().Create<Jojo>(spriteBatch, pSheet, spriteFont);
	//GunGeon::Blackboard::Instance().SetPlayer(mpJojo);


	mpJojo->Init(
		500.0f, XMFLOAT2((mScreenWidth * 0.5f) + 100, mScreenHeight * 0.5f),
		E_SORTID_SECOND, 200.0f);

	mpJojo->SetStaus(101, 10,100,0, 5, 1);

	GunGeon::Blackboard::Instance().SetPlayer(mpJojo);

	Jojo *m2pPlayer;
	m2pPlayer = MActorManager::Instance().Create<Jojo>(spriteBatch, pSheet, spriteFont);
	m2pPlayer->Init(500.0f, XMFLOAT2((72.0f), 72.0f), E_SORTID_FIRST, 200.0f);
	m2pPlayer->SetStaus(101, 100, 100, 0, 5, 2);

	auto pTexture2 = ResourceManager::Instance().GetShaderResource(L"Images\\\Enemy\\FotManSprites.png");
	auto pSheet2 = ResourceManager::Instance().GetSpriteSheet(L"Images\\\Enemy\\FotManSprites.xml", pTexture2);

	FotMan* pEnemy = MActorManager::Instance().Create<FotMan>(spriteBatch, pSheet2, spriteFont);
	pEnemy->Init(500.0f, XMFLOAT2(200.0f, 200.0f), E_SORTID_SECOND, 200.0f);
	pEnemy->SetStaus(100, 10, 100, 0, 3, 2);
	pEnemy->SetAI(40.0f, 500.0f, 100.0f, 0.8f, 0.8f);

	pEnemy = MActorManager::Instance().Create<FotMan>(spriteBatch, pSheet2, spriteFont);
	pEnemy->Init(500.0f, XMFLOAT2(400.0f, 200.0f), E_SORTID_SECOND, 200.0f);
	pEnemy->SetStaus(100, 10, 100, 0, 3, 1);
	pEnemy->SetAI(40.0f, 500.0f, 100.0f, 0.8f, 0.8f);

	pEnemy = MActorManager::Instance().Create<FotMan>(spriteBatch, pSheet2, spriteFont);
	pEnemy->Init(500.0f, XMFLOAT2(500.0f, 200.0f), E_SORTID_SECOND, 200.0f);
	pEnemy->SetStaus(100, 10, 100, 0, 3, 1);
	pEnemy->SetAI(40.0f, 500.0f, 100.0f, 0.8f, 0.8f);

	return S_OK;
}

E_SCENE Stage::Update(float dt)
{
	//MActorManager::Instance().Update(dt);

	//mpPlayer->SetPosition(mpWorld->GetmCameraPosition().x, mpWorld->GetmCameraPosition().y);
	//mpPlayer->SetCamerapos(mpWorld->GetmCameraPosition().x, mpWorld->GetmCameraPosition().y);

	//E_SCENE eResult = mpPlayer->GetScene();
	E_SCENE eResult = MActorManager::Instance().Update(dt);
	if (eResult > E_SCENE_NONPASS)
		return eResult;


	return E_SCENE_NONPASS;
}

void Stage::Render()
{
	SceneMgr::Instance().Render();
}

void Stage::Release()
{
}
