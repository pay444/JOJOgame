#pragma once

class ConsumItems : public MActor
{
public:
	ConsumItems();
	ConsumItems(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont);
	virtual ~ConsumItems();
protected:
	int mCountItem;							//�������� ���
public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible) = 0;
	virtual E_SCENE Update(float dt) = 0;
	virtual void Draw() = 0;
public:
	void SetCountItem(int count) { mCountItem = count; }
	int GetCountItem() { return mCountItem; }
};

