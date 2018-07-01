#pragma once

class UiSkills : public MActor
{
public:
	UiSkills();
	UiSkills(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont);
	virtual ~UiSkills();
private:
	bool mUISVisible;
	vector<XMFLOAT2> mVecUiSPos;
	Player* mpPlayer;
	int mCountSkill;	//직업에 따른 스킬 개수
	XMFLOAT2 mCancelBtnPos;
public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible);
	virtual E_SCENE Update(float dt);
	virtual void Draw();

public:
	void SetPlayer(Player * player) { mpPlayer = player; }

	void SetVisible(bool visible) { mUISVisible = visible; }
	bool GetVisible() { return mUISVisible; }

	Player * GetPlayer() { return mpPlayer; }
};

