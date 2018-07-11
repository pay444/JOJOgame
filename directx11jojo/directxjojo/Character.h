#pragma once
//#include "MActor.h"
//#include "MoveBox.h"
class EffectBase
{
protected:
	float mfElapsedTime;	//경과시간
	float mfDuration;		//지속시간
	bool mbActiove;			//이펙트가 활성화 되어있는지
protected:
	void Start(float time)
	{
		this->mbActiove = true;
		this->mfElapsedTime = 0.0f;
		this->mfDuration = time;
	}
	void Stop()
	{
		this->mbActiove = false;
		this->mfElapsedTime = 0.0f;
		this->mfDuration = 0.0;
	}
public:
	bool IsActive() { return mbActiove; }
	virtual void Update(float dt)
	{
		if (this->mfElapsedTime > this->mfDuration)
		{
			this->mbActiove = false;
			this->mfElapsedTime = this->mfDuration;
		}
		else
		{
			this->mfElapsedTime += dt;
		}
	}

};

class EffectShake : public EffectBase
{
	int mStrength;		//흔드는 변수
public:
	void Start(float time, int strength)
	{
		EffectBase::Start(time);
		this->mStrength = strength;
	}
	//현제시간에 보관된 값
	XMFLOAT2 GetValue()
	{
		XMFLOAT2 result;
		result.x = static_cast<float>(rand() % mStrength);
		result.y = static_cast<float>(rand() % mStrength);

		return result;
	}
};

class EffectTint : public EffectBase
{
private:
	//A 칼라에서 B칼라까지 보관
	Color mFrom;
	Color mTo;
public:
	void Start(float time, Color from, Color to)
	{
		EffectBase::Start(time);
		this->mFrom = from;
		this->mTo = to;
	}

	Color GetValue()
	{
		//백터에도 여러 효과가 잇음
		//Vector2::
		Color result;
		//t값은 0에서 1로 가는것  나누는 것이 선형보강에서 표준화하는것 현제시간에서 어디까지 보강되는것인지
		Color::Lerp(mFrom, mTo, this->mfElapsedTime / this->mfDuration, result);
		return result;
	}

};
class Character : public MActor
{
public:
	Character();
	Character(SpriteBatch* pBatch, SpriteSheet* pSheet, SpriteFont * pFont);
	Character(SpriteBatch* pBatch);
	virtual ~Character();
public:
	virtual void Init();
	virtual void DoDamage(Character* pAttacker) {}
	virtual E_SCENE Update(float dt);
	virtual void Draw();
	virtual void OnHit(MActor* pCollidee);
	virtual void OnHit(MActor* pCollider, MActor* pCollidee);
	virtual void DoDamage(MActor* pAttacker);
	void MoveStateCheck();
protected:
	void InitEffect();
	unique_ptr<EffectShake> mspShake;
	unique_ptr<EffectTint> mspTint;
protected:
	int mMaxHp;		//최대체력
	int mHealth;		//체력
	int mAttack;		//공격력
	int mMaxMana;		//최대 마나
	int mMana;			//마나
	int mMaxSp;		//최대 필살기 게이지
	int mSp;			//필살기 게이지
	int mStartIndex;
	bool mVisbleScope; //이동 범위를 보여줄지 안보여줄지 결정
	char * mAnimName;	//애니메이션 공격 상태를 저장할 변수
	char * mAnimName2;	//애니메이션 상태를 저장할 변수
	float limitDistance; //이동거리한계
	int mMoveDistance;	//이동거리
	int mAttackDistance; //근접공격거리
	int mActionTurn;	//행동 턴
	bool mFontVisible;	// 폰트 보여주는 여부
	Color mColor;		//턴종료시 그레이로 바꿔줌
	bool mColorAllow;	//색깔 바꿔주는 여부
	bool mMotion;		//행동이 끝나야 턴을 바꿔주는 변수
	bool mbCountAction;	//반격을 했는지 안했는지 확인하는 변수
						//MoveBox *mpMoveBox;
	int mClassType;		//현재 캐릭터의 직업

public:
	void SetStaus(int health, int attack,int mana,int sp, int moveDis, int AttackDis)
	{
		this->mMaxHp = health;
		this->mHealth = health;
		this->mAttack = attack;
		this->mMaxMana = mana;
		this->mMana = mana;
		this->mMaxSp = sp;
		this->mSp = sp;
		this->mMoveDistance = moveDis;
		this->mAttackDistance = AttackDis;
		//mpMoveBox->SetMoveDis(mMoveDistance);
	}
	
	void SetAniName(char* name) { mAnimName2 = name; }
	char* GetAniName() { return mAnimName2; }

	void SetColorAllow(bool colorBool) { mColorAllow = colorBool; }
	bool GetColorAllow() { return mColorAllow; }

	void SetHelth(int hp) { mHealth = hp; }
	int GetHealth() { return mHealth; }
	
	void SetMaxHp(int hp) { mMaxHp = hp; }
	int GetMaxHp() { return mMaxHp; }

	void SetAttack(int at) { mAttack = at; }
	int GetAttack() { return mAttack; }

	void SetMana(int mp) { mMana = mp; }
	int GetMana() { return mMana; }

	void SetMaxMana(int mp) { mMaxMana = mp; }
	int GetMaxMana() { return mMaxMana; }

	void SetSp(int sp) { mSp = sp; }
	int GetSp() { return mSp; }

	void SetMaxSp(int sp) { mMaxSp = sp; }
	int GetMaxSp() { return mMaxSp; }

	int GetMoveDistance() { return mMoveDistance; }
	int GetAttackDistance() { return mAttackDistance; }
	bool GetVisible() { return mVisbleScope; }

	void SetActionTurn(int actionTurn) { mActionTurn = actionTurn; }
	int GetActionTurn() { return mActionTurn; }

	void SetMotion(bool motion) { mMotion = motion; }
	bool GetMotion() { return mMotion; }

	void SetColor(Color color) { mColor = color; }
	Color GetColor() { return mColor; }

	void SetClassType(int type) { mClassType = type; }
	int GetClassType() { return mClassType; }

	void SetisCountAction(bool countAction) { mbCountAction = countAction; }
	bool GetisCountAction() { return mbCountAction; }

	void JoAstar_Start(const Vector2 &vDestPos, const Vector2 &vSorcePos);
	bool JoAStar_Move(float dt);
};

