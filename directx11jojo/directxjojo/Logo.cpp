#include "stdafx.h"
#include "Logo.h"
#include "MainGameMap.h"

Logo::Logo():
	mScreenWidth(STARTSCENE_SCERRN_WIDTH),
	mScreenHeight(STARTSCENE_SCERRN_HEIGHT)
{
}


Logo::~Logo()
{
}

HRESULT Logo::Initialize(SpriteBatch* spriteBatch, SpriteFont* spriteFont)
{
	//mpWorld = MActorManager::Instance().Create<World>(spriteBatch,spriteFont, mScreenWidth, mScreenHeight);
	//mpWorld->Init("Images\\Map\\stage3.tmx", E_SORTID_FOURTH);

	auto pTexture = ResourceManager::Instance().GetShaderResource(L"Images\\Production\\events.png");//(L"Images\\sprites.png");
	auto pSheet = ResourceManager::Instance().GetSpriteSheet(L"Images\\Production\\events.xml", pTexture); //(L"Images\\sprites.xml",pTexture);

	mpMainGameMap = MActorManager::Instance().Create<MainGameMap>(spriteBatch, pSheet,spriteFont);

	mpMainGameMap->Init(E_SORTID_SECOND, XMFLOAT2((mScreenWidth * 0.5f), mScreenHeight * 0.5f), false);
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
	////JoJoGun::Blackboard::Instance().SetPlayer(mpJojo);


	//mpJojo->Init(
	//	500.0f,XMFLOAT2((mScreenWidth * 0.5f)+100, mScreenHeight * 0.5f),
	//	E_SORTID_SECOND,200.0f);

	//mpJojo->SetStaus(100, 10,4,1);

	//JoJoGun::Blackboard::Instance().SetPlayer(mpJojo);

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
	if (MFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	{


	}
	if (MFramework::mKeyboardTracker.IsKeyPressed(Keyboard::D0))
	{
		//RECT rc;
		//GetWindowRect(SceneMgr::Instance().GetHWND(), &rc);
		//MoveWindow(SceneMgr::Instance().GetHWND(), rc.left, rc.top, 480, 600, true);
	}
	auto state = Keyboard::Get().GetState();

	if (state.D1)
		return E_SCENE_LOGO;
	else if (state.D2)
		return E_SCENE_STAGE;
	else if (state.D3)
		return E_SCENE_EVENT0;

	E_SCENE eResult = MActorManager::Instance().GameMainUpdate(dt);
	if (eResult > E_SCENE_NONPASS)
		return eResult;

	return E_SCENE_NONPASS;
}

void Logo::Render()
{
	MActorManager::Instance().Draw();
}

void Logo::Release()
{
}
