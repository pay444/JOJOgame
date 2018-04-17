#include "stdafx.h"
#include "Logo.h"


Logo::Logo():
	mScreenWidth(DEFAULT_SCREEN_WIDTH),
	mScreenHeight(DEFAULT_SCREEN_HEIGHT)
{
}


Logo::~Logo()
{
}

HRESULT Logo::Initialize(SpriteBatch* spriteBatch, SpriteFont* spriteFont)
{
	mpWorld = MActorManager::Instance().Create<World>(spriteBatch,spriteFont, mScreenWidth, mScreenHeight);
	mpWorld->Init("Images\\Map\\stage3.tmx", E_SORTID_FOURTH);

	auto pTexture = ResourceManager::Instance().GetShaderResource(L"Images\\jojo\\jojoSprites.png");//(L"Images\\sprites.png");
	auto pSheet = ResourceManager::Instance().GetSpriteSheet(L"Images\\jojo\\jojoSprites.xml", pTexture); //(L"Images\\sprites.xml",pTexture);

	//mpUi = MActorManager::Instance().Create<UI>(spriteBatch, pSheet, spriteFont);
	//mpUi->Init(E_SORTID_FIRST, XMFLOAT2((mScreenWidth * 0.5f) + 100, mScreenHeight * 0.5f), false);

	//mpMoveBox = MActorManager::Instance().Create<MoveBox>(spriteBatch, pSheet, spriteFont);
	//mpMoveBox->Init(E_SORTID_THIRD, XMFLOAT2((mScreenWidth * 0.5f) + 100, mScreenHeight * 0.5f),
	//	200.0f, false);

	//mpTurnGrapic = MActorManager::Instance().Create<TurnGrapic>(spriteBatch, pSheet, spriteFont);
	//mpTurnGrapic->Init(E_SORTID_FIRST, XMFLOAT2(mScreenWidth * 0.5f,
	//	mScreenHeight * 0.5f), false);

	//mpAttackBox = MActorManager::Instance().Create<AttackBox>(spriteBatch, pSheet, spriteFont);
	//mpAttackBox->Init(E_SORTID_THIRD, XMFLOAT2((mScreenWidth * 0.5f) + 100, mScreenHeight * 0.5f),false);
	////AttackBox::Instance().Init(spriteBatch, pSheet, spriteFont, E_SORTID_THIRD, 
	////	XMFLOAT2((mScreenWidth * 0.5f) + 100, mScreenHeight * 0.5f), false);

	//mpJojo = MActorManager::Instance().Create<Jojo>(spriteBatch, pSheet, spriteFont);
	////GunGeon::Blackboard::Instance().SetPlayer(mpJojo);


	//mpJojo->Init(
	//	500.0f,XMFLOAT2((mScreenWidth * 0.5f)+100, mScreenHeight * 0.5f),
	//	E_SORTID_SECOND,200.0f);

	//mpJojo->SetStaus(100, 10,4,1);

	//GunGeon::Blackboard::Instance().SetPlayer(mpJojo);

	//Jojo *m2pPlayer;
	//m2pPlayer = MActorManager::Instance().Create<Jojo>(spriteBatch, pSheet, spriteFont);
	//m2pPlayer->Init(500.0f, XMFLOAT2((72.0f), 72.0f), E_SORTID_FIRST, 200.0f);
	//m2pPlayer->SetStaus(100, 100, 5,2);

	//auto pTexture2 = ResourceManager::Instance().GetShaderResource(L"Images\\\Enemy\\FotManSprites.png");
	//auto pSheet2 = ResourceManager::Instance().GetSpriteSheet(L"Images\\\Enemy\\FotManSprites.xml", pTexture2);

	//FotMan* pEnemy = MActorManager::Instance().Create<FotMan>(spriteBatch, pSheet2, spriteFont);
	//pEnemy->Init(500.0f, XMFLOAT2(200.0f, 200.0f), E_SORTID_SECOND, 200.0f);
	//pEnemy->SetStaus(100, 20, 3,2);
	//pEnemy->SetAI(40.0f, 500.0f, 100.0f, 0.8f,0.8f);

	//pEnemy = MActorManager::Instance().Create<FotMan>(spriteBatch, pSheet2, spriteFont);
	//pEnemy->Init(500.0f, XMFLOAT2(400.0f, 200.0f), E_SORTID_SECOND, 200.0f);
	//pEnemy->SetStaus(100, 20, 3, 1);
	//pEnemy->SetAI(40.0f, 500.0f, 100.0f, 0.8f, 0.8f);

	//pEnemy = MActorManager::Instance().Create<FotMan>(spriteBatch, pSheet2, spriteFont);
	//pEnemy->Init(500.0f, XMFLOAT2(500.0f, 200.0f), E_SORTID_SECOND, 200.0f);
	//pEnemy->SetStaus(100, 10, 3, 1);
	//pEnemy->SetAI(40.0f, 500.0f, 100.0f, 0.8f, 0.8f);


	return S_OK;
}

E_SCENE Logo::Update(float dt)
{
	//E_SCENE eResult = mpPlayer->GetScene();
	//if (eResult > E_SCENE_NONPASS)
	//	mLogoScene = eResult;
	//else
	//{
	//	mLogoScene = E_SCENE_NONPASS;
	//}
	E_SCENE eResult = MActorManager::Instance().Update(dt);
	if (eResult > E_SCENE_NONPASS)
		return eResult;

	return E_SCENE_NONPASS;
}

void Logo::Render()
{
	SceneMgr::Instance().Render();
}

void Logo::Release()
{
}
