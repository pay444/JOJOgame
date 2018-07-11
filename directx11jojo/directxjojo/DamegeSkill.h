#pragma once
#include "Skill.h"

class DamegeSkill: public Skill
{
public:
	DamegeSkill();
	DamegeSkill(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont);
	virtual ~DamegeSkill();

protected:
	int mDamage;		//�̽�ų�� ���ݷ�
public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible);
	virtual E_SCENE Update(float dt);
	virtual void Draw();

public:
	int GetDamege() { return mDamage; }
};

