#pragma once
class EventCharacter : public MActor
{
public:
	EventCharacter();
	EventCharacter(SpriteBatch *pBatch, SpriteSheet *pSheet, SpriteFont * pFont);
	virtual ~EventCharacter();

protected:
	unique_ptr<MFSM> mspFSM;
	XMFLOAT2	mDestPos;		//µµÂøÁö
public:
	virtual void Init(
		float moveSpeed,
		XMFLOAT2 startpos, E_SORTID eSortID);
	virtual E_SCENE Update(float dt);
	virtual void Draw();

public:
	XMFLOAT2 GetDestPos() { return mDestPos; }
	void SetDestPos(XMFLOAT2 pos) { mDestPos = pos; }
};

