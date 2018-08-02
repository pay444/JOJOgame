#pragma once
#include "Skill.h"

class DamegeSkill: public Skill
{
public:
	DamegeSkill();
	DamegeSkill(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont);
	virtual ~DamegeSkill();

protected:
	int mDamage;		//이스킬의 공격력
	bool mSPVisible;
	Player* mpPlayer;
public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible);
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible, Animation* anim, int animCount);
	virtual E_SCENE Update(float dt);
	virtual void Draw();

public:
	virtual void SetSkillProperty(int elemental, int scope, int mpCost
		, wstring efficacy, int stateArea,int damage, float totalDelayTime, int activeArea
	, bool animSkill)
	{
		mElemental = elemental;
		mScope = scope;
		mMpCost = mpCost;
		mEfficacy = efficacy;
		miSkillSingleMulti = stateArea;
		mDamage = damage;
		mfTotalDelayTime = totalDelayTime;
		miActiveArea = activeArea;
		mbAnimSkill = animSkill;
	}

	virtual void SetDamege(int damege) { mDamage = damege; }
	virtual int GetDamege() { return mDamage; }

	virtual void SetPlayer(Player * player) { mpPlayer = player; }
	virtual Player * GetPlayer() { return mpPlayer; }

	virtual void SetVisible(bool visible) { mSPVisible = visible; }
	virtual bool GetVisible() { return mSPVisible; }
};

