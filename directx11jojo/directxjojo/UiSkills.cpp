#include "stdafx.h"
#include "UiSkills.h"
#include "Skill.h"
#include "SeonPung.h"
#include "MouseBox.h"
#include "HealSkill.h"

//std::string ReplaceAll(std::string &str, const std::string& from, const std::string& to)
//{
//	size_t start_pos = 0; //string처음부터 검사
//	while ((start_pos = str.find(from, start_pos)) != std::string::npos)  //from을 찾을 수 없을 때까지
//	{
//		str.replace(start_pos, from.length(), to);
//		start_pos += to.length(); // 중복검사를 피하고 from.length() > to.length()인 경우를 위해서
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
	mSkilTimeFlag(false)
{

}

UiSkills::~UiSkills()
{
	this->Release();
}

void UiSkills::Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible)
{
	auto pSHTexture = ResourceManager::Instance().GetShaderResource(L"Images\\Skill\\Skills1.png");
	auto pSHSheet = ResourceManager::Instance().GetSpriteSheet(L"Images\\Skill\\Skills1.xml", pSHTexture);

	Animation anim[] = { 
		{ "SkillUi", 1,{ { "skillUi", 0.3f } } },
	{ "WaterB", 1,{ { "waterBack", 0.3f } } },
	{ "FireB", 1,{ { "fireBack", 0.3f } } },
	{ "WindB", 1,{ { "windBack", 0.3f } } },
	{ "RandB", 1,{ { "randBack", 0.3f } } },
	{ "CancelBtn", 1,{ { "cancelBtn", 0.3f } } },
	{ "PBMp", 1,{ { "MpBar0", 0.3f } } },
	{ "AttackBox", 1,{ { "AttackBox0", 0.3f } }},
	{ "Water", 1,{ { "water", 0.3f } } },
	{ "Fire", 1,{ { "fire", 0.3f } } },
	{ "Wind", 1,{ { "wind", 0.3f } } },
	{ "Rand", 1,{ { "rand", 0.3f } } },
	{ "Baegho", 1,{ { "baegho", 0.3f } } },
	{ "FireBird", 1,{ { "fireBird", 0.3f } } },
	{ "BlueDragon", 1,{ { "blueDragon", 0.3f } } },
	{ "BasicBar", 1,{ { "skillBack", 0.3f } } },
	{ "Heal", 1,{ { "heal", 0.3f } } },

	};

	//이동거리
	//mMoveDistance = moveDis;
	//보이고 안보이고
	mUISVisible = visible;
	//mUISVisible = true;
	MActor::Init(anim, 17, eSortID);
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

		//클래스에 따른 스킬 개수 설정
		SkillRegister();
		//취소버튼의 위치 갱신
		mCancelBtnPos = mPosition + XMFLOAT2(82, 64);

		//해당 스킬을 클릭할시에 스킬이 선택된다
		if (MFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
		{
			auto mouse = Mouse::Get().GetState();

			POINT mousePos;
			mousePos.x = mouse.x + fScrollx;
			mousePos.y = mouse.y + fScrolly;

			RECT rc;
			//Ui가 보여주는 녀석들 중에서 한놈이 선택된다면
			//현재 스킬 녀석에 스킬이 들어있는 벡터에서 값을 가져온다.
			for (size_t i = 0; i < mVecUiSkillInfo.size(); i++)
			{
				map<string, vector<FrameInfo>>::iterator result;
				//속성값이 주어진다면 스킬 상자의 크기를 구해준다
				if (mspVecSkills[i]->GetElemental() >= 0)
				{
					result = mAnimations.find("BasicBar");
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

					//만약에 Mp가 없으면 작동안함
					if (mpPlayer->GetMana() < mpCurSkill->GetMpCost())
					{
						mpCurSkill = nullptr;
						break;
					}

					//CalArea(mpCurSkill->GetArea());
					mAreaVisible = true;
					//공격 범위를 보여주는 설정을 한다
					auto attackbox = MActorManager::Instance().GetClassAttackBox();
					attackbox->SetVisible(true);
					//나자신의위치 도 어택범위에 포함 한다
					int pindexcol = ((mpPlayer->GetPosition().x / JOJOTILESX));
					int pindexrow = ((mpPlayer->GetPosition().y / JOJOTILESY));
					int index = pindexrow * 20 + pindexcol;
					attackbox->GetVecAtScopeIndex()->push_back(make_unique<int>(index));
					attackbox->SetAttackDis(mpCurSkill->GetScope());
					//범위 공격이라면 해당공격범위를 보여준다.
					if (mpCurSkill->GetSingleMulti() == 1)
					{
						auto mouseBox = MActorManager::Instance().GetClassMouseBox();

						//스킬의 범위에 따른 변경(십자,마방진등)
						switch (mpCurSkill->GetActiveArea())
						{
							//십자
						case 1:
						{
							mouseBox->SetSkillAreaVisible(true);
							mouseBox->SetSkillArea(0);
						}
						break;
						//마방진
						case 2:
						{
							mouseBox->SetSkillAreaVisible(true);
							mouseBox->SetSkillArea(1);
						}
						break;
						default:
							break;
						}		
					}
					mUISVisible = false;
					break;
				}
				else
				{
					mAreaVisible = false;
				}
			}

			//취소 버튼이 클릭됬을때 책략창을 안보여준다
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

				//클릭음악
				FMOD_System_PlaySound(MActorManager::Instance().GetFMODSystem(), (*MActorManager::Instance().GetVecFMODSound())[7], 0, 0, &(*MActorManager::Instance().GetVecFMODChannal())[1]);

			}
		}
	
	}
	else
	{
		//mpCurSkill = nullptr;
		mVecUiSkillInfo.clear();
	}

	//시간 측정후에 스킬이 사라지게 만듬	
	if (mSkilTimeFlag)
	{
		mfActionElapsedTime += dt;
	}

	//스킬의 지속시간을 넘어가면 스킬을 없에주고 초기화
	if (mSkilTimeFlag && mfActionElapsedTime >= mpCurSkill->GetTotalDelayTime())
	{
		auto attackBox = MActorManager::Instance().GetClassAttackBox();
		vector<unique_ptr<TILE>> *pVecTile = MActorManager::Instance().GetTileInfo();
		//if (mpCurSkill->GetSingleMulti() == 0)
		//{
		//	//해당 스킬의 범위안에 들어온 캐릭터를 공격
		//	for (const auto &actor : (*MActorManager::Instance().GetActors()))
		//	{
		//		MActor* epActor = actor.get();
		//		if (attackBox->UIntersecRectScope(epActor) &&
		//			(typeid(*epActor) == typeid(Enemy)))
		//		{
		//			//epActor->OnHit(attackBox, mpPlayer,1.0f);
		//			break;
		//		}
		//	}
		//}
		if (mpCurSkill->GetSingleMulti() == 1)
		{
			if (typeid(*mpCurSkill) == typeid(DamegeSkill) || typeid(*mpCurSkill) == typeid(SeonPung))
			{
				//해당 스킬의 범위안에 들어온 캐릭터를 모두 공격
				for (const auto &actor : (*MActorManager::Instance().GetActors()))
				{
					MActor* epActor = actor.get();
					if ((typeid(*epActor) == typeid(Enemy)))
					{
						//스킬범위에 해당해야만 공격됨
						for (size_t i = 0; i < attackBox->GetVecSkillAreaIndex()->size(); i++)
						{
							auto vecSkillIndex = attackBox->GetVecSkillAreaIndex();
							auto index = (*vecSkillIndex)[i];

							RECT rc = RectMake((*pVecTile)[index]->vPos.x, (*pVecTile)[index]->vPos.y, JOJOTILESX, JOJOTILESY);
							POINT pt;
							pt.x = epActor->GetPosition().x;
							pt.y = epActor->GetPosition().y;
							if (PtInRect(&rc, pt))
							{
								// ,mpCurSkill->GetTotalDelayTime() + 0.5f
								((Character*)epActor)->OnHitSkillDamege(attackBox, mpPlayer);
								break;
							}
						}
					}
				}
			}
			else if (typeid(*mpCurSkill) == typeid(HealSkill))
			{
				//해당 스킬의 범위안에 들어온 캐릭터를 모두 회복
				for (const auto &actor : (*MActorManager::Instance().GetActors()))
				{
					MActor* ppActor = actor.get();
					if ((typeid(*ppActor) == typeid(Player) || typeid(*ppActor) == typeid(Jojo)))
					{
						//스킬범위에 해당해야만 회복됨
						for (size_t i = 0; i < attackBox->GetVecSkillAreaIndex()->size(); i++) 
						{
							auto vecSkillIndex = attackBox->GetVecSkillAreaIndex();
							auto index = (*vecSkillIndex)[i];

							RECT rc = RectMake((*pVecTile)[index]->vPos.x, (*pVecTile)[index]->vPos.y, JOJOTILESX, JOJOTILESY);
							POINT pt;
							pt.x = ppActor->GetPosition().x;
							pt.y = ppActor->GetPosition().y;
							if (PtInRect(&rc, pt))
							{
								// ,mpCurSkill->GetTotalDelayTime() + 0.5f
								//맞았으나 데미지 적용 공식은 없어짐
								//attackBox->SetAttackDamge(0);
								((Character*)ppActor)->OnHitHeal(attackBox, mpPlayer, 1.0f);
								//내최대 Hp보다 더 넘을경우 최대만큼만 넣어줌
								if (((Character*)ppActor)->GetMaxHp() <=
									((Character*)ppActor)->GetHealth() + ((HealSkill*)mpCurSkill)->GetHeal())
								{
									((Character*)ppActor)->SetHelth(((Character*)ppActor)->GetMaxHp());
								}
								else
								{
									((Character*)ppActor)->SetHelth(((Character*)ppActor)->GetHealth() + ((HealSkill*)mpCurSkill)->GetHeal());
								}

								//ppActor->OnHit(attackBox, mpPlayer);
								break;
							}
						}
					}
				}
			}
		}

		for (size_t i = 0; i < mspVecSkills.size(); i++)
		{
			mspVecSkills[i].reset();
		}
		mspVecSkills.clear();

		mpCurSkill = nullptr;
		mfActionElapsedTime = 0.0f;
		mSkilTimeFlag = false;



	}
	//스킬이 눌렷고 해당범위를 보여주는 상태일때
	if (mAreaVisible)
	{
		MActor* pCollider, *pCollidee;
		pCollider = nullptr;
		pCollidee = nullptr;
		//범위가 보여주는 상태에서 데미지스킬일경우 적 힐스킬일경우 아군을 누르면 스킬이 작동된다
		if (MFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
		{
			auto attackBox = MActorManager::Instance().GetClassAttackBox();
			auto mouseBox = MActorManager::Instance().GetClassMouseBox();

			//auto vecAtIndex = MActorManager::Instance().GetvecAtScopeIndex();

			//auto a = vecAtIndex[0];
			for (const auto &actor : (*MActorManager::Instance().GetActors()))
			{
				pCollider = actor.get();
				if (typeid(*pCollider) == typeid(AttackBox))
				{
					//내가 마우스로 찍은곳이 공격범위인지 확인
					if (((AttackBox*)pCollider)->AttackScopeSeek())
					{

						//해당 캐릭터를 찾아서 공격을 진행한다
						for (const auto &actor : (*MActorManager::Instance().GetActors()))
						{
							pCollidee = actor.get();
							//단일스킬 일경우
							if (mpCurSkill->GetSingleMulti() == 0)
							{

								if (((AttackBox*)pCollider)->UIntersecRectScope(pCollidee))
								{
									if (typeid(*pCollidee) == typeid(Enemy))
									{
										//어떤 스킬인지 에 따라 다른 행동을 진행
										if (typeid(*mpCurSkill) == typeid(DamegeSkill)
											|| typeid(*mpCurSkill) == typeid(SeonPung))
										{
											FMOD_System_PlaySound(MActorManager::Instance().GetFMODSystem(), (*MActorManager::Instance().GetVecFMODSound())[16], 0, 0, &(*MActorManager::Instance().GetVecFMODChannal())[1]);

											((AttackBox*)pCollider)->SetAttackDamge(((DamegeSkill*)mpCurSkill)->GetDamege());
											((Character*)pCollidee)->OnHitSkillDamege(pCollider, mpPlayer, 1.0f);
											//pCollidee->SetAnimation("HIT");

											//마나 감소
											mpPlayer->SetMana(mpPlayer->GetMana() - mpCurSkill->GetMpCost());
											MActorManager::Instance().GetClassUi()->SetVisible(false);

											attackBox->SetVisible(false);
											attackBox->Release();
											attackBox->SetCharacter(mpPlayer);
											mpCurSkill->SetPosition(pCollidee->GetPosition());

											mSkilTimeFlag = true;
											mAreaVisible = false;
											break;
										}
									}
									else if (typeid(*pCollidee) == typeid(Player) || typeid(*pCollidee) == typeid(Jojo))
									{
										//어떤 스킬인지 에 따라 다른 행동을 진행
										if (typeid(*mpCurSkill) == typeid(HealSkill))
										{
											//에너지가 가득차있는상태에서는 반응안함
											if (((Character*)pCollidee)->GetMaxHp() == ((Character*)pCollidee)->GetHealth())
											{
												break;
											}
											FMOD_System_PlaySound(MActorManager::Instance().GetFMODSystem(), (*MActorManager::Instance().GetVecFMODSound())[16], 0, 0, &(*MActorManager::Instance().GetVecFMODChannal())[1]);

											pCollidee->SetAnimation("POWER");
											//((AttackBox*)pCollider)->SetAttackDamge(0);
											((Character*)pCollidee)->OnHitHeal(pCollider, mpPlayer, 1.0f);

											//내최대 Hp보다 더 넘을경우 최대만큼만 넣어줌
											if (((Character*)pCollidee)->GetMaxHp() <=
												(((Character*)pCollidee)->GetHealth() + ((HealSkill*)mpCurSkill)->GetHeal()))
											{
												((Character*)pCollidee)->SetHelth(((Character*)pCollidee)->GetMaxHp());
											}
											else
											{
												((Character*)pCollidee)->SetHelth(((Character*)pCollidee)->GetHealth() + ((HealSkill*)mpCurSkill)->GetHeal());
											}

											//마나 감소
											mpPlayer->SetMana(mpPlayer->GetMana() - mpCurSkill->GetMpCost());
											MActorManager::Instance().GetClassUi()->SetVisible(false);

											attackBox->SetVisible(false);
											attackBox->Release();
											attackBox->SetCharacter(mpPlayer);
											mpCurSkill->SetPosition(pCollidee->GetPosition());

											mSkilTimeFlag = true;
											mAreaVisible = false;
											break;
										}

									}
								}
							}
							//다중 범위 스킬일 경우
							else if (mpCurSkill->GetSingleMulti() == 1)
							{
								vector<unique_ptr<TILE>> *pVecTile = MActorManager::Instance().GetTileInfo();
								if (((AttackBox*)pCollider)->UIntersecRectScope(pCollidee))
								{
									if (typeid(*pCollidee) == typeid(Enemy))
									{
										//스킬범위에 들어온 캐릭터 벡터에 기록
										attackBox->SkillScope(*(attackBox->GetVecSkillAreaIndex()), mpCurSkill->GetActiveArea(),JoJoGun::CampType::ENEMY);

										//어떤 스킬인지 에 따라 다른 행동을 진행
										if (typeid(*mpCurSkill) == typeid(DamegeSkill))
										{
											((AttackBox*)pCollider)->SetAttackDamge(((DamegeSkill*)mpCurSkill)->GetDamege());
											//애니메이션 스킬인지?
											if (mpCurSkill->GetAnumSkil())
											{
												//해당 녀석에 맞는 음악 재생
												switch (mpCurSkill->GetElemental())
												{
												//주작
												case 4:
												{
													FMOD_System_PlaySound(MActorManager::Instance().GetFMODSystem(), (*MActorManager::Instance().GetVecFMODSound())[3], 0, 0, &(*MActorManager::Instance().GetVecFMODChannal())[1]);
												}
												break;
												//나머지는 화룡
												default :
												{
													FMOD_System_PlaySound(MActorManager::Instance().GetFMODSystem(), (*MActorManager::Instance().GetVecFMODSound())[5], 0, 0, &(*MActorManager::Instance().GetVecFMODChannal())[1]);
												}
												break;
												}

											}

											//해당 스킬의 범위안에 들어온 캐릭터를 모두 공격
											for (const auto &actor : (*MActorManager::Instance().GetActors()))
											{
												MActor* epActor = actor.get();
												if ((typeid(*epActor) == typeid(Enemy)))
												{
													//스킬범위에 해당해야만 공격됨
													for (size_t i = 0; i < attackBox->GetVecSkillAreaIndex()->size(); i++)
													{
														auto vecSkillIndex = attackBox->GetVecSkillAreaIndex();
														auto index = (*vecSkillIndex)[i];

														RECT rc = RectMake((*pVecTile)[index]->vPos.x, (*pVecTile)[index]->vPos.y, JOJOTILESX, JOJOTILESY);
														POINT pt;
														pt.x = epActor->GetPosition().x;
														pt.y = epActor->GetPosition().y;
														if (PtInRect(&rc, pt))
														{
															epActor->SetAnimation("HIT");
															//epActor->OnHit(pCollider, mpPlayer, mpCurSkill->GetTotalDelayTime() + 1.0f);
															break;
														}
													}
												}
												//마나 감소
												mpPlayer->SetMana(mpPlayer->GetMana() - mpCurSkill->GetMpCost());
												MActorManager::Instance().GetClassUi()->SetVisible(false);

												attackBox->SetVisible(false);
												attackBox->Release();
												attackBox->SetCharacter(mpPlayer);
												mouseBox->SetSkillAreaVisible(false);
												mpCurSkill->SetPosition(pCollidee->GetPosition());

												mSkilTimeFlag = true;
												mAreaVisible = false;
												break;
											}
										}
									}
									else if (typeid(*pCollidee) == typeid(Player)
										|| typeid(*pCollidee) == typeid(Jojo))
									{
										//스킬범위에 들어온 캐릭터 벡터에 기록
										attackBox->SkillScope(*(attackBox->GetVecSkillAreaIndex()), mpCurSkill->GetActiveArea(), JoJoGun::CampType::PLAYER);

										//어떤 스킬인지 에 따라 다른 행동을 진행
										if (typeid(*mpCurSkill) == typeid(HealSkill))
										{
											//애니메이션 스킬인지?
											if (mpCurSkill->GetAnumSkil())
											{
												//해당 녀석에 맞는 음악 재생
												switch (mpCurSkill->GetElemental())
												{
													//백호
												case 6:
												{
													FMOD_System_PlaySound(MActorManager::Instance().GetFMODSystem(), (*MActorManager::Instance().GetVecFMODSound())[4], 0, 0, &(*MActorManager::Instance().GetVecFMODChannal())[1]);
												}
												break;
												//나머지는 아직 없음
												default:
												{
													//FMOD_System_PlaySound(MActorManager::Instance().GetFMODSystem(), (*MActorManager::Instance().GetVecFMODSound())[5], 0, 0, &(*MActorManager::Instance().GetVecFMODChannal())[1]);
												}
												break;
												}

											}
											//아니라면 일반 효과음재생
											else
											{
												FMOD_System_PlaySound(MActorManager::Instance().GetFMODSystem(), (*MActorManager::Instance().GetVecFMODSound())[6], 0, 0, &(*MActorManager::Instance().GetVecFMODChannal())[1]);

											}

											//해당 스킬의 범위안에 들어온 캐릭터를 모두 회복
											for (const auto &actor : (*MActorManager::Instance().GetActors()))
											{
												MActor* ppActor = actor.get();
												if ((typeid(*ppActor) == typeid(Player) || typeid(*ppActor) == typeid(Jojo)))
												{
													//스킬범위에 해당해야만 회복됨
													for (size_t i = 0; i < attackBox->GetVecSkillAreaIndex()->size(); i++)
													{
														auto vecSkillIndex = attackBox->GetVecSkillAreaIndex();
														auto index = (*vecSkillIndex)[i];

														RECT rc = RectMake((*pVecTile)[index]->vPos.x, (*pVecTile)[index]->vPos.y, JOJOTILESX, JOJOTILESY);
														POINT pt;
														pt.x = ppActor->GetPosition().x;
														pt.y = ppActor->GetPosition().y;
														if (PtInRect(&rc, pt))
														{
															ppActor->SetAnimation("POWER");
															//epActor->OnHit(pCollider, mpPlayer, mpCurSkill->GetTotalDelayTime() + 1.0f);
															break;
														}
													}

												}
											}
											//마나 감소
											mpPlayer->SetMana(mpPlayer->GetMana() - mpCurSkill->GetMpCost());
											MActorManager::Instance().GetClassUi()->SetVisible(false);

											attackBox->SetVisible(false);
											attackBox->Release();
											attackBox->SetCharacter(mpPlayer);
											mouseBox->SetSkillAreaVisible(false);
											mpCurSkill->SetPosition(pCollidee->GetPosition());

											mSkilTimeFlag = true;
											mAreaVisible = false;
											break;
										}
									}
								

								}

							}

						}
					}
				}
			}
		}
		//해당범위를 보여주는데 마우스 오른쪽 클릭을 하면 취소를 한다
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

			//마우스 스킬 이펙트 초기화
			auto mouseBox = MActorManager::Instance().GetClassMouseBox();
			mouseBox->SetSkillAreaVisible(false);

			//취소음악
			FMOD_System_PlaySound(MActorManager::Instance().GetFMODSystem(), (*MActorManager::Instance().GetVecFMODSound())[18], 0, 0, &(*MActorManager::Instance().GetVecFMODChannal())[1]);

		}
	}
	//선택된 스킬의 Update
	if (mSkilTimeFlag) //mpCurSkill != nullptr && 
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

	//이동범위를 보여줄때
	if (mUISVisible)
	{
		//스킬 기본 배경 UI 출력
		SetAnimation("SkillUi");
		mpSheet->Draw(mpBatch, *mpSpriteFrame
			, mWorldPos + mPosition - offset, tint);

		//해당 캐릭터의 mp 게이지 출력
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

		//Mp 글자 출력
		wchar_t wch[128];
		swprintf_s(wch, L"%d", mp);
		mpFont->DrawString(mpBatch, wch, XMFLOAT2(mPosition + XMFLOAT2(-20.0f, -75.0f) - offset), DirectX::Colors::Black, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.5f, 0.5f));
		swprintf_s(wch, L" / %d", static_cast<int>(maxMp));
		mpFont->DrawString(mpBatch, wch, XMFLOAT2(mPosition + XMFLOAT2(0.0f, -75.0f) - offset), DirectX::Colors::Black, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.5f, 0.5f));

		//해당 클래스 캐릭터 이름 출력
		//string str = typeid(*mpPlayer).name();
		//str.erase(0, 6);
		//wstring wstr;
		//mbstowcs(&wstr[0], str.c_str(), strlen(typeid(*mpPlayer).name()));
		mpFont->DrawString(mpBatch, mpPlayer->GetName().c_str(), XMFLOAT2(mPosition + XMFLOAT2(-120.0f, -77.0f) - offset), DirectX::Colors::Black, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.8f, 0.8f));

		//취소 버튼 출력
		SetAnimation("CancelBtn");
		mpSheet->Draw(mpBatch, *mpSpriteFrame, mWorldPos + mCancelBtnPos - offset, tint);
		
		//해당 스킬 출력
		//if (mpPlayer->GetClassType() == JoJoGun::ClassType::LORD)
		{
			for (int i = 0; i < mCountSkill; ++i)
			{
				if (mVecUiSkillInfo.size() == 0)
				{
					break;
				}
				SetAnimation("BasicBar");
				mpSheet->Draw(mpBatch, *mpSpriteFrame
					, mWorldPos + mVecUiSkillInfo[i].pos - offset
					, tint);

				//속성 판단

				//물
				if (mspVecSkills[i]->GetElemental() == 0)
				{
					SetAnimation("Water");
					mpSheet->Draw(mpBatch, *mpSpriteFrame
						, mWorldPos + mVecUiSkillInfo[i].pos
						+ XMFLOAT2(-109.0f, 0.0f) - offset
						, tint);
				}
				//불
				else if (mspVecSkills[i]->GetElemental() == 1)
				{
					SetAnimation("Fire");
					mpSheet->Draw(mpBatch, *mpSpriteFrame
						, mWorldPos + mVecUiSkillInfo[i].pos
						+ XMFLOAT2(-109.0f, 0.0f) - offset
						, tint);
				}
				//바람
				else if (mspVecSkills[i]->GetElemental() == 2)
				{
					SetAnimation("Wind");
					mpSheet->Draw(mpBatch, *mpSpriteFrame
						, mWorldPos + mVecUiSkillInfo[i].pos
						+ XMFLOAT2(-109.0f, 0.0f) - offset
						, tint);
				}
				//땅
				else if (mspVecSkills[i]->GetElemental() == 3)
				{
					SetAnimation("Rand");
					mpSheet->Draw(mpBatch, *mpSpriteFrame
						, mWorldPos + mVecUiSkillInfo[i].pos
						+ XMFLOAT2(-109.0f, 0.0f) - offset
						, tint);
				}
				//주작
				else if (mspVecSkills[i]->GetElemental() == 4)
				{
					SetAnimation("FireBird");
					mpSheet->Draw(mpBatch, *mpSpriteFrame
						, mWorldPos + mVecUiSkillInfo[i].pos
						+ XMFLOAT2(-109.0f, 0.0f) - offset
						, tint);
				}
				//청룡
				else if (mspVecSkills[i]->GetElemental() == 5)
				{
					SetAnimation("BlueDragon");
					mpSheet->Draw(mpBatch, *mpSpriteFrame
						, mWorldPos + mVecUiSkillInfo[i].pos
						+ XMFLOAT2(-109.0f, 0.0f) - offset
						, tint);
				}
				//백호
				else if (mspVecSkills[i]->GetElemental() == 6)
				{
					SetAnimation("Baegho");
					mpSheet->Draw(mpBatch, *mpSpriteFrame
						, mWorldPos + mVecUiSkillInfo[i].pos
						+ XMFLOAT2(-109.0f, 0.0f) - offset
						, tint);
				}
				//회복
				else if (mspVecSkills[i]->GetElemental() == 7)
				{
					SetAnimation("Heal");
					mpSheet->Draw(mpBatch, *mpSpriteFrame
						, mWorldPos + mVecUiSkillInfo[i].pos
						+ XMFLOAT2(-109.0f, 0.0f) - offset
						, tint);
				}

				//해당 스킬에 따른 설명 쓰기
				wchar_t wch[128];
				//클래스 이름 출력
				//string str = typeid(*mspVecSkills[i]).name();
				//str.erase(0, 6);
				//wstring wstr;
				//mbstowcs(&wstr[0], str.c_str(), strlen(typeid(*mspVecSkills[i]).name()));

				mpFont->DrawString(mpBatch, (mspVecSkills[i])->GetName().c_str()
					, mVecUiSkillInfo[i].pos + XMFLOAT2(-100.0f, -10.0f) - offset
					, DirectX::Colors::Black, 0.0f
					, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.8f, 0.8f));
				//해당클래스의 효과 출력
				mpFont->DrawString(mpBatch, (mspVecSkills[i])->GetEfficacyName().c_str()
					, mVecUiSkillInfo[i].pos + XMFLOAT2(-20.0f, -10.0f) - offset
					, DirectX::Colors::Black, 0.0f
					, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.8f, 0.8f));

				//스킬의 MP비용 출력
				swprintf_s(wch, L"%d", mspVecSkills[i]->GetMpCost());
				mpFont->DrawString(mpBatch, wch
					, mVecUiSkillInfo[i].pos + XMFLOAT2(80.0f, -10.0f) - offset
					, DirectX::Colors::Black, 0.0f
					, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.8f, 0.8f));
			}
		}


		//{ "Water", 1, { { "water", 0.3f } } },
		//{ "Fire", 1,{ { "fire", 0.3f } } },
		//{ "Wind", 1,{ { "wind", 0.3f } } },
		//{ "Rand", 1,{ { "rand", 0.3f } } },
		//{ "Baegho", 1,{ { "baegho", 0.3f } } },
		//{ "FireBird", 1,{ { "fireBird", 0.3f } } },
		//{ "BlueDragon", 1,{ { "blueDragon", 0.3f } } },
		//{ "BasicBar", 1,{ { "skillBack", 0.3f } } },
	}
	else
	{
		mVecUiSkillInfo.clear();
	}

	//선택된 스킬의 Draw
	if ( mSkilTimeFlag && mpCurSkill != nullptr)
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

	//케릭터의 위치타일에 공격거리를 넣음
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
	//타일의 위치 측정 초기화
	(*pVecTile)[GetTileIndex(mpPlayer->GetPosition())]->AttackNum = 0;
	for (int x = 0; x < mspVecAreaIndex.size(); ++x)
	{
		(*pVecTile)[*mspVecAreaIndex[x].get()]->AttackNum = 0;
	}

}

