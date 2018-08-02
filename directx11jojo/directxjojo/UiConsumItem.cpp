#include "stdafx.h"
#include "ConsumItems.h"
#include "UiConsumItem.h"
#include "HpBean.h"
#include "MpWater.h"

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
	mFlag(false),
	mCountItemType(2)
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
	{ "HpBean", 1,{ { "hpBean", 0.1f } } },
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

	MActor::Update(dt);

	//Ui�� �����ִ� �����϶�
	if (mUICVisible)
	{

		//�������� ���� ������ŭ UI �󿡼� ��ġ ����
		if (mVecUiConsumInfo.size() == 0)
		{
			for (int i = 0; i < mCountItemType; ++i)
			{
				float ysize = 40.0f;
				float uiPos = 18 * i;
				//XMFLOAT2(-3.0f, -10.0f)
				ANIMINFO aInfo = ANIMINFO(L"", mPosition + XMFLOAT2(-1.0f, uiPos - ysize));
				mVecUiConsumInfo.push_back(aInfo);
			}

			//�Ҹ� �������� ���Ϳ� ������ش�
			unique_ptr<HpBean> bean;
			bean = make_unique<HpBean>(mpBatch, mpSheet, mpFont);
			bean->Init(meSortID, mPosition, mUICVisible);
			mspVecConsumItems.push_back(move(bean));
			auto classname = 
				mspVecConsumItems[mspVecConsumItems.size() - 1].get();
			mVecUiConsumInfo[mspVecConsumItems.size() - 1].animName =
				classname->GetName();

			unique_ptr<MpWater> water;
			water = make_unique<MpWater>(mpBatch, mpSheet, mpFont);
			water->Init(meSortID, mPosition, mUICVisible);
			mspVecConsumItems.push_back(move(water));
			classname =
				mspVecConsumItems[mspVecConsumItems.size() - 1].get();
			mVecUiConsumInfo[mspVecConsumItems.size() - 1].animName =
				classname->GetName();
		}
		else
		{
			mVecUiConsumInfo.clear();
			for (int i = 0; i < mCountItemType; ++i)
			{
				float ysize = 40.0f;
				float uiPos = 18 * i;
				//XMFLOAT2(-3.0f, -10.0f)
				ANIMINFO aInfo = ANIMINFO(L"", mPosition + XMFLOAT2(-1.0f, uiPos - ysize));
				auto classname = mspVecConsumItems[i].get();
				aInfo.animName = classname->GetName();
				mVecUiConsumInfo.push_back(aInfo);
			}
		}

		//��� ��ư�� ��ġ ����
		mCancelBtnPos = mPosition + XMFLOAT2(82, 57);

		//�ش� �������� Ŭ���� �ÿ� ���õȴ�.
		if (MFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
		{
			auto mouse = Mouse::Get().GetState();
			float fScrollx = ScrollMgr::Instance().GetScroll().x;
			float fScrolly = ScrollMgr::Instance().GetScroll().y;
			POINT mousePos;
			mousePos.x = mouse.x + fScrollx;
			mousePos.y = mouse.y + fScrolly;

			RECT rc;
			//Ui�� �����ִ� �༮�� �߿��� �ѳ��� ���õȴٸ�
			//���� ��ų �༮�� ��ų�� ����ִ� ���Ϳ��� ���� �����´�.
			for (size_t i = 0; i < mVecUiConsumInfo.size(); i++)
			{
				map<string, vector<FrameInfo>>::iterator result;
				//if (mVecUiConsumInfo[i]->GetElemental() == 3)
				{
					result = mAnimations.find("ToolBack");
				}
				
				const SpriteFrame* pSpF;
				pSpF = mpSheet->Find(result->second.begin()->FrameName.c_str());
				float rcsubx = pSpF->sourceRect.right - pSpF->sourceRect.left;
				float rcsuby = pSpF->sourceRect.bottom - pSpF->sourceRect.top;

				rc.left = mVecUiConsumInfo[i].pos.x - rcsubx / 2;
				rc.right = mVecUiConsumInfo[i].pos.x + rcsubx / 2;
				rc.top = mVecUiConsumInfo[i].pos.y - rcsuby / 2;
				rc.bottom = mVecUiConsumInfo[i].pos.y + rcsuby / 2;
				if (PtInRect(&rc, mousePos))
				{
					mpCurItem = (mspVecConsumItems[i].get());
					//��� ��� �����ϸ� �۵�����
					if (mpCurItem->GetStock() <= 0)
					{
						break;
					}
					//CalArea(mpCurSkill->GetArea());
					mAreaVisible = true;
					//�������� ����Ҽ��ִ� ������ �����ִ� ������ �Ѵ�
					auto attackbox = MActorManager::Instance().GetClassAttackBox();
					attackbox->SetVisible(true);
					attackbox->SetAttackDis(0);
					attackbox->Release();
					attackbox->AttackCubeScope(true, *(attackbox->GetVecAtScopeIndex()));

					//attackbox->SetAttackDis(5);
					mUICVisible = false;
					break;
				}
				else
				{
					mAreaVisible = false;
				}
			}

			//��� ��ư�� Ŭ�������� ����â�� �Ⱥ����ش�
			auto pTexture = ResourceManager::Instance().GetShaderResource
			(L"Images\\Skill\\SkillUis.png");
			auto pSheet = ResourceManager::Instance().GetSpriteSheet
			(L"Images\\Skill\\SkillUis.xml", pTexture);
			auto result = mAnimations.find("CancelBtn");
			const SpriteFrame* pSpFrameCencel;

			pSpFrameCencel = pSheet->Find(result->second.begin()->FrameName.c_str());
			float rcsubx = pSpFrameCencel->sourceRect.right - pSpFrameCencel->sourceRect.left;
			float rcsuby = pSpFrameCencel->sourceRect.bottom - pSpFrameCencel->sourceRect.top;

			rc.left = mCancelBtnPos.x - rcsubx / 2;
			rc.right = mCancelBtnPos.x + rcsubx / 2;
			rc.top = mCancelBtnPos.y - rcsuby / 2;
			rc.bottom = mCancelBtnPos.y + rcsuby / 2;
			if (PtInRect(&rc, mousePos))
			{
				mpCurItem = nullptr;
				mUICVisible = false;
				//���������۵� Ŭ���� �ʱ�ȭ
				MActorManager::Instance().GetClassUi()->SetVisible(true);
				//for (size_t i = 0; i < mspVecConsumItems.size(); i++)
				//{
				//	mspVecConsumItems[i].reset();
				//}
				//mspVecConsumItems.clear();
			}
		}

	}
	else
	{
		//mVecUiConsumInfo.clear();
	}

	//�ð� �����Ŀ� ������ȿ���� ������� ����	
	if (mFlag)
	{
		mfActionElapsedTime += dt;
	}
	if (mfActionElapsedTime >= 0.8f)
	{
		//for (size_t i = 0; i < mspVecConsumItems.size(); i++)
		//{
		//	mspVecConsumItems[i].reset();
		//}
		//mspVecConsumItems.clear();

		mpCurItem = nullptr;
		mfActionElapsedTime = 0.0f;
		mFlag = false;
		//�÷��̾��� ������
		mpPlayer->SetActionTurn(2);
		//�÷��̾��� ������ �ٲ��ش�.
		mpPlayer->SetColorAllow(true);
		Color cr = Colors::Gray;
		mpPlayer->SetColor(cr);
		//mpPlayer->
	}

	//�������� ���ǰ� �ش������ �����ִ� �����϶�
	if (mAreaVisible)
	{
		MActor* pCollider, *pCollidee;
		pCollider = nullptr;
		pCollidee = nullptr;
		//������ �����ִ� ���¿��� �ڽ��̳� �÷��̾ ������ �۵��ȴ�
		if (MFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
		{
			//auto vecAtIndex = MActorManager::Instance().GetvecAtScopeIndex();

			//auto a = vecAtIndex[0];
			for (const auto &actor : (*MActorManager::Instance().GetActors()))
			{
				pCollider = actor.get();
				if (typeid(*pCollider) == typeid(AttackBox))
				{
					if (((AttackBox*)pCollider)->AttackScopeSeek())
					{
						for (const auto &actor : (*MActorManager::Instance().GetActors()))
						{
							pCollidee = actor.get();
							if (((AttackBox*)pCollider)->UIntersecRectScope(pCollidee) &&
								dynamic_cast<Player*>(pCollidee))
							{
								
								string classname= typeid(*mpCurItem).name();
								classname.erase(0, 6);
								if (classname == "HpBean")
								{
									//�ִ� Hp���� Ŀ���� �׸�ŭ���ִ´�
									if ((((Character*)pCollidee)->GetHealth()
											+ mpCurItem->GetPlus())
										>= ((Character*)pCollidee)->GetMaxHp())
									{
										((Character*)pCollidee)->SetHelth
										(((Character*)pCollidee)->GetMaxHp());
									}
									//�ƴҰ�� �� ���Ѹ�ŭ �־��ش�
									else
									{
										((Character*)pCollidee)->SetHelth
										(((Character*)pCollidee)->GetHealth() +
											mpCurItem->GetPlus());
									}
									
								}
								else if (classname == "MpWater")
								{
									//�ִ� Hp���� Ŀ���� �׸�ŭ���ִ´�
									if ((((Character*)pCollidee)->GetMana()
										+ mpCurItem->GetPlus())
										>= ((Character*)pCollidee)->GetMaxMana())
									{
										((Character*)pCollidee)->SetMana
										(((Character*)pCollidee)->GetMaxMana());
									}
									//�ƴҰ�� �� ���Ѹ�ŭ �־��ش�
									else
									{
										((Character*)pCollidee)->SetMana
										(((Character*)pCollidee)->GetMana() +
											mpCurItem->GetPlus());
									}
								}
								//((Character*)pCollidee)->SetHelth();
								MActorManager::Instance().GetClassUi()->SetVisible(false);

								auto attackBox = MActorManager::Instance().GetClassAttackBox();
								attackBox->SetVisible(false);
								attackBox->Release();
								mpPlayer->SetAnimation("POWER");
								mpPlayer->SetActionTime(0.0f);
								mpPlayer->SetMotionFlag(true);
								//mpPlayer = attackBox->GetCharacter();
								mpCurItem->SetPosition(
									pCollidee->GetPosition() + XMFLOAT2(0.0f,0.0f));
								mpCurItem->SetStock(mpCurItem->GetStock()-1);
								mFlag = true;
								mAreaVisible = false;

								break;
							}

						}
					}
				}
			}
		}
		//�ش������ �����ִµ� ���콺 ������ Ŭ���� �ϸ� ��Ҹ� �Ѵ�
		if (MFramework::mMouseTracker.rightButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
		{
			auto attackBox = MActorManager::Instance().GetClassAttackBox();
			attackBox->SetVisible(false);
			attackBox->Release();
			MActorManager::Instance().GetClassUi()->SetVisible(true);
			//for (size_t i = 0; i < mspVecConsumItems.size(); i++)
			//{
			//	mspVecConsumItems[i].reset();
			//}
			//mspVecConsumItems.clear();
			mAreaVisible = false;
		}
	}
	//���õ� �������� Update
	if (mpCurItem!= nullptr && mFlag)
	{
		mpCurItem->Update(dt);
	}

	return E_SCENE_NONPASS;
}

void UiConsumItem::Draw()
{
	XMFLOAT2 offset = XMFLOAT2(0, 0);
	Color tint = Colors::White;
	Color blackTint = Colors::Black;
	offset.x = (int)ScrollMgr::Instance().GetScroll().x;
	offset.y = (int)ScrollMgr::Instance().GetScroll().y;

	auto mouse = Mouse::Get().GetState();

	//�̵������� �����ٶ�
	if (mUICVisible)
	{
		//������ �⺻ ��� UI ���
		SetAnimation("UiItem");
		mpSheet->Draw(mpBatch, *mpSpriteFrame
			, mWorldPos + mPosition - offset, tint);

		//��ҹ�ư ���
		auto pTexture = ResourceManager::Instance().GetShaderResource
		(L"Images\\Skill\\SkillUis.png");
		auto pSheet = ResourceManager::Instance().GetSpriteSheet
		(L"Images\\Skill\\SkillUis.xml", pTexture);

		auto result = mAnimations.find("CancelBtn");
		mpSpriteFrame2 = pSheet->Find(result->second.begin()->FrameName.c_str());

		pSheet->Draw(mpBatch, *mpSpriteFrame2, mWorldPos + mCancelBtnPos - offset, tint);

		
		for (int i = 0; i < mspVecConsumItems.size(); ++i)
		{
			wchar_t wch[128];
			//������� ����� �׸��� �ٲ��ش�.
			SetAnimation("ToolBack");
			mpSheet->Draw(mpBatch, *mpSpriteFrame
				, mWorldPos + mVecUiConsumInfo[i].pos - offset
				, tint);

			if (mVecUiConsumInfo[i].animName == L"��")
			{
				SetAnimation("HpBean");
				//swprintf_s(wch, L"HpIncrease");
				//�ش� ���� ȿ�� ���
				//mpFont->DrawString(mpBatch, mspVecConsumItems[i]->GetEfficacyName().c_str()
				//	, mVecUiConsumInfo[i].pos + XMFLOAT2(1.0f, -10.0f) - offset
				//	, DirectX::Colors::Black, 0.0f
				//	, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.8f, 0.8f));

			}
			else if (mVecUiConsumInfo[i].animName == L"�ź�")
			{
				SetAnimation("MpWater");
				//�ش� ���� ȿ�� ���
				//swprintf_s(wch, L"MpIncrease");
				//mpFont->DrawString(mpBatch, mspVecConsumItems[i]->GetEfficacyName().c_str()
				//	, mVecUiConsumInfo[i].pos + XMFLOAT2(1.0f, -10.0f) - offset
				//	, DirectX::Colors::Black, 0.0f
				//	, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.8f, 0.8f));

			}
			//�ش� ���� ȿ�� ���
			mpFont->DrawString(mpBatch, mspVecConsumItems[i]->GetEfficacyName().c_str()
				, mVecUiConsumInfo[i].pos + XMFLOAT2(1.0f, -10.0f) - offset
				, DirectX::Colors::Black, 0.0f
				, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.8f, 0.8f));

			//�ش� ������ �׸��� �����
			mpSheet->Draw(mpBatch, *mpSpriteFrame
				, mWorldPos + mVecUiConsumInfo[i].pos + XMFLOAT2(-110.0f,0.0f)
				- offset, tint);

			//�ش� �ϴ� ������ �̸��� ������
			//Ŭ���� �̸� ���
			//string str = typeid(*mspVecConsumItems[i]).name();
			//str.erase(0, 6);
			//wstring wstr;
			//mbstowcs(&wstr[0], str.c_str(), strlen(typeid(*mspVecConsumItems[i]).name()));
			mpFont->DrawString(mpBatch, mspVecConsumItems[i]->GetName().c_str()
				, mVecUiConsumInfo[i].pos + XMFLOAT2(-100.0f, -10.0f) - offset
				, DirectX::Colors::Black, 0.0f
				, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.8f, 0.8f));

			//������ ���
			swprintf_s(wch, L"%d", mspVecConsumItems[i]->GetStock());
			mpFont->DrawString(mpBatch, wch
				, mVecUiConsumInfo[i].pos + XMFLOAT2(80.0f, -10.0f) - offset
				, DirectX::Colors::Black, 0.0f
				, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.8f, 0.8f));			

		}
	}
	else
	{
		//mVecUiConsumInfo.clear();
	}
	//���õ� ��ų�� Draw
	if (mpCurItem != nullptr && mFlag)
	{
		mpCurItem->Draw();
	}
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

void UiConsumItem::CalArea()
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
