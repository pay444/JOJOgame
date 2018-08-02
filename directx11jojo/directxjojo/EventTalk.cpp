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
	mName = L"대화창";
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
		//초상화 띄우기
		if (mPickName == L"병사1")
		{
			SetAnimation("Soldier1Face");
		}
		else if (mPickName == L"병사2")
		{
			SetAnimation("Soldier2Face");
		}
		else if (mPickName == L"조조")
		{
			SetAnimation("JoJoFace");
		}
		else if (mPickName == L"허차장")
		{ 
			SetAnimation("HuchuFace");
		}
		//오른쪽으로 띄울때
		if (mbLeftRight)
		{
			//초상화 띄우기
			mpSheet->Draw(mpBatch, *mpSpriteFrame
				, mWorldPos + mPosition + XMFLOAT2(250, 130), tint
				, 0, 1.0f, DirectX::SpriteEffects_None, 0.0f);

			//대사나올 대화 상자 띄우기
			SetAnimation("WordBallon");
			mpSheet->Draw(mpBatch, *mpSpriteFrame
				, mWorldPos + mPosition+ XMFLOAT2(10,138), tint
				, 0, 1.0f, DirectX::SpriteEffects_FlipHorizontally, 0.0f);

			//말하는 녀석 머리 위에 상자 띄우기
			SetAnimation("SWordBalloon");
			mpSheet->Draw(mpBatch, *mpSpriteFrame
				, mWorldPos + mpTarget->GetPosition() + XMFLOAT2(30, -40), tint
				, 0, 1.0f, DirectX::SpriteEffects_None, 0.0f);

			XMFLOAT2 talkBoxPos = mPosition + XMFLOAT2(10, 138);
			talkBoxPos = talkBoxPos + XMFLOAT2(-162,-32);
			//해당캐릭터 이름 띄우기
			wchar_t wch[64];
			auto mouse = Mouse::Get().GetState();
			mpFont->DrawString(mpBatch, mPickName.c_str()
				, mWorldPos + talkBoxPos
				, DirectX::Colors::Blue, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.7f, 0.7f));

			//대사 띄우기
			//swprintf_s(wch, L"%d", index);
			swprintf_s(wch, L"x = %d y = %d", mouse.x, mouse.y);//mTileInfo[index]->underObject
			mpFont->DrawString(mpBatch, mScript.c_str()
				, mWorldPos + talkBoxPos + XMFLOAT2(0,12)
				, DirectX::Colors::Black, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.75f, 0.75f));
		}
		//왼쪽일때
		else
		{
			//초상화 띄우기
			mpSheet->Draw(mpBatch, *mpSpriteFrame
				, mWorldPos + mPosition + XMFLOAT2(-250, 130), tint
				, 0, 1.0f, DirectX::SpriteEffects_None, 0.0f);

			//대사나올 대화 상자 띄우기
			SetAnimation("WordBallon");
			mpSheet->Draw(mpBatch, *mpSpriteFrame
				, mWorldPos + mPosition + XMFLOAT2(-10, 138), tint
				, 0, 1.0f, DirectX::SpriteEffects_None, 0.0f);

			//말하는 녀석 머리 위에 상자 띄우기
			SetAnimation("SWordBalloon");
			mpSheet->Draw(mpBatch, *mpSpriteFrame
				, mWorldPos + mpTarget->GetPosition() + XMFLOAT2(-30, -40), tint
				, 0, 1.0f, DirectX::SpriteEffects_FlipHorizontally, 0.0f);

			XMFLOAT2 talkBoxPos = mPosition + XMFLOAT2(-10, 138);
			talkBoxPos = talkBoxPos + XMFLOAT2(-147, -32);
			//해당캐릭터 이름 띄우기
			wchar_t wch[64];
			auto mouse = Mouse::Get().GetState();
			mpFont->DrawString(mpBatch, mPickName.c_str()
				, mWorldPos + talkBoxPos
				, DirectX::Colors::Blue, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.7f, 0.7f));

			//대사 띄우기
			//swprintf_s(wch, L"%d", index);
			swprintf_s(wch, L"x = %d y = %d", mouse.x, mouse.y);//mTileInfo[index]->underObject
			mpFont->DrawString(mpBatch, mScript.c_str()
				, mWorldPos + talkBoxPos + XMFLOAT2(0, 12)
				, DirectX::Colors::Black, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.75f, 0.75f));

		}

	}

}
