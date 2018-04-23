#pragma once

//class Character;
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
	Character* mCharacter;
	SpriteFrame mSFHp;
	SpriteFrame mSFMp;
	SpriteFrame mSFSp;
public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible);
	virtual E_SCENE Update(float dt);
	virtual void Draw();

	void SetVisible(bool visible) { mPBVisible = visible; }
	bool GetVisible() { return mPBVisible; }

	bool GetEndTime() { return mEndTIme; }

	void SetCharacter(Character* ch) { mCharacter = ch; }
	Character * GetCharacter() { return mCharacter; }

};

