#pragma once
class MainGameMap : public MActor
{
public:
	MainGameMap();
	MainGameMap(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont);
	virtual ~MainGameMap();

private:
	//Sprite
public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible);
	virtual E_SCENE Update(float dt);
	virtual void Draw();
};

