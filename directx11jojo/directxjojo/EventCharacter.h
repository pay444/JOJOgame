#pragma once
class EventCharacter : public MActor
{
public:
	EventCharacter();
	EventCharacter(SpriteBatch *pBatch, SpriteSheet *pSheet, SpriteFont * pFont);
	virtual ~EventCharacter();

protected:
	unique_ptr<MFSM> mspFSM;
	XMFLOAT2	mDestPos;		//도착지
	bool mMouseposVible;
	int mHorizonVerticall;		//가로 세로 반대로 그리기 정렬변수
	string mAnimName;			//현재 애니메이션 이름
public:
	virtual void Init(
		float moveSpeed,
		XMFLOAT2 startpos, E_SORTID eSortID);
	virtual E_SCENE Update(float dt);
	virtual void Draw();

public:
	XMFLOAT2 GetDestPos() { return mDestPos; }
	void SetDestPos(XMFLOAT2 pos) { mDestPos = pos; }

	void SetHorizonVerticall(int horizonverticall) { mHorizonVerticall = horizonverticall; }
	int GetHorizonVerticall() {	return mHorizonVerticall;}

	void SetAnimName(string str) { mAnimName = str; }
	string GetAnimName() { return mAnimName; }

	MFSM* GetFSM() { return mspFSM.get(); }
};

