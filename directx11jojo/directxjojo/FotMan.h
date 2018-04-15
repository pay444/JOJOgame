#pragma once
#include "Enemy.h"

class FotMan :public Enemy
{
public:
	FotMan();
	FotMan(SpriteBatch* pBatch, SpriteSheet *pSheet, SpriteFont * pFont);
	~FotMan();
public:
	virtual void Init(
		float moveSpeed,
		XMFLOAT2 startpos, E_SORTID eSortID, float limitdis);
	virtual E_SCENE Update(float dt);
};

