#pragma once
#include "ConsumItems.h"

class MpWater : public ConsumItems
{
public:
	MpWater();
	MpWater(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont);
	virtual ~MpWater();
private:
	//int mMpPlus;
public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible);
	virtual E_SCENE Update(float dt);
	virtual void Draw();
public:
	//int GetMpPlus() { return mMpPlus; }

};

