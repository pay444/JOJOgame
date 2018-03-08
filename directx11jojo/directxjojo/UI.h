#pragma once
class UI : public SGAActor
{
public:
	UI();
	UI(SpriteBatch * pBatch, SGASpriteSheet * pSheet, SpriteFont * pFont);
	~UI();
private:
	bool mUIVisible;
	vector<XMFLOAT2> mVecUiPos;
	Player* mpPlayer;
public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible);
	virtual E_SCENE Update(float dt);
	virtual void Draw();

	bool CheckAttackArea();
	bool CheckWaitArea();

	void SetPlayer(Player * player) { mpPlayer = player; }
	void SetVisible(bool visible) { mUIVisible = visible; }

	bool GetVisible() { return mUIVisible; }
	Player * GetPlayer() { return mpPlayer; }
};

