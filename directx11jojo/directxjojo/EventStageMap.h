#pragma once
class EventStageMap : public MActor
{
public:
	EventStageMap();
	EventStageMap(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont);
	virtual ~EventStageMap();
public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible);
	virtual E_SCENE Update(float dt);
	virtual void Draw();
};

