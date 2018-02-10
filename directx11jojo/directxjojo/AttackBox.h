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
	int mAttackDistance;//�ɸ����� �̵��Ÿ�
	vector<XMFLOAT2> vecAtScopePos;		//������ �ش��ϴ� ��ġ ����
	vector<int> vecAtScopeIndex;			//������ �ش��ϴ� Ÿ�� �ε��� ����
	vector<int> vecAtScopeIndex2;			//������ �ش��ϴ� Ÿ�� �ε���2 �Ȼ����
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

