#pragma once
class ConsumItems;

class UiConsumItem :public MActor
{
public:
	UiConsumItem();
	UiConsumItem(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont);
	virtual ~UiConsumItem();

private:
	bool mUICVisible;
	vector<ANIMINFO> mVecUiConsumInfo;		//Ui안의 상자들 위치조정변수
	vector<unique_ptr<ConsumItems>>	mspVecConsumItems;		//아이템클래스가들어있는 벡터
	vector<unique_ptr<int>> mspVecAreaIndex;//범위에 해당하는 타일 인덱스 안사라짐
	Player* mpPlayer;
	ConsumItems*	mpCurItem;				//실행할 스킬에 해당하는 클래스
	int mCountItemType;						//아이템의 종류 개수
	XMFLOAT2 mCancelBtnPos;					//취소버튼의 위치
	const SpriteFrame* mpSpriteFrame2;		//다른이미지 집합에 있는 UI그림을 가져오는 변수
	bool mAreaVisible;						//아이템사용 범위를 보여줄지 말지를 결정함
	bool mFlag;								//아이템 범위출력후 적을 클릭하면 플래그가 섬
public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible);
	virtual E_SCENE Update(float dt);
	virtual void Draw();
public:
	void Release();
public:
	void SetPlayer(Player * player) { mpPlayer = player; }
	Player * GetPlayer() { return mpPlayer; }

	void SetVisible(bool visible) { mUICVisible = visible; }
	bool GetVisible() { return mUICVisible; }

	void SetAreaVisible(bool visible) { mAreaVisible = visible; }
	bool GetAreaVisible() { return mAreaVisible; }

	void SetCountItemType(int count) { mCountItemType = count; }
	int GetCountItemType() { return mCountItemType;	}

	bool GetFlag() { return mFlag; }
	vector<ANIMINFO>* GetVecAnimInfo() { return &mVecUiConsumInfo; }
	vector<unique_ptr<ConsumItems>>* GetVecClassItem() { return &mspVecConsumItems; }
public:
	void CalArea();

};

