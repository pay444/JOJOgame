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

	//�ִϸ��̼��� ������ ������ �����ϴ� �༮�̶�� �ð� ����
	if (mbAnimSkill)
	{
		mfAnimTime += dt;
	}
	else
	{
		mfAnimTime = 0.0f;
	}

	//�ִϸ��̼��� ������ ���� �༮�� �����Ѵ�.
	if (mbAnimSkill && mfAnimTime >= mfDelayAnimTime)
	{
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
			//�ִϸ��̼��� �׷��ִٰ� ������ �ð��� ������ �Ϲ����� ������ ����Ʈ�� �׷��ش�.
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
		//�׷��ִٰ� �� �ִϸ��̼��� ������ ������ �̶�� �ٲ����
		//mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition - offset, tint);
			//atkBox->

	}
	break;

	default:
		break;
	}

}
