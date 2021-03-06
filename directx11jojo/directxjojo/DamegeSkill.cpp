#include "stdafx.h"
#include "DamegeSkill.h"


DamegeSkill::DamegeSkill()
{
}


DamegeSkill::DamegeSkill(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont)
	:Skill(pBatch,pSheet,pFont)
{

}

DamegeSkill::~DamegeSkill()
{
}

void DamegeSkill::Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible)
{

}

void DamegeSkill::Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible, Animation* anim, int animCount)
{
	MActor::Init(anim, animCount, eSortID);
	SetPosition(pos);
	
	SetAnimation(anim->Name);
	mbAnimSkill = false;
	mfAnimTime = 0.0f;
	miSkillSingleMulti = 0;
}

E_SCENE DamegeSkill::Update(float dt)
{
	//E_SCENE eResult =

	MActor::Update(dt);

	//애니메이션이 나오고 공격을 시작하는 녀석이라면 시간 측정
	if (mbAnimSkill)
	{
		mfAnimTime += dt;
	}
	else
	{
		mfAnimTime = 0.0f;
	}

	//애니메이션이 끝나면 다음 녀석을 실행한다. //단일 스킬일 경우도 애니메이션은 아니지만 들어옴
	if (mbAnimSkill && mfAnimTime >= mfDelayAnimTime)
	{
		//속성에 따른 음악재생
		switch (mElemental)
		{
		//불
		case 1:
		{
			FMOD_System_PlaySound(MActorManager::Instance().GetFMODSystem(), (*MActorManager::Instance().GetVecFMODSound())[1], 0, 0, &(*MActorManager::Instance().GetVecFMODChannal())[1]);

		}
		break;
		//바람
		case 2:
		{
			FMOD_System_PlaySound(MActorManager::Instance().GetFMODSystem(), (*MActorManager::Instance().GetVecFMODSound())[2], 0, 0, &(*MActorManager::Instance().GetVecFMODChannal())[1]);
		}
		break;
		//주작도 불
		case 4:
		{
			FMOD_System_PlaySound(MActorManager::Instance().GetFMODSystem(), (*MActorManager::Instance().GetVecFMODSound())[1], 0, 0, &(*MActorManager::Instance().GetVecFMODChannal())[1]);

		}
		break;
		default:
			break;
		}

		mbAnimSkill = false;
		mfAnimTime = 0.0f;

	}
	return E_SCENE_NONPASS;
}

void DamegeSkill::Draw()
{
	XMFLOAT2 offset = XMFLOAT2(0, 0);
	Color tint = Colors::White;
	offset.x = (int)ScrollMgr::Instance().GetScroll().x;
	offset.y = (int)ScrollMgr::Instance().GetScroll().y;


	//싱글 스킬인지 멀티 스킬인지 에 따라 그리는 방식이 달라짐
	switch (miSkillSingleMulti)
	{
		//싱글 일때
	case 0:
	{
		mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition - offset, tint);
	}
	break;
	//멀티. 즉 여러명을 때리는 스킬일때
	case 1:
	{
		//애니메이션 스킬일 경우
		if (mbAnimSkill)
		{
			//애니메이션을 그려주다가 딜레이 시간이 끝난후 일반적인 데미지 이펙트를 그려준다.
			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition - offset, tint);
		}
		else
		{
			SetAnimation("Effect");
			vector<unique_ptr<TILE>> *pVecTile = MActorManager::Instance().GetTileInfo();
			auto atkBox = MActorManager::Instance().GetClassAttackBox();
			auto vecAtkSkillAreaIndex = atkBox->GetVecSkillAreaIndex();
			for (size_t i = 0; i <vecAtkSkillAreaIndex->size(); i++)
			{
				auto index = (*vecAtkSkillAreaIndex)[i];
				XMFLOAT2 pos = (*pVecTile)[index]->vPos + Vector2(JOJOTILESX/2, JOJOTILESY/2);
				mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + pos - offset, tint);

			}
		}
		//그려주다가 그 애니메이션이 마지막 프레임 이라면 바꿔버림
		//mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition - offset, tint);
			//atkBox->

	}
	break;

	default:
		break;
	}

}
