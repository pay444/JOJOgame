#pragma once
#include "Player.h"

class Jojo : public Player
{
public:
	Jojo();
	Jojo(SpriteBatch* pBatch, SGASpriteSheet *pSheet, SpriteFont * pFont);
	virtual ~Jojo();
public:
	virtual void Init(
		float moveSpeed,
		XMFLOAT2 startpos, E_SORTID eSortID,float limitdis);
	virtual E_SCENE Update(float dt);
};

