#include "stdafx.h"
#include "ProgresiveBar.h"

std::string ReplaceAll(std::string &str, const std::string& from, const std::string& to) 
{
	size_t start_pos = 0; //string처음부터 검사
	while ((start_pos = str.find(from, start_pos)) != std::string::npos)  //from을 찾을 수 없을 때까지
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // 중복검사를 피하고 from.length() > to.length()인 경우를 위해서
	}
	return str;
}

ProgresiveBar::ProgresiveBar() 
{
}


ProgresiveBar::ProgresiveBar(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont)
	:MActor(pBatch,pSheet,pFont),mPBVisible(false), mCharacter(nullptr)
{
	mfTEalpsdtime = 0.0f;
	mEndTIme = false;
}

ProgresiveBar::~ProgresiveBar()
{
}

void ProgresiveBar::Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible)
{
	Animation anim[] = {
		{ "PBHp", 1,{ { "HpBar0", 0.3f } } },
		{ "PBMp", 1,{ { "MpBar0", 0.3f } } },
	{ "PBSp", 1,{ { "SpBar0", 0.3f } } },
	{ "HpIcon", 1,{ { "HpIcon0", 0.3f } } },
	{ "MpIcon", 1,{ { "MpIcon0", 0.3f } } },
	{ "SpIcon", 1,{ { "SpIcon0", 0.3f } } },
	{ "uiFan", 1,{ { "uiFan0", 0.3f } } },
	};

	//보이고 안보이고
	mPBVisible = visible;
	MActor::Init(anim, 7, eSortID);
	SetPosition(pos);
	SetAnimation("uiFan");
}

E_SCENE ProgresiveBar::Update(float dt)
{
	MActor::Update(dt);
	mEndTIme = false;
	//보여주는것이 활성화 되었을경우
	if (mPBVisible)
	{
		mfTEalpsdtime += dt;

	}

	if (mfTEalpsdtime > 1.0f)
	{
		//mPBVisible = false;
		//mEndTIme = true;
		//mfTEalpsdtime = 0.0f;
	}
	auto key = Keyboard::Get().GetState();
	
	if (key.F)
	{
		mPBVisible = !mPBVisible;
	}

	//E_SCENE eResult = MActor::Update(dt);

	//if (eResult > E_SCENE_NONPASS)
	//	return eResult;

	return E_SCENE_NONPASS;
}

