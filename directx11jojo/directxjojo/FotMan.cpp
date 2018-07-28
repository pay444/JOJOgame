#include "stdafx.h"
#include "FotMan.h"


FotMan::FotMan()
{
}

FotMan::FotMan(SpriteBatch * pBatch, SpriteSheet * pSheet, SpriteFont * pFont)
	: Enemy(pBatch, pSheet, pFont)
{
}


FotMan::~FotMan()
{
}

void FotMan::Init(float moveSpeed, XMFLOAT2 startpos, E_SORTID eSortID, float limitdis)
{
	Animation anim[] = {
		{ "DOWN", 2,{ { "hwangMove_001", 0.3f },
		{ "hwangMove_002", 0.3f }, }
		},
		{ "LEFT", 2,{ { "hwangMove_005", 0.3f },
		{ "hwangMove_006", 0.3f }, }
		},
		{ "RIGHT", 2,{ { "hwangMove_012", 0.3f },
		{ "hwangMove_013", 0.3f }, }
		},
		{ "UP", 2,{ { "hwangMove_003", 0.3f },
		{ "hwangMove_004", 0.3f }, }
		},
		{ "DATTACK", 4,{ { "hwangAtk_001", 0.3f },
		{ "hwangAtk_002", 0.1f },
		{ "hwangAtk_003", 0.1f } ,
		{ "hwangAtk_004", 0.3f } }
		},
		{ "UATTACK", 4,{ { "hwangAtk_005", 0.3f },
		{ "hwangAtk_006", 0.1f },
		{ "hwangAtk_007", 0.1f } ,
		{ "hwangAtk_008", 0.3f } }
		},
		{ "RATTACK", 4,{ { "hwangAtk_013", 0.3f },
		{ "hwangAtk_014", 0.1f },
		{ "hwangAtk_015", 0.1f } ,
		{ "hwangAtk_016", 0.3f } }
		},
		{ "LATTACK", 4,{ { "hwangAtk_009", 0.3f },
		{ "hwangAtk_010", 0.1f },
		{ "hwangAtk_011", 0.1f } ,
		{ "hwangAtk_012", 0.3f } }
		},
		{ "DEAD", 2,{ { "hwangMove_010", 0.3f },
		{ "hwangMove_011", 0.3f },}
		},
		{ "HIT", 1,{ { "hwangSpc_004", 0.5f },
		}
		},
		{ "POWER", 1,{ { "hwangSpc_005", 0.5f },
		}
		},
	};
	//Animation anim[] = {
	//	{ "DOWN", 2,{ { "FotmanBWalk0", 0.3f },
	//{ "FotmanBWalk1", 0.3f }, }
	//	},
	//{ "LEFT", 2,{ { "FotmanLWalk0", 0.3f },
	//{ "FotmanLWalk1", 0.3f }, }
	//},
	//{ "RIGHT", 2,{ { "FotmanRWalk0", 0.3f },
	//{ "FotmanRWalk1", 0.3f }, }
	//},
	//{ "UP", 2,{ { "FotmanTWalk0", 0.3f },
	//{ "FotmanTWalk1", 0.3f }, }
	//},
	//{ "DATTACK", 4,{ { "FotManAttack8", 0.3f },
	//{ "FotManAttack9", 0.1f },
	//{ "FotManAttack10", 0.1f } ,
	//{ "FotManAttack11", 0.3f } }
	//},
	//{ "UATTACK", 4,{ { "FotManAttack0", 0.3f },
	//{ "FotManAttack1", 0.1f },
	//{ "FotManAttack2", 0.1f } ,
	//{ "FotManAttack3", 0.3f } }
	//},
	//{ "RATTACK", 4,{ { "FotManAttack12", 0.3f },
	//{ "FotManAttack13", 0.1f },
	//{ "FotManAttack14", 0.1f } ,
	//{ "FotManAttack15", 0.3f } }
	//},
	//{ "LATTACK", 4,{ { "FotManAttack04", 0.3f },
	//{ "FotManAttack5", 0.1f },
	//{ "FotManAttack6", 0.1f } ,
	//{ "FotManAttack7", 0.3f } }
	//},
	//{ "DEAD", 2,{ { "FotManDead0", 0.3f },
	//{ "FotManDead1", 0.3f }, }
	//},
	//{ "HIT", 1,{ { "FotManHit0", 0.5f },
	//}
	//},
	//{ "POWER", 1,{ { "FotManPower0", 0.5f },
	//}
	//},
	//};
	SetCamp(JoJoGun::CampType::ENEMY);
	
	SetClassType(JoJoGun::ClassType::NOMAL);

	MActor::Init(anim, 11, eSortID);

	Enemy::Init(moveSpeed, startpos, eSortID);

	mName = L"È²°ÇÀû";
	//SetSortID(eSortID);
}

E_SCENE FotMan::Update(float dt)
{
	Enemy::Update(dt);

	return E_SCENE_NONPASS;
}
