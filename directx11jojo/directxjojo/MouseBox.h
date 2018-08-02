#pragma once
#include "MActor.h"
class MouseBox :
	public MActor
{
public:
	MouseBox();
	MouseBox(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont);
	virtual ~MouseBox();
private:
	bool mbSkillAreaVisible;		//스킬범위를 보여줄지 여부
	int	miSkillArea;			//스킬 범위 어떤 형식인지 0십자 1마방진
public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible);
	virtual E_SCENE Update(float dt);
	virtual void Draw();

public:
	void SetSkillAreaVisible(bool visible) { mbSkillAreaVisible = visible; }
	bool GetSkillAreaVisible() { return mbSkillAreaVisible; }

	void SetSkillArea(int area) { miSkillArea = area; }
	int GetSkillArea() { return miSkillArea; }
};

