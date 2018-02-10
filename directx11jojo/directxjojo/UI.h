#pragma once
class UI : public SGAActor
{
public:
	UI();
	UI(SpriteBatch * pBatch, SGASpriteSheet * pSheet, SpriteFont * pFont);
	~UI();
private:
	bool mUIVisible;

public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos,bool visible);
	virtual E_SCENE Update(float dt);
	virtual void Draw();

	bool CheckAttackArea();
	void SetVisible(bool visible) { mUIVisible = visible; }
};

