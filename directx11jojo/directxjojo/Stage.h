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

class Stage:
	public Scene
{
public:
	Stage();
	virtual ~Stage();

private:
	E_SCENE mStageScene;
	Enemy * mpEnemy;
	Player * mpPlayer;
	Jojo * mpJojo;

	World * mpWorld;
	MoveBox * mpMoveBox;
	UI * mpUi;
	AttackBox * mpAttackBox;
	TurnGrapic *mpTurnGrapic;
	ProgresiveBar * mpProgresiveBar;
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

