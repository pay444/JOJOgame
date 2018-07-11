#include "stdafx.h"
#include "UiSkills.h"
#include "Skill.h"
#include "SeonPung.h"

//std::string ReplaceAll(std::string &str, const std::string& from, const std::string& to)
//{
//	size_t start_pos = 0; //stringó������ �˻�
//	while ((start_pos = str.find(from, start_pos)) != std::string::npos)  //from�� ã�� �� ���� ������
//	{
//		str.replace(start_pos, from.length(), to);
//		start_pos += to.length(); // �ߺ��˻縦 ���ϰ� from.length() > to.length()�� ��츦 ���ؼ�
//	}
//	return str;
//}

UiSkills::UiSkills()
{
}


UiSkills::UiSkills(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont)
	:MActor(pBatch, pSheet, pFont),
	mpPlayer(nullptr),
	mUISVisible(false),
	mCountSkill(1),
	mCancelBtnPos(0, 0),
	mpCurSkill(nullptr),
	mAreaVisible(false),
	mFlag(false)
{

}

UiSkills::~UiSkills()
{
	this->Release();
}

void UiSkills::Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible)
{
	Animation anim[] = { 
		{ "SkillUi", 1,{ { "skillUi", 0.3f } } },
	{ "WaterB", 1,{ { "waterBack", 0.3f } } },
	{ "FireB", 1,{ { "fireBack", 0.3f } } },
	{ "WindB", 1,{ { "windBack", 0.3f } } },
	{ "RandB", 1,{ { "randBack", 0.3f } } },
	{ "CancelBtn", 1,{ { "cancelBtn", 0.3f } } },
	{ "PBMp", 1,{ { "MpBar0", 0.3f } } },
	{ "AttackBox", 1,{ { "AttackBox0", 0.3f } }},
	};

	//�̵��Ÿ�
	//mMoveDistance = moveDis;
	//���̰� �Ⱥ��̰�
	mUISVisible = visible;
	//mUISVisible = true;
	MActor::Init(anim, 8, eSortID);
	SetPosition(pos + XMFLOAT2(100.0f, 0.0f));
	SetAnimation("SkillUi");
	mCancelBtnPos = mPosition + XMFLOAT2(82, 64);
}

