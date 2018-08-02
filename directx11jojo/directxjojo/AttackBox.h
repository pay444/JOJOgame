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
	int mAttackDistance;//ĳ������ �̵��Ÿ�
	int mAttackDamge;	//�ش� ĳ������ ���ݷ�
	int mCamp;			//�ش� ĳ������ ����
	vector<XMFLOAT2> vecAtScopePos;		//������ �ش��ϴ� ��ġ ����
	vector<int> mVecSkillAtScopeIndex;					//������ �ش��ϴ� ���̿ö�� �ִ� Ÿ�� �ε��� ����
	vector<unique_ptr<int>> mspVecAtScopeIndex;			//������ �ش��ϴ� Ÿ�� �ε���2 �Ȼ����
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

