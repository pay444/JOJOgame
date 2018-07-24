#include "stdafx.h"
#include "SeonPung.h"


SeonPung::SeonPung()
{
}


SeonPung::SeonPung(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont)
	:DamegeSkill(pBatch,pSheet,pFont)
{

}

SeonPung::~SeonPung()
{
}

void SeonPung::Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible)
{
	Animation anim[] = {
		{ "Wind", 20,{ 
	{ "wind_001", 0.04f },
	{ "wind_002", 0.04f },
	{ "wind_003", 0.04f },
	{ "wind_004", 0.04f },
	{ "wind_005", 0.04f },
	{ "wind_006", 0.04f },
	{ "wind_007", 0.04f },
	{ "wind_008", 0.04f },
	{ "wind_009", 0.04f },
	{ "wind_010", 0.04f },
	{ "wind_011", 0.04f },
	{ "wind_012", 0.04f },
	{ "wind_013", 0.04f },
	{ "wind_014", 0.04f },
	{ "wind_015", 0.04f },
	{ "wind_016", 0.04f },
	{ "wind_017", 0.04f },
	{ "wind_018", 0.04f },
	{ "wind_019", 0.04f },
	{ "wind_020", 0.04f },
	}
	},
	};
	MActor::Init(anim, 1, eSortID);
	SetPosition(pos + XMFLOAT2(100.0f, 0.0f));
	SetAnimation("Wind");
	mMpCost = 6;
	mElemental = 3;
	mArea = 4;
	mDamage = 10;
	mName = L"선풍";
	mEfficacy = L"공격(풍)";
}

E_SCENE SeonPung::Update(float dt)
{
	DamegeSkill::Update(dt);
	if (mSPVisible)
	{

	}
	return E_SCENE_NONPASS;
}

void SeonPung::Draw()
{
	XMFLOAT2 offset = XMFLOAT2(0, 0);
	Color tint = Colors::White;
	offset.x = (int)ScrollMgr::Instance().GetScroll().x;
	offset.y = (int)ScrollMgr::Instance().GetScroll().y;

	mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition - offset, tint);

	/*
	//공격 범위 출력 게이지 출력
	auto pTexture = ResourceManager::Instance().GetShaderResource(L"Images\\jojo\\jojoSprites.png");//(L"Images\\sprites.png");
	auto pSheet = ResourceManager::Instance().GetSpriteSheet(L"Images\\jojo\\jojoSprites.xml", pTexture); //(L"Images\\sprites.xml",pTexture);
	
	auto result = mAnimations.find("AttackBox");
	mpSpriteFrame2 = pSheet->Find(result->second.begin()->FrameName.c_str());
	SpriteFrame mSFMp;
	SpriteFrame sfSource;
	//mpSpriteFrame = mpSheet->Find(mCurFrame->FrameName.c_str());

	int mp = mpPlayer->GetMana();
	float maxMp = mpPlayer->GetMaxMana();
	mSFMp = *mpSpriteFrame2;
	sfSource = *mpSpriteFrame2;

	sfSource.sourceRect.right = sfSource.sourceRect.left + (sfSource.sourceRect.right - sfSource.sourceRect.left) / 2;
	mSFMp.sourceRect.right = mSFMp.sourceRect.left + (mSFMp.sourceRect.right - mSFMp.sourceRect.left) / 2;

	float mpTemp = float(mp * (mSFMp.sourceRect.right - mSFMp.sourceRect.left)) / maxMp;
	mSFMp.sourceRect.right = mSFMp.sourceRect.left + (int)mpTemp;

	pSheet->Draw(mpBatch, sfSource, mWorldPos + mPosition + XMFLOAT2(50.0f, -67.0f) - offset, blackTint, 0.0f, 1.5f);
	pSheet->Draw(mpBatch, mSFMp, mWorldPos + mPosition + XMFLOAT2(50.0f, -67.0f) - offset, tint, 0.0f, 1.5f);
	*/
}
