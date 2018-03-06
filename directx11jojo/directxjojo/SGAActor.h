#pragma once
#include "AStar.h"

class SGAActor
{
public:
	SGAActor();
	SGAActor(SpriteBatch* pBatch, SGASpriteSheet* pSheet, SpriteFont * pFont);
	virtual ~SGAActor();

public:
	virtual void Init(SpriteBatch* pBatch, SGASpriteSheet *pSheet, Animation* anims, int animCount, E_SORTID eSortID);
	virtual void Init(Animation* anims, int animCount, E_SORTID eSortID);
	virtual void Init(Animation* anims, int animCount);
	virtual E_SCENE Update(float dt);
	virtual void Draw();
	virtual void SetAnimation(string name);
	virtual bool IntersecRect(SGAActor* pActor);
	//virtual bool IntersecRectScope(SGAActor* pActor);
	virtual void OnHit(SGAActor* pCollider) {};
	virtual void OnHit(SGAActor* pCollider, SGAActor* pCollidee) {};
protected:
	map<string, vector<FrameInfo>>	mAnimations;
	SGASpriteSheet*	mpSheet;
	SpriteBatch*	mpBatch;
	SpriteFont* mpFont;
	unique_ptr<AStar> mpJoAStar;

protected:
	string						mCurAnim;
	vector<FrameInfo>*			mFrames;
	vector<FrameInfo>::iterator mCurFrame;
	const SpriteFrame*			mpSpriteFrame;
	float						mfElapsedTime;
	float						mfActionElapsedTime;
	E_SCENE			meScene;

protected:
	XMFLOAT2		mPosition;
	XMFLOAT2		mWorldPos;
	E_SORTID		meSortID;
	bool			mbDestroyed;
	bool			mbEnabledCollision;
	int				mCamp;
	float			mfMoveSpeed;	//초당 이동거리
public:
	void SetPosition(XMFLOAT2 pos) { mPosition = pos; }
	void SetPosition(int x, int y) { mPosition.x += static_cast<float>(x); mPosition.y += static_cast<float>(y); }
	XMFLOAT2 GetPosition() { return mPosition; }
	void SetDestroyed() { mbDestroyed = true; }
	bool GetDestroyed() { return mbDestroyed; }
	void SetEnabledCollision(bool flag) { mbEnabledCollision = flag; };
	bool GetEnabledCollision() { return mbEnabledCollision; };
	E_SORTID GetSortID() { return meSortID; }
	void SetSortID(E_SORTID eSortID) { meSortID = eSortID; }
	E_SCENE GetScene() { return meScene; }
	void SetScene(E_SCENE eScene) { meScene = eScene; }
	RECT inline GetBound()
	{
		RECT rct;
		ZeroMemory(&rct, sizeof(RECT));

		if (mpSpriteFrame != NULL)
		{
			rct = mpSpriteFrame->sourceRect;
			OffsetRect(&rct,
				static_cast<int>(mPosition.x - mpSpriteFrame->pivot.x) - rct.left,
				static_cast<int>(mPosition.y - mpSpriteFrame->pivot.y) - rct.top);
		}
		return rct;
	}
	virtual int GetCamp() { return mCamp; }
	virtual void SetCamp(int camp) { mCamp = camp; }
	void SetActionTime(float time) { mfActionElapsedTime = time; }

	float GetActionTime() { return mfActionElapsedTime; }
	int GetTileIndex(const Vector2 vPos);
	bool CollisionMouseToTile(const Vector2 vPos, const TILE* pTileInfo);
};

