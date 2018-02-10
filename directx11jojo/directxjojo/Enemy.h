#pragma once
class Enemy : public Character
{
public:
	Enemy();
	Enemy(SpriteBatch *pBatch, SGASpriteSheet *pSheet, SpriteFont * pFont);
	~Enemy();
public:
	virtual void Init(
		float moveSpeed,
		XMFLOAT2 startpos, E_SORTID eSortID);
	virtual E_SCENE Update(float dt);
protected:
	XMFLOAT2 mmousePos;
};

