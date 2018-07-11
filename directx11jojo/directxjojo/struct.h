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

typedef struct tagNode
{
	int iIndex;
	float fCost;
	float gCost;
	float hCost;
	tagNode* pParent;
}NODE;

typedef struct TileSet
{
	int width;
	int height;
	int count;
	int columns;
};
typedef struct TILE
{
	Vector2 vPos;
	BYTE byDrawID;
	BYTE byOption;
	unsigned int underObject;
	unsigned int moveNum;
	unsigned int AttackNum;
};

//typedef struct tagNode
//{
//	float fCost;
//	int iIndex;
//	unique_ptr<tagNode> &pParent;
//}NODE;