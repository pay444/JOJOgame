#include "stdafx.h"
#include "EventScene.h"
#include "EventBaseMap.h"

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

	Animation anim[] = {
		{ "EventMap0", 1,{
			{ "eventMap0", 0.3f },
	} },
	};
	((MActor*)mpEventMap)->Init(anim, 1);
	((MActor*)mpEventMap)->SetAnimation("EventMap0");

	mpEventMap->Init(E_SORTID_SECOND, XMFLOAT2((mScreenWidth * 0.5f), mScreenHeight * 0.5f), false);

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

	E_SCENE eResult = MActorManager::Instance().EventUpdate(dt);
	if (eResult > E_SCENE_NONPASS)
		return eResult;

	return E_SCENE_NONPASS;
}

void EventScene::Render()
{
	//SceneMgr::Instance().Render();
	MActorManager::Instance().Draw();
}

void EventScene::Release()
{
}
