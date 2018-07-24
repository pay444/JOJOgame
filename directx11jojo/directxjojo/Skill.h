#pragma once
class Skill : public MActor
{
public:
	Skill();
	Skill(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont);
	virtual ~Skill();
protected:
	int mElemental;		//스킬의 속성
	int mMpCost;		//이 스킬을 쓰는데 드는 비용
	int mArea;			//이 스킬의 범위
	wstring mEfficacy;	//효과 이름 출력을 위한 변수
public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible) = 0;
	virtual E_SCENE Update(float dt) = 0;
	virtual void Draw() = 0;
public:
	wstring GetEfficacyName() { return mEfficacy; }
	int GetElemental() { return mElemental; }
	int GetMpCost() { return mMpCost; }
	int GetArea() { return mArea; }
};

