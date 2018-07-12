#include "stdafx.h"
#include "MActorManager.h"


MActorManager::MActorManager() :
	tmpPos(XMFLOAT2(0.0f, 0.0f)),
	posIndex2(0),
	posIndex3(0),
	mUiCheck(false),
	mTurn(true),
	mPlayerCount(0),
	mEnemyCount(0),
	mEndTurnPlayerCount(0),
	mEndTurnEnemyCount(0),
	mEnemyControll(0),
	mTurnBool(false),
	mbCountAtkFlag(false),
	mfActionTime(0.0f),
	mCountChracter(nullptr)
{
}


MActorManager::~MActorManager()
{
	
}

E_SCENE MActorManager::Update(float dt)
{
	//SortActors();
	//���� �ٲ��� ����
	for (const auto &actor : mActors)
	{
		MActor* pActor;
		pActor = actor.get();

		//���߿� ���� ������ ���ΰ�
		if (dynamic_cast<Enemy*>(pActor))
		{
			if (((Enemy*)pActor)->GetCode() == *mVecEenemyIndex[mEnemyControll])
			{
				((Enemy*)pActor)->SetActionBool(true);
			}
		}

		E_SCENE eResult = actor->Update(dt);
		if (eResult > E_SCENE_NONPASS)
		{
			return eResult;
		}

	}

	//��� �ڱ� ������ ĳ���Ͱ� �ൿ�� ������ ���� �ٲ�
	CheckAllActionTurn();


	//���� ����� �÷��̾ �������� ������ ���ʹ��� Ÿ�� ����
	if (!mTurn)
	{
		CheckEnemyTarget();
	}

	auto ui = MActorManager::Instance().GetClassUi();
	//UI�� �����ִ� â�� ��Ȱ��ȭ �϶��� ���ùڽ� �̵��� �Ѵ�
	if (!mUiCheck 
		&& ui != NULL
		&& !ui->GetVisible())
	{
		//Ui�� �Ⱥ��̰� �� �μ� Ui�� �Ⱥ��϶���
		auto uiSkills = ui->GetUiSkills();
		auto uiConsumItems = ui->GetUiConsumItem();
		if (!uiSkills->GetAreaVisible()
			&& !uiSkills->GetVisible()
			&& !uiConsumItems->GetAreaVisible()
			&& !uiConsumItems->GetVisible())
		{
			//ui�μӵ��� ������ ������ �ٽ� ���۵�
			if (!uiSkills->GetFlag() && !uiConsumItems->GetFlag())
			{
				//Ŭ���� �ش���� ��ġ�� �����ִ� ���θ� �Ѱ���
				RePosAndVisiMB();
				RePosAndVisiAt();
			}
		}
		//���������� Uiâ�� ���������� Ȱ��
		RePosProgresiveBar();
		
		if (mTurn)
		{
			RePosAndVisiUI();
		}

	}

	//�ൿ �ð� ����
	if (mbCountAtkFlag)
	{
		mfActionTime += dt;
	}
	//�ൿ Ȯ��
	CheckAction();

	if (MFramework::mMouseTracker.rightButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	{
		mUiCheck = false;
	}
	//MoveBox::Instance().Update(dt);
	//AttackBox::Instance().Update(dt);
	//CheckCollidion();

	auto iter = mActors.begin();
	while (iter != mActors.cend())
	{
		auto pActor = iter->get();

		
		if (pActor->GetDestroyed())
		{
			vector<unique_ptr<TILE>> *pVecTile = MActorManager::Instance().GetTileInfo();

			//ĳ���� ������ �����Ḧ ���� ���� �ʱ�ȭ
			if (dynamic_cast<Player*>(pActor))
			{
				mPlayerCount--;
				mEndTurnPlayerCount = 0;
				(*pVecTile)[pActor->GetTileIndex(pActor->GetPosition())]->byOption = 0;
			}
			else if (dynamic_cast<Enemy*>(pActor))
			{
				mEnemyCount--;
				mEndTurnEnemyCount = 0;
				(*pVecTile)[pActor->GetTileIndex(pActor->GetPosition())]->byOption = 0;
			}
			iter->reset();
			*iter = nullptr;
			iter = mActors.erase(iter);	//����� ���ͷ����͵� ���������� �Ѿ

		}
		else
		{
			++iter;
		}

	}

	auto state = Keyboard::Get().GetState();

	if (state.R)
	{
		MActorManager::Instance().SetMBVisible(false);
		//MoveBox::Instance()->SetVisible(false);
		MActorManager::Instance().SetUIVisible(false);
		MActorManager::Instance().SetAtVisible(false);
		MActorManager::Instance().SetClickCount(0);
		mClickCount = 0;
		mUiCheck = false;
		mTurn = true;
		//mActionTurn = 0;
	}
	if (state.T)
	{
		mTurn = false;
	}
	if (state.Y)
	{
		mTurn = true;
	}



	return E_SCENE_NONPASS;
}

void MActorManager::CheckCollidion()
{
	auto iter1 = mActors.begin();
	MActor* pCollider, *pCollidee;
	while (iter1 != mActors.cend())
	{
		pCollider = iter1->get();

		if (pCollider->GetEnabledCollision() == false)
			continue;
		auto iter2 = iter1;
		std::advance(iter2, 1);	//iterater 1ĭ ���� iter++

		while (iter2 != mActors.cend())
		{
			pCollidee = iter2->get();

			if (pCollider->IntersecRect(pCollidee))
				pCollider->OnHit(pCollidee);


			iter2++;
		}
		iter1++;
	}
}

void MActorManager::CheckAction()
{
	auto iter1 = mActors.begin();
	MActor* pCollider, *pCollidee;
	pCollider = nullptr;
	pCollidee = nullptr;
	vector<int> *vecAtScopeIndx;
	for (const auto &actor : mActors)
	{
		pCollider = actor.get();
		if (typeid(*pCollider) == typeid(UI))
		{
			UI * pUi = ((UI*)pCollider);
			//���� UI�� ��������
			if (((UI*)pCollider)->CheckAttackArea() && ((Character*)pUi->GetPlayer())->GetActionTurn() < 2)
			{
				mUiCheck = true;
				SetAtVisible(true);
				((UI*)pCollider)->SetVisible(false);
				GetClassProgresiveBar()->SetVisible(false);
				break;
			} 
			//��� ��ư�� ��������
			else if (((UI*)pCollider)->CheckCancelArea() && ((Character*)pUi->GetPlayer())->GetActionTurn() < 2)
			{
				((UI*)pCollider)->SetVisible(false);
				mClickCount = 0;
				break;
			}
			//��� UI�� ��������
			else if (((UI*)pCollider)->CheckWaitArea() && ((Character*)pUi->GetPlayer())->GetActionTurn() < 2)
			{
				Color color = Colors::Gray;
				((Character*)pUi->GetPlayer())->SetColor(color);
				((Character*)pUi->GetPlayer())->SetActionTurn(2);
				MActorManager::Instance().GetClassUi()->SetVisible(false);
				MActorManager::Instance().GetClassAttackBox()->SetVisible(false);
				GetClassProgresiveBar()->SetVisible(false);
				//mUiCheck = true;
				//SetAtVisible(true);
				//((UI*)pCollider)->SetVisible(false);
				break;
			}
			//å�� ��ư�� ��������
			else if (((UI*)pCollider)->CheckSkillArea() && ((Character*)pUi->GetPlayer())->GetActionTurn() < 2)
			{
				((UI*)pCollider)->GetUiSkills()->SetVisible(true);
				((UI*)pCollider)->SetVisible(false);
				//Color color = Colors::Gray;
				//((Character*)pUi->GetPlayer())->SetColor(color);
				//((Character*)pUi->GetPlayer())->SetActionTurn(2);
				//MActorManager::Instance().GetClassUi()->SetVisible(false);
				//MActorManager::Instance().GetClassAttackBox()->SetVisible(false);
				//mUiCheck = true;
				//SetAtVisible(true);
				//((UI*)pCollider)->SetVisible(false);
				break;
			}
			//���� ��ư�� ��������
			else if (((UI*)pCollider)->CheckConsumItemArea() && ((Character*)pUi->GetPlayer())->GetActionTurn() < 2)
			{
				((UI*)pCollider)->GetUiConsumItem()->SetVisible(true);
				((UI*)pCollider)->SetVisible(false);
				//Color color = Colors::Gray;
				//((Character*)pUi->GetPlayer())->SetColor(color);
				//((Character*)pUi->GetPlayer())->SetActionTurn(2);
				//MActorManager::Instance().GetClassUi()->SetVisible(false);
				//MActorManager::Instance().GetClassAttackBox()->SetVisible(false);
				//mUiCheck = true;
				//SetAtVisible(true);
				//((UI*)pCollider)->SetVisible(false);
				break;
			}

		}

	}
	//ui ���ù�ư�� �������� ���� ���� Ŭ���Ͽ���
	//���ڸ��� ĳ���Ͱ� �ְ� ���� �������� Ȯ�����Ŀ� ����.
	if (mUiCheck)
	{
		if (MFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
		{
			//auto vecAtIndex = MActorManager::Instance().GetvecAtScopeIndex();

			//auto a = vecAtIndex[0];
			for (const auto &actor : mActors)
			{
				pCollider = actor.get();
				if (typeid(*pCollider) == typeid(AttackBox))
				{

					if (((AttackBox*)pCollider)->AttackScopeSeek())
					{
						for (const auto &actor : mActors)
						{
							pCollidee = actor.get();
							if (((AttackBox*)pCollider)->UIntersecRectScope(pCollidee) &&
								dynamic_cast<Character*>(pCollidee))
							{
								pCollidee->OnHit(pCollider, ((AttackBox*)pCollider)->GetCharacter());
								
								//((AttackBox*)pCollider)->GetCharacter()->OnHit(pCollider, pCollidee);
								MActorManager::Instance().SetAtVisible(false);
								mCountChracter = pCollidee;
								//�ݰ��� �ߴ��������� ���� �ʱ�ȭ ������ �༮���� �ݰ��� Ȱ��ȭ �ϱ����� ó��
								((Character*)mCountChracter)->SetisCountAction(false);
								mClickCount = 0;
								mUiCheck = false;
								//�ݰ��� ���� ��������
								mbCountAtkFlag = true;
								mfActionTime = 0.0f;
								break;
							}

						}
					}
				}
			}
		}
	}

	//�ݰ��� ������ �� �ֱ�

	if (mbCountAtkFlag 
		&&mfActionTime >= 2.0f)//((Character*)mCountChracter)->GetisCountAction()
	{
		//auto enemyCurFrame = mCountChracter->GetCurFrame();
		//auto enemyFrames = mCountChracter->GetFrames();
		//auto c = mCountChracter->GetCurAnim();
		//c.erase(0, 1);
		//auto result = mCountChracter->GetAnimaions().find()
		//if()
		//auto c = enemyCurFrame->FrameName;
		//enemyCurFrame++;
		//if (c == "ATTACK" &&enemyCurFrame == enemyFrames->cend())

		//if(c == "ATTACK" && enemyCurFrame == enemyFrames->cend()
		//	&& mCountChracter->GetisEndAnim())
		{
			mbCountAtkFlag = false;
			mfActionTime = 0.0f;
			mCountChracter = nullptr;
		}

	}
	//�ݰ� �ൿ ���� �ݰ� �÷��װ� ��������
	if (mbCountAtkFlag)
	{
		auto atbox = GetClassAttackBox();
		Color cr = Colors::Gray;
		// ���� ĳ������ ������ ���ߴٸ� �ݰ��Ѵ� 
		//�ݰ��� �༮�� Hp�� �ݰ��� �ߴ��� ���ߴ��� Ȯ���Ѵ�
		if (atbox->GetCharacter()->GetColor() == cr
			&& mCountChracter != nullptr 
			&& ((Character*)mCountChracter)->GetHealth() > 0
			&& !((Character*)mCountChracter)->GetisCountAction())
		{
			MActor* pPlayer = atbox->GetCharacter();
			XMFLOAT2 f2Scrool = XMFLOAT2(ScrollMgr::Instance().GetScroll().x
				, ScrollMgr::Instance().GetScroll().y);
			//GetClassAttackBox()->SetCharacter((Character*)mCountChracter);
			atbox->Release();
			atbox->SetPosition(((Character*)mCountChracter)->GetPosition() + f2Scrool);
			atbox->SetAttackDis(((Character*)mCountChracter)->GetAttackDistance());
			atbox->AttackScope();
			if (atbox->AttackScopeSeekPick(pPlayer->GetPosition()))
			{
				pPlayer->OnHit(atbox, mCountChracter);
				((Character*)mCountChracter)->SetisCountAction(true);
				((Character*)mCountChracter)->SetActionTurn(0);
				//mCountChracter = nullptr;
			}
			
		}
		
		//mfActionTime += dt;
	}
}

void MActorManager::CheckAllActionTurn()
{
	TurnGrapic* turnGrapic = nullptr;

	for (const auto &actor : mActors)
	{
		MActor* pActor;
		pActor = actor.get();

		if (dynamic_cast<Character*>(pActor))
		{
			if (pActor->GetCamp() == GunGeon::CampType::PLAYER)
			{
				if (((Character*)pActor)->GetActionTurn() == 2)
				{

					mEndTurnPlayerCount++;
					if (mEndTurnPlayerCount == mPlayerCount)
					{

						for (const auto &actor : mActors)
						{
							if (typeid(*actor) == typeid(TurnGrapic))
							{
								turnGrapic = (TurnGrapic*)actor.get();
								break;
							}
						}
						Color color = Colors::Gray;
						//��� ĳ������ �ൿ�� ������ �ִϸ��̼� ����غ� �Ѵ�.
						if (!mTurnBool
							&& (GetClassAttackBox()->GetCharacter()->GetColor() ==color)
							&& !mbCountAtkFlag)
						{
							turnGrapic->SetAnimation("eTurn");
							//turnGrapic->SetVisible(true);
							//AttackBox �� ��ġ���ִ� ĳ������ ����� true�� �ٲ��ְ� 
							//�׷��� Chractor �ʿ����ϱ׷����� Ȱ��ȭ ������
							GetClassAttackBox()->GetCharacter()->SetMotion(true);
							mTurnBool = true;
						}
		
						//�ϳѾ�� �׷����� ������ �ٲ���
						if (turnGrapic->GetEndTime() && turnGrapic->GetVisible() == false
							)
						{
							//GetClassAttackBox()->GetCharacter()->SetMotion(false);
							mTurnBool = false;
							mTurn = false;
							MActorManager::Instance().SetMBVisible(false);
							MActorManager::Instance().SetUIVisible(false);
							MActorManager::Instance().SetAtVisible(false);
							mClickCount = 0;
							for (const auto &actor : mActors)
							{
								MActor* pActor;
								pActor = actor.get();
								if (dynamic_cast<Player*>(pActor))
								{
									((Character*)pActor)->SetActionTurn(0);
									//�ݰ��ߴ��������� �� �ʱ�ȭ
									((Character*)pActor)->SetisCountAction(false);
								}
							}
							
						}
						break;
	
					}

				}
			}
			else if (pActor->GetCamp() == GunGeon::CampType::MONSTER)
			{
				if (((Character*)pActor)->GetActionTurn() == 2)
				{
					for (const auto &actor : mActors)
					{
						if (typeid(*actor) == typeid(TurnGrapic))
						{
							turnGrapic = (TurnGrapic*)actor.get();
							break;
						}
					}
					mEndTurnEnemyCount++;
					if (mEndTurnEnemyCount == mEnemyCount)
					{
						for (const auto &actor : mActors)
						{
							if (typeid(*actor) == typeid(TurnGrapic))
							{
								turnGrapic = (TurnGrapic*)actor.get();
								break;
							}
						}
						Color color = Colors::Gray;
						if (!mTurnBool
							&& GetClassAttackBox()->GetCharacter()->GetColor() == color)
						{
							turnGrapic->SetAnimation("pTurn");
							//turnGrapic->SetVisible(true);
							//AttackBox �� ��ġ���ִ� ĳ������ ����� true�� �ٲ��ְ� 
							//�׷��� Chractor �ʿ����ϱ׷����� Ȱ��ȭ ������
							GetClassAttackBox()->GetCharacter()->SetMotion(true);
							mTurnBool = true;
						}
						if (turnGrapic->GetEndTime() 
							&& turnGrapic->GetVisible() == false)
						{
							//GetClassAttackBox()->GetCharacter()->SetMotion(false);
							mTurnBool = false;
							mTurn = true;
							MActorManager::Instance().SetMBVisible(false);
							MActorManager::Instance().SetUIVisible(false);
							MActorManager::Instance().SetAtVisible(false);
							mClickCount = 0;
							mEnemyControll = 0;
							for (const auto &actor : mActors)
							{
								MActor* pActor;
								pActor = actor.get();
								if (dynamic_cast<Enemy*>(pActor))
								{
									((Character*)pActor)->SetActionTurn(0);
									//�ݰ��ߴ��������� �� �ʱ�ȭ
									((Character*)pActor)->SetisCountAction(false);
								}
							}
							mUiCheck = false;
							break;
						}
						break;

					}
				}
			}
		}

	}
	mEndTurnPlayerCount = 0;
	mEndTurnEnemyCount = 0;
}

void MActorManager::RePosAndVisiMB()
{
	//Ŭ���� �ش���� ��ġ�� �����ִ� ���θ� ���� �ڽ� ���� �Ѱ���
	if (MFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	{
		int posIndex = 0;
		int mouseIndex = 0;
		XMFLOAT2 pos;
		bool visible = false;
		float fScrollx = ScrollMgr::Instance().GetScroll().x;
		float fScrolly = ScrollMgr::Instance().GetScroll().y;


		for (const auto &actor : mActors)
		{
			auto mouse = Mouse::Get().GetState();

			Vector2 mousePos = Vector2(mouse.x + fScrollx, mouse.y + fScrolly);
			mouseIndex = actor->GetTileIndex(mousePos);

			MActor* pCollider;
			pCollider = actor.get();

			//ĳ���͸� ��ӹ޴� �༮�̶�� ����ڽ� ����
			if (dynamic_cast<Character*>(pCollider))//(typeid(*actor) == typeid(*pCollider))//|| typeid(*actor) == typeid(FotMan))
			{
				pos = actor->GetPosition();
				posIndex = actor->GetTileIndex(pos);

				int movedis = ((Character*)actor.get())->GetMoveDistance();


				//���� ĳ���Ϳ� ���콺��ġ ����
				if (mouseIndex == posIndex)
				{
					visible = ((Character*)actor.get())->GetVisible();


					if (dynamic_cast<Enemy*>(pCollider))
					{
						mClickCount = 0;
						for (const auto &actor : mActors)
						{
							if (typeid(*actor) == typeid(UI))
							{
								((UI *)actor.get())->SetVisible(false);
								break;
							}
						}

					}

					//�׵ڿ� ����ڽ� Ŭ������ ã���� ����
					for (const auto &actor : mActors)
					{
						mousePos = Vector2(mouse.x + fScrollx, mouse.y + fScrolly);
						mouseIndex = actor->GetTileIndex(mousePos);
						if (typeid(*actor) == typeid(MoveBox))
						{
							if (mouseIndex == posIndex)
							{
								if ((pCollider) == ((MoveBox *)actor.get())->GetPlayer())
								{
									mClickCount++;
								}
								else if ((pCollider) != ((MoveBox *)actor.get())->GetPlayer() && ((MoveBox *)actor.get())->GetPlayer() != nullptr)
								{
									mClickCount = 1;
								}
								else
								{
									mClickCount++;
								}

								if (posIndex != posIndex3)
								{
									((MoveBox *)actor.get())->Release();
									//mClickCount = 0;
								}

								actor->SetPosition(pos);
								((MoveBox *)actor.get())->SetPlayer((Player*)pCollider);
								((MoveBox *)actor.get())->SetMoveDis(movedis);
								((MoveBox *)actor.get())->SetVisible(visible);

								posIndex3 = posIndex;
								break;
							}

						}
					}
					//MoveBox::Instance()->SetPosition(pos);
					//MoveBox::Instance()->SetMoveDis(movedis);
					//MoveBox::Instance()->SetVisible(visible);
				}
				else
				{
					for (const auto &actor : mActors)
					{
						if (typeid(*actor) == typeid(MoveBox))
						{
							((AttackBox *)actor.get())->Release();

							((MoveBox *)actor.get())->SetVisible(visible);

							break;
						}
					}
					//MoveBox::Instance()->SetVisible(visible);
				}
			}
		}
	}
}

void MActorManager::RePosAndVisiUI()
{
	if (MFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	{
		int posIndex = 0;
		int mouseIndex = 0;
		XMFLOAT2 pos;
		bool visible = false;
		float fScrollx = ScrollMgr::Instance().GetScroll().x;
		float fScrolly = ScrollMgr::Instance().GetScroll().y;


		for (const auto &actor : mActors)
		{
			auto mouse = Mouse::Get().GetState();

			Vector2 mousePos = Vector2(mouse.x + fScrollx, mouse.y + fScrolly);
			mouseIndex = actor->GetTileIndex(mousePos);

			MActor* pCollider;
			pCollider = actor.get();

			//UI ����
			if (dynamic_cast<Player*>(pCollider))
			{
				bool MBVisible = false;
				pos = actor->GetPosition();
				posIndex = actor->GetTileIndex(pos);

				for (const auto &actor : mActors)
				{
					if (typeid(*actor) == typeid(MoveBox))
						MBVisible = ((MoveBox*)actor.get())->GetVisible();
					break;
				}
				//���� Ŭ���� ĳ���͸� �ֱ�
				if (mouseIndex == posIndex)
				{
					GetClassUi()->SetPlayer((Player*)pCollider);
				}
				if (mClickCount >= 2 && mouseIndex == posIndex && ((Character*)pCollider)->GetActionTurn() < 2)
				{
					for (const auto &actor : mActors)
					{
						if (typeid(*actor) == typeid(UI))
						{
							//MoveBox::Instance()->SetVisible(false);

							SetMBVisible(false);
							SetAtVisible(false);
							actor->SetPosition(pos + XMFLOAT2(100.0f, 0.0f));
							((UI *)actor.get())->SetVisible(true);
							((UI *)actor.get())->SetPlayer((Player*)pCollider);
							mClickCount = 0;
							break;
						}

					}
				}
				else if (MBVisible == false && mouseIndex != posIndex)
				{
					for (auto &actor : mActors)
					{
						if (typeid(*actor) == typeid(UI))
						{
							//((UI *)actor.get())->SetVisible(false);
							//mClickCount = 0;
							break;
						}
					}

				}
			}
		}
	}
}

void MActorManager::RePosAndVisiAt()
{
	//Ŭ���� �ش���� ��ġ�� �����ִ� ���θ� ���� �ڽ� ���� �Ѱ���
	if (MFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	{
		int posIndex = 0;
		int mouseIndex = 0;
		XMFLOAT2 pos;
		XMFLOAT2 pos2;
		bool visible = false;
		float fScrollx = ScrollMgr::Instance().GetScroll().x;
		float fScrolly = ScrollMgr::Instance().GetScroll().y;

		for (const auto &actor : mActors)
		{
			auto mouse = Mouse::Get().GetState();

			Vector2 mousePos = Vector2(mouse.x + fScrollx, mouse.y + fScrolly);
			mouseIndex = actor->GetTileIndex(mousePos);

			MActor* pCollider;
			pCollider = actor.get();

			//ĳ���͸� ��ӹ޴� �༮�̶�� ���ݹڽ� ����
			if (dynamic_cast<Character*>(pCollider))
			{
				pos = pCollider->GetPosition();
				posIndex = pCollider->GetTileIndex(pos);

				int attackdis = ((Character*)actor.get())->GetAttackDistance();
				int attackDag = ((Character*)actor.get())->GetAttack();
				int camp = ((Character*)actor.get())->GetCamp();
				//���� ĳ���Ϳ� ���콺��ġ ����
				if (mouseIndex == posIndex)
				{

					visible = ((Character*)actor.get())->GetVisible();

					//�׵ڿ� ����ڽ� Ŭ������ ã���� ����
					for (const auto &actor : mActors)
					{

						if (typeid(*actor) == typeid(AttackBox))
						{
							if (posIndex != posIndex2)
							{
								((AttackBox *)actor.get())->Release();
							}

							actor->SetPosition(pos);
							((AttackBox *)actor.get())->SetCharacter((Character*)pCollider);

							((AttackBox *)actor.get())->SetAttackDis(attackdis);

							((AttackBox *)actor.get())->SetVisible(visible);

							((AttackBox *)actor.get())->SetAttackDamge(attackDag);

							((AttackBox *)actor.get())->SetCamp(camp);
							posIndex2 = posIndex;
							break;
						}
					}
					//AttackBox::Instance().SetPosition(pos);
					//AttackBox::Instance().SetAttackDis(Attackdis);
					//AttackBox::Instance().GetVecAtScopeIndex().clear();
					//AttackBox::Instance().SetVisible(visible);
				}
				else //if(!GetUICheckArea() && mouseIndex == posIndex)
				{
					//if(posIndex != posIndex2)
					for (const auto &actor : mActors)
					{
						if (typeid(*actor) == typeid(AttackBox))
						{
							((AttackBox *)actor.get())->Release();

							((AttackBox *)actor.get())->SetVisible(visible);
							break;
						}
					}
					//AttackBox::Instance().GetVecAtScopeIndex().clear();
					//AttackBox::Instance().SetVisible(visible);
				}
			}
		}
	}
}

void MActorManager::Draw()
{
	//MoveBox::Instance().Draw();
	//AttackBox::Instance().Draw();
	for (const auto &actor : mActors)
	{
		actor->Draw();

	}

}

void MActorManager::Release()
{
	//�̰��� �ٲ�
	//for ( auto &actor : mActors)
	//{
	//	actor.reset();

	//}

	//���� �ٲ���ų� �������
	mbCountAtkFlag = false;
	mCountChracter = nullptr;
	mfActionTime = 0.0f;
	mClickCount = 0;
	tmpPos = XMFLOAT2(0.0f, 0.0f);
	posIndex2 = 0;
	posIndex3 = 0;
	mUiCheck = false;
	mTurn = true;
	mPlayerCount = 0;
	mEnemyCount = 0;
	mEndTurnPlayerCount = 0;
	mEndTurnEnemyCount = 0;
	mEnemyControll = 0;
	mTurnBool = false;
	mVecEenemyIndex.clear();
	auto iter1 = mVecEenemyIndex.begin();
	while (iter1 != mVecEenemyIndex.end())
	{
		iter1->reset();
		iter1 = mVecEenemyIndex.erase(iter1);	//����� ���ͷ����͵� ���������� �Ѿ
	}
	auto iter = mActors.begin();
	while (iter != mActors.end())
	{
		//auto pActor = iter->get();

		//if (typeid(*pActor) == typeid(MoveBox))
		//{
		//	++iter;
		//	if (iter == mActors.end())
		//	{
		//		break;
		//	}

		//}

		iter->reset();
		iter = mActors.erase(iter);	//����� ���ͷ����͵� ���������� �Ѿ
									//if (iter != mActors.end())
									//	std::advance(iter, 1);
									//else
									//	break;
	}
}

void MActorManager::SortActors()
{
	list<unique_ptr <MActor>> mSortActors[E_SORTID_END];

	for (auto &actor : mActors)
	{
		E_SORTID eID = actor->GetSortID();
		mSortActors[eID].push_back(move(actor));
	}

	for (int i = 0; i < E_SORTID_END; ++i)
	{
		for (auto &actor2 : mSortActors[i])
		{
			mActors.push_back(move(actor2));
		}

	}

	auto iter1 = mActors.begin();
	while (iter1 != mActors.cend())
	{
		if (iter1->get() == NULL)
		{
			iter1->reset();
			iter1 = mActors.erase(iter1);	//����� ���ͷ����͵� ���������� �Ѿ
		}
		else
		{
			++iter1;
		}

	}
	int x = 0;
	mActors.reverse();
}

void MActorManager::CheckEnemyTarget()
{
	struct SGAActorAnddis
	{
		MActor* pActor;
		float distance;
	};
	struct less_than_distance
	{
		inline bool operator() (const SGAActorAnddis& struct1, const SGAActorAnddis& struct2)
		{
			return (struct1.distance < struct2.distance);
		}
	};

	//������ ���� ����� �÷��̾� �� �����´�.
	for (const auto &actor : mActors)
	{
		MActor* pEnemyActor;
		pEnemyActor = actor.get();
		vector<SGAActorAnddis> vecActorAndindex;

		if (dynamic_cast<Enemy*>(pEnemyActor))
		{
			for (const auto &actor : mActors)
			{
				MActor* pPlayerActor;
				pPlayerActor = actor.get();
				if (dynamic_cast<Player*>(pPlayerActor) && ((Character*)pPlayerActor)->GetHealth() > 0)
				{
					SGAActorAnddis actorAnddis;

					float dist = Vector2::Distance(pEnemyActor->GetPosition(), pPlayerActor->GetPosition());
					actorAnddis.pActor = pPlayerActor;
					actorAnddis.distance = dist;

					vecActorAndindex.push_back(actorAnddis);

				}
			}
			//�÷��̾ �ƹ��������� Ż��
			if (vecActorAndindex.empty())
			{
				break;
			}
			std::sort(vecActorAndindex.begin(), vecActorAndindex.end(),less_than_distance());
			((Enemy*)pEnemyActor)->SetTarget(vecActorAndindex[0].pActor);
		}
		vecActorAndindex.clear();

	}
}

E_SCENE MActorManager::GetScene()
{
	return meScene;
}

bool MActorManager::GetMBSeekScope()
{
	for (auto &actor : mActors)
	{
		if (typeid(*actor) == typeid(MoveBox))
		{
			return ((MoveBox*)actor.get())->GetSeekScope();
		}
	}
	return false;
}

bool MActorManager::GetUICheckArea()
{
	for (const auto &actor : mActors)
	{
		if (typeid(*actor) == typeid(UI))
		{
			return ((UI *)actor.get())->CheckAttackArea();
		}
	}
	return false;
}

vector<unique_ptr<int>>* MActorManager::GetvecAtScopeIndex()
{

	for (const auto &actor : mActors)
	{
		if (typeid(*actor) == typeid(AttackBox))
		{
			return ((AttackBox *)actor.get())->GetVecAtScopeIndex();
		}
	}
}

UI * MActorManager::GetClassUi()
{
	for (auto &actor : mActors)
	{
		if (typeid(*actor) == typeid(UI))
		{
			return ((UI*)(actor.get()));
			break;
		}
	}
	return nullptr;
}

AttackBox * MActorManager::GetClassAttackBox()
{
	for (auto &actor : mActors)
	{
		if (typeid(*actor) == typeid(AttackBox))
		{
			return ((AttackBox*)(actor.get()));
			break;
		}
	}
	return nullptr;
}

MoveBox * MActorManager::GetClassMoveBox()
{
	for (auto &actor : mActors)
	{
		if (typeid(*actor) == typeid(MoveBox))
		{
			return ((MoveBox*)(actor.get()));
			break;
		}
	}
	return nullptr;
}


void MActorManager::SetMBVisible(bool visible)
{
	for (auto &actor : mActors)
	{
		if (typeid(*actor) == typeid(MoveBox))
		{
			((MoveBox*)(actor.get()))->SetVisible(visible);
			break;
		}
		//if (typeid(*actor) == typeid(UI))
		//{
		//	((UI*)(actor.get()))->SetVisible(visible);
		//}
	}
}

vector<unique_ptr<TILE>>* MActorManager::GetTileInfo()
{
	for (auto &actor : mActors)
	{
		if (typeid(*actor) == typeid(World))
		{
			return ((World*)(actor.get()))->GetvecTileInfo();
		}
	}
	return nullptr;
}


void MActorManager::SetUIVisible(bool visible)
{
	for (auto &actor : mActors)
	{
		if (typeid(*actor) == typeid(UI))
		{
			((UI*)actor.get())->SetVisible(visible);
			break;
		}
	}
}

void MActorManager::SetAtVisible(bool visible)
{
	for (auto &actor : mActors)
	{
		if (typeid(*actor) == typeid(AttackBox))
		{
			((AttackBox*)actor.get())->SetVisible(visible);
			break;
		}
	}
}

class TurnGrapic* MActorManager::GetClassTurnGrapic()
{
	for (auto &actor : mActors)
	{
		if (typeid(*actor) == typeid(TurnGrapic))
		{
			return ((TurnGrapic*)actor.get());
			break;
		}
	}
	return nullptr;
}

ProgresiveBar * MActorManager::GetClassProgresiveBar()
{
	for (auto &actor : mActors)
	{
		if (typeid(*actor) == typeid(ProgresiveBar))
		{
			return ((ProgresiveBar*)actor.get());
			break;
		}
	}
	return nullptr;
}

void MActorManager::RePosProgresiveBar()
{
	//�ش���� ��ġ�� �����ִ� ���θ� ���α׷��ú� �ٿ��� �Ѱ���
	//if (MFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	{
		int posIndex = 0;
		int mouseIndex = 0;
		XMFLOAT2 pos;
		XMFLOAT2 pos2;
		bool visible = false;
		float fScrollx = ScrollMgr::Instance().GetScroll().x;
		float fScrolly = ScrollMgr::Instance().GetScroll().y;

		for (const auto &actor : mActors)
		{
			auto mouse = Mouse::Get().GetState();   

			Vector2 mousePos = Vector2(mouse.x + fScrollx, mouse.y + fScrolly);
			POINT pmousePos;
			pmousePos.x = mousePos.x;
			pmousePos.y = mousePos.y;
			//mouseIndex = actor->GetTileIndex(mousePos);

			MActor* pCollider;
			pCollider = actor.get();

			//ĳ���͸� ��ӹ޴� �༮�̶�� + �ش� ���� ü���� 0�̻��϶����α׷��ú� ������ ������
			if (dynamic_cast<Character*>(pCollider) && ((Character*)pCollider)->GetHealth()>0)
			{
				pos = pCollider->GetPosition();
				//posIndex = pCollider->GetTileIndex(pos);
				RECT rc;
				rc = RectMakeCenter(pos.x, pos.y, 48, 48);
				//���� ĳ���Ϳ� ���콺��ġ ����
				//if (mouseIndex == posIndex)
				if(PtInRect(&rc,pmousePos))
				{

					//�׵ڿ� ���α׷��ú� Ŭ������ ã���� ����
					for (const auto &actor : mActors)
					{

						if (typeid(*actor) == typeid(ProgresiveBar))
						{
							actor->SetPosition((pos + XMFLOAT2(150.0f, 0.0f)));
							((ProgresiveBar *)actor.get())->SetCharacter((Character*)pCollider);

							((ProgresiveBar *)actor.get())->SetVisible(true);
							break;
						}
					}
					break;
				}
				//�ƴҰ�� �Ⱥ�����
				else 
				{
					for (const auto &actor : mActors)
					{
						if (typeid(*actor) == typeid(ProgresiveBar))
						{

							((ProgresiveBar *)actor.get())->SetVisible(false);
							break;
						}
					}
				}
			}
		}
	}
}

//void MActorManager::InsertMap(string str, unique_ptr<MActor> actor)
//{
//	mMapActors.insert(pair<string, unique_ptr<MActor>>(str, move(actor)));
//}

