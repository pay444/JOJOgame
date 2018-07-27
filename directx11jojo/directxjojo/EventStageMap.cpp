#include "stdafx.h"
#include "EventStageMap.h"


EventStageMap::EventStageMap()
{
}


EventStageMap::EventStageMap(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont)
	:MActor(pBatch,pSheet,pFont)
{

}

EventStageMap::~EventStageMap()
{
}

void EventStageMap::Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible)
{
	Animation anim[] = {
		{ "StageStart", 1,{ { "stageStart", 0.3f } } },
	};
	mVisble = false;
	mfElpTime = 0.0f;
	//�̵��Ÿ�
	//mMoveDistance = moveDis;
	//���̰� �Ⱥ��̰�
	//mUISVisible = true;
	MActor::Init(anim, 1, eSortID);
	SetPosition(pos);
	SetAnimation("StageStart");
}

E_SCENE EventStageMap::Update(float dt)
{
	if (MFramework::mKeyboardTracker.IsKeyPressed(Keyboard::D0))
	{
		mVisble = !mVisble;
	}

	//�����ð� ����� ���� �ٲ���
	if (mVisble)
	{
		mfElpTime += dt;
	}
	if (mfElpTime >= 2.0f)
	{
		switch (mScene)
		{
		case 1:
			return E_SCENE_STAGE;
			break;
		default:
			break;
		}
	}
	return E_SCENE_NONPASS;
}

void EventStageMap::Draw()
{
	XMFLOAT2 offset = XMFLOAT2(0, 0);
	Color tint = Colors::White;
	if (mVisble)
	{
		//��� �� ���
		mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition, tint
			, 0, 1.2f, DirectX::SpriteEffects_None, 0.0f);

		mStageName = L"��õ����";
		RECT rc;	
		GetClientRect(SceneMgr::Instance().GetHWND(), &rc);
		//� �������� �� ���� ���� ���
		mpFont->DrawString(mpBatch, mStageName.c_str(),
			XMFLOAT2(rc.right / 2 - 90, rc.bottom / 2 - 20), DirectX::Colors::White,
			0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(2.0f, 2.0f));

	}

}
