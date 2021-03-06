#include "stdafx.h"
#include "UI.h"


UI::UI():MActor()
{
}

UI::UI(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont)
	:MActor(pBatch,pSheet,pFont),
	mpPlayer(nullptr),
	mUIVisible(false)
{
	
	auto pTexture = ResourceManager::Instance().GetShaderResource(L"Images\\Skill\\SkillUis.png");
	auto spSheet = ResourceManager::Instance().GetSpriteSheet(L"Images\\Skill\\SkillUis.xml", pTexture);
	
	mspUiSkill = make_unique<UiSkills>(pBatch, spSheet, pFont);

	pTexture = ResourceManager::Instance().GetShaderResource
	(L"Images\\Item\\ConsumItems.png");
	spSheet = ResourceManager::Instance().GetSpriteSheet
	(L"Images\\Item\\ConsumItems.xml", pTexture);

	mspUiConsumItem = make_unique<UiConsumItem>(pBatch, spSheet, pFont);
}


UI::~UI()
{
	this->Release();
}

void UI::Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible)
{
	Animation anim[] = {
		{ "Ui0", 1,{ { "Ui0", 0.3f }}},
		{ "Ui1", 1,{ { "Ui1", 0.3f } } },
		{ "Ui2", 1,{ { "Ui2", 0.3f } } },
		{ "Ui3", 1,{ { "Ui3", 0.3f } } },
		{ "Ui4", 1,{ { "Ui4", 0.3f } } },
		{ "Ui5", 1,{ { "Ui5", 0.3f } } },
		{ "Ui6", 1,{ { "Ui6", 0.3f } } },
		{ "Ui7", 1,{ { "Ui7", 0.3f } } },
	};

	//이동거리
	//mMoveDistance = moveDis;
	//보이고 안보이고
	mUIVisible = visible;
	MActor::Init(anim, 8,eSortID);
	SetPosition(pos + XMFLOAT2(100.0f,0.0f));
	SetAnimation("Ui0");

	mspUiSkill->Init(eSortID, pos, visible);
	mspUiConsumItem->Init(eSortID, pos, visible);
}

E_SCENE UI::Update(float dt)
{
	MActor::Update(dt);

	if (mUIVisible)
	{
		//mspUiSkill->SetPosition(mPosition + XMFLOAT2(100.0f, 0.0f));
		//mspUiSkill->SetPlayer(mpPlayer);
		//mspUiConsumItem->SetPosition(mPosition + XMFLOAT2(100.0f, 0.0f));
		//mspUiConsumItem->SetPlayer(mpPlayer);

		//화면 밖으로 넘어간다면 왼쪽에 위치시켜줌 기보은 오른쪽위치
		XMFLOAT2 pos = mPosition + XMFLOAT2(84.0f, 22.0f*8);
		RECT crc;
		GetClientRect(SceneMgr::Instance().GetHWND(), &crc);
		auto screen = XMFLOAT2(crc.right, crc.bottom);
		if (pos.x >= screen.x)
		{
			mPosition = mPosition + XMFLOAT2(-200.0f, 0);
		}
		else if (pos.y >= screen.y)
		{
			mPosition = XMFLOAT2(mPosition.x, (screen.y-(22.0f*8)));
		}

		//7개의 UI그림 위치 지정
		if (mVecUiPos.size() == 0)
		{

			for (int i = 0; i < 8; ++i)
			{
				float uiPos = 21 * i;
				mVecUiPos.push_back(mPosition + XMFLOAT2(0.0f, uiPos));
			}
		}
	}
	else
	{
		mVecUiPos.clear();
	}

	mspUiSkill->Update(dt);

	mspUiConsumItem->Update(dt);
	//E_SCENE eResult = MActor::Update(dt);

	//if (eResult > E_SCENE_NONPASS)
	//	return eResult;

	//auto state = Keyboard::Get().GetState();
	//if (state.W)
	//{
	//	return E_SCENE_STAGE;
	//}

	return E_SCENE_NONPASS;
}

void UI::Draw()
{
	XMFLOAT2 offset = XMFLOAT2(0, 0);
	Color tint = Colors::White;

	offset.x = (int)ScrollMgr::Instance().GetScroll().x;
	offset.y = (int)ScrollMgr::Instance().GetScroll().y;

	auto mouse = Mouse::Get().GetState();

	//이동범위를 보여줄때
	if (mUIVisible)
	{
		//if (MFramework::mMouseTracker.rightButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
		//{


		//}

		if (mVecUiPos.size() <= 0)
			return;

		for (int i = 0; i < 8; ++i)
		{
			string name;
			char ch[10];
			sprintf_s(ch, sizeof(ch),"Ui%d", i);
			name = ch;
			SetAnimation(name);

			mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mVecUiPos[i] - offset, tint);
		}
		
	}
	else
	{
		mVecUiPos.clear();
	}
	
	mspUiSkill->Draw();
	mspUiConsumItem->Draw();
}

