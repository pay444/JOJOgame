#pragma once
class TurnGrapic : public SGAActor
{
public:
	TurnGrapic();
	TurnGrapic(SpriteBatch * pBatch, SGASpriteSheet * pSheet, SpriteFont * pFont);
	virtual ~TurnGrapic();

private:
	bool mTGVisible;
	XMFLOAT2 mTGPos;
public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible);
	virtual E_SCENE Update(float dt);
	virtual void Draw();

	void SetVisible(bool visible) { mTGVisible = visible; }
	bool GetVisible() { return mTGVisible; }
};

