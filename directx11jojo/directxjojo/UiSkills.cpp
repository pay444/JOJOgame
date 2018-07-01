#include "stdafx.h"
#include "UiSkills.h"


UiSkills::UiSkills()
{
}


UiSkills::UiSkills(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont)
	:MActor(pBatch, pSheet, pFont),
	mpPlayer(nullptr),
	mUISVisible(false),
	mCountSkill(1),
	mCancelBtnPos(0,0)
{

}

UiSkills::~UiSkills()
{
}

void UiSkills::Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible)
{
	Animation anim[] = { 
		{ "SkillUi", 1,{ { "skillUi", 0.3f } } },
	{ "PropertyBack0", 1,{ { "waterBack", 0.3f } } },
	{ "PropertyBack1", 1,{ { "fireBack", 0.3f } } },
	{ "PropertyBack2", 1,{ { "windBack", 0.3f } } },
	{ "PropertyBack3", 1,{ { "randBack", 0.3f } } },
	{ "CancelBtn", 1,{ { "cancelBtn", 0.3f } } },
	};

	//이동거리
	//mMoveDistance = moveDis;
	//보이고 안보이고
	mUISVisible = visible;
	//mUISVisible = true;
	MActor::Init(anim, 6, eSortID);
	SetPosition(pos + XMFLOAT2(100.0f, 0.0f));
	SetAnimation("SkillUi");
	mCancelBtnPos = mPosition + XMFLOAT2(82, 64);
}

E_SCENE UiSkills::Update(float dt)
{
	MActor::Update(dt);

	if (mUISVisible)
	{
		float fScrollx = ScrollMgr::Instance().GetScroll().x;
		float fScrolly = ScrollMgr::Instance().GetScroll().y;

		//클래스에 따른 스킬 개수 설정
		if (mpPlayer->GetClassType() == GunGeon::ClassType::MONARCH)
		{
			mCountSkill = 1;
		}

		//해당 클래스의 스킬개수만큼 UI 상에서 위치 지정
		if (mVecUiSPos.size() == 0)
		{
			for (int i = 0; i < mCountSkill; ++i)
			{
				float ysize = 30.0f;
				float uiPos = 18 * i;
				mVecUiSPos.push_back(mPosition + XMFLOAT2(1.0f, uiPos - ysize));
			}
		}
		//취소버튼의 위치 갱신
		mCancelBtnPos = mPosition + XMFLOAT2(82, 64);

		auto mouse = Mouse::Get().GetState();

		POINT mousePos;
		mousePos.x = mouse.x + fScrollx;
		mousePos.y = mouse.y + fScrolly;

		//취소 버튼이 클릭됬을때 안보여준다
		RECT rc;
		rc.left = mCancelBtnPos.x - mCancelBtnPos.x / 2;
		rc.right = mCancelBtnPos.x + mCancelBtnPos.x / 2;
		rc.top = mCancelBtnPos.y - mCancelBtnPos.y / 2;
		rc.bottom = mCancelBtnPos.y + mCancelBtnPos.y / 2;
		if (PtInRect(&rc, mousePos))
		{
			if (MFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
			{
				mUISVisible = false;
			}
		}
	}
	else
	{
		mVecUiSPos.clear();
	}

	return E_SCENE_NONPASS;
}

void UiSkills::Draw()
{
	XMFLOAT2 offset = XMFLOAT2(0, 0);
	Color tint = Colors::White;

	offset.x = (int)ScrollMgr::Instance().GetScroll().x;
	offset.y = (int)ScrollMgr::Instance().GetScroll().y;

	auto mouse = Mouse::Get().GetState();

	//이동범위를 보여줄때
	if (mUISVisible)
	{
		//스킬 기본 UI 출력
		SetAnimation("SkillUi");
		mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition - offset, tint);

		//취소 버튼 출력
		SetAnimation("CancelBtn");
		mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mCancelBtnPos - offset, tint);
		
		//해당 캐릭터의 클래스가 군주 라면
		if (mpPlayer->GetClassType() == GunGeon::ClassType::MONARCH)
		{
			for (int i = 0; i < mCountSkill; ++i)
			{
				if (mVecUiSPos.size() == 0)
				{
					break;
				}
				//string name;
				//char ch[10];
				//sprintf_s(ch, sizeof(ch), "PropertyBack%d", i);
				//name = ch;
				SetAnimation("PropertyBack2");
				mpSheet->Draw(mpBatch, *mpSpriteFrame
					, mWorldPos + mVecUiSPos[i] - offset
					, tint);
			}
		}


		//if (MFramework::mMouseTracker.rightButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
		//{


		//}



	}
	else
	{
		mVecUiSPos.clear();
	}
}
