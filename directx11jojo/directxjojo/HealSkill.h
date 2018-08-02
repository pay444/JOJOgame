#pragma once
#include "Skill.h"
class HealSkill :
	public Skill
{
public:
	HealSkill();
	HealSkill(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont);
	virtual ~HealSkill();
protected:
	int mHeal;		//이스킬의 힐량
public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible);
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible, Animation* anim, int animCount);
	virtual E_SCENE Update(float dt);
	virtual void Draw();
public:
	virtual void SetSkillProperty(int elemental, int scope, int mpCost
		, wstring efficacy, int stateArea, int heal, float totalDelayTime, int activeArea
		, bool animSkill)
	{
		mElemental = elemental;
		mScope = scope;
		mMpCost = mpCost;
		mEfficacy = efficacy;
		miSkillSingleMulti = stateArea;
		mHeal = heal;
		mfTotalDelayTime = totalDelayTime;
		miActiveArea = activeArea;
		mbAnimSkill = animSkill;
	}

	virtual void SetHeal(int heal) { mHeal= heal; }
	virtual int GetHeal() { return mHeal; }
};

