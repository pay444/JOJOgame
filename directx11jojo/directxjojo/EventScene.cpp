#include "stdafx.h"
#include "EventScene.h"
#include "EventBaseMap.h"
#include "EventCharacter.h"
#include "EventTalk.h"
#include "EventStageMap.h"

EventScene::EventScene():
	mScreenWidth(STARTSCENE_SCERRN_WIDTH),
	mScreenHeight(STARTSCENE_SCERRN_HEIGHT)
{
}


EventScene::~EventScene()
{
}

HRESULT EventScene::Initialize(SpriteBatch * spriteBatch, SpriteFont * spriteFont)
{
	auto pTexture = ResourceManager::Instance().GetShaderResource(L"Images\\Production\\events.png");//(L"Images\\sprites.png");
	auto pSheet = ResourceManager::Instance().GetSpriteSheet(L"Images\\Production\\events.xml", pTexture); //(L"Images\\sprites.xml",pTexture);

	mpEventMap = MActorManager::Instance().Create<EventBaseMap>(spriteBatch, pSheet, spriteFont);
	Animation anim1[] = {
		{ "EventMap0", 1,{
			{ "eventMap0", 0.3f },
	} },
	};
	((MActor*)mpEventMap)->Init(anim1, 1);
	((MActor*)mpEventMap)->SetAnimation("EventMap0");
	mpEventMap->Init(E_SORTID_THIRD, XMFLOAT2((mScreenWidth * 0.5f), mScreenHeight * 0.5f), false);

	//이벤트 캐릭터 생성
	mpEventChr = MActorManager::Instance().Create<EventCharacter>(spriteBatch, pSheet, spriteFont);
	Animation anim2[] = {
		{ "StandF", 1,{
			{ "jojoActoinF_001", 0.3f },
	} },
		{ "WalkF", 2,{
			{ "jojoActoinF_002", 0.3f },
	{ "jojoActoinF_003", 0.3f },
	} },
	{ "HandUpF", 1,{
		{ "jojoActoinF_006", 0.3f },
	} },
	{ "PointF", 1,{
		{ "jojoActoinF_020", 0.3f },
	} },
	};
	((MActor*)mpEventChr)->Init(anim2, 4);
	((MActor*)mpEventChr)->SetAnimation("StandF");
	((MActor*)mpEventChr)->SetName(L"조조");
	mpEventChr->Init(100.0f, XMFLOAT2((mScreenWidth + 32), -82), E_SORTID_SECOND);

	mpEventChr = MActorManager::Instance().Create<EventCharacter>(spriteBatch, pSheet, spriteFont);
	Animation anim3[] = { 
		{ "StandF", 1,{
			{ "soldierF_001", 0.3f },
	} },
	{ "StandB", 1,{
		{ "soldierB_001", 0.3f },
	} },
	{ "WalkF", 2,{
		{ "soldierF_002", 0.3f },
	{ "soldierF_003", 0.3f },
	} },
	{ "HandUpF", 1,{
		{ "soldierF_006", 0.3f },
	} },
	{ "PointF", 1,{
		{ "soldierF_020", 0.3f },
	} },
	};
	((MActor*)mpEventChr)->Init(anim3, 5);
	((MActor*)mpEventChr)->SetAnimation("StandF");
	((MActor*)mpEventChr)->SetName(L"병사1");
	mpEventChr->Init(100.0f, XMFLOAT2(((mScreenWidth ) ), -50), E_SORTID_SECOND);
	
	mpEventChr = MActorManager::Instance().Create<EventCharacter>(spriteBatch, pSheet, spriteFont);
	((MActor*)mpEventChr)->Init(anim3, 4);
	((MActor*)mpEventChr)->SetAnimation("StandF");
	((MActor*)mpEventChr)->SetName(L"병사2");
	mpEventChr->Init(100.0f, XMFLOAT2(((mScreenWidth + 64)), -112), E_SORTID_SECOND);

	mpEventTalk	= MActorManager::Instance().Create<EventTalk>(spriteBatch, pSheet, spriteFont);
	mpEventTalk->Init(XMFLOAT2(mScreenWidth*0.5f, mScreenHeight *0.5f),E_SORTID_FIRST);
	
	mpEventChr = MActorManager::Instance().Create<EventCharacter>(spriteBatch, pSheet, spriteFont);
	Animation anim4[] = {
		{ "StandF", 1,{
			{ "huchaF_001", 0.3f },
	} },
	{ "StandB", 1,{
		{ "huchaB_001", 0.3f },
	} },
	{ "WalkF", 2,{
		{ "huchaF_002", 0.3f },
	{ "huchaF_003", 0.3f },
	} },
	{ "WalkB", 2,{
		{ "huchaB_002", 0.3f },
	{ "huchaB_003", 0.3f },
	} },
	{ "HandUpF", 1,{
		{ "soldierF_006", 0.3f },
	} },
	{ "PointF", 1,{
		{ "soldierF_020", 0.3f },
	} },
	{ "GreetingB", 1,{
		{ "huchaB_009", 0.3f },
	} },

	};
	((MActor*)mpEventChr)->Init(anim4, 7);
	((MActor*)mpEventChr)->SetAnimation("StandB");
	((MActor*)mpEventChr)->SetName(L"허차장");
	mpEventChr->Init(100.0f, XMFLOAT2(-32, mScreenHeight +32), E_SORTID_SECOND);

	
	mpEventStageMap = MActorManager::Instance().Create<EventStageMap>(spriteBatch, pSheet, spriteFont);
	mpEventStageMap->Init(E_SORTID_FIRST, XMFLOAT2(mScreenWidth *0.5f, mScreenHeight*0.5f), false);
	EventEditorMgr::Instance().Init();

	// 사운드 생성
	FMOD_SOUND* pSound = NULL;
	FMOD_System_CreateSound(MActorManager::Instance().GetFMODSystem(), "Sound\\Se_e_01event0.wav", FMOD_DEFAULT | FMOD_LOOP_NORMAL, 0, &pSound);
	MActorManager::Instance().GetVecFMODSound()->push_back(pSound);

	// 사운드 재생
	FMOD_System_PlaySound(MActorManager::Instance().GetFMODSystem(), pSound, 0, 0, &(*MActorManager::Instance().GetVecFMODChannal())[0]);

	pSound = NULL;
	FMOD_System_CreateSound(MActorManager::Instance().GetFMODSystem(), "Sound/Se04startStage.wav", FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &pSound);
	MActorManager::Instance().GetVecFMODSound()->push_back(pSound);

	return S_OK;
}

E_SCENE EventScene::Update(float dt)
{
	auto state = Keyboard::Get().GetState();

	if (state.D1)
		return E_SCENE_LOGO;
	else if (state.D2)
		return E_SCENE_STAGE;
	else if (state.D3)
		return E_SCENE_EVENT0;
	EventEditorMgr::Instance().Update(dt);
	E_SCENE eResult = MActorManager::Instance().EventUpdate(dt);
	if (eResult > E_SCENE_NONPASS)
		return eResult;

	return E_SCENE_NONPASS;
}

void EventScene::Render()
{
	//SceneMgr::Instance().Render();
	MActorManager::Instance().Draw();
	EventEditorMgr::Instance().Draw();
}

void EventScene::Release()
{
	
}
