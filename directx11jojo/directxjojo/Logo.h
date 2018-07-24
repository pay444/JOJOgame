#pragma once
#include "Scene.h"

class MainGameMap;
class Logo:
	public Scene
{
private:
	E_SCENE mLogoScene;

protected:
	UINT		mScreenWidth;
	UINT		mScreenHeight;
	unique_ptr<SpriteBatch>		mspSpriteBatch;
public:
	Logo();
	virtual ~Logo();

private:
	MainGameMap * mpMainGameMap;
public:
	virtual HRESULT Initialize(SpriteBatch* spriteBatch,SpriteFont* spriteFont);
	virtual E_SCENE Update(float dt);
	virtual void Render();
	virtual void Release();

public:
	virtual E_SCENE GetScene() { return mLogoScene; }
	virtual void SetScene(E_SCENE eScene) { mLogoScene = eScene; }
};

