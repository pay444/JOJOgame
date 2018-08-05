#pragma once
class Skill : public MActor
{
public:
	Skill();
	Skill(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont);
	virtual ~Skill();
protected:
	int mElemental;		//��ų�� �Ӽ�  0 �� 1 �� 2 �ٶ� 3 �� 4 Ư���� �� ����, 5 û�� 6 ��ȣ 7 ȸ��
	int mMpCost;		//�� ��ų�� ���µ� ��� ���
	int mScope;			//��ų�� �������
	wstring mEfficacy;	//ȿ�� �̸� ����� ���� ����
	int miSkillSingleMulti;		//�ش� ��ų�� ��������(0) ��Ƽ��������(1) û��(2)
	float mfTotalDelayTime;	//�ش� ��ų�� �� �����ð�
	//float 
	int miActiveArea;		//Ȱ��ȭ�� ���� 0�� ���� 1�� ���� 2�� ������ 3û��(����������) 4��ȣ(��üȸ��)
	bool mbAnimSkill;		//�ִϸ��̼� ����� �����ִ� ��ų���� ȭ��,��ȣ,����,û��
	float mfAnimTime;		//ù��° �ִϸ��̼� ����� ���� �༮�� ����ϴ� �ð����
	float mfDelayAnimTime;	//ù��° �ִϸ��̼� �� ������ �ð�
public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible) = 0;
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible, Animation* anim,int animCount) = 0;
	virtual E_SCENE Update(float dt) = 0;
	virtual void Draw() = 0;
public:
	virtual void SetSkillProperty(int elemental,int scope, int mpCost
		, wstring efficacy, int stateArea,float totalDelayTime,int activeArea,bool animSkill)
	{
		mElemental = elemental;
		mMpCost = mpCost;
		mScope = scope;
		mEfficacy = efficacy;
		miSkillSingleMulti = stateArea;
		mfTotalDelayTime = totalDelayTime;
		miActiveArea = activeArea;
		mbAnimSkill = animSkill;
	}

	void SetDelayAnimTime(float time) { mfDelayAnimTime = time; }
	float GetDelayAnimTime() { return mfDelayAnimTime; }

	wstring GetEfficacyName() { return mEfficacy; }
	int GetElemental() { return mElemental; }
	int GetMpCost() { return mMpCost; }
	int GetSingleMulti() { return miSkillSingleMulti; }
	float GetTotalDelayTime() { return mfTotalDelayTime; }
	int GetActiveArea(){return miActiveArea;}
	int GetScope() { return mScope; }
	bool GetAnumSkil() { return mbAnimSkill; }
};

