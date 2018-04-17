#pragma once
#include "Scene.h"
#include "World.h"
#include "Jojo.h"
#include "FotMan.h"
#include "MoveBox.h"
#include "UI.h"
#include "AttackBox.h"
#include "TurnGrapic.h"
#include "ProgresiveBar.h"

class Logo:
	public Scene
{
private:
	E_SCENE mLogoScene;
	Player * mpPlayer;
	Jojo * mpJojo;
	
	World * mpWorld;
	MoveBox * mpMoveBox;
	UI * mpUi;
	AttackBox * mpAttackBox;
	TurnGrapic *mpTurnGrapic;
protected:
	UINT		mScreenWidth;
	UINT		mScreenHeight;
	unique_ptr<SpriteBatch>		mspSpriteBatch;
public:
	Logo();
	virtual ~Logo();
public:
	virtual HRESULT Initialize(SpriteBatch* spriteBatch,SpriteFont* spriteFont);
	virtual E_SCENE Update(float dt);
	virtual void Render();
	virtual void Release();

public:
	virtual E_SCENE GetScene() { return mLogoScene; }
	virtual void SetScene(E_SCENE eScene) { mLogoScene = eScene; }
};

