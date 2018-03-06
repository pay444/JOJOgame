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
		{ "DATTACK", 4,{ { "jojoAttack0", 0.1f },
		{ "jojoAttack1", 0.1f },
		{ "jojoAttack2", 0.1f } ,
		{ "jojoAttack3", 0.1f } }
		},
		{ "UATTACK", 4,{ { "jojoAttack4", 0.1f },
		{ "jojoAttack5", 0.1f },
		{ "jojoAttack6", 0.1f } ,
		{ "jojoAttack7", 0.1f } } 
		},
		{ "RATTACK", 4,{ { "jojoAttack8", 0.1f },
		{ "jojoAttack9", 0.1f },
		{ "jojoAttack10", 0.1f } ,
		{ "jojoAttack11", 0.1f } }
		},
		{ "LATTACK", 4,{ { "jojoAttack12", 0.1f },
		{ "jojoAttack13", 0.1f },
		{ "jojoAttack14", 0.1f } ,
		{ "jojoAttack15", 0.1f } }
		},

	};
	limitDistance = limitdis;

	SetCamp(GunGeon::CampType::PLAYER);

	SGAActor::Init(anim, 8, eSortID);

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
