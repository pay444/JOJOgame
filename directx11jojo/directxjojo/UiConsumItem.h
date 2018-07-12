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
	vector<ANIMINFO> mVecUiConsumInfo;
	vector<unique_ptr<ConsumItems>>	mspVecConsumItems;
	vector<unique_ptr<int>> mspVecAreaIndex;//������ �ش��ϴ� Ÿ�� �ε��� �Ȼ����
	Player* mpPlayer;
	ConsumItems*	mpCurItem;				//������ ��ų�� �ش��ϴ� Ŭ����
	int mCountItemType;						//�������� ���� ����
	XMFLOAT2 mCancelBtnPos;					//��ҹ�ư�� ��ġ
	const SpriteFrame* mpSpriteFrame2;		//�ٸ��̹��� ���տ� �ִ� UI�׸��� �������� ����
	bool mAreaVisible;						//�����ۻ�� ������ �������� ������ ������
	bool mFlag;								//������ ��������� ���� Ŭ���ϸ� �÷��װ� ��
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
public:
	void CalArea(int distance);

};

