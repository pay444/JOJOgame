#include "stdafx.h"
#include "EventEditorMgr.h"
#include "EventCharacter.h"
#include "EventTalk.h"
#include "EventStageMap.h"
EventEditorMgr::EventEditorMgr()
	:mOrder(0),
	misMoveTalkFlag(false),
	misDeleteFlag(false),
	mfElpDelayTime(0.0f),
	mfTimer(0.0f),
	mfDelay(0.0f)
{
}


EventEditorMgr::~EventEditorMgr()
{
}

void EventEditorMgr::Init()
{
	mpActors = MActorManager::Instance().GetActors();
	mVecEvents.push_back(L"Move ����1 (288,114) 0 WalkF");
	mVecEvents.push_back(L"Move ���� (375,80) 0 WalkF");
	mVecEvents.push_back(L"Move ����2 (443,32) 0 WalkF");
	mVecEvents.push_back(L"Move ������ (241,251) 0 WalkB");
	mVecEvents.push_back(L"Stand 3.5");
	mVecEvents.push_back(L"Talk ����1 right 0 StandB (�����, ���θ��ʽÿ�!");
	mVecEvents.push_back(L"Talk ���� left 0 StandF (�˰� �ִ�!");
	mVecEvents.push_back(L"Move ����1 (170,178) 0 WalkF");
	mVecEvents.push_back(L"Move ���� (250,145) 0 WalkF");
	mVecEvents.push_back(L"Move ����2 (333,95) 0 WalkF");
	mVecEvents.push_back(L"Move ������ (367,193) 0 WalkB");
	mVecEvents.push_back(L"Stand 1.2");
	mVecEvents.push_back(L"Talk ������ right 1 StandB (��ٸ��ÿ�!");
	mVecEvents.push_back(L"Talk ����1 left 1 StandF (���� �ʴ�!\n������ �����̳�!");
	mVecEvents.push_back(L"Talk ���� left 0 StandF (��ٷ���");
	mVecEvents.push_back(L"Talk ���� left 1 StandF (������ ���� ���ΰ�, ������?\n���� ���� �⺴������ ���� �ʹ��̶� �Ͽ�");
	mVecEvents.push_back(L"Talk ������ right 1 GreetingB (�ƴ�, �Ͱ��� �λ��� ��ô\n�ű��ؼ� ������\n���� �ɰ� ��������.");
	mVecEvents.push_back(L"Talk ����1 left 1 StandF (�̳�, �츮 ����Կ���\n���� �Ƿ��� ����!");
	mVecEvents.push_back(L"Talk ���� left 0 StandF (��� ��ٷ���");
	mVecEvents.push_back(L"Talk ���� left 1 StandF (������, �״�� �����ΰ�?");
	mVecEvents.push_back(L"Talk ������ right 1 StandB (��.");
	mVecEvents.push_back(L"Talk ���� left 1 StandF (����ֱ�,\n���� �λ��� ��� ���� �ִ°�?");
	mVecEvents.push_back(L"Talk ������ right 1 StandB (������� ġ�������� �ɽ��Դϴٸ�\n�ʳ����� �����̶� �Ҹ� ���Դϴ�.");
	mVecEvents.push_back(L"Talk ����1 left 1 StandF (��, �����!");
	mVecEvents.push_back(L"Talk ���� left 1 StandF (��ٸ���� ����.");
	mVecEvents.push_back(L"Talk ���� left 1 StandF (������, ���� ������\n�����Ѵٴ� ���ΰ�?");
	mVecEvents.push_back(L"Talk ������ right 1 StandB (������,\n�ƴ�, �׷� ���� ���߽��ϴ�.");
	mVecEvents.push_back(L"Talk ���� left 1 StandF (��������� �ʾƵ� ����.\n������ �����̶��......");
	mVecEvents.push_back(L"Talk ���� left 1 HandUpF (����, ���� �糪�̷� �¾����\n������ ����, �װ͵� �����̶��\n�����̰���.");
	mVecEvents.push_back(L"Talk ������ right 1 StandB (�׷����ϱ�.\n�ƴ�, �Ƿʸ� ���߱���.\n�ε� �ؾ� �ֽʽÿ�.");
	mVecEvents.push_back(L"Talk ������ right 1 StandB (�׷�, ������ ���ڽ��ϴ�.");
	mVecEvents.push_back(L"Talk ���� left 1 StandF (......��,\n�׷�, ������, �� ����.");
	mVecEvents.push_back(L"Talk ���� left 0 PointF (�����϶�!\n�������� ������, ��õ�̴�!");
	mVecEvents.push_back(L"Talk ����1 right 0 StandB (��!");
	mVecEvents.push_back(L"Talk ����2 left 0 StandF (��!");
	mVecEvents.push_back(L"Move ����1 (-45,254) 0 WalkF");
	mVecEvents.push_back(L"Move ���� (12,254) 0 WalkF");
	mVecEvents.push_back(L"Move ����2 (92,216) 0 WalkF");
	mVecEvents.push_back(L"Move ������ (602,70) 0 WalkB");
	mVecEvents.push_back(L"Stand 3.0");
	mVecEvents.push_back(L"Talk ������ right 0 StandF (������ ������ �Ǳ⸦ ���ϴٴ�.\n����, ����. ������ �ڽ��� �ϴ�\n���� �ɾ�ô� ���� ����.");
	mVecEvents.push_back(L"Talk ������ right 0 StandF (������.");
	mVecEvents.push_back(L"Move ����1 (-45,254) 0 WalkF");
	mVecEvents.push_back(L"Move ���� (-45,254) 0 WalkF");
	mVecEvents.push_back(L"Move ����2 (-45,254) 0 WalkF");
	mVecEvents.push_back(L"Move ������ (680,-32) 0 WalkB");
	mVecEvents.push_back(L"Stand 2.0");
	mVecEvents.push_back(L"End ��õ���� 1");
}

