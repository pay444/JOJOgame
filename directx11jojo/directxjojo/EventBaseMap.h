#pragma once
class EventBaseMap : public MActor
{
public:
	EventBaseMap();
	EventBaseMap(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont);
	virtual ~EventBaseMap();

public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible);
	virtual E_SCENE Update(float dt);
	virtual void Draw();
};

