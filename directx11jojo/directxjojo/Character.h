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
	int mHealth;
	int mAttack;
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
						//MoveBox *mpMoveBox;
public:
	void SetStaus(int health, int attack, int moveDis, int AttackDis)
	{
		this->mHealth = health;
		this->mAttack = attack;
		this->mMoveDistance = moveDis;
		this->mAttackDistance = AttackDis;
		//mpMoveBox->SetMoveDis(mMoveDistance);
	}
	void SetActionTurn(int actionTurn) { mActionTurn = actionTurn; }
	void SetColor(Color color) { mColor = color; }
	void SetColorAllow(bool colorBool) { mColorAllow = colorBool; }
	void SetAniName(char* name) { mAnimName2 = name; }

	char* GetAniName() { return mAnimName2; }
	bool GetColorAllow() { return mColorAllow; }
	int GetHealth() { return mHealth; }
	int GetAttack() { return mAttack; }
	int GetMoveDistance() { return mMoveDistance; }
	int GetAttackDistance() { return mAttackDistance; }
	bool GetVisible() { return mVisbleScope; }
	int GetActionTurn() { return mActionTurn; }

	void SetMotion(bool motion) { mMotion = motion; }
	bool GetMotion() { return mMotion; }
	void JoAstar_Start(const Vector2 &vDestPos, const Vector2 &vSorcePos);
	bool JoAStar_Move(float dt);
};

