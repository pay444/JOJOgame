#pragma once
#include "MFramework.h"
#include "CommonStates.h"
#include "World.h"

class GameMain :
	public MFramework
{
public:
	GameMain();
	virtual ~GameMain();

private:
	unique_ptr<CommonStates>	mspStates;
	//unique_ptr<World>			mspStage;
	//World * mspStage;
	Vector2 offset;

	Player* mpPlayer;
public:
	virtual void InitD3D(HWND hWnd);
	virtual void ClearD3D();
	virtual void Update(float dt);
	virtual void Render();

};

