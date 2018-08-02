#pragma once
#include "AStar.h"

class MActor;

class iActorDelegate
{
public:
	virtual void OnAction(MActor* pSender) = 0;
};

class MActor
{
public:
	MActor();
	MActor(SpriteBatch* pBatch, SpriteSheet* pSheet, SpriteFont * pFont);
	virtual ~MActor();

public:
	virtual void Init(SpriteBatch* pBatch, SpriteSheet *pSheet, Animation* anims, int animCount, E_SORTID eSortID);
	virtual void Init(Animation* anims, int animCount, E_SORTID eSortID);
	virtual void Init(Animation* anims, int animCount);
	virtual E_SCENE Update(float dt);
	virtual void Draw();
	virtual void SetAnimation(string name);
	virtual bool IntersecRect(MActor* pActor);
	//virtual bool IntersecRectScope(MActor* pActor);
	virtual void OnHit(MActor* pCollider) {};
	virtual void OnHit(MActor* pCollider, MActor* pCollidee,float delayTime = 0.8f) {};
protected:
	map<string, vector<FrameInfo>>	mAnimations;
	SpriteSheet*	mpSheet;
	SpriteBatch*	mpBatch;
	SpriteFont* mpFont;
	unique_ptr<AStar> mpJoAStar;
	iActorDelegate* mpDelegate;
protected:
	string						mCurAnim;
	vector<FrameInfo>*			mFrames;
	vector<FrameInfo>::iterator mCurFrame;
	const SpriteFrame*			mpSpriteFrame;
	float						mfElapsedTime;
	float						mfActionElapsedTime;
	E_SCENE			meScene;
	wstring			mName;			//클래스의 이름
protected:
	XMFLOAT2		mPosition;
	XMFLOAT2		mWorldPos;
	E_SORTID		meSortID;
	bool			mbDestroyed;
	bool			mbEnabledCollision;
	float			mfMoveSpeed;	//초당 이동거리
	bool			mbisEndAnim;	//마지막 에니메이션에 와있는지
public:
	void SetName(wstring wstr) { mName = wstr; }
	wstring GetName() { return mName; }

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

	void SetActionTime(float time) { mfActionElapsedTime = time; }
	float GetActionTime() { return mfActionElapsedTime; }

	int GetTileIndex(const Vector2 vPos);

	void SetDelegate(iActorDelegate* delegate) { mpDelegate = delegate; }
	iActorDelegate* GetDelegate() { return mpDelegate; }

	void SetisEndAnim(bool end) { mbisEndAnim = end; }
	bool GetisEndAnim() {	return mbisEndAnim;	}

	vector<FrameInfo>*			GetFrames() { return mFrames; }
	vector<FrameInfo>::iterator GetCurFrame() { return mCurFrame; }
	map<string, vector<FrameInfo>> GetAnimaions() { return mAnimations; }
	string	GetCurAnim() { return mCurAnim; }
	float GetMoveSpeed() { return mfMoveSpeed; }

public:
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

	RECT inline GetBound(const SpriteFrame* spriteFrame)
	{
		RECT rct;
		ZeroMemory(&rct, sizeof(RECT));

		if (spriteFrame != NULL)
		{
			rct = spriteFrame->sourceRect;
			OffsetRect(&rct,
				static_cast<int>(mPosition.x - spriteFrame->pivot.x) - rct.left,
				static_cast<int>(mPosition.y - spriteFrame->pivot.y) - rct.top);
		}
		return rct;
	}
	bool CollisionMouseToTile(const Vector2 vPos, const TILE* pTileInfo);
};

