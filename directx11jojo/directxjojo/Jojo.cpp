#include "stdafx.h"
#include "Jojo.h"


Jojo::Jojo()
{
}

Jojo::Jojo(SpriteBatch * pBatch, SGASpriteSheet * pSheet, SpriteFont * pFont)
	:Player(pBatch, pSheet, pFont)
{
}


Jojo::~Jojo()
{
}

void Jojo::Init(float moveSpeed, XMFLOAT2 startpos, E_SORTID eSortID, float limitdis)
{
	Animation anim[] = {
		{ "DOWN", 2,{ { "jojoBWalk0", 0.3f },
		{ "jojoBWalk1", 0.3f }, }
		},
		{ "LEFT", 2,{ { "jojoLWalk0", 0.3f },
		{ "jojoLWalk1", 0.3f }, }
		},
		{ "RIGHT", 2,{ { "jojoRWalk0", 0.3f },
		{ "jojoRWalk1", 0.3f }, }
		},
		{ "UP", 2,{ { "jojoTWalk0", 0.3f },
		{ "jojoTWalk1", 0.3f }, }
		},
	};
	limitDistance = limitdis;

	SetCamp(GunGeon::CampType::PLAYER);

	SGAActor::Init(anim, 4, eSortID);

	Player::Init(moveSpeed, startpos, eSortID);

	//SetSortID(eSortID);
}

E_SCENE Jojo::Update(float dt)
{	
	Player::Update(dt);

	//auto state = Keyboard::Get().GetState();

	//if (state.D2)
	//	meScene = E_SCENE_LOGO;
	//else if (state.D3)
	//	meScene = E_SCENE_STAGE;
	//else
	//	meScene = E_SCENE_NONPASS;

	//return meScene;
	return E_SCENE_NONPASS;
}
