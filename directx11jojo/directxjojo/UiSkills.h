#pragma once
struct ANIMINFO
{
	string animName;
	XMFLOAT2 pos;
	ANIMINFO(string name, XMFLOAT2 pos_)
	{
		animName = name;
		pos = pos_;
	}
};
class Skill;
class UiSkills : public MActor
{
public:
	UiSkills();
	UiSkills(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont);
	virtual ~UiSkills();
private:
	bool mUISVisible;
	vector<ANIMINFO> mVecUiSkillInfo;
	vector<unique_ptr<Skill>>	mspVecSkills;
	vector<unique_ptr<int>> mspVecAreaIndex;			//������ �ش��ϴ� Ÿ�� �ε��� �Ȼ����
	Player* mpPlayer;
	Skill*	mpCurSkill;						//������ ��ų�� �ش��ϴ� Ŭ����
	int mCountSkill;						//������ ���� ��ų ����
	XMFLOAT2 mCancelBtnPos;					//��ҹ�ư�� ��ġ
	const SpriteFrame* mpSpriteFrame2;		//�������ִ� UI�׸��� �������� ����
	bool mAreaVisible;						//��ų�� ������ �������� ������ ������
	bool mFlag;								//��ų Ȱ��ȭ�� ���� Ŭ���ϸ� �÷��װ� ��
public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible);
	virtual E_SCENE Update(float dt);
	virtual void Draw();
public:
	void Release();
public:
	void SetPlayer(Player * player) { mpPlayer = player; }
	Player * GetPlayer() { return mpPlayer; }

	void SetVisible(bool visible) { mUISVisible = visible; }
	bool GetVisible() { return mUISVisible; }

	void SetAreaVisible(bool visible) { mAreaVisible = visible; }
	bool GetAreaVisible() { return mAreaVisible; }

	bool GetFlag() { return mFlag; }
public:
	void CalArea(int distance);
};