void UI::Release()
{
	mspUiSkill.reset();
	mspUiSkill = nullptr;

	mspUiConsumItem.reset();
	mspUiConsumItem = nullptr;
}

bool UI::CheckAttackArea()
{
	if (MFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	{
		if (mUIVisible)
		{

			float fScrollx = ScrollMgr::Instance().GetScroll().x;
			float fScrolly = ScrollMgr::Instance().GetScroll().y;
			auto mouse = Mouse::Get().GetState();

			XMFLOAT2 mousePos = XMFLOAT2(mouse.x + fScrollx, mouse.y + fScrolly);

			//UI 그림의 사각형 위치
			/*RECT rct = GetBound();*/

			//XMFLOAT2 uiAttackPosx = XMFLOAT2(rct.left, rct.right);
			//XMFLOAT2 uiAttackPosy = XMFLOAT2(rct.top, rct.bottom);
			//그림의 위치를 통해서 마우스의 위치 확인하여 반응 구현
			//if (mousePos.x >= uiAttackPosx.x && mousePos.x <= uiAttackPosx.y &&
			//	mousePos.y >= uiAttackPosy.x && mousePos.y <= ((uiAttackPosy.y - uiAttackPosy.x) / 8 + uiAttackPosy.x))
			//{
			//	return true;
			//}
			
			RECT src = GetBound();
			RECT trg;
			trg.left = mousePos.x;
			trg.right = mousePos.x+5;
			trg.top = mousePos.y;
			trg.bottom = mousePos.y+5;
			RECT intersect;
			return	::IntersectRect(&intersect, &src, &trg);
		}
	}
	return false;
}

bool UI::CheckWaitArea()
{
	if (MFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	{
		if (mUIVisible)
		{

			float fScrollx = ScrollMgr::Instance().GetScroll().x;
			float fScrolly = ScrollMgr::Instance().GetScroll().y;
			auto mouse = Mouse::Get().GetState();

			XMFLOAT2 mousePos = XMFLOAT2(mouse.x + fScrollx, mouse.y + fScrolly);

			////UI 그림의 사각형 위치
			//RECT rct = GetBound();

			//XMFLOAT2 uiWaitPosx = XMFLOAT2(rct.left, rct.right);
			//XMFLOAT2 uiWaitPosy = XMFLOAT2(rct.top, rct.bottom);
			//float oneBoxPos = ((uiWaitPosy.y - uiWaitPosy.x) / 8);
			//float oneBoxpos2 = oneBoxPos * 7 + uiWaitPosy.x;

			////그림의 위치를 통해서 마우스의 위치 확인하여 반응 구현
			//if (mousePos.x >= uiWaitPosx.x && 
			//	mousePos.x <= uiWaitPosx.y &&
			//	mousePos.y >= uiWaitPosy.x &&
			//	mousePos.y >= oneBoxpos2-oneBoxPos &&
			//	mousePos.y <= oneBoxpos2)
			//{
			//	return true;
			//}
			RECT src = GetBound();
			int UiBlock = src.bottom - src.top;
			src.top += UiBlock *6;
			src.bottom += UiBlock *6;
			RECT trg;
			trg.left = mousePos.x;
			trg.right = mousePos.x + 5;
			trg.top = mousePos.y;
			trg.bottom = mousePos.y + 5;
			RECT intersect;
			return	::IntersectRect(&intersect, &src, &trg);


		}
	}
	return false;
}

bool UI::CheckSkillArea()
{
	if (MFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	{
		if (mUIVisible)
		{

			float fScrollx = ScrollMgr::Instance().GetScroll().x;
			float fScrolly = ScrollMgr::Instance().GetScroll().y;
			auto mouse = Mouse::Get().GetState();

			XMFLOAT2 mousePos = XMFLOAT2(mouse.x + fScrollx, mouse.y + fScrolly);

			////UI 그림의 사각형 위치
			//RECT rct = GetBound();

			//XMFLOAT2 uiWaitPosx = XMFLOAT2(rct.left, rct.right);
			//XMFLOAT2 uiWaitPosy = XMFLOAT2(rct.top, rct.bottom);
			//float oneBoxPos = ((uiWaitPosy.y - uiWaitPosy.x) / 8);
			//float oneBoxpos2 = oneBoxPos * 7 + uiWaitPosy.x;

			////그림의 위치를 통해서 마우스의 위치 확인하여 반응 구현
			//if (mousePos.x >= uiWaitPosx.x && 
			//	mousePos.x <= uiWaitPosx.y &&
			//	mousePos.y >= uiWaitPosy.x &&
			//	mousePos.y >= oneBoxpos2-oneBoxPos &&
			//	mousePos.y <= oneBoxpos2)
			//{
			//	return true;
			//}
			RECT src = GetBound();
			int UiBlock = src.bottom - src.top;
			src.top += UiBlock * 1;
			src.bottom += UiBlock * 1;
			RECT trg;
			trg.left = mousePos.x;
			trg.right = mousePos.x + 5;
			trg.top = mousePos.y;
			trg.bottom = mousePos.y + 5;
			RECT intersect;
			return	::IntersectRect(&intersect, &src, &trg);
		}
	}
	return false;
}

bool UI::CheckCancelArea()
{
	if (MFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	{
		if (mUIVisible)
		{

			float fScrollx = ScrollMgr::Instance().GetScroll().x;
			float fScrolly = ScrollMgr::Instance().GetScroll().y;
			auto mouse = Mouse::Get().GetState();

			XMFLOAT2 mousePos = XMFLOAT2(mouse.x + fScrollx, mouse.y + fScrolly);

			RECT src = GetBound();
			int UiBlock = src.bottom - src.top;
			src.top += UiBlock * 7;
			src.bottom += UiBlock * 7;
			RECT trg;
			trg.left = mousePos.x;
			trg.right = mousePos.x + 5;
			trg.top = mousePos.y;
			trg.bottom = mousePos.y + 5;
			RECT intersect;
			return	::IntersectRect(&intersect, &src, &trg);
		}
	}
	return false;
}

bool UI::CheckConsumItemArea()
{
	if (MFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	{
		if (mUIVisible)
		{

			float fScrollx = ScrollMgr::Instance().GetScroll().x;
			float fScrolly = ScrollMgr::Instance().GetScroll().y;
			auto mouse = Mouse::Get().GetState();

			XMFLOAT2 mousePos = XMFLOAT2(mouse.x + fScrollx, mouse.y + fScrolly);

			RECT src = GetBound();
			int UiBlock = src.bottom - src.top;
			src.top += UiBlock * 2;
			src.bottom += UiBlock * 2;
			RECT trg;
			trg.left = mousePos.x;
			trg.right = mousePos.x + 5;
			trg.top = mousePos.y;
			trg.bottom = mousePos.y + 5;
			RECT intersect;
			return	::IntersectRect(&intersect, &src, &trg);
		}
	}
	return false;
}

bool UI::CheckUltimateArea()
{
	if (MFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	{
		if (mUIVisible)
		{

			float fScrollx = ScrollMgr::Instance().GetScroll().x;
			float fScrolly = ScrollMgr::Instance().GetScroll().y;
			auto mouse = Mouse::Get().GetState();

			XMFLOAT2 mousePos = XMFLOAT2(mouse.x + fScrollx, mouse.y + fScrolly);

			RECT src = GetBound();
			int UiBlock = src.bottom - src.top;
			src.top += UiBlock * 5;
			src.bottom += UiBlock * 5;
			RECT trg;
			trg.left = mousePos.x;
			trg.right = mousePos.x + 5;
			trg.top = mousePos.y;
			trg.bottom = mousePos.y + 5;
			RECT intersect;
			return	::IntersectRect(&intersect, &src, &trg);
		}
	}
	return false;
}

bool UI::CheckPincerAttArea()
{
	if (MFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	{
		if (mUIVisible)
		{

			float fScrollx = ScrollMgr::Instance().GetScroll().x;
			float fScrolly = ScrollMgr::Instance().GetScroll().y;
			auto mouse = Mouse::Get().GetState();

			XMFLOAT2 mousePos = XMFLOAT2(mouse.x + fScrollx, mouse.y + fScrolly);

			RECT src = GetBound();
			int UiBlock = src.bottom - src.top;
			src.top += UiBlock * 4;
			src.bottom += UiBlock * 4;
			RECT trg;
			trg.left = mousePos.x;
			trg.right = mousePos.x + 5;
			trg.top = mousePos.y;
			trg.bottom = mousePos.y + 5;
			RECT intersect;
			return	::IntersectRect(&intersect, &src, &trg);
		}
	}
	return false;
}
