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

	//mpPlayer = MActorManager::Instance().Create<Player>(spriteBatch, pSheet);
	//GunGeon::Blackboard::Instance().SetPlayer(mpPlayer);

	//mpPlayer->Init(
	//	100.0f,
	//	XMFLOAT2(mScreenWidth * 0.5f, mScreenHeight * 0.5f), E_SORTID_FIRST);

	//mpPlayer->SetStaus(100, 10);

	//mpPlayer->SetScene(mStageScene);
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
	MActorManager::Instance().Draw();
}

void Stage::Release()
{
}
