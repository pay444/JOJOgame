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
	int mAttackDistance;//ĳ������ �̵��Ÿ�
	int mAttackDamge;	//�ش� ĳ������ ���ݷ�
	int mCamp;			//�ش� ĳ������ ����
	vector<XMFLOAT2> vecAtScopePos;		//������ �ش��ϴ� ��ġ ����
	vector<int> mVecAtScopeIndex;			//������ �ش��ϴ� Ÿ�� �ε��� ����
	vector<unique_ptr<int>> mspVecAtScopeIndex;			//������ �ش��ϴ� Ÿ�� �ε���2 �Ȼ����
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
	void AttackCubeScope(bool isChracterPospush);
	bool AttackScopeSeek();
	bool AttackScopeSeekPick(XMFLOAT2 pos);
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
	
};

