#pragma once
//#include "MActor.h"
#include "Character.h"
#include "UI.h"

class Player:
	public Character
{
public:
	Player();
	Player(SpriteBatch* pBatch, SpriteSheet *pSheet, SpriteFont * pFont);
	virtual ~Player();
public:
	virtual void Init(
		float moveSpeed,
		XMFLOAT2 startpos, E_SORTID eSortID);
	virtual E_SCENE Update(float dt);
protected:
	XMFLOAT2 mmousePos;
	XMFLOAT2 mvForward;	//나의 정면의 벡터
	int mStartIndex;	//이동하기 시작한 인덱스를 저장해놓음
	int mGoalIndex;		//이동후에 생긴 벽을 다시 초기화해줌
						//UI *mpUi;
	//float mfMoveSpeed;	//초당 이동거리
	//vector<vector<Vector2>> tilepos;
public:
	Vector2 mpCamerapos;
protected:
	virtual void UpdateMove(float dt);
public:

	void SetCamerapos(float x, float y) { mpCamerapos.x = x; mpCamerapos.y = y; }
};

