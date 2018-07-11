#pragma once
class Skill : public MActor
{
public:
	Skill();
	Skill(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont);
	virtual ~Skill();
protected:
	int mElemental;		//��ų�� �Ӽ�
	int mMpCost;		//�� ��ų�� ���µ� ��� ���
	int mArea;			//�� ��ų�� ����

public:
	virtual void Init(E_SORTID eSortID, XMFLOAT2 pos, bool visible) = 0;
	virtual E_SCENE Update(float dt) = 0;
	virtual void Draw() = 0;
public:
	int GetElemental() { return mElemental; }
	int GetMpCost() { return mMpCost; }
	int GetArea() { return mArea; }
};
