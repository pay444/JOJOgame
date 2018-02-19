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
	int mAttackDistance;//캐릭터의 이동거리
	int mAttackDamge;	//해당 캐릭터의 공격력
	int mCamp;			//해당 캐릭터의 진영
	vector<XMFLOAT2> vecAtScopePos;		//범위에 해당하는 위치 저장
	vector<int> vecAtScopeIndex;			//범위에 해당하는 타일 인덱스 저장
	vector<unique_ptr<int>> vecAtScopeIndex2;			//범위에 해당하는 타일 인덱스2 안사라짐

public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible);
	virtual E_SCENE Update(float dt);
	virtual void Draw();
	virtual void OnHit(SGAActor* pCollidee);
	void Release();
	
	bool IntersecRectScope(SGAActor* pActor);
	void AttackScope();
	bool AttackScopeSeek();
	void SetAttackDis(int AttackDis) { mAttackDistance = AttackDis; }
	void SetAttackDamge(int damge) { mAttackDamge = damge; }
	void SetCamp(int camp) { mCamp = camp; }

	int GetAttack() { return mAttackDamge; }
	bool GetAtScopeSeek() { return mAtSeekScope; }
	RECT GetBoundScope();
	vector<unique_ptr<int>>* GetVecAtScopeIndex() { return &vecAtScopeIndex2; }
	vector<XMFLOAT2> GetVecAtScopePos() { return vecAtScopePos; }
	void SetVisible(bool visible) { mAtVisible = visible; }
};