void ProgresiveBar::Draw()
{
	XMFLOAT2 offset = XMFLOAT2(0, 0);
	Color tint = Colors::White;
	Color blackTint = Colors::Black;

	//offset.x = (int)ScrollMgr::Instance().GetScroll().x;
	//offset.y = (int)ScrollMgr::Instance().GetScroll().y;

	auto mouse = Mouse::Get().GetState();

	//프로그레시브바가 활성화 된다면, UI가 안켜졌을때
	if (mPBVisible)
	{

		//값이 없다면 빠져나옴
		if (!mCharacter)
		{
			return;
		}
		//에너지가 0이면 출력안하도록
		if (mCharacter->GetHealth() <= 0 || MActorManager::Instance().GetClassUi()->GetVisible())
		{
			mPBVisible = false;
			return;
		}

		//UiFan 출력
		SetAnimation("uiFan");
		mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition - offset, tint);

		//클래스 이름 출력
		wchar_t wch[128];
		auto className = typeid(*mCharacter).name();
		string str = className;
		string str2 = ReplaceAll(str, string("class "), string(""));
		className = str2.c_str();
		mbstowcs(&wch[0], className, 128);
		mpFont->DrawString(mpBatch, wch, XMFLOAT2(mPosition + XMFLOAT2(-80.0f, -60.0f)), DirectX::Colors::White, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.5f, 0.5f));

		//어느편인지 출력
		switch (mCharacter->GetCamp())
		{
		case 0:
			//swprintf_s(wch, L"Enemy_Idle %d", mCode);
			break;
		case 1:
			swprintf_s(wch, L"Player");
			mpFont->DrawString(mpBatch, wch, XMFLOAT2(mPosition + XMFLOAT2(-80.0f, 40.0f)), DirectX::Colors::Green, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.5f, 0.5f));
			break;
		case 2:
			swprintf_s(wch, L"Enemy");
			mpFont->DrawString(mpBatch, wch, XMFLOAT2(mPosition + XMFLOAT2(-80.0f, 40.0f)), DirectX::Colors::Red, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.5f, 0.5f));
			break;
		case 3:
			swprintf_s(wch, L"NPC");
			mpFont->DrawString(mpBatch, wch, XMFLOAT2(mPosition + XMFLOAT2(-80.0f, 40.0f)), DirectX::Colors::Orange, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.5f, 0.5f));
		default:
			//swprintf_s(wch, L"", mCode);
			break;
		}

		//Hp바 출력
		SetAnimation("PBHp");
		int hp = mCharacter->GetHealth();
		float maxHp = mCharacter->GetMaxHp();
		mSFHp = *mpSpriteFrame;
		float hpTemp = float(hp * mpSpriteFrame->sourceRect.right) / maxHp;
		mSFHp.sourceRect.right = (int)hpTemp;
		mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition + XMFLOAT2(20.0f, -25.0f) - offset, blackTint);
		mpSheet->Draw(mpBatch, mSFHp, mWorldPos + mPosition + XMFLOAT2(20.0f, -25.0f) - offset, tint);

		//Hp 글자 출력
		swprintf_s(wch, L"%d",hp);
		mpFont->DrawString(mpBatch, wch, XMFLOAT2(mPosition + XMFLOAT2(-20.0f, -40.0f)), DirectX::Colors::White, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.5f, 0.5f));
		swprintf_s(wch, L" / %d",static_cast<int>(maxHp));
		mpFont->DrawString(mpBatch, wch, XMFLOAT2(mPosition + XMFLOAT2(10.0f, -40.0f)), DirectX::Colors::White, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.5f, 0.5f));
		

		//Mp바 출력
		SetAnimation("PBMp");
		int mp = mCharacter->GetMana();
		float maxMp = mCharacter->GetMaxMana();
		mSFMp = *mpSpriteFrame;
		float mpTemp = float(mp * mpSpriteFrame->sourceRect.right) / maxMp;
		mSFMp.sourceRect.right = (int)mpTemp;	
		mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition + XMFLOAT2(20.0f, 0.0f) - offset, blackTint);
		mpSheet->Draw(mpBatch, mSFMp, mWorldPos + mPosition + XMFLOAT2(20.0f, 0.0f) - offset, tint);
		
		//Mp 글자 출력
		swprintf_s(wch, L"%d", mp);
		mpFont->DrawString(mpBatch, wch, XMFLOAT2(mPosition + XMFLOAT2(-20.0f, -15.0f)), DirectX::Colors::White, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.5f, 0.5f));
		swprintf_s(wch, L" / %d", static_cast<int>(maxMp));
		mpFont->DrawString(mpBatch, wch, XMFLOAT2(mPosition + XMFLOAT2(10.0f, -15.0f)), DirectX::Colors::White, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.5f, 0.5f));

		//Sp바 출력
		SetAnimation("PBSp");
		int sp = mCharacter->GetSp();
		float maxSp = mCharacter->GetMaxSp();
		mSFSp = *mpSpriteFrame;
		float spTemp = float(sp * mpSpriteFrame->sourceRect.right) / maxSp;
		mSFSp.sourceRect.right = (int)spTemp;
		mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition + XMFLOAT2(20.0f, 25.0f) - offset, blackTint);
		mpSheet->Draw(mpBatch, mSFSp , mWorldPos + mPosition + XMFLOAT2(20.0f, 25.0f) - offset, tint);
		
		//Sp 글자 출력
		swprintf_s(wch, L"%d", sp);
		mpFont->DrawString(mpBatch, wch, XMFLOAT2(mPosition + XMFLOAT2(-20.0f, 10.0f)), DirectX::Colors::White, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.5f, 0.5f));
		swprintf_s(wch, L" / %d", static_cast<int>(maxSp));
		mpFont->DrawString(mpBatch, wch, XMFLOAT2(mPosition + XMFLOAT2(10.0f, 10.0f)), DirectX::Colors::White, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.5f, 0.5f));


	}
	
}
