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
	mVecEvents.push_back(L"Move 병사1 (288,114) 0 WalkF");
	mVecEvents.push_back(L"Move 조조 (375,80) 0 WalkF");
	mVecEvents.push_back(L"Move 병사2 (443,32) 0 WalkF");
	mVecEvents.push_back(L"Move 허차장 (241,251) 0 WalkB");
	mVecEvents.push_back(L"Stand 3.5");
	mVecEvents.push_back(L"Talk 병사1 right 0 StandB (대장님, 서두르십시오!");
	mVecEvents.push_back(L"Talk 조조 left 0 StandF (알고 있다!");
	mVecEvents.push_back(L"Move 병사1 (170,178) 0 WalkF");
	mVecEvents.push_back(L"Move 조조 (250,145) 0 WalkF");
	mVecEvents.push_back(L"Move 병사2 (333,95) 0 WalkF");
	mVecEvents.push_back(L"Move 허차장 (367,193) 0 WalkB");
	mVecEvents.push_back(L"Stand 1.2");
	mVecEvents.push_back(L"Talk 허차장 right 1 StandB (기다리시오!");
	mVecEvents.push_back(L"Talk 병사1 left 1 StandF (뭐냐 너는!\n방해할 생각이냐!");
	mVecEvents.push_back(L"Talk 조조 left 0 StandF (기다려라");
	mVecEvents.push_back(L"Talk 조조 left 1 StandF (나에게 무슨 일인가, 노인장?\n나는 관군 기병대장인 조조 맹덕이라 하오");
	mVecEvents.push_back(L"Talk 허차장 right 1 GreetingB (아니, 귀공의 인상이 무척\n신기해서 무심코\n말을 걸고 말았지요.");
	mVecEvents.push_back(L"Talk 병사1 left 1 StandF (이놈, 우리 대장님에게\n무슨 실례의 말을!");
	mVecEvents.push_back(L"Talk 조조 left 0 StandF (잠깐 기다려라");
	mVecEvents.push_back(L"Talk 조조 left 1 StandF (노인장, 그대는 관상가인가?");
	mVecEvents.push_back(L"Talk 허차장 right 1 StandB (예.");
	mVecEvents.push_back(L"Talk 조조 left 1 StandF (재미있군,\n나의 인상은 어떻게 나와 있는가?");
	mVecEvents.push_back(L"Talk 허차장 right 1 StandB (대장님은 치세에서는 능신입니다만\n훗날에는 간웅이라 불릴 것입니다.");
	mVecEvents.push_back(L"Talk 병사1 left 1 StandF (뭐, 뭐라고!");
	mVecEvents.push_back(L"Talk 조조 left 1 StandF (기다리라고 했지.");
	mVecEvents.push_back(L"Talk 조조 left 1 StandF (노인장, 장차 난세가\n도래한다는 말인가?");
	mVecEvents.push_back(L"Talk 허차장 right 1 StandB (흐흐흐,\n아니, 그런 말은 안했습니다.");
	mVecEvents.push_back(L"Talk 조조 left 1 StandF (얼버무리지 않아도 좋다.\n하지만 간웅이라니......");
	mVecEvents.push_back(L"Talk 조조 left 1 HandUpF (흐흐, 나도 사나이로 태어났으니\n난세의 간웅, 그것도 본심이라면\n본심이겠지.");
	mVecEvents.push_back(L"Talk 허차장 right 1 StandB (그렇습니까.\n아니, 실례를 범했군요.\n부디 잊어 주십시오.");
	mVecEvents.push_back(L"Talk 허차장 right 1 StandB (그럼, 무운을 빌겠습니다.");
	mVecEvents.push_back(L"Talk 조조 left 1 StandF (......음,\n그럼, 노인장, 잘 가오.");
	mVecEvents.push_back(L"Talk 조조 left 0 PointF (진군하라!\n목적지는 격전지, 영천이다!");
	mVecEvents.push_back(L"Talk 병사1 right 0 StandB (예!");
	mVecEvents.push_back(L"Talk 병사2 left 0 StandF (예!");
	mVecEvents.push_back(L"Move 병사1 (-45,254) 0 WalkF");
	mVecEvents.push_back(L"Move 조조 (12,254) 0 WalkF");
	mVecEvents.push_back(L"Move 병사2 (92,216) 0 WalkF");
	mVecEvents.push_back(L"Move 허차장 (602,70) 0 WalkB");
	mVecEvents.push_back(L"Stand 3.0");
	mVecEvents.push_back(L"Talk 허차장 right 0 StandF (스스로 간웅이 되기를 원하다니.\n좋지, 좋아. 조조님 자신이 믿는\n길을 걸어가시는 것이 좋지.");
	mVecEvents.push_back(L"Talk 허차장 right 0 StandF (훗훗후.");
	mVecEvents.push_back(L"Move 병사1 (-45,254) 0 WalkF");
	mVecEvents.push_back(L"Move 조조 (-45,254) 0 WalkF");
	mVecEvents.push_back(L"Move 병사2 (-45,254) 0 WalkF");
	mVecEvents.push_back(L"Move 허차장 (680,-32) 0 WalkB");
	mVecEvents.push_back(L"Stand 2.0");
	mVecEvents.push_back(L"End 영천전투 1");
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
	//딜레이 시간이 지나면 다음것을 체크함
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

	//비어있다면 리턴
	if (mVecEvents.empty())
	{
		return;
	}

	//이제 이벤트가 없다면 빠져나옴
	if (mOrder > mVecEvents.size() - 1)
	{
		return;
	}
	//값을가져와서 상태를 체크한다
	swscanf(mVecEvents[mOrder].c_str(), L"%s %s"
		, &statewchr, &classwchr);
	statewstr = statewchr;
	className = classwchr;
	//만약 이동 상태라면 캐릭터를 이동시켜준다
	if (statewstr == L"Move")
	{
		for (auto &actor : (*mpActors))
		{
			MActor* pActor = actor.get();
			//해당 캐릭터를 찾으면
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
	//딜레이를 줌
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
			//해당 캐릭터를 찾으면
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


				//(L"Talk 병사1 left StandB (대장님, 서두르십시오!)");

				//swstring을 string 으로 변환
				string animNamestr;
				animNamestr.assign(animName.begin(), animName.end());
				//해당 캐릭터의 애니메이션 변환
				pActor->SetAnimation(animNamestr);
				//misMoveTalkFlag = true;

				//대사 창 설정
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
