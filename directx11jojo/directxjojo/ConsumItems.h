#pragma once

class ConsumItems : public MActor
{
public:
	ConsumItems();
	ConsumItems(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont);
	virtual ~ConsumItems();
protected:
	int mStock;				//아이템의 재고
	int mPlus;				//체력이나 마나를 얼마나 증가시키는가에 대한 변수
	wstring mEfficacy;		//효과 이름 출력을 위한변수
public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible) = 0;
	virtual E_SCENE Update(float dt) = 0;
	virtual void Draw() = 0;
public:
	void SetStock(int stock) { mStock = stock; }
	int GetStock() { return mStock; }

	wstring GetEfficacyName() { return mEfficacy; }
	int GetPlus() { return mPlus; }
};

