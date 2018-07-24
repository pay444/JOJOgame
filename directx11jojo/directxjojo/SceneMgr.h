#pragma once
#include "Scene.h"
#include "Logo.h"
#include "Stage.h"
#include "EventScene.h"

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
	HWND mHWnd;		//윈도우의 핸들가져오기
	Logo *mpLogo;
	Stage * mpStage;
	EventScene * mpEventScene;
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

	void SetHWND(HWND hWnd) { mHWnd = hWnd; }
	HWND GetHWND() { return mHWnd; }
};