E_SCENE UiSkills::Update(float dt)
{

	//if (MFramework::mMouseTracker.rightButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)

	//if (MFramework::mKeyboardTracker.released.N)
	//{
	//	mpPlayer->SetMana(mpPlayer->GetMana() - 1);
	//}

	MActor::Update(dt);
	
	if (mUISVisible)
	{
		float fScrollx = ScrollMgr::Instance().GetScroll().x;
		float fScrolly = ScrollMgr::Instance().GetScroll().y;

		//Ŭ������ ���� ��ų ���� ����
		if (mpPlayer->GetClassType() == GunGeon::ClassType::MONARCH)
		{
			mCountSkill = 1;

			//�ش� Ŭ������ ��ų������ŭ UI �󿡼� ��ġ ����
			if (mVecUiSkillInfo.size() == 0)
			{
				for (int i = 0; i < mCountSkill; ++i)
				{
					float ysize = 30.0f;
					float uiPos = 18 * i;
					ANIMINFO aInfo = ANIMINFO("", mPosition + XMFLOAT2(1.0f, uiPos - ysize));
					mVecUiSkillInfo.push_back(aInfo);
				}
				
				//��ų�� ���Ϳ� ������ش�
				unique_ptr<SeonPung> seonp;
				seonp = make_unique<SeonPung>(mpBatch, mpSheet, mpFont);
				seonp->Init(meSortID, mPosition, mUISVisible);
				mspVecSkills.push_back(move(seonp));
				auto classname = mspVecSkills[mspVecSkills.size() - 1].get();
				mVecUiSkillInfo[mVecUiSkillInfo.size() - 1].animName =
					typeid(*(classname)).name();
				mVecUiSkillInfo[mVecUiSkillInfo.size() - 1].animName.erase(0,6);
			}

		}
		//��ҹ�ư�� ��ġ ����
		mCancelBtnPos = mPosition + XMFLOAT2(82, 64);

		//�ش� ��ų�� Ŭ���ҽÿ� ��ų�� ���õȴ�
		if (MFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
		{
			auto mouse = Mouse::Get().GetState();

			POINT mousePos;
			mousePos.x = mouse.x + fScrollx;
			mousePos.y = mouse.y + fScrolly;

			RECT rc;
			//Ui�� �����ִ� �༮�� �߿��� �ѳ��� ���õȴٸ� ���� ��ų �༮�� ��ų�� ����ִ� ���Ϳ��� ���� �����´�.
			for (size_t i = 0; i < mVecUiSkillInfo.size(); i++)
			{
				map<string, vector<FrameInfo>>::iterator result;
				if (mspVecSkills[i]->GetElemental() == 3)
				{
					result = mAnimations.find("WindB");
				}

				const SpriteFrame* pSpF;
				pSpF = mpSheet->Find(result->second.begin()->FrameName.c_str());
				float rcsubx = pSpF->sourceRect.right - pSpF->sourceRect.left;
				float rcsuby = pSpF->sourceRect.bottom - pSpF->sourceRect.top;

				rc.left = mVecUiSkillInfo[i].pos.x - rcsubx / 2;
				rc.right = mVecUiSkillInfo[i].pos.x + rcsubx / 2;
				rc.top = mVecUiSkillInfo[i].pos.y - rcsuby / 2;
				rc.bottom = mVecUiSkillInfo[i].pos.y + rcsuby / 2;
				if (PtInRect(&rc, mousePos))
				{
					mpCurSkill = (mspVecSkills[i].get());
					//CalArea(mpCurSkill->GetArea());
					mAreaVisible = true;
					//���� ������ �����ִ� ������ �Ѵ�
					auto attackbox = MActorManager::Instance().GetClassAttackBox();
					attackbox->SetVisible(true);
					attackbox->SetAttackDis(mpCurSkill->GetArea());
					mUISVisible = false;
					break;
				}
				else
				{
					mAreaVisible = false;
				}
			}

			//��� ��ư�� Ŭ�������� å��â�� �Ⱥ����ش�
			auto result = mAnimations.find("CancelBtn");
			const SpriteFrame* pSpFrameCencel;

			pSpFrameCencel = mpSheet->Find(result->second.begin()->FrameName.c_str());
			float rcsubx = pSpFrameCencel->sourceRect.right - pSpFrameCencel->sourceRect.left;
			float rcsuby = pSpFrameCencel->sourceRect.bottom - pSpFrameCencel->sourceRect.top;

			rc.left = mCancelBtnPos.x - rcsubx / 2;
			rc.right = mCancelBtnPos.x + rcsubx / 2;
			rc.top = mCancelBtnPos.y - rcsuby / 2;
			rc.bottom = mCancelBtnPos.y + rcsuby / 2;
			if (PtInRect(&rc, mousePos))
			{
				mpCurSkill = nullptr;
				mUISVisible = false;
				MActorManager::Instance().GetClassUi()->SetVisible(true);
				for (size_t i = 0; i < mspVecSkills.size(); i++)
				{
					mspVecSkills[i].reset();
				}
				mspVecSkills.clear();
			}
		}
	
	}
	else
	{
		//mpCurSkill = nullptr;
		mVecUiSkillInfo.clear();
	}

	//�ð� �����Ŀ� ��ų�� ������� ����	
	if (mFlag)
	{
		mfActionElapsedTime += dt;
	}

	
	if (mfActionElapsedTime >= 0.8f)
	{
		for (size_t i = 0; i < mspVecSkills.size(); i++)
		{
			mspVecSkills[i].reset();
		}
		mspVecSkills.clear();

		mpCurSkill = nullptr;
		mfActionElapsedTime = 0.0f;
		mFlag = false;
	}
	//��ų�� ���ǰ� �ش������ �����ִ� �����϶�
	if (mAreaVisible)
	{

		MActor* pCollider, *pCollidee;
		pCollider = nullptr;
		pCollidee = nullptr;
		//������ �����ִ� ���¿��� ���� ������ ��ų�� �۵��ȴ�
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
								dynamic_cast<Enemy*>(pCollidee))
							{
								((AttackBox*)pCollider)->SetAttackDamge(((DamegeSkill*)mpCurSkill)->GetDamege());
								pCollidee->OnHit(pCollider, mpPlayer);
								//���� ����
								mpPlayer->SetMana(mpPlayer->GetMana() - mpCurSkill->GetMpCost());
								MActorManager::Instance().GetClassUi()->SetVisible(false);
								
								auto attackBox = MActorManager::Instance().GetClassAttackBox();
								attackBox->SetVisible(false);
								attackBox->Release();
								attackBox->SetCharacter(mpPlayer);
								mpCurSkill->SetPosition(pCollidee->GetPosition());

								mFlag = true;
								mAreaVisible = false;
								

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
			for (size_t i = 0; i < mspVecSkills.size(); i++)
			{
				mspVecSkills[i].reset();
			}
			mspVecSkills.clear();
			mAreaVisible = false;
		}
	}
	//���õ� ��ų�� Update
	if (mpCurSkill != nullptr && mFlag)
	{
		mpCurSkill->Update(dt);
	}
	return E_SCENE_NONPASS;
}

