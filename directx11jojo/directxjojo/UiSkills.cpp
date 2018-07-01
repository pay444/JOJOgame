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

	//�̵��Ÿ�
	//mMoveDistance = moveDis;
	//���̰� �Ⱥ��̰�
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

		//Ŭ������ ���� ��ų ���� ����
		if (mpPlayer->GetClassType() == GunGeon::ClassType::MONARCH)
		{
			mCountSkill = 1;
		}

		//�ش� Ŭ������ ��ų������ŭ UI �󿡼� ��ġ ����
		if (mVecUiSPos.size() == 0)
		{
			for (int i = 0; i < mCountSkill; ++i)
			{
				float ysize = 30.0f;
				float uiPos = 18 * i;
				mVecUiSPos.push_back(mPosition + XMFLOAT2(1.0f, uiPos - ysize));
			}
		}
		//��ҹ�ư�� ��ġ ����
		mCancelBtnPos = mPosition + XMFLOAT2(82, 64);

		auto mouse = Mouse::Get().GetState();

		POINT mousePos;
		mousePos.x = mouse.x + fScrollx;
		mousePos.y = mouse.y + fScrolly;

		//��� ��ư�� Ŭ�������� �Ⱥ����ش�
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

	//�̵������� �����ٶ�
	if (mUISVisible)
	{
		//��ų �⺻ UI ���
		SetAnimation("SkillUi");
		mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition - offset, tint);

		//��� ��ư ���
		SetAnimation("CancelBtn");
		mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mCancelBtnPos - offset, tint);
		
		//�ش� ĳ������ Ŭ������ ���� ���
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
