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
	bool mMultiSkillAtVisible;
	int mAttackDistance;//캐릭터의 이동거리
	int mAttackDamge;	//해당 캐릭터의 공격력
	int mCamp;			//해당 캐릭터의 진영
	vector<XMFLOAT2> vecAtScopePos;		//범위에 해당하는 위치 저장
	vector<int> mVecSkillAtScopeIndex;					//범위에 해당하는 적이올라와 있는 타일 인덱스 저장
	vector<unique_ptr<int>> mspVecAtScopeIndex;			//범위에 해당하는 타일 인덱스2 안사라짐
	Character* mpCharacter;
public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible);
	virtual E_SCENE Update(float dt);
	virtual void Draw();
	virtual void OnHit(MActor* pCollidee);
	void Release();
public:
	bool UIntersecRectScope(MActor* pActor);
	bool AIIntersecRectScope(MActor* pActor);
	void AttackScope();
	void AttackCubeScope(bool isChracterPospush, vector<unique_ptr<int>> &vecAtScopeIndex);
	bool SkillScope(vector<int> &vecAtSkillScopeIndex,int activeArea,int camp);
	bool AttackScopeSeek();
	bool AttackScopeSeekPick(XMFLOAT2 pos);
public:
	void SetSkillMultiVisible(bool visible) { mMultiSkillAtVisible = visible; }
	bool GetSkillMultiVisible() { return mMultiSkillAtVisible; }
	
	void SetAttackDis(int AttackDis) { mAttackDistance = AttackDis; }
	void SetAttackDamge(int damge) { mAttackDamge = damge; }
	void SetCamp(int camp) { mCamp = camp; }
	void SetCharacter(Character * character) { mpCharacter = character; }
	void SetVisible(bool visible) { mAtVisible = visible; }

	vector<unique_ptr<int>>* GetVecAtScopeIndex() { return &mspVecAtScopeIndex; }

	bool GetVisible() { return mAtVisible; }
	Character* GetCharacter() { return mpCharacter; }
	int GetAttack() { return mAttackDamge; }
	bool GetAtScopeSeek() { return mAtSeekScope; }
	RECT GetBoundScope();
	RECT GetAtBoundScope(int i);
	vector<XMFLOAT2> GetVecAtScopePos() { return vecAtScopePos; }
	vector<int>* GetVecSkillAreaIndex() { return &mVecSkillAtScopeIndex; }
	
};

