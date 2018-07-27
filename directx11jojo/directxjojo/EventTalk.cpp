#include "stdafx.h"
#include "EventTalk.h"


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
	{ "JoJoFace", 1,{ { "jojoLFace", 0.3f },
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
	SetPosition(startpos);
	SetAnimation("HuchuFace");
	MActor::Init(anim, 6, eSortID);
	mbVisible = false;
	mName = L"대화창";
}

E_SCENE EventTalk::Update(float dt)
{
	if (MFramework::mKeyboardTracker.IsKeyPressed(Keyboard::D0))
	{
		mbVisible = !mbVisible;
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
		mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition, tint
			, 0, 1.0f, DirectX::SpriteEffects_None, 0.0f);

		//해당캐릭터 이름 띄우기
		wchar_t wch[64];
		auto mouse = Mouse::Get().GetState();

		//swprintf_s(wch, L"%d", index);
		swprintf_s(wch, L"x = %d y = %d", mouse.x, mouse.y);//mTileInfo[index]->underObject
		mpFont->DrawString(mpBatch, mPickName.c_str(), mWorldPos + mPosition, DirectX::Colors::Black, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.5f, 0.5f));

		

	}

}