void UiSkills::SkillRegister()
{
	if (mpPlayer->GetClassType() == JoJoGun::ClassType::LORD)
	{
		mCountSkill = 1;

		//해당 클래스의 스킬개수만큼 UI 상에서 위치 지정
		if (mVecUiSkillInfo.size() == 0)
		{
			for (int i = 0; i < mCountSkill; ++i)
			{
				float ysize = 30.0f;
				float uiPos = 18 * i;
				ANIMINFO aInfo = ANIMINFO(L"", mPosition + XMFLOAT2(1.0f, uiPos - ysize));
				mVecUiSkillInfo.push_back(aInfo);
			}

			//스킬을 벡터에 등록해준다
			unique_ptr<SeonPung> seonp;
			seonp = make_unique<SeonPung>(mpBatch, mpSheet, mpFont);
			seonp->Init(meSortID, mPosition, mUISVisible);
			mspVecSkills.push_back(move(seonp));
			auto classname = mspVecSkills[mspVecSkills.size() - 1].get();
			mVecUiSkillInfo[mVecUiSkillInfo.size() - 1].animName =
				classname->GetName();
		}

	}
	else if (mpPlayer->GetClassType() == JoJoGun::ClassType::TACTICIAN)
	{
		mCountSkill = 3;

		//해당 클래스의 스킬개수만큼 UI 상에서 위치 지정
		if (mVecUiSkillInfo.size() == 0)
		{
			for (int i = 0; i < mCountSkill; ++i)
			{
				float ysize = 30.0f;
				float uiPos = 18 * i;
				ANIMINFO aInfo = ANIMINFO(L"", mPosition + XMFLOAT2(1.0f, uiPos - ysize));
				mVecUiSkillInfo.push_back(aInfo);
			}

			//스킬을 벡터에 등록해준다 
			unique_ptr<DamegeSkill> fireBird;
			fireBird = make_unique<DamegeSkill>(mpBatch, mpSheet, mpFont);
			Animation FBanim[] = {
				{ "FireBird", 30,{
			{ "fireBird_001", 0.1f },
			{ "fireBird_002", 0.1f },
			{ "fireBird_003", 0.1f },
			{ "fireBird_004", 0.1f },
			{ "fireBird_005", 0.1f },
			{ "fireBird_006", 0.1f },
			{ "fireBird_007", 0.1f },
			{ "fireBird_008", 0.1f },
			{ "fireBird_009", 0.1f },
			{ "fireBird_010", 0.1f },
			{ "fireBird_011", 0.1f },
			{ "fireBird_012", 0.1f },
			{ "fireBird_013", 0.1f },
			{ "fireBird_014", 0.1f },
			{ "fireBird_015", 0.1f },
			{ "fireBird_016", 0.1f },
			{ "fireBird_017", 0.1f },
			{ "fireBird_018", 0.1f },
			{ "fireBird_019", 0.1f },
			{ "fireBird_020", 0.1f },
			{ "fireBird_021", 0.1f },
			{ "fireBird_022", 0.1f },
			{ "fireBird_023", 0.1f },
			{ "fireBird_024", 0.1f },
			{ "fireBird_025", 0.1f },
			{ "fireBird_026", 0.1f },
			{ "fireBird_027", 0.1f },
			{ "fireBird_028", 0.1f },
			{ "fireBird_029", 0.1f },
			{ "fireBird_030", 0.1f },
			}
				},
			{ "Effect", 10,{
			{ "fireEffect_001", 0.1f },
			{ "fireEffect_002", 0.1f },
			{ "fireEffect_003", 0.1f },
			{ "fireEffect_004", 0.1f },
			{ "fireEffect_005", 0.1f },
			{ "fireEffect_006", 0.1f },
			{ "fireEffect_007", 0.1f },
			{ "fireEffect_008", 0.2f },
			{ "fireEffect_009", 0.1f },
			{ "fireEffect_010", 0.1f },
			}
			},
			};
			fireBird->Init(meSortID, mPosition, false, FBanim,2);
			fireBird->SetSkillProperty(4, 4 ,50, L"공격(화)"
				, 1, 25, 4.0f, 2,true);
			fireBird->SetName(L"주작");
			//주작의 애니메이션(첫번째) 딜레이 시간입력
			fireBird->SetDelayAnimTime(3.0f);
			mspVecSkills.push_back(move(fireBird));
			auto classname = mspVecSkills[mspVecSkills.size() - 1].get();
			mVecUiSkillInfo[mVecUiSkillInfo.size() - 1].animName =
				classname->GetName();

			//스킬을 벡터에 등록해준다
			unique_ptr<DamegeSkill> fireDragon;
			fireDragon = make_unique<DamegeSkill>(mpBatch, mpSheet, mpFont);
			Animation FDanim[] = {
			{ "FireDragon", 25,{
			{ "fireDragon_001", 0.1f },
			{ "fireDragon_002", 0.1f },
			{ "fireDragon_003", 0.1f },
			{ "fireDragon_004", 0.1f },
			{ "fireDragon_005", 0.1f },
			{ "fireDragon_006", 0.1f },
			{ "fireDragon_007", 0.1f },
			{ "fireDragon_008", 0.1f },
			{ "fireDragon_009", 0.1f },
			{ "fireDragon_010", 0.1f },
			{ "fireDragon_011", 0.1f },
			{ "fireDragon_012", 0.1f },
			{ "fireDragon_013", 0.1f },
			{ "fireDragon_014", 0.1f },
			{ "fireDragon_015", 0.1f },
			{ "fireDragon_016", 0.1f },
			{ "fireDragon_017", 0.1f },
			{ "fireDragon_018", 0.1f },
			{ "fireDragon_019", 0.1f },
			{ "fireDragon_020", 0.1f },
			{ "fireDragon_021", 0.1f },
			{ "fireDragon_022", 0.1f },
			{ "fireDragon_023", 0.1f },
			{ "fireDragon_024", 0.1f },
			{ "fireDragon_025", 0.1f },
			}
				},
			{ "Effect", 10,{
			{ "fireEffect_001", 0.1f },
			{ "fireEffect_002", 0.1f },
			{ "fireEffect_003", 0.1f },
			{ "fireEffect_004", 0.1f },
			{ "fireEffect_005", 0.1f },
			{ "fireEffect_006", 0.1f },
			{ "fireEffect_007", 0.1f },
			{ "fireEffect_008", 0.1f },
			{ "fireEffect_009", 0.1f },
			{ "fireEffect_010", 0.1f },
			}
			},
			};
			fireDragon->Init(meSortID, mPosition, false, FDanim, 2);
			fireDragon->SetSkillProperty(1, 4, 25, L"공격(화)"
				, 1, 40, 3.5f, 1, true);
			fireDragon->SetName(L"화룡");
			//첫번째 애니메이션의 딜레이 시간입력
			fireDragon->SetDelayAnimTime(2.5f);
			mspVecSkills.push_back(move(fireDragon));
			classname = mspVecSkills[mspVecSkills.size() - 1].get();
			mVecUiSkillInfo[mVecUiSkillInfo.size() - 1].animName =
				classname->GetName();

			//스킬을 벡터에 등록해준다
			unique_ptr<DamegeSkill> windDragon;
			windDragon = make_unique<DamegeSkill>(mpBatch, mpSheet, mpFont);
			Animation WDanim[] = {
				{ "WindDragon", 25,{
			{ "windDragon_001", 0.1f },
			{ "windDragon_002", 0.1f },
			{ "windDragon_003", 0.1f },
			{ "windDragon_004", 0.1f },
			{ "windDragon_005", 0.1f },
			{ "windDragon_006", 0.1f },
			{ "windDragon_007", 0.1f },
			{ "windDragon_008", 0.1f },
			{ "windDragon_009", 0.1f },
			{ "windDragon_010", 0.1f },
			{ "windDragon_011", 0.1f },
			{ "windDragon_012", 0.1f },
			{ "windDragon_013", 0.1f },
			{ "windDragon_014", 0.1f },
			{ "windDragon_015", 0.1f },
			{ "windDragon_016", 0.1f },
			{ "windDragon_017", 0.1f },
			{ "windDragon_018", 0.1f },
			{ "windDragon_019", 0.1f },
			{ "windDragon_020", 0.1f },
			{ "windDragon_021", 0.1f },
			{ "windDragon_022", 0.1f },
			{ "windDragon_023", 0.1f },
			{ "windDragon_024", 0.1f },
			{ "windDragon_025", 0.1f },
			}
				},
			{ "Effect", 20,{
			{ "wind_001", 0.05f },
			{ "wind_002", 0.05f },
			{ "wind_003", 0.05f },
			{ "wind_004", 0.05f },
			{ "wind_005", 0.05f },
			{ "wind_006", 0.05f },
			{ "wind_007", 0.05f },
			{ "wind_008", 0.05f },
			{ "wind_009", 0.05f },
			{ "wind_010", 0.05f },
			{ "wind_011", 0.05f },
			{ "wind_012", 0.05f },
			{ "wind_013", 0.05f },
			{ "wind_014", 0.05f },
			{ "wind_015", 0.05f },
			{ "wind_016", 0.05f },
			{ "wind_017", 0.05f },
			{ "wind_018", 0.05f },
			{ "wind_019", 0.05f },
			{ "wind_020", 0.05f },
			}				   
			},
			};
			windDragon->Init(meSortID, mPosition, false, WDanim, 2);
			windDragon->SetSkillProperty(2, 4, 25, L"공격(풍)"
				, 1, 40, 3.5f, 1, true);
			windDragon->SetName(L"풍룡");
			//첫번째 애니메이션의 딜레이 시간입력
			windDragon->SetDelayAnimTime(2.5f);
			mspVecSkills.push_back(move(windDragon));
			classname = mspVecSkills[mspVecSkills.size() - 1].get();
			mVecUiSkillInfo[mVecUiSkillInfo.size() - 1].animName =
				classname->GetName();
		}
	}
	else if (mpPlayer->GetClassType() == JoJoGun::ClassType::SUPPORTER)
	{
		mCountSkill = 3;

		//해당 클래스의 스킬개수만큼 UI 상에서 위치 지정
		if (mVecUiSkillInfo.size() == 0)
		{
			for (int i = 0; i < mCountSkill; ++i)
			{
				float ysize = 30.0f;
				float uiPos = 18 * i;
				ANIMINFO aInfo = ANIMINFO(L"", mPosition + XMFLOAT2(1.0f, uiPos - ysize));
				mVecUiSkillInfo.push_back(aInfo);
			}
			auto pSHTexture = ResourceManager::Instance().GetShaderResource(L"Images\\Skill\\Skills1.png");
			auto pSHSheet = ResourceManager::Instance().GetSpriteSheet(L"Images\\Skill\\Skills1.xml", pSHTexture);

			//스킬을 벡터에 등록해준다 
			unique_ptr<HealSkill> heal;
			heal = make_unique<HealSkill>(mpBatch, pSHSheet, mpFont);
			Animation SHanim[] = {
			{ "Effect", 21,{
			{ "healEffect_001", 0.05f },
			{ "healEffect_002", 0.05f },
			{ "healEffect_003", 0.05f },
			{ "healEffect_004", 0.05f },
			{ "healEffect_005", 0.05f },
			{ "healEffect_006", 0.05f },
			{ "healEffect_007", 0.05f },
			{ "healEffect_008", 0.05f },
			{ "healEffect_009", 0.05f },
			{ "healEffect_010", 0.05f },
			{ "healEffect_011", 0.05f },
			{ "healEffect_012", 0.05f },
			{ "healEffect_013", 0.05f },
			{ "healEffect_014", 0.05f },
			{ "healEffect_015", 0.05f },
			{ "healEffect_016", 0.05f },
			{ "healEffect_017", 0.05f },
			{ "healEffect_018", 0.05f },
			{ "healEffect_019", 0.05f },
			{ "healEffect_020", 0.05f },
			{ "healEffect_021", 0.05f },
			}
			},
			};
			heal->Init(meSortID, mPosition, false, SHanim, 1);
			heal->SetSkillProperty(7, 4, 10, L"회복(소)"
				, 0, 20, 1.0f, 0, true);
			heal->SetName(L"소보급");
			mspVecSkills.push_back(move(heal));
			auto classname = mspVecSkills[mspVecSkills.size() - 1].get();
			mVecUiSkillInfo[mVecUiSkillInfo.size() - 1].animName =
				classname->GetName();

			//구원대 추가
			unique_ptr<HealSkill> healCrossMulti;
			healCrossMulti = make_unique<HealSkill>(mpBatch, pSHSheet, mpFont);
			healCrossMulti->Init(meSortID, mPosition, false, SHanim, 1);
			healCrossMulti->SetSkillProperty(7, 4, 25, L"회복(소)"
				, 1, 20, 1.0f, 1, false);
			//healCrossMulti->SetDelayAnimTime(3.2f);
			healCrossMulti->SetName(L"구원대");
			mspVecSkills.push_back(move(healCrossMulti));
			classname = mspVecSkills[mspVecSkills.size() - 1].get();
			mVecUiSkillInfo[mVecUiSkillInfo.size() - 1].animName =
				classname->GetName();

			//스킬을 벡터에 등록해준다
			unique_ptr<HealSkill> baeho;
			baeho = make_unique<HealSkill>(mpBatch, pSHSheet, mpFont);
			Animation SBanim[] = {
				{ "Baegho", 32,{
			{ "baegho_001", 0.1f },
			{ "baegho_002", 0.1f },
			{ "baegho_003", 0.1f },
			{ "baegho_004", 0.1f },
			{ "baegho_005", 0.1f },
			{ "baegho_006", 0.1f },
			{ "baegho_007", 0.1f },
			{ "baegho_008", 0.1f },
			{ "baegho_009", 0.1f },
			{ "baegho_010", 0.1f },
			{ "baegho_011", 0.1f },
			{ "baegho_012", 0.1f },
			{ "baegho_013", 0.1f },
			{ "baegho_014", 0.1f },
			{ "baegho_015", 0.1f },
			{ "baegho_016", 0.1f },
			{ "baegho_017", 0.1f },
			{ "baegho_018", 0.1f },
			{ "baegho_019", 0.1f },
			{ "baegho_020", 0.1f },
			{ "baegho_021", 0.1f },
			{ "baegho_022", 0.1f },
			{ "baegho_023", 0.1f },
			{ "baegho_024", 0.1f },
			{ "baegho_025", 0.1f },
			{ "baegho_026", 0.1f },
			{ "baegho_027", 0.1f },
			{ "baegho_028", 0.1f },
			{ "baegho_029", 0.1f },
			{ "baegho_030", 0.1f },
			{ "baegho_031", 0.1f },
			{ "baegho_032", 0.1f },
			}
				},
				{ "Effect", 21,{
			{ "healEffect_001", 0.05f },
			{ "healEffect_002", 0.05f },
			{ "healEffect_003", 0.05f },
			{ "healEffect_004", 0.05f },
			{ "healEffect_005", 0.05f },
			{ "healEffect_006", 0.05f },
			{ "healEffect_007", 0.05f },
			{ "healEffect_008", 0.05f },
			{ "healEffect_009", 0.05f },
			{ "healEffect_010", 0.05f },
			{ "healEffect_011", 0.05f },
			{ "healEffect_012", 0.05f },
			{ "healEffect_013", 0.05f },
			{ "healEffect_014", 0.05f },
			{ "healEffect_015", 0.05f },
			{ "healEffect_016", 0.05f },
			{ "healEffect_017", 0.05f },
			{ "healEffect_018", 0.05f },
			{ "healEffect_019", 0.05f },
			{ "healEffect_020", 0.05f },
			{ "healEffect_021", 0.05f },
			}
				},
			};
			baeho->Init(meSortID, mPosition, false, SBanim, 2);
			baeho->SetSkillProperty(6, 30, 50, L"회복(중)"
				, 1, 70, 4.25f, 4, true);
			baeho->SetDelayAnimTime(3.2f);
			baeho->SetName(L"백호");
			mspVecSkills.push_back(move(baeho));
			classname = mspVecSkills[mspVecSkills.size() - 1].get();
			mVecUiSkillInfo[mVecUiSkillInfo.size() - 1].animName =
				classname->GetName();
		}

	}
}