void UiSkills::Draw()
{
	XMFLOAT2 offset = XMFLOAT2(0, 0);
	Color tint = Colors::White;
	Color blackTint = Colors::Black;
	offset.x = (int)ScrollMgr::Instance().GetScroll().x;
	offset.y = (int)ScrollMgr::Instance().GetScroll().y;

	auto mouse = Mouse::Get().GetState();

	//�̵������� �����ٶ�
	if (mUISVisible)
	{
		//��ų �⺻ ��� UI ���
		SetAnimation("SkillUi");
		mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mPosition - offset, tint);

		//�ش� ĳ������ mp ������ ���
		auto pTexture = ResourceManager::Instance().GetShaderResource(L"Images\\jojo\\jojoSprites.png");//(L"Images\\sprites.png");
		auto pSheet = ResourceManager::Instance().GetSpriteSheet(L"Images\\jojo\\jojoSprites.xml", pTexture); //(L"Images\\sprites.xml",pTexture);

		auto result = mAnimations.find("PBMp");
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

		//Mp ���� ���
		wchar_t wch[128];
		swprintf_s(wch, L"%d", mp);
		mpFont->DrawString(mpBatch, wch, XMFLOAT2(mPosition + XMFLOAT2(-20.0f, -75.0f) - offset), DirectX::Colors::Black, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.5f, 0.5f));
		swprintf_s(wch, L" / %d", static_cast<int>(maxMp));
		mpFont->DrawString(mpBatch, wch, XMFLOAT2(mPosition + XMFLOAT2(0.0f, -75.0f) - offset), DirectX::Colors::Black, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.5f, 0.5f));

		//�ش� Ŭ���� �̸� ���
		string str = typeid(*mpPlayer).name();
		str.erase(0, 6);
		wstring wstr;
		mbstowcs(&wstr[0], str.c_str(), strlen(typeid(*mpPlayer).name()));
	
		mpFont->DrawString(mpBatch, wstr.c_str(), XMFLOAT2(mPosition + XMFLOAT2(-120.0f, -77.0f) - offset), DirectX::Colors::Black, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.5f, 0.5f));

		//��� ��ư ���
		SetAnimation("CancelBtn");
		mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mCancelBtnPos - offset, tint);
		
		//�ش� ĳ������ Ŭ������ ���� ���
		if (mpPlayer->GetClassType() == GunGeon::ClassType::MONARCH)
		{
			for (int i = 0; i < mCountSkill; ++i)
			{
				if (mVecUiSkillInfo.size() == 0)
				{
					break;
				}
				if (mVecUiSkillInfo[i].animName == "SeonPung")
				{
					SetAnimation("WindB");
				}
				
				mpSheet->Draw(mpBatch, *mpSpriteFrame
					, mWorldPos + mVecUiSkillInfo[i].pos - offset
					, tint);
				//�ش� ��ų�� ���� ���� ����
				for (size_t i = 0; i < mspVecSkills.size(); i++)
				{
					wchar_t wch[128];
					//Ŭ���� �̸� ���
					string str = typeid(*mspVecSkills[i]).name();
					str.erase(0, 6);
					wstring wstr;
					mbstowcs(&wstr[0], str.c_str(), strlen(typeid(*mspVecSkills[i]).name()));
					mpFont->DrawString(mpBatch, wstr.c_str()
						, mVecUiSkillInfo[i].pos+XMFLOAT2(-100.0f,-10.0f) - offset
						, DirectX::Colors::Black, 0.0f
						, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.5f, 0.5f));
					
					//��ų�� MP��� ���
					swprintf_s(wch, L"%d", mspVecSkills[i]->GetMpCost());
					mpFont->DrawString(mpBatch, wch
						, mVecUiSkillInfo[i].pos + XMFLOAT2(80.0f, -10.0f) - offset
						, DirectX::Colors::Black, 0.0f
						, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.5f, 0.5f));
					//mspVecSkills[i]->
				}
			}
		}
	}
	else
	{
		mVecUiSkillInfo.clear();
	}

	//��ų�� ������ �����ش�
	if (mAreaVisible)
	{
		auto pTexture = ResourceManager::Instance().GetShaderResource(L"Images\\jojo\\jojoSprites.png");//(L"Images\\sprites.png");
		auto pSheet = ResourceManager::Instance().GetSpriteSheet(L"Images\\jojo\\jojoSprites.xml", pTexture); //(L"Images\\sprites.xml",pTexture);

		auto result = mAnimations.find("AttackBox");
		mpSpriteFrame2 = pSheet->Find(result->second.begin()->FrameName.c_str());

		vector<unique_ptr<TILE>> *pVecTile = MActorManager::Instance().GetTileInfo();

		for (size_t i = 0; i < mspVecAreaIndex.size(); i++)
		{
			Vector2 vTilePos = Vector2(
				(*pVecTile)[*mspVecAreaIndex[i].get()]->vPos.x + JOJOTILESX / 2,
				(*pVecTile)[*mspVecAreaIndex[i].get()]->vPos.y + JOJOTILESY / 2);
			pSheet->Draw(mpBatch, *mpSpriteFrame2, mWorldPos + vTilePos - offset, tint);

		}
		
	}
	else
	{
		//Release();
	}

	//���õ� ��ų�� Draw
	if (mpCurSkill != nullptr && mFlag)
	{
		mpCurSkill->Draw();
	}
}

