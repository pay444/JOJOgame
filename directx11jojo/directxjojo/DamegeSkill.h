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
public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible);
	virtual E_SCENE Update(float dt);
	virtual void Draw();

public:
	int GetDamege() { return mDamage; }
};

