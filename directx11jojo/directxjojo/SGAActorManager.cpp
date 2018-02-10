#include "stdafx.h"
#include "SGAActorManager.h"


SGAActorManager::SGAActorManager()
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


	//CheckCollidion();
	//CheckAction();
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

	//클릭한 해당놈의 위치와 보여주는 여부를 무브 박스 에게 넘겨줌
	RePosAndVisi();

	AtRePosAndVisi();

	//클릭한 해당놈의의 위치와 보여주는 여부를 UI에게 넘겨줌
	UIRePosAndVisi();



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
		if (typeid(*actor)==typeid(UI))
		{
			if (((UI*)pCollider)->CheckAttackArea())
			{
				for (const auto &actor : mActors)
				{
					if (typeid(*actor) == typeid(AttackBox))
					{
						//vecAtScopeIndx = &((AttackBox*)actor.get())->GetVecAtScopeIndex();
						break;
					}
				}
				for (const auto &actor : mActors)
				{
					if (typeid(*actor) == typeid(AttackBox))
					{
						//vecAtScopeIndx = &((AttackBox*)actor.get())->GetVecAtScopeIndex();
					}
				}
			}
			
		}
	}
}

void SGAActorManager::RePosAndVisi()
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

					visible = ((Character*)actor.get())->GetVisible();

					if (dynamic_cast<Player*>(pCollider))
					{
						mClickCount++;
					}
					if (dynamic_cast<Enemy*>(pCollider))
					{
						mClickCount = 0;
						for (const auto &actor : mActors)
						{
							if (typeid(*actor)==typeid(UI))
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
								actor->SetPosition(pos);
								((MoveBox *)actor.get())->SetMoveDis(movedis);
								((MoveBox *)actor.get())->SetVisible(visible);
							}

						}
					}
				}
				else
				{
					for (const auto &actor : mActors)
					{
						if (typeid(*actor) == typeid(MoveBox))
						{
							((MoveBox *)actor.get())->SetVisible(visible);
						}
					}

				}
			}
		}
	}
}

void SGAActorManager::UIRePosAndVisi()
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
				}

				if (mClickCount >= 2 && mouseIndex == posIndex)
				{
					for (const auto &actor : mActors)
					{
						if (typeid(*actor) == typeid(UI))
						{
							SetMBVisible(false);
							SetAtVisible(false);
							actor->SetPosition(pos + XMFLOAT2(100.0f, 0.0f));
							((UI *)actor.get())->SetVisible(true);
							mClickCount = 0;
						}
						if (typeid(*actor) == typeid(AttackBox))
						{
							//((AttackBox *)actor.get())->SetVisible(true);
						}

					}
				}
				else if (MBVisible == false && mouseIndex != posIndex)
				{
					for (auto &actor : mActors)
					{
						if (typeid(*actor) == typeid(UI))
						{
							((UI *)actor.get())->SetVisible(false);
							mClickCount = 0;
						}
						if (typeid(*actor) == typeid(AttackBox))
						{
							//((AttackBox *)actor.get())->SetVisible(false);
						}
					}

				}
			}
		}
	}
}

void SGAActorManager::AtRePosAndVisi()
{
	//클릭한 해당놈의 위치와 보여주는 여부를 공격 박스 에게 넘겨줌
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

			//캐릭터를 상속받는 녀석이라면 공격박스 통제
			if (dynamic_cast<Character*>(pCollider))
			{
				pos = actor->GetPosition();
				posIndex = actor->GetTileIndex(pos);
				
				int Attackdis = ((Character*)actor.get())->GetAttackDistance();

				//먼저 캐릭터와 마우스위치 판정
				if (mouseIndex == posIndex)
				{

					visible = ((Character*)actor.get())->GetVisible();

					//그뒤에 무브박스 클래스를 찾은후 대입
					for (const auto &actor : mActors)
					{

						if (typeid(*actor) == typeid(AttackBox))
						{
							if (mouseIndex == posIndex)
							{
								actor->SetPosition(pos);
								((AttackBox *)actor.get())->SetAttackDis(Attackdis);
								((AttackBox *)actor.get())->SetVisible(visible);
							}

						}
					}
				}
				else
				{
					for (const auto &actor : mActors)
					{
						if (typeid(*actor) == typeid(AttackBox))
						{
							((AttackBox *)actor.get())->SetVisible(visible);
						}
					}

				}
			}
		}
	}
}

void SGAActorManager::Draw()
{
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
		auto pActor = iter->get();

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
		auto pActor = iter1->get();

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

void SGAActorManager::SetMBVisible(bool visible)
{
	for (auto &actor : mActors)
	{
		if (typeid(*actor) == typeid(MoveBox))
		{
			((MoveBox*)(actor.get()))->SetVisible(visible);
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

vector<unique_ptr<int>>* SGAActorManager::GetCharacterPosIndex()
{
	vector <unique_ptr<int>> vecCharacterPosIndex;
	SGAActor* pActor;

	for (const auto &actor : mActors)
	{
		pActor = actor.get();
		if (dynamic_cast<Character*>(pActor))
		{
			int positionIndex = actor->GetTileIndex(((Character*)actor.get())->GetPosition());
			vecCharacterPosIndex.push_back(unique_ptr<int>(new int(positionIndex)));
		}
	}
	
	return &vecCharacterPosIndex;
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

