#pragma once
class Enemy : public Character
{
public:
	Enemy();
	Enemy(SpriteBatch *pBatch, SGASpriteSheet *pSheet, SpriteFont * pFont);
	virtual ~Enemy();
public:
	virtual void Init(
		float moveSpeed,
		XMFLOAT2 startpos, E_SORTID eSortID);
	virtual E_SCENE Update(float dt);
protected:
	XMFLOAT2 mmousePos;

	unique_ptr<SGAFSM> mspFSM;
	SGAActor* mpTarget;
	float mfSpeed;
	float mfDetectRange;
	float mfAttackRange;
	float mfAttackDelay;
public:
	void SetAI(float speed, float detectRange, float attackRange, float attackDelay)
	{
		this->mfSpeed = speed;
		this->mfDetectRange = detectRange;
		this->mfAttackRange = attackRange;
		this->mfAttackDelay = attackDelay;
	}
	void SetTarget(SGAActor* pActor) { this->mpTarget = pActor; }
	float GetDetectRange() { return mfDetectRange; }
	float GetAttackRange() { return mfAttackRange; }
	float GetAttackDelay() { return mfAttackDelay; }
	float GetSpeed() { return mfSpeed; }
};

