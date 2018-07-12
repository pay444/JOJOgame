#pragma once
#include "ConsumItems.h"

class HpBean : public ConsumItems
{
public:
	HpBean();
	HpBean(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont);
	virtual ~HpBean();
protected:
	//int mHpPlus;		
public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible);
	virtual E_SCENE Update(float dt);
	virtual void Draw();

public:
	//int GetHpPlus() { return mHpPlus; }
};

