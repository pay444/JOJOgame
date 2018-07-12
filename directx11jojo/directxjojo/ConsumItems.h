#pragma once

class ConsumItems : public MActor
{
public:
	ConsumItems();
	ConsumItems(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont);
	virtual ~ConsumItems();
protected:
	int mStock;				//�������� ���
	int mPlus;				//ü���̳� ������ �󸶳� ������Ű�°��� ���� ����
public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible) = 0;
	virtual E_SCENE Update(float dt) = 0;
	virtual void Draw() = 0;
public:
	void SetStock(int stock) { mStock = stock; }
	int GetStock() { return mStock; }

	int GetPlus() { return mPlus; }
};

