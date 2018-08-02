#include "stdafx.h"
#include "EventTalk.h"
#include "EventCharacter.h"

EventTalk::EventTalk()
{
}


EventTalk::EventTalk(SpriteBatch* pBatch, SpriteSheet* pSheet, SpriteFont * pFont)
	:MActor(pBatch,pSheet,pFont)
{

}

EventTalk::~EventTalk()
{
}

void EventTalk::Init(XMFLOAT2 startpos, E_SORTID eSortID)
{
	Animation anim[] = {
		{ "HuchuFace", 1,{ { "huchuFace", 0.3f },}
		},
	{ "JoJoFace", 1,{ { "jojoFace", 0.3f },
	}
	},
	{ "Soldier1Face", 1,{ { "soldier1Face", 0.3f },
	}
	},
	{ "Soldier2Face", 1,{ { "soldier2Face", 0.3f },
	 }
	},
	{ "SWordBalloon", 1,{ { "sWordBalloon", 0.3f },
	}
	},
	{ "WordBallon", 1,{ { "wordBallon", 0.3f },
	}
	},
	};
	MActor::Init(anim, 6, eSortID);
	SetPosition(startpos);
	SetAnimation("HuchuFace");
	mbVisible = false;
	mName = L"��ȭâ";
}

E_SCENE EventTalk::Update(float dt)
{
	if (MFramework::mKeyboardTracker.IsKeyPressed(Keyboard::D0))
	{
		//mbVisible = !mbVisible;
	}
	else if	(MFramework::mKeyboardTracker.IsKeyPressed(Keyboard::D9))
	{
		mbLeftRight = !mbLeftRight;
	}
	return E_SCENE_NONPASS;
}

void EventTalk::Draw()
{
	XMFLOAT2 offset = XMFLOAT2(0, 0);
	Color tint = Colors::White;

	if (mbVisible)
	{
		//�ʻ�ȭ ����
		if (mPickName == L"����1")
		{
			SetAnimation("Soldier1Face");
		}
		else if (mPickName == L"����2")
		{
			SetAnimation("Soldier2Face");
		}
		else if (mPickName == L"����")
		{
			SetAnimation("JoJoFace");
		}
		else if (mPickName == L"������")
		{ 
			SetAnimation("HuchuFace");
		}
		//���������� ��ﶧ
		if (mbLeftRight)
		{
			//�ʻ�ȭ ����
			mpSheet->Draw(mpBatch, *mpSpriteFrame
				, mWorldPos + mPosition + XMFLOAT2(250, 130), tint
				, 0, 1.0f, DirectX::SpriteEffects_None, 0.0f);

			//��糪�� ��ȭ ���� ����
			SetAnimation("WordBallon");
			mpSheet->Draw(mpBatch, *mpSpriteFrame
				, mWorldPos + mPosition+ XMFLOAT2(10,138), tint
				, 0, 1.0f, DirectX::SpriteEffects_FlipHorizontally, 0.0f);

			//���ϴ� �༮ �Ӹ� ���� ���� ����
			SetAnimation("SWordBalloon");
			mpSheet->Draw(mpBatch, *mpSpriteFrame
				, mWorldPos + mpTarget->GetPosition() + XMFLOAT2(30, -40), tint
				, 0, 1.0f, DirectX::SpriteEffects_None, 0.0f);

			XMFLOAT2 talkBoxPos = mPosition + XMFLOAT2(10, 138);
			talkBoxPos = talkBoxPos + XMFLOAT2(-162,-32);
			//�ش�ĳ���� �̸� ����
			wchar_t wch[64];
			auto mouse = Mouse::Get().GetState();
			mpFont->DrawString(mpBatch, mPickName.c_str()
				, mWorldPos + talkBoxPos
				, DirectX::Colors::Blue, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.7f, 0.7f));

			//��� ����
			//swprintf_s(wch, L"%d", index);
			swprintf_s(wch, L"x = %d y = %d", mouse.x, mouse.y);//mTileInfo[index]->underObject
			mpFont->DrawString(mpBatch, mScript.c_str()
				, mWorldPos + talkBoxPos + XMFLOAT2(0,12)
				, DirectX::Colors::Black, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.75f, 0.75f));
		}
		//�����϶�
		else
		{
			//�ʻ�ȭ ����
			mpSheet->Draw(mpBatch, *mpSpriteFrame
				, mWorldPos + mPosition + XMFLOAT2(-250, 130), tint
				, 0, 1.0f, DirectX::SpriteEffects_None, 0.0f);

			//��糪�� ��ȭ ���� ����
			SetAnimation("WordBallon");
			mpSheet->Draw(mpBatch, *mpSpriteFrame
				, mWorldPos + mPosition + XMFLOAT2(-10, 138), tint
				, 0, 1.0f, DirectX::SpriteEffects_None, 0.0f);

			//���ϴ� �༮ �Ӹ� ���� ���� ����
			SetAnimation("SWordBalloon");
			mpSheet->Draw(mpBatch, *mpSpriteFrame
				, mWorldPos + mpTarget->GetPosition() + XMFLOAT2(-30, -40), tint
				, 0, 1.0f, DirectX::SpriteEffects_FlipHorizontally, 0.0f);

			XMFLOAT2 talkBoxPos = mPosition + XMFLOAT2(-10, 138);
			talkBoxPos = talkBoxPos + XMFLOAT2(-147, -32);
			//�ش�ĳ���� �̸� ����
			wchar_t wch[64];
			auto mouse = Mouse::Get().GetState();
			mpFont->DrawString(mpBatch, mPickName.c_str()
				, mWorldPos + talkBoxPos
				, DirectX::Colors::Blue, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.7f, 0.7f));

			//��� ����
			//swprintf_s(wch, L"%d", index);
			swprintf_s(wch, L"x = %d y = %d", mouse.x, mouse.y);//mTileInfo[index]->underObject
			mpFont->DrawString(mpBatch, mScript.c_str()
				, mWorldPos + talkBoxPos + XMFLOAT2(0, 12)
				, DirectX::Colors::Black, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.75f, 0.75f));

		}

	}

}