void UiSkills::Release()
{
	//mVecAtScopeIndex.clear();
	auto iter = mspVecAreaIndex.begin();
	while (iter != mspVecAreaIndex.end())
	{
		iter->reset();
		iter = mspVecAreaIndex.erase(iter);
	}
}

void UiSkills::CalArea(int distance)
{
	float fScrollx = ScrollMgr::Instance().GetScroll().x;
	float fScrolly = ScrollMgr::Instance().GetScroll().y;
	auto mouse = Mouse::Get().GetState();
	XMFLOAT2 mMousePos = XMFLOAT2(mouse.x + fScrollx, mouse.y + fScrolly);

	vector<unique_ptr<TILE>> *pVecTile = MActorManager::Instance().GetTileInfo();

	//�ɸ����� ��ġŸ�Ͽ� ���ݰŸ��� ����
	(*pVecTile)[GetTileIndex(mpPlayer->GetPosition())]->AttackNum = distance;

	for (int mDis = distance; mDis > 0; --mDis)
	{
		for (int i = 0; i < 20; ++i)
		{
			for (int j = 0; j < 20; ++j)
			{
				if ((*pVecTile)[i * (20) + j]->AttackNum == mDis)
				{
					if ((*pVecTile)[i * (20) + j]->AttackNum > 0)
					{
						if (i - 1 >= 0 && (*pVecTile)[(i - 1) * (20) + j]->AttackNum < mDis)
						{
							(*pVecTile)[(i - 1) * (20) + j]->AttackNum = mDis - 1;
							//vecAtScopeIndex.push_back((i - 1) * (20) + j);
							mspVecAreaIndex.push_back(make_unique<int>((i - 1) * (20) + j));
						}
						if (i + 1 < 20 && (*pVecTile)[(i + 1) * (20) + j]->AttackNum < mDis)
						{
							(*pVecTile)[(i + 1) * (20) + j]->AttackNum = mDis - 1;
							//vecAtScopeIndex.push_back((i + 1) * (20) + j);
							mspVecAreaIndex.push_back(make_unique<int>((i + 1) * (20) + j));

						}

						if (j + 1 < 20 && (*pVecTile)[i * (20) + (j + 1)]->AttackNum < mDis)
						{
							(*pVecTile)[i * (20) + (j + 1)]->AttackNum = mDis - 1;
							//vecAtScopeIndex.push_back(i * (20) + (j + 1));
							mspVecAreaIndex.push_back(make_unique<int>(i * (20) + (j + 1)));
						}

						if (j - 1 >= 0 && (*pVecTile)[i * (20) + (j - 1)]->AttackNum < mDis)
						{
							(*pVecTile)[i * (20) + (j - 1)]->AttackNum = mDis - 1;
							//vecAtScopeIndex.push_back(i * (20) + (j - 1));
							mspVecAreaIndex.push_back(make_unique<int>(i * (20) + (j - 1)));
						}


					}
				}
			}
		}
	}
	//Ÿ���� ��ġ ���� �ʱ�ȭ
	(*pVecTile)[GetTileIndex(mpPlayer->GetPosition())]->AttackNum = 0;
	for (int x = 0; x < mspVecAreaIndex.size(); ++x)
	{
		(*pVecTile)[*mspVecAreaIndex[x].get()]->AttackNum = 0;
	}

}
