#pragma once
class EventTalk;
class EventStageMap;
class EventEditorMgr final
{
public:
	//레퍼런스 표현
	static EventEditorMgr& Instance()
	{
		static EventEditorMgr instance;
		return instance;
	}
private:
	EventEditorMgr();
	EventEditorMgr(EventEditorMgr const&);
	void operator = (EventEditorMgr const&);
public:
	~EventEditorMgr();
public:
	list<unique_ptr<MActor>>* mpActors;
	vector<wstring> mVecEvents;
	int mOrder;					//행동이 몇번째인지
	float mfTimer;				//이벤트 인풋 타이머
	float mfElpDelayTime;			//이벤트 딜레이 타임
	float mfDelay;				//얼마나 지연시킬것인가
	bool misMoveTalkFlag;		//이 플래그가 올라가면 업데이트의 자동 행동이 멈춤
	bool misDeleteFlag;			//지연시간 플래그
public:
	void Init();
	E_SCENE Update(float dt);					//게임 전투씬에서의 Update();
	void Release();
	void Draw();
public:
	void EventCheck();
public:
	void SetActors(list<unique_ptr<MActor>>* pactor) { mpActors = pactor; }
	list<unique_ptr<MActor>>* GetActors() { return mpActors; }

	void SetOrder(int order) { mOrder = order; }
	int GetOrder() { return mOrder; }

	EventTalk* GetClassTalk();
	EventStageMap* GetClassSMap();
};

