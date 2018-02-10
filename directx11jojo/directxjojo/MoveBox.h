#pragma once

class MoveBox : public SGAActor
{
public:
	MoveBox();
	MoveBox(SpriteBatch * pBatch, SGASpriteSheet * pSheet, SpriteFont * pFont);
	virtual ~MoveBox();

private:
	float mLimitDis;
	bool mVisible;
	bool mSeekScope;
	int mMoveDistance;//케릭터의 이동거리
	vector<XMFLOAT2> vecScopePos;		//범위에 해당하는 위치 저장
	vector<int> vecScopeIndex;			//범위에 해당하는 타일 인덱스 저장

public:
	virtual void Init(E_SORTID eSortID,XMFLOAT2 pos, float limitDis, bool visible);
	virtual E_SCENE Update(float dt);
	virtual void Draw();
	void TileScope();
	//virtual void OnHit(SGAActor* pCollidee);
		//unique_ptr<MoveBox> mpMoveBox;
	void SetVisible(bool visible) { mVisible = visible; }
	bool GetVisible() {return mVisible; }
	void SetMoveDis(int moveDis) { mMoveDistance = moveDis; }
	vector<XMFLOAT2> GetVecScopePos() { return vecScopePos; }
	bool GetSeekScope() { return mSeekScope; }
	bool ScopeSeek();

};

