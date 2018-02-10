#include "stdafx.h"
#include "SceneMgr.h"


SceneMgr::SceneMgr()
{
}


SceneMgr::~SceneMgr()
{
}



HRESULT SceneMgr::InitScene(SpriteBatch *spritBatch,SpriteFont* spriteFont,E_SCENE eScene)
{
	mSpriteBatch = spritBatch;
	mSpriteFont = spriteFont;
	switch (eScene)
	{
	case E_SCENE_LOGO:
		SGAActorManager::Instance().Release();
		mspScene = make_unique<Logo>();
		//mspScene->SetScene(meScene);
		//mpLogo = dynamic_cast<Logo*>(mspScene.get());
		//mpLogo->SetScene(eScene_);
		break;
	case E_SCENE_STAGE:
		SGAActorManager::Instance().Release();
		mspScene = make_unique<Stage>();
		//mspScene->SetScene(meScene);
		//mpStage = dynamic_cast<Stage*>(mspScene.get());
		//mpStage ->SetScene(eScene_);
		break;
	}

	if (FAILED(mspScene->Initialize(spritBatch,spriteFont)))
	{
		MessageBox(NULL, L"CScene::Initialze() Failed !", NULL, NULL);
		return E_FAIL;
	}
	SGAActorManager::Instance().SortActors();
	return S_OK;
}

void SceneMgr::Update(float dt)
{
	

	E_SCENE eResult = mspScene->Update(dt);

	if (eResult > E_SCENE_NONPASS)
	{
		//this->SetEScene(eResult);
		this->InitScene(mSpriteBatch,mSpriteFont,eResult);
	}
		
}

void SceneMgr::Render()
{
	SGAActorManager::Instance().Draw();
}

void SceneMgr::Release()
{
}
