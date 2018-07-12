#pragma once
#include "ConsumItems.h"

class HpBean : public ConsumItems
{
public:
	HpBean();
	HpBean(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont);
	virtual ~HpBean();
protected:
	int mHpPlus;		//체력을 얼마나 증가시키는가 에 대한 변수
public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible);
	virtual E_SCENE Update(float dt);
	virtual void Draw();

public:
	int GetHpPlus() { return mHpPlus; }
};

