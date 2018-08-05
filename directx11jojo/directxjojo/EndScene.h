#pragma once
#include "Scene.h"

class EventBaseMap;

class EndScene :
	public Scene
{
private:
	E_SCENE mEventScene;

public:
	EndScene();
	virtual ~EndScene();
protected:
	UINT		mScreenWidth;
	UINT		mScreenHeight;
private:
	EventBaseMap * mpEventMap;
public:
	virtual HRESULT Initialize(SpriteBatch* spriteBatch, SpriteFont* spriteFont);
	virtual E_SCENE Update(float dt);
	virtual void Render();
	virtual void Release();

public:
	virtual E_SCENE GetScene() { return mEventScene; }
	virtual void SetScene(E_SCENE eScene) { mEventScene = eScene; }
};

