#include "stdafx.h"
#include "MouseBox.h"


MouseBox::MouseBox()
{
}


MouseBox::MouseBox(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont)
	:MActor(pBatch,pSheet,pFont)
{

}

MouseBox::~MouseBox()
{
}

void MouseBox::Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible)
{
	Animation anim[] = {
		{ "MouseBox", 1,{ { "mouseBox", 0.3f } }
		},
	{ "Mouse", 1,{ { "mouse", 0.3f } }
	},
	{ "SkillArea", 1,{ { "skillArea", 0.3f } }
	},
	{ "Click", 2,{ { "click1", 0.3f }
	,{ "click2", 0.3f } }
	},
	};
	mbSkillAreaVisible = visible;
	MActor::Init(anim, 4, eSortID);
	SetPosition(pos);
	SetAnimation(anim->Name);
	miSkillArea = 0;
}

E_SCENE MouseBox::Update(float dt)
{
	MActor::Update(dt);
	//if (MFramework::mKeyboardTracker.IsKeyPressed(Keyboard::D9))
	//{
	//	mbSkillAreaVisible = !mbSkillAreaVisible;
	//	//miSkillArea = 1;
	//}
	return E_SCENE_NONPASS;
}

void MouseBox::Draw()
{
	XMFLOAT2 offset = XMFLOAT2(0, 0);
	Color tint = Colors::White;
	auto mouse = Mouse::Get().GetState();

	float fScrollx = ScrollMgr::Instance().GetScroll().x;
	float fScrolly = ScrollMgr::Instance().GetScroll().y;
	int x, y;
	x = (mouse.x + fScrollx) / 48;
	y = (mouse.y + fScrolly) / 48;
	x = x * 48 + 24 - fScrollx;
	y = y * 48 + 24 - fScrolly;
	XMFLOAT2 mousePos = XMFLOAT2(x, y);
	mPosition = mousePos;
	SetAnimation("MouseBox");
	mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mousePos - offset, tint);

	//��ų����Ȱ��ȭ�ÿ� ��ų������ �׷��ش� 
	if (mbSkillAreaVisible)
	{
		vector<unique_ptr<TILE>> *pVecTile = MActorManager::Instance().GetTileInfo();
		auto atkBox = MActorManager::Instance().GetClassAttackBox();
		
		SetAnimation("SkillArea");
		switch (miSkillArea)
		{
			//�����϶�
		case 0:
		{
			//��
			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos
				+ mPosition + XMFLOAT2(0,-48) - offset, tint);
			//�Ʒ�
			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos
				+ mPosition + XMFLOAT2(0,48) - offset, tint);
			//����
			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos
				+ mPosition + XMFLOAT2(-48,0) - offset, tint);
			//������
			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos
				+ mPosition + XMFLOAT2(48,0) - offset, tint);

		}
		break;
		//�������϶�
		case 1:
		{
			//��
			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos
				+ mPosition + XMFLOAT2(0, -48) - offset, tint);
			//����
			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos
				+ mPosition + XMFLOAT2(48, -48) - offset, tint);
			//����
			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos
				+ mPosition + XMFLOAT2(-48, -48) - offset, tint);
			//�Ʒ�
			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos
				+ mPosition + XMFLOAT2(0, 48) - offset, tint);
			//�Ʒ���
			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos
				+ mPosition + XMFLOAT2(48, 48) - offset, tint);
			//�Ʒ� ��
			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos
				+ mPosition + XMFLOAT2(-48, 48) - offset, tint);
			//����
			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos
				+ mPosition + XMFLOAT2(-48, 0) - offset, tint);
			//������
			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos
				+ mPosition + XMFLOAT2(48, 0) - offset, tint);
		}
		break;
		default:
			break;
		}
		//auto vecSkillAreaIndex = atkBox->GetVecSkillAreaIndex();
		//for (size_t i = 0; i < vecSkillAreaIndex->size(); i++)
		{
			//auto index = (*vecSkillAreaIndex)[i];

			//
		}
		//switch (miSkillArea)
		//{
		////����
		//case 0:
		//{
		//	auto vecSkillAreaIndex = atkBox->GetVecSkillAreaIndex();
		//	for (size_t i = 0; i < vecSkillAreaIndex->size(); i++)
		//	{
		//		auto index = (*vecSkillAreaIndex)[i];
		//		mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos
		//			+ (*pVecTile)[index]->vPos - offset, tint);
		//	}
		//}
		//break;
		////������
		//case 1:
		//{
		//	auto vecSkillAreaIndex = atkBox->GetVecSkillAreaIndex();
		//	for (size_t i = 0; i < vecSkillAreaIndex->size(); i++)
		//	{
		//		auto index = (*vecSkillAreaIndex)[i];
		//		mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos
		//			+ (*pVecTile)[index]->vPos - offset, tint);
		//	}
		//}
		//break;
		//default:
		//	break;
		//}
	}
}
