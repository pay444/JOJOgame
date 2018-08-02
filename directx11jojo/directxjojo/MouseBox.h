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
	bool mbSkillAreaVisible;		//��ų������ �������� ����
	int	miSkillArea;			//��ų ���� � �������� 0���� 1������
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

