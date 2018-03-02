#include "stdafx.h"
#include "SGAActorManager.h"


SGAActorManager::SGAActorManager() :
	tmpPos(XMFLOAT2(0.0f, 0.0f)),
	posIndex2(0),
	mUiCheck(false),
	mTurn(true),
	mActionAter(true)
{
}


SGAActorManager::~SGAActorManager()
{
}

E_SCENE SGAActorManager::Update(float dt)
{
	//SortActors();
	//값이 바뀌지 않음
	for (const auto &actor : mActors)
	{

		E_SCENE eResult = actor->Update(dt);
		if (eResult > E_SCENE_NONPASS)
		{
			return eResult;
		}

	}

	//가장 가까운 플레이어가 누구인지 측정후 에너미의 타겟 설정
	CheckEnemyTarget();


	if (!mUiCheck)
	{
		//클릭한 해당놈의 위치와 보여주는 여부를 넘겨줌
		RePosAndVisiMB();
		
		RePosAndVisiAt();

		if (mTurn)
		{
			RePosAndVisiUI();
		}
	}

	CheckAction();

	if (SGAFramework::mMouseTracker.rightButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
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
			iter->reset();
			iter = mActors.erase(iter);	//지우고 이터레이터도 다음것으로 넘어감
		}
		else
		{
			++iter;
		}

	}

	auto state = Keyboard::Get().GetState();

	if (state.R)
	{
		SGAActorManager::Instance().SetMBVisible(false);
		//MoveBox::Instance()->SetVisible(false);
		SGAActorManager::Instance().SetUIVisible(false);
		SGAActorManager::Instance().SetAtVisible(false);
		SGAActorManager::Instance().SetClickCount(0);
		mClickCount = 0;
		mUiCheck = false;
		mTurn = true;
		//mActionTurn = 0;
	}
	if (state.T)
	{
		mTurn = false;
	}



	return E_SCENE_NONPASS;
}

void SGAActorManager::CheckCollidion()
{
	auto iter1 = mActors.begin();
	SGAActor* pCollider, *pCollidee;
	while (iter1 != mActors.cend())
	{
		pCollider = iter1->get();

		if (pCollider->GetEnabledCollision() == false)
			continue;
		auto iter2 = iter1;
		std::advance(iter2, 1);	//iterater 1칸 전진 iter++

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

void SGAActorManager::CheckAction()
{
	auto iter1 = mActors.begin();
	SGAActor* pCollider, *pCollidee;
	vector<int> *vecAtScopeIndx;
	for (const auto &actor : mActors)
	{
		pCollider = actor.get();
		if (typeid(*pCollider) == typeid(UI))
		{
			UI * pUi = ((UI*)pCollider);
			//공격 UI를 눌렀을때
			if (((UI*)pCollider)->CheckAttackArea() && ((Character*)pUi->GetPlayer())->GetActionTurn() < 2)
			{
				mUiCheck = true;
				SetAtVisible(true);
				((UI*)pCollider)->SetVisible(false);
				break;
			}

		}

	}
	//ui 어택버튼이 눌렸을때 어택 범위 클릭하였고
	//그자리에 캐릭터가 있고 공격 가능한지 확인한후에 공격.
	if (mUiCheck)
	{
		if (SGAFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
		{
			auto vecAtIndex = SGAActorManager::Instance().GetvecAtScopeIndex();

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
							if (((AttackBox*)pCollider)->IntersecRectScope(pCollidee) &&
								dynamic_cast<Character*>(pCollidee))
							{

								pCollidee->OnHit(pCollider, ((AttackBox*)pCollider)->GetCharacter());
								SGAActorManager::Instance().SetAtVisible(false);
								mClickCount = 0;
								mUiCheck = false;
							}

						}
					}
				}
			}
		}
	}
}

