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
	vector<unique_ptr<int>> mspVecAreaIndex;			//범위에 해당하는 타일 인덱스 안사라짐
	Player* mpPlayer;
	Skill*	mpCurSkill;						//실행할 스킬에 해당하는 클래스
	int mCountSkill;						//직업에 따른 스킬 개수
	XMFLOAT2 mCancelBtnPos;					//취소버튼의 위치
	const SpriteFrame* mpSpriteFrame2;		//조조에있는 UI그림을 가져오는 변수
	bool mAreaVisible;						//스킬의 범위를 보여줄지 말지를 결정함
	bool mFlag;								//스킬 활성화후 적을 클릭하면 플래그가 섬
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

