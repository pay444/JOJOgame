#pragma once
class Character;

class MActorManager final// : public iActorDelegate
{
public:
	// ������ǥ��
	/*static MActorManager* INstance2()
	{
		static MActorManager instance;
		return &instance;
	}*/
	//���۷��� ǥ��
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
	int posIndex2;				//���� �ڽ� �� ������ �������ִ� ��ġ����
	int posIndex3;				//���� �ڽ� �� ������ �������ִ� ��ġ����
	int mPlayerCount;			//�÷��̾ ����ִ���
	int mEnemyCount;			//���ʹ̰� ����ִ���
	int mEndTurnPlayerCount;	//������� �÷��̾ �������
	int mEndTurnEnemyCount;		//������� ���� �������
	int mEnemyControll;			//���� ������ �������� ��������
	bool mUiCheck;				//ui���ݹ�ư�� ���Ǵ����� ���ѿ���
	bool mTurn;					//��ü ������
	bool mTurnBool;				//TurnGrapic ���� �Һ���

	bool mbCountAtkFlag;		//���� ������ ���� �����°��� Ȯ���ϴ� �Һ���
	bool mbPincerAtkFlag;		//���� �÷���
	int miPickPincerIndex;		//���õ� ���� ���� �༮�� �ε��� ��ȣ
	int miCurPincerCharIndex;	//���� �߿� ������� �����ϱ� ���� ���� ���������ϰ��ִ���
	float mfActionTime;			//�ൿ �ð� ���庯��
	MActor* mCountChracter;		//�ݰ��� �༮�� ������
	bool mbUltimateFlag;		//�ʻ���� �÷���
	
	XMFLOAT2 tmpPos;
	vector<unique_ptr<int>> mVecEenemyIndex;	//���� ������ �������� ��������
	vector<int> mVecPicerTileIndex;				//������ �Ÿ�� ���� ĳ���Ͱ� �ö��ִ��� üũ
	vector<Character*> mVecAttAreaCharacter;		//�� ���ݹ����� ����ִ� ĳ���͸� ����ִ� ����
	vector<PINCERINFO> mVecPincerCharacter;			//�����Ҽ��ִ� ĳ���͸� ����ִ� ����
public:
	E_SCENE Update(float dt);
	E_SCENE GameMainUpdate(float dt);
	E_SCENE EventUpdate(float dt);
	void Draw();
	void Release();

public:
	bool CheckPicerAtk();
	void CheckCollidion();
	void CheckAction();
	void CheckAllActionTurn();
	void RePosAndVisiMB();
	void RePosAndVisiUI();
	void RePosAndVisiAt();
	void RePosProgresiveBar();

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
	//���ø��� ����ȭ ��� ���ڸ� �޾Ƶ� ��밡���ϵ���
	template<class T,class...Types> inline
		T* Create(Types&&...args)
	{
		mActors.push_back(
			unique_ptr<T>(						//����Ʈ�����ͷ� ����� ���Ϳ� �������
				new T(forward<Types>(args)...)	//�����ڿ� ���� �Ķ���Ͱ�
				)
		);
		// �÷��̾ �������
		if (dynamic_cast<Player*>(mActors.back().get()))
		{
			mPlayerCount++;
		}
		//���ʹ̰� �������
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