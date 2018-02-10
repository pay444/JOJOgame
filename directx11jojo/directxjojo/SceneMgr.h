#pragma once
#include "Scene.h"
#include "Logo.h"
#include "Stage.h"

class SceneMgr final
{
public:
	//레퍼런스 표현
	static SceneMgr& Instance()
	{
		static SceneMgr instance;
		return instance;
	}
private:
	SceneMgr();
	SceneMgr(SceneMgr const&);
	void operator = (SceneMgr const&);
public:
	virtual ~SceneMgr();
private:
	unique_ptr<Scene> mspScene;
	//unique_ptr<Logo> mspLogo;
	//unique_ptr<Stage> mspStage;
	Logo *mpLogo;
	Stage * mpStage;
	SpriteBatch* mSpriteBatch;
	SpriteFont* mSpriteFont;
	E_SCENE meScene;
public:
	HRESULT InitScene(SpriteBatch *spritBatch, SpriteFont* spriteFont, E_SCENE eScene);
	void Update(float dt);
	void Render();
	void Release();
public:
	void SetEScene(E_SCENE eScene) { meScene = eScene; }
};

