#include "stdafx.h"
#include "FotMan.h"


FotMan::FotMan()
{
}

FotMan::FotMan(SpriteBatch * pBatch, SGASpriteSheet * pSheet, SpriteFont * pFont)
	: Enemy(pBatch, pSheet, pFont)
{
}


FotMan::~FotMan()
{
}

void FotMan::Init(float moveSpeed, XMFLOAT2 startpos, E_SORTID eSortID, float limitdis)
{
	Animation anim[] = {
		{ "DOWN", 2,{ { "FotmanBWalk0", 0.3f },
		{ "FotmanBWalk1", 0.3f }, }
		},
		{ "LEFT", 2,{ { "FotmanLWalk0", 0.3f },
		{ "FotmanLWalk1", 0.3f }, }
		},
		{ "RIGHT", 2,{ { "FotmanRWalk0", 0.3f },
		{ "FotmanRWalk1", 0.3f }, }
		},
		{ "UP", 2,{ { "FotmanTWalk0", 0.3f },
		{ "FotmanTWalk1", 0.3f }, }
		},
	};
	SetCamp(GunGeon::CampType::MONSTER);

	SGAActor::Init(anim, 4, eSortID);

	Enemy::Init(moveSpeed, startpos, eSortID);

	//SetSortID(eSortID);
}

E_SCENE FotMan::Update(float dt)
{
	Enemy::Update(dt);

	return E_SCENE_NONPASS;
}
