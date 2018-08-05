#include "stdafx.h"
#include "HealSkill.h"


HealSkill::HealSkill()
{
}


HealSkill::HealSkill(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont)
	:Skill(pBatch,pSheet,pFont)
{

}

HealSkill::~HealSkill()
{
}

void HealSkill::Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible)
{
}

void HealSkill::Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible, Animation * anim, int animCount)
{
	MActor::Init(anim, animCount, eSortID);
	SetPosition(pos);

	SetAnimation(anim->Name);
	mbAnimSkill = false;
	mfAnimTime = 0.0f;
	miSkillSingleMulti = 0;
}

E_SCENE HealSkill::Update(float dt)
{
	MActor::Update(dt);

	//�ִϸ��̼��� ������ ������ �����ϴ� �༮�̶�� �ð� ����
	if (mbAnimSkill)
	{
		mfAnimTime += dt;
	}
	else
	{
		mfAnimTime = 0.0f;
	}

	//�ִϸ��̼��� ������ ���� �༮�� �����Ѵ�.  //���� ��ų�� ��쵵 �ִϸ��̼��� �ƴ����� ����
	if (mbAnimSkill && mfAnimTime >= mfDelayAnimTime)
	{
		//�Ӽ��� ���� �������
		switch (mElemental)
		{
		case 6:
		{
			FMOD_System_PlaySound(MActorManager::Instance().GetFMODSystem(), (*MActorManager::Instance().GetVecFMODSound())[6], 0, 0, &(*MActorManager::Instance().GetVecFMODChannal())[1]);

		}
		break;
		default:
		{
			FMOD_System_PlaySound(MActorManager::Instance().GetFMODSystem(), (*MActorManager::Instance().GetVecFMODSound())[6], 0, 0, &(*MActorManager::Instance().GetVecFMODChannal())[1]);
		}
		break;
		}
		mbAnimSkill = false;
		mfAnimTime = 0.0f;

	}
	return E_SCENE_NONPASS;
}

void HealSkill::Draw()
{
	XMFLOAT2 offset = XMFLOAT2(0, 0);
	Color tint = Colors::White;
	offset.x = (int)ScrollMgr::Instance().GetScroll().x;
	offset.y = (int)ScrollMgr::Instance().GetScroll().y;


	//�̱� ��ų���� ��Ƽ ��ų���� �� ���� �׸��� ����� �޶���
	switch (miSkillSingleMulti)
	{
		//�̱� �϶�
	case 0:
	{
		mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition - offset, tint);
	}
	break;
	//��Ƽ. �� �������� ������ ��ų�϶�
	case 1:
	{
		//�ִϸ��̼� ��ų�� ���
		if (mbAnimSkill)
		{
			//�ִϸ��̼��� �׷��ִٰ� ������ �ð��� ������ �Ϲ����� ����Ʈ�� �׷��ش�.
			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition - offset, tint);
		}
		else
		{
			SetAnimation("Effect");
			vector<unique_ptr<TILE>> *pVecTile = MActorManager::Instance().GetTileInfo();
			auto atkBox = MActorManager::Instance().GetClassAttackBox();
			auto vecAtkSkillAreaIndex = atkBox->GetVecSkillAreaIndex();
			for (size_t i = 0; i < vecAtkSkillAreaIndex->size(); i++)
			{
				auto index = (*vecAtkSkillAreaIndex)[i];
				XMFLOAT2 pos = (*pVecTile)[index]->vPos + Vector2(JOJOTILESX / 2, JOJOTILESY / 2);
				mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + pos - offset, tint);

			}
		}
	}
	break;

	default:
		break;
	}
}
