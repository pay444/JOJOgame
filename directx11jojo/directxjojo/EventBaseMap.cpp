#include "stdafx.h"
#include "EventBaseMap.h"


EventBaseMap::EventBaseMap()
{
}


EventBaseMap::EventBaseMap(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont)
	:MActor(pBatch,pSheet,pFont)
{

}

EventBaseMap::~EventBaseMap()
{
}

void EventBaseMap::Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible)
{
	//Animation anim[] = {
	//	{ "EventMap0", 1,{
	//		{ "eventMap0", 0.3f },
	//} },
	//};
	//MActor::Init(anim, 1, eSortID);
	SetPosition(pos);
	RECT rc;
	GetWindowRect(SceneMgr::Instance().GetHWND(), &rc);
	MoveWindow(SceneMgr::Instance().GetHWND()
		, rc.left, rc.top, 658, 438, true);

}

E_SCENE EventBaseMap::Update(float dt)
{
	RECT rc;
	GetWindowRect(SceneMgr::Instance().GetHWND(), &rc);

	if (MFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	{

	}

	return E_SCENE_NONPASS;
}

void EventBaseMap::Draw()
{
	XMFLOAT2 offset = XMFLOAT2(0, 0);
	Color tint = Colors::White;
	//void XM_CALLCONV SpriteSheet::Draw(SpriteBatch * batch,
	//	const SpriteFrame & frame, XMFLOAT2 const & position,
	//	FXMVECTOR color, float rotation
	//	, float scale, SpriteEffects effects, float layer) const

	mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition, tint
		, 0, 1.0f, DirectX::SpriteEffects_FlipHorizontally, 0.0f);

}
