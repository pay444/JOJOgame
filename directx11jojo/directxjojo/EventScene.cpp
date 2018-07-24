#include "stdafx.h"
#include "EventScene.h"


EventScene::EventScene()
{
}


EventScene::~EventScene()
{
}

HRESULT EventScene::Initialize(SpriteBatch * spriteBatch, SpriteFont * spriteFont)
{
	auto pTexture = ResourceManager::Instance().GetShaderResource(L"Images\\Production\\events.png");//(L"Images\\sprites.png");
	auto pSheet = ResourceManager::Instance().GetSpriteSheet(L"Images\\Production\\events.xml", pTexture); //(L"Images\\sprites.xml",pTexture);

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
