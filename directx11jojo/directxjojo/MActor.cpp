#include "stdafx.h"
//#include "MActor.h"


MActor::MActor() : 
	mpSheet(NULL),
	mFrames(NULL),
	mpSpriteFrame(NULL),
	mfElapsedTime(0.0f),
	mfActionElapsedTime(0.0f),
	mPosition(0.0f, 0.0f),
	mWorldPos(0.0f,0.0f),
	mbDestroyed(false),
	mbEnabledCollision(true),
	meSortID(E_SORTID_FOURTH),
	meScene(E_SCENE_NONPASS),
	mbisEndAnim(false)
{
	mName = L"0";
	mAnimations.clear();
}

MActor::MActor(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont)
	: MActor() // c++11 이후로 기본 생성자를 불러올수있음
{
	mpSheet = pSheet;
	mpBatch = pBatch;
	mpFont = pFont;
}


MActor::~MActor()
{
	//int x = 0;
}

void MActor::Init(SpriteBatch* pBatch, SpriteSheet *pSheet, Animation *anims, int animCount,E_SORTID eSortID)
{
	mpSheet = pSheet;
	mpBatch = pBatch;

	Init(anims, animCount, eSortID); //연관이 되는 부분등은 빼서 함수로써 사용해야함
}

void MActor::Init(Animation * anims, int animCount,E_SORTID eSortID)
{
	for (int i = 0; i < animCount; ++i)
	{
		vector<FrameInfo> frames;
		for (int j = 0; j < anims[i].TotalFrames; ++j)
		{
			FrameInfo frame;
			frame.DelayPerFrame = anims[i].FrameInfos[j].DelayPerFrame;
			frame.FrameName = anims[i].FrameInfos[j].FrameName;
			frames.push_back(frame);
		}

		mAnimations.insert(pair<string, vector<FrameInfo>>(anims[i].Name, frames));
	}
	meSortID = eSortID;
}

void MActor::Init(Animation * anims, int animCount)
{
	for (int i = 0; i < animCount; ++i)
	{
		vector<FrameInfo> frames;
		for (int j = 0; j < anims[i].TotalFrames; ++j)
		{
			FrameInfo frame;
			frame.DelayPerFrame = anims[i].FrameInfos[j].DelayPerFrame;
			frame.FrameName = anims[i].FrameInfos[j].FrameName;
			frames.push_back(frame);
		}

		mAnimations.insert(pair<string, vector<FrameInfo>>(anims[i].Name, frames));
	}
}

E_SCENE MActor::Update(float dt)
{
	if (mpSheet == NULL)
		return E_SCENE_NONPASS;

	mfElapsedTime += dt;
	//mfActionElapsedTime += dt;

	if (mFrames == NULL)
		return E_SCENE_NONPASS;

	if (mCurFrame->DelayPerFrame != 0.0f 
		&& mfElapsedTime > mCurFrame->DelayPerFrame)
	{
		mCurFrame++;
		mfElapsedTime = 0.0f;
		
		if (mCurFrame == mFrames->cend())
		{
			mCurFrame = mFrames->begin();
			mbisEndAnim = false;
		}
		else if (mCurFrame == mFrames->cend() - 1)
		{
			mbisEndAnim = true;
		}
		else
		{
			mbisEndAnim = false;
		}

		mpSpriteFrame = mpSheet->Find(mCurFrame->FrameName.c_str());
	}

	return meScene;
}

void MActor::Draw()
{
	if (mpBatch == NULL || mpSheet == NULL)
		return;

	if (mpSpriteFrame == NULL)
		return;

	mpSheet->Draw(mpBatch, *mpSpriteFrame, mPosition);
}

void MActor::SetAnimation(string name)
{
	if (mCurAnim == name)
		return;

	mCurAnim = name;

	auto result = mAnimations.find(name);

	if (result == mAnimations.cend())
	{
		OutputDebugString(L"Can not find animation!");
		return;
	}

	mFrames = &result->second;
	mCurFrame = result->second.begin();
	mfElapsedTime = 0.0f;
	mpSpriteFrame = mpSheet->Find(mCurFrame->FrameName.c_str());
}

bool MActor::IntersecRect(MActor* pActor)
{
	RECT src = GetBound();
	RECT trg = pActor->GetBound();
	RECT intersect;
	return	::IntersectRect(&intersect, &src, &trg);
}

int MActor::GetTileIndex(const Vector2 vPos)
{
	const vector<unique_ptr<TILE>>* spVecTIle =
		MActorManager::Instance().GetTileInfo();

	for (UINT i = 0; i < spVecTIle->size(); ++i)
	{
		if (CollisionMouseToTile(vPos, (*spVecTIle)[i].get()))
			return i;
	}
	return -1;
}

bool MActor::CollisionMouseToTile(const Vector2 vPos, const TILE * pTileInfo)
{
	Vector2 vPoint[4];
	vPoint[0] = Vector2(
		pTileInfo->vPos.x,
		pTileInfo->vPos.y);	// 왼쪽위점
	vPoint[1] = Vector2(
		pTileInfo->vPos.x + JOJOTILESX,
		pTileInfo->vPos.y);	// 오른쪽 위점
	vPoint[2] = Vector2(
		pTileInfo->vPos.x,
		pTileInfo->vPos.y + JOJOTILESY);	// 왼쪽 아래점
	vPoint[3] = Vector2(
		pTileInfo->vPos.x + JOJOTILESX,
		pTileInfo->vPos.y + JOJOTILESY);	// 오른쪽 아래점

	if ((vPos.x >= vPoint[0].x && vPos.y >= vPoint[0].y) && // 왼쪽 위점,
		(vPos.x <= vPoint[1].x && vPos.y >= vPoint[1].y) && // 오른쪽 위점,
		(vPos.x >= vPoint[2].x && vPos.y <= vPoint[2].y) &&// 왼쪽 아래점
		(vPos.x <= vPoint[3].x && vPos.y <= vPoint[3].y))  // 오른쪽 아래점
	{
		return true;
	}
	return false;
}
