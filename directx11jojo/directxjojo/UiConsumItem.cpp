#include "stdafx.h"
#include "ConsumItems.h"
#include "UiConsumItem.h"


UiConsumItem::UiConsumItem()
{
}


UiConsumItem::UiConsumItem(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont)
	:MActor(pBatch, pSheet, pFont),
	mpPlayer(nullptr),
	mUICVisible(false),
	mCancelBtnPos(0, 0),
	mpCurItem(nullptr),
	mAreaVisible(false),
	mFlag(false)
{

}

UiConsumItem::~UiConsumItem()
{
	this->Release();
}

void UiConsumItem::Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible)
{
	Animation anim[] = {
	{ "UiItem", 1,{ { "uiItem", 0.1f } } },
	{ "ToolBack", 1,{ { "toolBack", 0.1f } } },
	{ "Bean", 1,{ { "hpBean", 0.1f } } },
	{ "MpWater", 1,{ { "mpWater", 0.1f } } },
	{ "AttackBox", 1,{ { "AttackBox0", 0.3f } } },
	{ "CancelBtn", 1,{ { "cancelBtn", 0.3f } } },
	};

	//�̵��Ÿ�
	//mMoveDistance = moveDis;
	//���̰� �Ⱥ��̰�
	mUICVisible = visible;
	MActor::Init(anim, 6 ,eSortID);
	SetPosition(pos + XMFLOAT2(100.0f, 0.0f));
	SetAnimation("UiItem");
	mCancelBtnPos = mPosition + XMFLOAT2(82, 64);
}

E_SCENE UiConsumItem::Update(float dt)
{
	float fScrollx = ScrollMgr::Instance().GetScroll().x;
	float fScrolly = ScrollMgr::Instance().GetScroll().y;
	MActor::Update(dt);

	//Ui�� �����ִ� �����϶�
	if (mUICVisible)
	{
		//��� ��ư�� ��ġ ����
		mCancelBtnPos = mPosition + XMFLOAT2(82, 64);

		//�ش� ��ų�� Ŭ���� �ÿ� ��ų�� ���õȴ�.
		if (MFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
		{
			auto mouse = Mouse::Get().GetState();

			POINT mousePos;
			mousePos.x = mouse.x + fScrollx;
			mousePos.y = mouse.y + fScrolly;

		}
	}
	return E_SCENE_NONPASS;
}

void UiConsumItem::Draw()
{
}

void UiConsumItem::Release()
{
	auto iter = mspVecAreaIndex.begin();
	while (iter != mspVecAreaIndex.end())
	{
		iter->reset();
		iter = mspVecAreaIndex.erase(iter);
	}
}

void UiConsumItem::CalArea(int distance)
{
	int JoTileCx = 20;
	int JoTileCy = 20;
	//ĳ���� ������ �׸� ĭ�� ���Ϳ� �־���
	int tileOnPlayerIndex = GetTileIndex(mpPlayer->GetPosition());
	//���;ȿ� �÷��̾ ���ִ� ��ġ�� �־���
	mspVecAreaIndex.push_back(make_unique<int>(tileOnPlayerIndex));
	//8���� Ȯ���� ���� ���Ϳ� �־���
	//��
	if ((tileOnPlayerIndex) >= JoTileCx)
	{
		mspVecAreaIndex.push_back(make_unique<int>(tileOnPlayerIndex-JoTileCx));
	}
	//����
	if ((tileOnPlayerIndex) >= JoTileCx && tileOnPlayerIndex 
		% JoTileCx != JoTileCx - 1)
	{
		mspVecAreaIndex.push_back(
			make_unique<int>(tileOnPlayerIndex - (JoTileCx - 1)));
	}
	//��
	if (tileOnPlayerIndex % JoTileCx != JoTileCx - 1)
	{
		mspVecAreaIndex.push_back(make_unique<int>(tileOnPlayerIndex + 1));
	}
	//����
	if ((tileOnPlayerIndex) < JoTileCx * JoTileCy - JoTileCx 
		&& tileOnPlayerIndex % JoTileCx != JoTileCx - 1)
	{
		mspVecAreaIndex.push_back(
			make_unique<int>(tileOnPlayerIndex + (JoTileCx + 1)));
	}
	//�Ʒ�
	if ((tileOnPlayerIndex) < JoTileCx * JoTileCy - JoTileCx)
	{
		mspVecAreaIndex.push_back(make_unique<int>(tileOnPlayerIndex + JoTileCx));
	}
	//�޾�
	if ((tileOnPlayerIndex) < JoTileCx * JoTileCy - JoTileCx 
		&& tileOnPlayerIndex % JoTileCx != 0)
	{
		mspVecAreaIndex.push_back(
			make_unique<int>(tileOnPlayerIndex + (JoTileCx + 1)));

	}
	//��
	if (tileOnPlayerIndex % JoTileCx != 0)
	{
		mspVecAreaIndex.push_back(
			make_unique<int>(tileOnPlayerIndex - 1));
	}
	//����
	if ((tileOnPlayerIndex) >= JoTileCx
		&& tileOnPlayerIndex % JoTileCx != 0)
	{
		mspVecAreaIndex.push_back(
			make_unique<int>(tileOnPlayerIndex - (JoTileCx + 1)));
	}


}
