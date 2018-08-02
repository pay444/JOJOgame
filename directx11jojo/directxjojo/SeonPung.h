#pragma once
#include "DamegeSkill.h"

class SeonPung : public DamegeSkill
{
public:
	SeonPung();
	SeonPung(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont);
	virtual ~SeonPung();
private:

public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible);
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible, Animation* anim, int animCount);

	virtual E_SCENE Update(float dt);
	virtual void Draw();

public:
};

