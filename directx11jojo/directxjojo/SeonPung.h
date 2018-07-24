#pragma once
#include "DamegeSkill.h"

class SeonPung : public DamegeSkill
{
public:
	SeonPung();
	SeonPung(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont);
	virtual ~SeonPung();
private:
	bool mSPVisible;
	Player* mpPlayer;
public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible);
	virtual E_SCENE Update(float dt);
	virtual void Draw();

public:
	void SetPlayer(Player * player) { mpPlayer = player; }
	Player * GetPlayer() { return mpPlayer; }

	void SetVisible(bool visible) { mSPVisible = visible; }
	bool GetVisible() { return mSPVisible; }
};

