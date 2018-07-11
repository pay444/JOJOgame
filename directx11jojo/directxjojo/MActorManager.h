#pragma once

class MActorManager final// : public iActorDelegate
{
public:
	// 포인터표현
	/*static MActorManager* INstance2()
	{
		static MActorManager instance;
		return &instance;
	}*/
	//레퍼런스 표현
	static MActorManager& Instance()
	{
		static MActorManager instance;
		return instance;
	}
private:
	MActorManager();
	MActorManager(MActorManager const&);
	void operator = (MActorManager const&);
public:
	~MActorManager();
private:
	E_SCENE meScene;
private:
	int mClickCount;
	int posIndex2;				//어택 박스 의 삭제에 영향을주는 위치변수
	int posIndex3;				//무브 박스 의 삭제에 영향을주는 위치변수
	int mPlayerCount;			//플레이어가 몇명있는지
	int mEnemyCount;			//에너미가 몇명있는지
	int mEndTurnPlayerCount;	//턴종료된 플레이어가 몇명인지
	int mEndTurnEnemyCount;		//턴종료되 적이 몇명인지
	int mEnemyControll;			//적을 누구를 움직일지 결정해줌
	bool mUiCheck;				//ui공격버튼이 눌렷는지에 대한여부
	bool mTurn;					//전체 턴제어
	bool mTurnBool;				//TurnGrapic 제어 불변수

	bool mbCountAtkFlag;		//내가 떄리면 적도 때리는것을 확인하는 불변수
	float mfActionTime;			//행동 시간 저장변수
	MActor* mCountChracter;		//반격할 녀석의 포인터
	
	XMFLOAT2 tmpPos;
	vector<unique_ptr<int>> mVecEenemyIndex;	//적을 누구를 움직일지 영향을줌
public:
	E_SCENE Update(float dt);
	void CheckCollidion();
	void CheckAction();
	void CheckAllActionTurn();
	void RePosAndVisiMB();
	void RePosAndVisiUI();
	void RePosAndVisiAt();
	void RePosProgresiveBar();

	void Draw();
	void Release();
	void SortActors();
	void CheckEnemyTarget();
	
	E_SCENE GetScene();
	bool GetMBSeekScope();
	bool GetUICheckArea();
	vector<unique_ptr<int>>* GetvecAtScopeIndex();
	bool GetTurn() { return mTurn; }

	class UI *GetClassUi();
	class AttackBox* GetClassAttackBox();
	class MoveBox* GetClassMoveBox();
	class TurnGrapic* GetClassTurnGrapic();
	class ProgresiveBar* GetClassProgresiveBar();
	//Player * GetPlayer() { return nullptr; }

	//void SetUiPos(XMFLOAT2 pos);
	void SetMBVisible(bool visible);
	vector<unique_ptr<TILE>>* GetTileInfo();
	void SetUIVisible(bool visible);
	void SetClickCount(int count) { mClickCount = count; }
	void SetAtVisible(bool visible);
	void SetUiCheck(bool uiCheck) { mUiCheck = uiCheck; }
	void SetTurn(bool turn) { mTurn = turn; }
	void SetEnemyControllCount(int enemyCount) { mEnemyControll = enemyCount; }
	
	int GetEnemyCount() { return mEnemyCount; }
	vector<unique_ptr<int>>* GetVecEnemyIndex() { return &mVecEenemyIndex; }
	int GetEnemyControllCount() { return mEnemyControll; }

	int GetPlayerCount() { return mPlayerCount; }

	void SetEndTurnPlayerCount(int num) { mEndTurnPlayerCount = num; }
	int GetEndTurnPlayerCount() { return mEndTurnPlayerCount; }

	int GetEnemtCount() { return mEnemyCount; }

	void SetEndTurnEnemyCount(int num) { mEndTurnEnemyCount = num; }
	int GetEndTurnEnemyCount() { return mEndTurnEnemyCount; }

	list<unique_ptr<MActor>>* GetActors() { return &mActors; }
	//void InsertMap(string str, unique_ptr<MActor> actor);
public:
	//템플릿의 통일화 몇개의 인자를 받아도 사용가능하도록
	template<class T,class...Types> inline
		T* Create(Types&&...args)
	{
		mActors.push_back(
			unique_ptr<T>(						//스마트포인터로 만들고 엑터에 집어넣음
				new T(forward<Types>(args)...)	//생성자에 들어가는 파라미터값
				)
		);
		// 플레이어가 몇명인지
		if (dynamic_cast<Player*>(mActors.back().get()))
		{
			mPlayerCount++;
		}
		//에너미가 몇명인지
		else if (dynamic_cast<Enemy*>(mActors.back().get()))
		{
			((Enemy*)mActors.back().get())->SetCode(mEnemyCount);
			mVecEenemyIndex.push_back(make_unique<int>(mEnemyCount));
			mEnemyCount++;

		}
		return dynamic_cast<T*>(mActors.back().get());
	}
private:
	//map<string, unique_ptr <MActor>> mMapActors;
	list<unique_ptr <MActor>> mActors;
	//vector<TILE*> mVectile;
};