#include "stdafx.h"
#include "EventEditorMgr.h"
#include "EventCharacter.h"

EventEditorMgr::EventEditorMgr()
	:mOrder(0),
	misMoveTalkFlag(false)
{
}


EventEditorMgr::~EventEditorMgr()
{
}

void EventEditorMgr::Init()
{
	mpActors = MActorManager::Instance().GetActors();
	mVecEvents.push_back(L"Move ����1 (288,114)");
	mVecEvents.push_back(L"Move ���� (375,80)");
	mVecEvents.push_back(L"Move ����2 (443,32)");
	mVecEvents.push_back(L"Stand");
	mVecEvents.push_back(L"Talk");

}

E_SCENE EventEditorMgr::Update(float dt)
{
	mfTimer += dt;

	wchar_t statewchr[254];
	wstring statewstr = L"";

	//���������ͼ� ���¸� üũ�Ѵ�
	swscanf(mVecEvents[mOrder].c_str(), L"%s"
		, &statewchr);
	statewstr = statewchr;
	//&& statewstr != L"Move"
	if (mfTimer >= 1.0f && !misMoveTalkFlag)
	{
		EventCheck();
		mfTimer = 0.0f;
	}
	
	return E_SCENE_NONPASS;
}

void EventEditorMgr::Release()
{
	mOrder = 0;					
	mfTimer = 0.0f;				
	misMoveTalkFlag = false;
	mVecEvents.clear();
}

void EventEditorMgr::Draw()
{
	
}

void EventEditorMgr::EventCheck()
{
	wchar_t statewchr[254];
	wchar_t classwchr[254];

	wstring statewstr = L"";
	wstring className = L"";

	//����ִٸ� ����
	if (mVecEvents.empty())
	{
		return;
	}

	//���������ͼ� ���¸� üũ�Ѵ�
	swscanf(mVecEvents[mOrder].c_str(), L"%s %s"
		, &statewchr, &classwchr);
	statewstr = statewchr;
	className = classwchr;
	//���� �̵� ���¶�� ĳ���͸� �̵������ش�
	if (statewstr == L"Move")
	{
		for (auto &actor : (*mpActors))
		{
			MActor* pActor = actor.get();
			//�ش� ĳ���͸� ã����
			if (pActor->GetName() == className)
			{
				int destPosx = 0;
				int destPosy = 0;
				swscanf(mVecEvents[mOrder].c_str(), L"%s %s (%d,%d)"
					, &statewchr, &classwchr, &destPosx, &destPosy);
				//misMoveTalkFlag = true;
				((EventCharacter*)pActor)->SetDestPos(XMFLOAT2(destPosx,destPosy));
				((EventCharacter*)pActor)->GetFSM()->ChangeState(JoJoGun::ProductionState::Production_Move);
				mOrder++;
				break;
			}
		}

	}
	else if (statewstr == L"Stand")
	{
		//for (auto &actor : (*mpActors))
		//{
		//	MActor* pActor = actor.get();
		//	if (typeid(*pActor) ==typeid(EventCharacter))
		//	{
		//		//((EventCharacter*)pActor)->GetFSM()->ChangeState(JoJoGun::ProductionState::Production_Idle);
		//		//mOrder++;
		//	}
		//}
	}
	else if (statewstr == L"Talk")
	{
		for (auto &actor : (*mpActors))
		{
			MActor* pActor = actor.get();
			//�ش� ĳ���͸� ã����
			if (pActor->GetName() == className)
			{
				//misMoveTalkFlag = true;

				//((EventCharacter*)pActor)->SetDestPos(XMFLOAT2(destPosx, destPosy));
				((EventCharacter*)pActor)->GetFSM()->ChangeState(JoJoGun::ProductionState::Production_Talk);
				//mOrder++;
				break;

			}
		}
	}
}