E_SCENE EventEditorMgr::Update(float dt)
{
	mfTimer += dt;
	wchar_t statewchr[254];
	wstring statewstr = L"";

	if (MFramework::mMouseTracker.rightButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	{
		if (misMoveTalkFlag)
		{
			misMoveTalkFlag = false;
			mOrder++;
		}
	}

	if (mfTimer >= 0.2f && !misMoveTalkFlag)
	{
		EventCheck();
		mfTimer = 0.0f;
	}

	if (misDeleteFlag)
	{
		mfElpDelayTime += dt;
	}
	//������ �ð��� ������ �������� üũ��
	if (misDeleteFlag && mfElpDelayTime >= mfDelay)
	{
		misDeleteFlag = false;
		mfElpDelayTime = 0.0f;
		mOrder++;
		EventCheck();
	}

	return E_SCENE_NONPASS;
}

void EventEditorMgr::Release()
{
	mOrder = 0;					
	mfTimer = 0.0f;				
	misMoveTalkFlag = false;
	mfElpDelayTime = 0;			
	mfDelay = 0;				
	misDeleteFlag = false;		

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

	//���� �̺�Ʈ�� ���ٸ� ��������
	if (mOrder > mVecEvents.size() - 1)
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
				wchar_t wch[256];
				wstring wstr;
				string str;
				int destPosx = 0;
				int destPosy = 0;
				int horizonverticall = 0;
				swscanf(mVecEvents[mOrder].c_str(), L"%s %s (%d,%d) %d %s"
					, &statewchr, &classwchr, &destPosx, &destPosy, &horizonverticall,&wch);
				//misMoveTalkFlag = true;
				wstr = wch;
				str.assign(wstr.begin(), wstr.end());
				((EventCharacter*)pActor)->SetAnimation(str);
				((EventCharacter*)pActor)->SetAnimName(str);
				((EventCharacter*)pActor)->SetHorizonVerticall(horizonverticall);
				((EventCharacter*)pActor)->SetDestPos(XMFLOAT2(destPosx,destPosy));
				((EventCharacter*)pActor)->GetFSM()->ChangeState(JoJoGun::ProductionState::Production_Move);

				auto talk = GetClassTalk();
				talk->SetVisible(false);
				mOrder++;
				break;
			}
		}

	}
	//�����̸� ��
	else if (statewstr == L"Stand")
	{
		misDeleteFlag = true;
		wchar_t wch[100];
		float delay;
		swscanf(mVecEvents[mOrder].c_str(), L"%s %f"
			, &wch, &mfDelay);
		
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
				misMoveTalkFlag = true;
				wchar_t statewchr[254];
				wchar_t classwchr[254];
				wchar_t TLRwchr[254];
				wchar_t CLRwchr[254];
				wchar_t animwchr[254];
				wstring TLRwstr = L"";
				wstring CLRwstr = L"";
				wstring animName = L"";
				swscanf(mVecEvents[mOrder].c_str(), L"%s %s %s %s %s"
					, &statewchr, &classwchr,&TLRwchr,&CLRwchr, &animwchr);
				TLRwstr = TLRwchr;
				CLRwstr = CLRwchr;
				animName = animwchr;


				//(L"Talk ����1 left StandB (�����, ���θ��ʽÿ�!)");

				//swstring�� string ���� ��ȯ
				string animNamestr;
				animNamestr.assign(animName.begin(), animName.end());
				//�ش� ĳ������ �ִϸ��̼� ��ȯ
				pActor->SetAnimation(animNamestr);
				//misMoveTalkFlag = true;

				//��� â ����
				wstring wstr = mVecEvents[mOrder];
				int wstrSize = wstr.size();
				int wstrStart = wstr.find(L"(");
				wstr = wstr.substr(wstrStart+1);
				auto pTalk = GetClassTalk();
				bool LR = false;;
				if (TLRwstr == L"left")
				{
					LR = false;
				}
				else
				{
					LR = true;
				}

				if (CLRwstr == L"0")
				{
					((EventCharacter*)pActor)->SetHorizonVerticall(SpriteEffects_None);
				}
				else
				{
					((EventCharacter*)pActor)->SetHorizonVerticall(SpriteEffects_FlipHorizontally);

				}
				pTalk->SetInfo(wstr, className, LR,static_cast<EventCharacter*>(pActor));
				pTalk->SetVisible(true);
				//((EventCharacter*)pActor)->SetDestPos(XMFLOAT2(destPosx, destPosy));
				//((EventCharacter*)pActor)->GetFSM()->ChangeState(JoJoGun::ProductionState::Production_Talk);
				//mOrder++;
				break;

			}
		}
	}
	else if (statewstr == L"End")
	{
		wchar_t statewchr[254];
		wchar_t stageNamewchr[254];
	//	wchar_t scenewch[254]; 
		wstring stageName = L"";
		//wstring scenewstr = L"";
		int scene;
		swscanf(mVecEvents[mOrder].c_str(), L"%s %s %d"
			, &statewchr, &stageNamewchr,&scene);
		stageName = stageNamewchr;
		//scenewstr = scenewch;

		auto eventSMap = GetClassSMap();
		eventSMap->SetVisible(true);
		eventSMap->SetStageName(stageName);
		eventSMap->SetScene(scene);
	}

}

EventTalk* EventEditorMgr::GetClassTalk()
{
	for (auto &actor: (*mpActors))
	{
		MActor* pActor = actor.get();

		if (typeid(*pActor) == typeid(EventTalk))
		{
			return static_cast<EventTalk*>(pActor);
		}
	}
}

EventStageMap* EventEditorMgr::GetClassSMap()
{
	for (auto &actor : (*mpActors))
	{
		MActor* pActor = actor.get();

		if (typeid(*pActor) == typeid(EventStageMap))
		{
			return static_cast<EventStageMap*>(pActor);
		}
	}
}
