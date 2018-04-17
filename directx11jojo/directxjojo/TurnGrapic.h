#pragma once
class TurnGrapic : public MActor
{
public:
	TurnGrapic();
	TurnGrapic(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont);
	virtual ~TurnGrapic();

private:
	bool mTGVisible;
	bool mEndTIme;
	float mfTEalpsdtime;
public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible);
	virtual E_SCENE Update(float dt);
	virtual void Draw();

	void SetVisible(bool visible) { mTGVisible = visible; }
	bool GetVisible() { return mTGVisible; }

	bool GetEndTime() { return mEndTIme; }
};

