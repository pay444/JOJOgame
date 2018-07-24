#include "stdafx.h"
#include "MainGameMap.h"


MainGameMap::MainGameMap()
{
}


MainGameMap::MainGameMap(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont)
	:MActor(pBatch, pSheet, pFont)
{

}

MainGameMap::~MainGameMap()
{
}

void MainGameMap::Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible)
{
	
	Animation anim[] = {
		{ "GameMain", 1,{
			{ "gameMain", 0.3f },
	}},
	{ "StartBtn", 1,{
		{ "startBtn", 0.3f },
	} },
	};
	MActor::Init(anim, 2, eSortID);
	SetPosition(pos);
	SetAnimation("GameMain");

	RECT rc;
	GetWindowRect(SceneMgr::Instance().GetHWND(), &rc);
	MoveWindow(SceneMgr::Instance().GetHWND()
		, rc.left, rc.top, 658, 438, true);

	//RECT wr = { 0, 0, static_cast<LONG>(STARTSCENE_SCERRN_WIDTH), static_cast<LONG>(STARTSCENE_SCERRN_HEIGHT) };
	//AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);


}

E_SCENE MainGameMap::Update(float dt)
{
	RECT rc;
	GetWindowRect(SceneMgr::Instance().GetHWND(), &rc);

	//게임시작을 누르면 이벤트 신으로 넘어감
	if (MFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	{
		SetAnimation("StartBtn");
		RECT colrc,soucerc;
		XMFLOAT2 btnPos = mPosition + XMFLOAT2(215.0f, -134.0f);
		soucerc = GetBound();
		float souceWidth = soucerc.right - soucerc.left;
		float souceHeight = soucerc.bottom - soucerc.top;

		colrc.left = btnPos.x - souceWidth/2;
		colrc.top = btnPos.y - souceHeight/2;
		colrc.right = btnPos.x + souceWidth / 2;
		colrc.bottom = btnPos.y + souceHeight / 2;
		auto mouse = Mouse::Get().GetState();
		POINT mousepos;
		mousepos.x = mouse.x;
		mousepos.y = mouse.y;

		//게임시작과 마우스의 충돌 조사
		if (PtInRect(&colrc,mousepos))
		{
			return E_SCENE_EVENT0;
		}
	}

	return E_SCENE_NONPASS;
}

void MainGameMap::Draw()
{
	XMFLOAT2 offset = XMFLOAT2(0, 0);
	Color tint = Colors::White;

	SetAnimation("GameMain");

	mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition , tint);

	SetAnimation("StartBtn");

	mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition + XMFLOAT2(215.0f,-134.0f), tint);

}
