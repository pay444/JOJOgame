#pragma once
class Skill : public MActor
{
public:
	Skill();
	Skill(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont);
	virtual ~Skill();
protected:
	int mElemental;		//스킬의 속성  0 물 1 불 2 바람 3 땅 4 특별한 것 주작, 5 청룡 6 백호 7 회복
	int mMpCost;		//이 스킬을 쓰는데 드는 비용
	int mScope;			//스킬의 적용범위
	wstring mEfficacy;	//효과 이름 출력을 위한 변수
	int miSkillSingleMulti;		//해당 스킬이 단일인지(0) 멀티범위인지(1) 청룡(2)
	float mfTotalDelayTime;	//해당 스킬의 총 지연시간
	//float 
	int miActiveArea;		//활성화된 범위 0은 단일 1은 십자 2는 마방진 3청룡(랜덤때리기) 4백호(전체회복)
	bool mbAnimSkill;		//애니메이션 출력후 보여주는 스킬인지 화룡,백호,주작,청룡
	float mfAnimTime;		//첫번째 애니메이션 출력후 다음 녀석을 출력하는 시간기록
	float mfDelayAnimTime;	//첫번째 애니메이션 의 딜레이 시간
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

