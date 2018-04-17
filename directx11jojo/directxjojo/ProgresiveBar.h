#pragma once
class ProgresiveBar :public MActor
{
public:
	ProgresiveBar();
	ProgresiveBar(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont);
	virtual ~ProgresiveBar();

private:
	bool mPBVisible;
	bool mEndTIme;
	float mfTEalpsdtime;
	XMFLOAT2 mTGPos;
public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible);
	virtual E_SCENE Update(float dt);
	virtual void Draw();

	void SetVisible(bool visible) { mPBVisible = visible; }
	bool GetVisible() { return mPBVisible; }

	bool GetEndTime() { return mEndTIme; }
};

