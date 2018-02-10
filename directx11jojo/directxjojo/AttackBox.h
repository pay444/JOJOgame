#pragma once
class AttackBox : public SGAActor
{
public:
	AttackBox();
	AttackBox(SpriteBatch * pBatch, SGASpriteSheet * pSheet, SpriteFont * pFont);
	~AttackBox();
private:
	bool mAtVisible;
	bool mAtSeekScope;
	int mAttackDistance;//케릭터의 이동거리
	vector<XMFLOAT2> vecAtScopePos;		//범위에 해당하는 위치 저장
	vector<int> vecAtScopeIndex;			//범위에 해당하는 타일 인덱스 저장
	vector<int> vecAtScopeIndex2;			//범위에 해당하는 타일 인덱스2 안사라짐
public:
public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible);
	virtual E_SCENE Update(float dt);
	virtual void Draw();

	void AttackScope();
	bool AttackScopeSeek();
	void SetAttackDis(int AttackDis) { mAttackDistance = AttackDis; }
	vector<int> GetVecAtScopeIndex() { return vecAtScopeIndex; }
	vector<XMFLOAT2> GetVecAtScopePos() { return vecAtScopePos; }
	void SetVisible(bool visible) { mAtVisible = visible; }
};

