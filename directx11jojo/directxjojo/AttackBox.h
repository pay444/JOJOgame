#pragma once
class AttackBox : public MActor
{
public:
	AttackBox();
	AttackBox(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont);
	~AttackBox();
private:
	bool mAtVisible;
	bool mAtSeekScope;
	int mAttackDistance;//캐릭터의 이동거리
	int mAttackDamge;	//해당 캐릭터의 공격력
	int mCamp;			//해당 캐릭터의 진영
	vector<XMFLOAT2> vecAtScopePos;		//범위에 해당하는 위치 저장
	vector<int> mVecAtScopeIndex;			//범위에 해당하는 타일 인덱스 저장
	vector<unique_ptr<int>> mspVecAtScopeIndex;			//범위에 해당하는 타일 인덱스2 안사라짐
	Character* mpCharacter;
public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible);
	virtual E_SCENE Update(float dt);
	virtual void Draw();
	virtual void OnHit(MActor* pCollidee);
	void Release();

	bool UIntersecRectScope(MActor* pActor);
	bool AIIntersecRectScope(MActor* pActor);
	void AttackScope();
	bool AttackScopeSeek();
	bool AttackScopeSeekPick(XMFLOAT2 pos);
	void SetAttackDis(int AttackDis) { mAttackDistance = AttackDis; }
	void SetAttackDamge(int damge) { mAttackDamge = damge; }
	void SetCamp(int camp) { mCamp = camp; }
	void SetCharacter(Character * character) { mpCharacter = character; }
	void SetVisible(bool visible) { mAtVisible = visible; }

	bool GetVisible() { return mAtVisible; }
	Character* GetCharacter() { return mpCharacter; }
	int GetAttack() { return mAttackDamge; }
	bool GetAtScopeSeek() { return mAtSeekScope; }
	RECT GetBoundScope();
	RECT GetAtBoundScope(int i);
	vector<unique_ptr<int>>* GetVecAtScopeIndex() { return &mspVecAtScopeIndex; }
	vector<XMFLOAT2> GetVecAtScopePos() { return vecAtScopePos; }
	
};

