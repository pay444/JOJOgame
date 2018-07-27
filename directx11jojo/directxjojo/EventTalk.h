#pragma once

class EventCharacter;
class EventTalk :public MActor
{

public:
	EventTalk();
	EventTalk(SpriteBatch* pBatch, SpriteSheet* pSheet, SpriteFont * pFont);
	virtual ~EventTalk();
private:
	bool mbVisible;				//�����ִ� ����
	bool mbLeftRight;			//���������ʿ� �������� ���ʿ� ��������
	wstring mPickName;			//���õ� ĳ������ �̸�
	string mAnimName;			//�ش� �ִϸ��̼� ����
	//EventCharacter* mpEventChr;		//�ش� �༮�� ������
public:
	virtual void Init(XMFLOAT2 startpos, E_SORTID eSortID);
	virtual E_SCENE Update(float dt);
	virtual void Draw();

public:
	void SetVisible(bool visible) { mbVisible = visible; }
	bool GetVisible() { return mbVisible; }

	void SetLeftRight(bool leftright) { mbLeftRight = leftright; }
	bool GetLeftRight() { return mbLeftRight; }

	void SetPickName(wstring wstr) { mPickName = wstr; }
	wstring GetPickName() { return mPickName; }

	//void SetEventChr(EventCharacter* chr) { mpEventChr = chr; }
	//EventCharacter* GetEventChr() { return mpEventChr; }
	//void SetPickName(wstring wstr) { mPickName = wstr; }
	//wstring GetPickName() { return mPickName; }
};

