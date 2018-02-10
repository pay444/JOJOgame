#pragma once
#include "Scene.h"
#include "World.h"
#include "Player.h"

class Stage:
	public Scene
{
public:
	Stage();
	virtual ~Stage();

private:
	E_SCENE mStageScene;
	Player* mpPlayer;
	World * mpWorld;
protected:
	UINT		mScreenWidth;
	UINT		mScreenHeight;
	//unique_ptr<SpriteBatch>		mspSpriteBatch;
public:
	virtual HRESULT Initialize(SpriteBatch *spritBatch,SpriteFont * spriteFont);
	virtual E_SCENE Update(float dt);
	virtual void Render();
	virtual void Release();
	virtual E_SCENE GetScene() { return mStageScene; }
	virtual void SetScene(E_SCENE eScene) { mStageScene = eScene; }
};

