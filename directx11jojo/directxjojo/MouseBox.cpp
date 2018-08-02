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

	//스킬범위활성화시에 스킬범위도 그려준다 
	if (mbSkillAreaVisible)
	{
		vector<unique_ptr<TILE>> *pVecTile = MActorManager::Instance().GetTileInfo();
		auto atkBox = MActorManager::Instance().GetClassAttackBox();
		
		SetAnimation("SkillArea");
		switch (miSkillArea)
		{
			//십자일때
		case 0:
		{
			//위
			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos
				+ mPosition + XMFLOAT2(0,-48) - offset, tint);
			//아래
			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos
				+ mPosition + XMFLOAT2(0,48) - offset, tint);
			//왼쪽
			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos
				+ mPosition + XMFLOAT2(-48,0) - offset, tint);
			//오른쪽
			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos
				+ mPosition + XMFLOAT2(48,0) - offset, tint);

		}
		break;
		//마방진일때
		case 1:
		{
			//위
			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos
				+ mPosition + XMFLOAT2(0, -48) - offset, tint);
			//위오
			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos
				+ mPosition + XMFLOAT2(48, -48) - offset, tint);
			//위왼
			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos
				+ mPosition + XMFLOAT2(-48, -48) - offset, tint);
			//아래
			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos
				+ mPosition + XMFLOAT2(0, 48) - offset, tint);
			//아래오
			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos
				+ mPosition + XMFLOAT2(48, 48) - offset, tint);
			//아래 왼
			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos
				+ mPosition + XMFLOAT2(-48, 48) - offset, tint);
			//왼쪽
			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos
				+ mPosition + XMFLOAT2(-48, 0) - offset, tint);
			//오른쪽
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
		////십자
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
		////마방진
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
