#include "stdafx.h"
#include "Jojo.h"


Jojo::Jojo()
{
}

Jojo::Jojo(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont)
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
		{ "DATTACK", 4,{ { "jojoAttack0", 0.3f },
		{ "jojoAttack1", 0.1f },
		{ "jojoAttack2", 0.1f } ,
		{ "jojoAttack3", 0.3f } }
		},
		{ "UATTACK", 4,{ { "jojoAttack4", 0.3f },
		{ "jojoAttack5", 0.1f },
		{ "jojoAttack6", 0.1f } ,
		{ "jojoAttack7", 0.3f } } 
		},
		{ "RATTACK", 4,{ { "jojoAttack8", 0.3f },
		{ "jojoAttack9", 0.1f },
		{ "jojoAttack10", 0.1f } ,
		{ "jojoAttack11", 0.3f } }
		},
		{ "LATTACK", 4,{ { "jojoAttack12", 0.3f },
		{ "jojoAttack13", 0.1f },
		{ "jojoAttack14", 0.1f } ,
		{ "jojoAttack15", 0.3f } }
		},
		{ "DEAD", 2,{ { "jojoDead0", 0.3f },
		{ "jojoDead1", 0.3f }, }
		},
		{ "HIT", 1,{ { "jojoHit0", 0.5f },
		}
		},
		{ "POWER", 1,{ { "jojoPower0", 0.5f },
		}
		},
	};
	limitDistance = limitdis;
	mName = L"Á¶Á¶";
	SetCamp(GunGeon::CampType::PLAYER);
	SetClassType(GunGeon::ClassType::MONARCH);
	MActor::Init(anim, 11, eSortID);

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
