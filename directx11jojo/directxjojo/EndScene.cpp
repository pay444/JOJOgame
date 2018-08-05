#include "stdafx.h"
#include "EndScene.h"
#include "EventBaseMap.h"

EndScene::EndScene():
	mScreenWidth(STARTSCENE_SCERRN_WIDTH),
	mScreenHeight(STARTSCENE_SCERRN_HEIGHT)
{
}


EndScene::~EndScene()
{
}

HRESULT EndScene::Initialize(SpriteBatch * spriteBatch, SpriteFont * spriteFont)
{
	auto pTexture = ResourceManager::Instance().GetShaderResource(L"Images\\Production\\events.png");//(L"Images\\sprites.png");
	auto pSheet = ResourceManager::Instance().GetSpriteSheet(L"Images\\Production\\events.xml", pTexture); //(L"Images\\sprites.xml",pTexture);

	mpEventMap = MActorManager::Instance().Create<EventBaseMap>(spriteBatch, pSheet, spriteFont);
	Animation anim1[] = {
		{ "GameOver", 1,{
			{ "gameOver", 0.3f },
	} },
	};
	((MActor*)mpEventMap)->Init(anim1, 1);
	((MActor*)mpEventMap)->SetAnimation(anim1->Name);
	mpEventMap->Init(E_SORTID_THIRD, XMFLOAT2((mScreenWidth * 0.5f), mScreenHeight * 0.5f), false);

	// 荤款靛 积己
	FMOD_SOUND* pSound = NULL;
	FMOD_System_CreateSound(MActorManager::Instance().GetFMODSystem(), "Sound\\Se40end.wav", FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &pSound);
	MActorManager::Instance().GetVecFMODSound()->push_back(pSound);

	// 荤款靛 犁积
	FMOD_System_PlaySound(MActorManager::Instance().GetFMODSystem(), pSound, 0, 0, &(*MActorManager::Instance().GetVecFMODChannal())[0]);


	return S_OK;
}

E_SCENE EndScene::Update(float dt)
{
	auto state = Keyboard::Get().GetState();

	if (state.D1)
		return E_SCENE_LOGO;
	else if (state.D2)
		return E_SCENE_STAGE;
	else if (state.D3)
		return E_SCENE_EVENT0;
	//EventEditorMgr::Instance().Update(dt);
	E_SCENE eResult = MActorManager::Instance().EventUpdate(dt);
	if (eResult > E_SCENE_NONPASS)
		return eResult;

	return E_SCENE_NONPASS;
}

void EndScene::Render()
{
	MActorManager::Instance().Draw();
	//EventEditorMgr::Instance().Draw();
}

void EndScene::Release()
{
}
