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
	int mMoveDistance;//�ɸ����� �̵��Ÿ�
	vector<XMFLOAT2> vecScopePos;		//������ �ش��ϴ� ��ġ ����
	vector<int> vecScopeIndex;			//������ �ش��ϴ� Ÿ�� �ε��� ����

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

