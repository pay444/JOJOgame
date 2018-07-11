#pragma once
class Enemy : public Character
{
public:
	Enemy();
	Enemy(SpriteBatch *pBatch, SpriteSheet *pSheet, SpriteFont * pFont);
	virtual ~Enemy();
public:
	virtual void Init(
		float moveSpeed,
		XMFLOAT2 startpos, E_SORTID eSortID);
	virtual E_SCENE Update(float dt);
	virtual void Draw();
protected:
	XMFLOAT2 mmousePos;
	unique_ptr<MFSM> mspFSM;
	MActor* mpTarget;
	float mfSpeed;
	float mfDetectRange;
	float mfAttackRange;
	float mfAttackDelay;
	float mfMoveDelay;
	bool mFontVisible;
	int mCode;			//실행 순서를 위한 코드부여
	bool mActionBool;	//실행 순서를 위한 BOOL 부여
	float mfCountAtTime;	//반격 딜레이 주는 시간
	bool	mbCountAtkFlag;	//반격을 하기위한 플래그
public:
	void SetAI(float speed, float detectRange, float attackRange, float attackDelay, float moveDelay)
	{
		this->mfSpeed = speed;
		this->mfDetectRange = detectRange;
		this->mfAttackRange = attackRange;
		this->mfAttackDelay = attackDelay;
		this->mfMoveDelay = moveDelay;
	}
	void SetTarget(MActor* pActor) { this->mpTarget = pActor; }
	MActor* GetTarget() { return mpTarget; }

	void SetCode(int code) { mCode = code; }
	int GetCode() { return mCode; }

	void SetActionBool(bool action) { mActionBool = action; }
	bool GetActionBool() { return mActionBool; }

	float GetDetectRange() { return mfDetectRange; }
	float GetAttackRange() { return mfAttackRange; }
	float GetAttackDelay() { return mfAttackDelay; }
	float GetSpeed() { return mfSpeed; }
	float GetMoveDelay() { return mfMoveDelay; }
	
	void SetCountAtFlag(bool flag) { mbCountAtkFlag = flag; }
	bool GetCountAtFlag() { return mbCountAtkFlag; }
};

