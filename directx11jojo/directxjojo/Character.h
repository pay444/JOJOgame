#pragma once
//#include "MActor.h"
//#include "MoveBox.h"
class EffectBase
{
protected:
	float mfElapsedTime;	//����ð�
	float mfDuration;		//���ӽð�
	bool mbActiove;			//����Ʈ�� Ȱ��ȭ �Ǿ��ִ���
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
	int mStrength;		//���� ����
public:
	void Start(float time, int strength)
	{
		EffectBase::Start(time);
		this->mStrength = strength;
	}
	//�����ð��� ������ ��
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
	//A Į�󿡼� BĮ����� ����
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
		//���Ϳ��� ���� ȿ���� ����
		//Vector2::
		Color result;
		//t���� 0���� 1�� ���°�  ������ ���� ������������ ǥ��ȭ�ϴ°� �����ð����� ������ �����Ǵ°�����
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
	int mMaxHp;		//�ִ�ü��
	int mHealth;		//ü��
	int mAttack;		//���ݷ�
	int mMaxMana;		//�ִ� ����
	int mMana;			//����
	int mMaxSp;		//�ִ� �ʻ�� ������
	int mSp;			//�ʻ�� ������
	int mStartIndex;
	bool mVisbleScope; //�̵� ������ �������� �Ⱥ������� ����
	char * mAnimName;	//�ִϸ��̼� ���� ���¸� ������ ����
	char * mAnimName2;	//�ִϸ��̼� ���¸� ������ ����
	float limitDistance; //�̵��Ÿ��Ѱ�
	int mMoveDistance;	//�̵��Ÿ�
	int mAttackDistance; //�������ݰŸ�
	int mActionTurn;	//�ൿ ��
	bool mFontVisible;	// ��Ʈ �����ִ� ����
	Color mColor;		//������� �׷��̷� �ٲ���
	bool mColorAllow;	//���� �ٲ��ִ� ����
	bool mMotion;		//�ൿ�� ������ ���� �ٲ��ִ� ����
	bool mbCountAction;	//�ݰ��� �ߴ��� ���ߴ��� Ȯ���ϴ� ����
						//MoveBox *mpMoveBox;
	int mClassType;		//���� ĳ������ ����

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

