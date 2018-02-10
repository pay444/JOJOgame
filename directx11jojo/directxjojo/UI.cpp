#include "stdafx.h"
#include "UI.h"


UI::UI()
{
}

UI::UI(SpriteBatch * pBatch, SGASpriteSheet * pSheet, SpriteFont * pFont)
	:SGAActor(pBatch,pSheet,pFont)
{
}


UI::~UI()
{
}

void UI::Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible)
{
	Animation anim[] = {
		{ "Ui", 1,{ { "Ui", 0.3f }}
		},
	};

	//이동거리
	//mMoveDistance = moveDis;
	//보이고 안보이고
	mUIVisible = visible;
	SGAActor::Init(anim, 1,eSortID);
	SetPosition(pos + XMFLOAT2(100.0f,0.0f));
	SetAnimation("Ui");
}

E_SCENE UI::Update(float dt)
{


	E_SCENE eResult = SGAActor::Update(dt);


	


	if (eResult > E_SCENE_NONPASS)
		return eResult;

	return E_SCENE_NONPASS;
}

void UI::Draw()
{
	XMFLOAT2 offset = XMFLOAT2(0, 0);
	Color tint = Colors::White;

	offset.x = (int)ScrollMgr::Instance().GetScroll().x;
	offset.y = (int)ScrollMgr::Instance().GetScroll().y;

	auto mouse = Mouse::Get().GetState();

	//이동범위를 보여줄때
	if (mUIVisible)
	{
		//if (SGAFramework::mMouseTracker.rightButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
		//{

		//}
		mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition - offset, tint);
	}
	
}

bool UI::CheckAttackArea()
{
	if (mUIVisible)
	{
		if (SGAFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
		{
			float fScrollx = ScrollMgr::Instance().GetScroll().x;
			float fScrolly = ScrollMgr::Instance().GetScroll().y;
			auto mouse = Mouse::Get().GetState();

			XMFLOAT2 mousePos = XMFLOAT2(mouse.x + fScrollx, mouse.y + fScrolly);

			//UI 그림의 사각형 위치
			RECT rct = GetBound();

			XMFLOAT2 uiAttackPosx = XMFLOAT2(rct.left, rct.right);
			XMFLOAT2 uiAttackPosy = XMFLOAT2(rct.top, rct.bottom);
			//그림의 위치를 통해서 마우스의 위치 확인하여 반응 구현
			if (mousePos.x >= uiAttackPosx.x && mousePos.x <= uiAttackPosx.y &&
				mousePos.y >= uiAttackPosy.x && mousePos.y <= ((uiAttackPosy.y - uiAttackPosy.x) / 8 + uiAttackPosy.x))
			{
				return true;
			}
		}
	}
	return false;
}