void SGAActorManager::RePosAndVisiMB()
{
	//클릭한 해당놈의 위치와 보여주는 여부를 무브 박스 에게 넘겨줌
	if (SGAFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
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

			SGAActor* pCollider;
			pCollider = actor.get();

			//캐릭터를 상속받는 녀석이라면 무브박스 통제
			if (dynamic_cast<Character*>(pCollider))//(typeid(*actor) == typeid(*pCollider))//|| typeid(*actor) == typeid(FotMan))
			{
				pos = actor->GetPosition();
				posIndex = actor->GetTileIndex(pos);

				int movedis = ((Character*)actor.get())->GetMoveDistance();


				//먼저 캐릭터와 마우스위치 판정
				if (mouseIndex == posIndex)
				{

					mActionAter = true;
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

					//그뒤에 무브박스 클래스를 찾은후 대입
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

								if (posIndex != posIndex2)
								{
									((MoveBox *)actor.get())->Release();
									//mClickCount = 0;
								}

								actor->SetPosition(pos);
								((MoveBox *)actor.get())->SetPlayer((Player*)pCollider);
								((MoveBox *)actor.get())->SetMoveDis(movedis);
								((MoveBox *)actor.get())->SetVisible(visible);

								posIndex2 = posIndex;
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

void SGAActorManager::RePosAndVisiUI()
{
	if (SGAFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
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

			SGAActor* pCollider;
			pCollider = actor.get();

			//UI 통제
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
				//내가 클릭한 캐릭터만 넣기
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

void SGAActorManager::RePosAndVisiAt()
{
	//클릭한 해당놈의 위치와 보여주는 여부를 공격 박스 에게 넘겨줌
	if (SGAFramework::mMouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
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

			SGAActor* pCollider;
			pCollider = actor.get();

			//캐릭터를 상속받는 녀석이라면 공격박스 통제
			if (dynamic_cast<Character*>(pCollider))
			{
				pos = pCollider->GetPosition();
				posIndex = pCollider->GetTileIndex(pos);

				int attackdis = ((Character*)actor.get())->GetAttackDistance();
				int attackDag = ((Character*)actor.get())->GetAttack();
				int camp = ((Character*)actor.get())->GetCamp();
				//먼저 캐릭터와 마우스위치 판정
				if (mouseIndex == posIndex)
				{

					visible = ((Character*)actor.get())->GetVisible();

					//그뒤에 무브박스 클래스를 찾은후 대입
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

void SGAActorManager::Draw()
{
	//MoveBox::Instance().Draw();
	//AttackBox::Instance().Draw();
	for (const auto &actor : mActors)
	{
		actor->Draw();

	}

}

void SGAActorManager::Release()
{
	//이것은 바뀜
	//for ( auto &actor : mActors)
	//{
	//	actor.reset();

	//}
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
		iter = mActors.erase(iter);	//지우고 이터레이터도 다음것으로 넘어감
									//if (iter != mActors.end())
									//	std::advance(iter, 1);
									//else
									//	break;
	}
}

void SGAActorManager::SortActors()
{
	list<unique_ptr <SGAActor>> mSortActors[E_SORTID_END];

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
			iter1 = mActors.erase(iter1);	//지우고 이터레이터도 다음것으로 넘어감
		}
		else
		{
			++iter1;
		}

	}
	int x = 0;
	mActors.reverse();
}

void SGAActorManager::CheckEnemyTarget()
{
	struct SGAActorAnddis
	{
		SGAActor* pActor;
		float distance;
	};
	struct less_than_distance
	{
		inline bool operator() (const SGAActorAnddis& struct1, const SGAActorAnddis& struct2)
		{
			return (struct1.distance < struct2.distance);
		}
	};

	//적에서 가장 가까운 플레이어 를 가져온다.
	for (const auto &actor : mActors)
	{
		SGAActor* pEnemyActor;
		pEnemyActor = actor.get();
		vector<SGAActorAnddis> vecActorAndindex;
		if (dynamic_cast<Enemy*>(pEnemyActor))
		{
			for (const auto &actor : mActors)
			{
				SGAActor* pPlayerActor;
				pPlayerActor = actor.get();
				if (dynamic_cast<Player*>(pPlayerActor))
				{
					SGAActorAnddis actorAnddis;

					float dist = Vector2::Distance(pEnemyActor->GetPosition(), pPlayerActor->GetPosition());
					actorAnddis.pActor = pPlayerActor;
					actorAnddis.distance = dist;

					vecActorAndindex.push_back(actorAnddis);

				}
			}

			std::sort(vecActorAndindex.begin(), vecActorAndindex.end(),less_than_distance());
			((Enemy*)pEnemyActor)->SetTarget(vecActorAndindex[0].pActor);
		}
		vecActorAndindex.clear();

	}
}

E_SCENE SGAActorManager::GetScene()
{
	return meScene;
}

bool SGAActorManager::GetMBSeekScope()
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

bool SGAActorManager::GetUICheckArea()
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

vector<unique_ptr<int>>* SGAActorManager::GetvecAtScopeIndex()
{

	for (const auto &actor : mActors)
	{
		if (typeid(*actor) == typeid(AttackBox))
		{
			return ((AttackBox *)actor.get())->GetVecAtScopeIndex();
		}
	}
}

UI * SGAActorManager::GetClassUi()
{
	for (auto &actor : mActors)
	{
		if (typeid(*actor) == typeid(UI))
		{
			return ((UI*)(actor.get()));
			//break;
		}
	}
	return nullptr;
}

AttackBox * SGAActorManager::GetClassAttackBox()
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

MoveBox * SGAActorManager::GetClassMoveBox()
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


void SGAActorManager::SetMBVisible(bool visible)
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

vector<unique_ptr<TILE>>* SGAActorManager::GetTileInfo()
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


void SGAActorManager::SetUIVisible(bool visible)
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

void SGAActorManager::SetAtVisible(bool visible)
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

//void SGAActorManager::InsertMap(string str, unique_ptr<SGAActor> actor)
//{
//	mMapActors.insert(pair<string, unique_ptr<SGAActor>>(str, move(actor)));
//}

