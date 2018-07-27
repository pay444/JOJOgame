#pragma once

class EventCharacter;
class EventTalk :public MActor
{

public:
	EventTalk();
	EventTalk(SpriteBatch* pBatch, SpriteSheet* pSheet, SpriteFont * pFont);
	virtual ~EventTalk();
private:
	bool mbVisible;				//보여주는 여부
	bool mbLeftRight;			//글을오른쪽에 보여줄지 왼쪽에 보여줄지
	wstring mScript;			//대사
	wstring mPickName;			//선택된 캐릭터의 이름
	string mAnimName;			//해당 애니메이션 설정

	EventCharacter*		mpTarget;		//해당녀석의 머리위로 이동후 말풍선 띄움
	//EventCharacter* mpEventChr;		//해당 녀석의 포인터
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

	void SetTarget(EventCharacter* ptarget) { mpTarget = ptarget; }
	EventCharacter* GetTarget() { return mpTarget; }
	void SetInfo(wstring script, wstring pickNmae, bool LR, EventCharacter* ptarget)
	{
		mpTarget = ptarget;
		mScript = script;
		mPickName = pickNmae;
		mbLeftRight = LR;
	}

	//void SetEventChr(EventCharacter* chr) { mpEventChr = chr; }
	//EventCharacter* GetEventChr() { return mpEventChr; }
	//void SetPickName(wstring wstr) { mPickName = wstr; }
	//wstring GetPickName() { return mPickName; }
};

