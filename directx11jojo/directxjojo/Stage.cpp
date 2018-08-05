#include "stdafx.h"
#include "Stage.h"
#include "MouseBox.h"

Stage::Stage():
	mScreenWidth(DEFAULT_SCREEN_WIDTH),
	mScreenHeight(DEFAULT_SCREEN_HEIGHT)
{
}


Stage::~Stage()
{
	Release();
}

HRESULT Stage::Initialize(SpriteBatch *spriteBatch, SpriteFont * spriteFont)
{
	RECT rc;
	GetWindowRect(SceneMgr::Instance().GetHWND(), &rc);

	MoveWindow(SceneMgr::Instance().GetHWND()
		, rc.left, rc.top, DEFAULT_SCREEN_WIDTH+15, DEFAULT_SCREEN_HEIGHT, true);

	mpWorld = MActorManager::Instance().Create<World>(spriteBatch, spriteFont, mScreenWidth, mScreenHeight);
	mpWorld->Init("Images\\Map\\stage3.tmx", E_SORTID_FOURTH);

	auto pTexture = ResourceManager::Instance().GetShaderResource(L"Images\\jojo\\jojoSprites.png");//(L"Images\\sprites.png");
	auto pSheet = ResourceManager::Instance().GetSpriteSheet(L"Images\\jojo\\jojoSprites.xml", pTexture); //(L"Images\\sprites.xml",pTexture);


	mpUi = MActorManager::Instance().Create<UI>(spriteBatch, pSheet, spriteFont);
	mpUi->Init(E_SORTID_FIRST, XMFLOAT2((mScreenWidth * 0.5f) + 100, mScreenHeight * 0.5f), false);

	auto pMTexture = ResourceManager::Instance().GetShaderResource(L"Images\\Mouse\\Mouses.png");//(L"Images\\sprites.png");
	auto pMSheet = ResourceManager::Instance().GetSpriteSheet(L"Images\\Mouse\\Mouses.xml", pMTexture); //(L"Images\\sprites.xml",pTexture);

	mpMouseBox = MActorManager::Instance().Create<MouseBox>(spriteBatch, pMSheet, spriteFont);
	mpMouseBox->Init(E_SORTID_FIRST, XMFLOAT2(0, 0), false);

	mpAttackBox = MActorManager::Instance().Create<AttackBox>(spriteBatch, pSheet, spriteFont);
	mpAttackBox->Init(E_SORTID_THIRD, XMFLOAT2((mScreenWidth * 0.5f) + 100, mScreenHeight * 0.5f), false);

	mpMoveBox = MActorManager::Instance().Create<MoveBox>(spriteBatch, pSheet, spriteFont);
	mpMoveBox->Init(E_SORTID_THIRD, XMFLOAT2((mScreenWidth * 0.5f) + 100, mScreenHeight * 0.5f),
		200.0f, false);

	mpTurnGrapic = MActorManager::Instance().Create<TurnGrapic>(spriteBatch, pSheet, spriteFont);
	mpTurnGrapic->Init(E_SORTID_FIRST, XMFLOAT2(mScreenWidth * 0.5f,
		mScreenHeight * 0.5f), false);

	mpProgresiveBar = MActorManager::Instance().Create<ProgresiveBar>(spriteBatch, pSheet, spriteFont);
	mpProgresiveBar->Init(E_SORTID_FIRST, XMFLOAT2(mScreenWidth * 0.5f,
		mScreenHeight * 0.5f), false);
	

	//AttackBox::Instance().Init(spriteBatch, pSheet, spriteFont, E_SORTID_THIRD, 
	//	XMFLOAT2((mScreenWidth * 0.5f) + 100, mScreenHeight * 0.5f), false);

	mpJojo = MActorManager::Instance().Create<Jojo>(spriteBatch, pSheet, spriteFont);
	mpJojo->Init(
		500.0f, XMFLOAT2((mScreenWidth * 0.5f) + 100, mScreenHeight * 0.5f),
		E_SORTID_SECOND, 200.0f);
	mpJojo->SetStaus(101, 10, 100, 255, 12, 3);
	JoJoGun::Blackboard::Instance().SetPlayer(mpJojo);

	//Jojo *m2pPlayer;
	//m2pPlayer = MActorManager::Instance().Create<Jojo>(spriteBatch, pSheet, spriteFont);
	//m2pPlayer->Init(500.0f, XMFLOAT2((145.0f), 145.0f), E_SORTID_FIRST, 200.0f);
	//m2pPlayer->SetStaus(101, 10, 100, 255, 12, 2);

	//m2pPlayer = MActorManager::Instance().Create<Jojo>(spriteBatch, pSheet, spriteFont);
	//m2pPlayer->Init(500.0f, XMFLOAT2((193.0f), 145.0f), E_SORTID_FIRST, 200.0f);
	//m2pPlayer->SetStaus(101, 10, 100, 255, 12, 2);

	//m2pPlayer = MActorManager::Instance().Create<Jojo>(spriteBatch, pSheet, spriteFont);
	//m2pPlayer->Init(500.0f, XMFLOAT2((241.0f), 145.0f), E_SORTID_FIRST, 200.0f);
	//m2pPlayer->SetStaus(101, 10, 100, 255, 12, 3);

	auto pAllPexture = ResourceManager::Instance().GetShaderResource(L"Images\\\Player\\Players.png");
	auto pAllPSheet = ResourceManager::Instance().GetSpriteSheet(L"Images\\\Player\\Players.xml", pAllPexture);

	//관우추가
	mpPlayer = MActorManager::Instance().Create<Player>(spriteBatch, pAllPSheet, spriteFont);
	Animation Ganim[] = {
		{ "DOWN", 2,{ 
	{ "gwanuMove_001", 0.3f },
	{ "gwanuMove_002", 0.3f }, }
		},
	{ "LEFT", 2,{ 
	{ "gwanuMove_005", 0.3f },
	{ "gwanuMove_006", 0.3f }, }
	},
	{ "RIGHT", 2,{ 
	{ "gwanuMove_012", 0.3f },
	{ "gwanuMove_013", 0.3f }, }
	},
	{ "UP", 2,{ 
	{ "gwanuMove_003", 0.3f },
	{ "gwanuMove_004", 0.3f }, }
	},
	{ "DATTACK", 4,{ 
	{ "gwanuAtk_001", 0.3f },
	{ "gwanuAtk_002", 0.1f },
	{ "gwanuAtk_003", 0.1f } ,
	{ "gwanuAtk_004", 0.3f } }
	},
	{ "UATTACK", 4,{ 
	{ "gwanuAtk_005", 0.3f },
	{ "gwanuAtk_006", 0.1f },
	{ "gwanuAtk_007", 0.1f } ,
	{ "gwanuAtk_008", 0.3f } }
	},
	{ "RATTACK", 4,{
	{ "gwanuAtk_013", 0.3f },
	{ "gwanuAtk_014", 0.1f },
	{ "gwanuAtk_015", 0.1f } ,
	{ "gwanuAtk_016", 0.3f } }
	},
	{ "LATTACK", 4,{
	{ "gwanuAtk_009", 0.3f },
	{ "gwanuAtk_010", 0.1f },
	{ "gwanuAtk_011", 0.1f } ,
	{ "gwanuAtk_012", 0.3f } }
	},
	{ "DEAD", 2,{ 
	{ "gwanuMove_010", 0.3f },
	{ "gwanuMove_011", 0.3f }, }
	},
	{ "HIT", 1,{ { "gwanuSpc_004", 0.5f },
	}
	},
	{ "POWER", 1,{ { "gwanuSpc_005", 0.5f },
	}
	},
	};
	mpPlayer->SetCamp(JoJoGun::CampType::PLAYER);
	mpPlayer->SetClassType(JoJoGun::ClassType::CAVALRY);
	mpPlayer->SetName(L"관우");
	((MActor*)mpPlayer)->Init(Ganim, 11, E_SORTID_SECOND);
	mpPlayer->Init(500.0f, XMFLOAT2(291.0f, 145.0f), E_SORTID_SECOND);
	mpPlayer->SetStaus(100, 10, 100, 255, 4, 2);

	//이전추가
	mpPlayer = MActorManager::Instance().Create<Player>(spriteBatch, pAllPSheet, spriteFont);
	Animation Ianim[] = {
		{ "DOWN", 2,{
	{ "ijeonMove_001", 0.3f },
	{ "ijeonMove_002", 0.3f }, }
		},
	{ "LEFT", 2,{
	{ "ijeonMove_005", 0.3f },
	{ "ijeonMove_006", 0.3f }, }
	},
	{ "RIGHT", 2,{
	{ "ijeonMove_012", 0.3f },
	{ "ijeonMove_013", 0.3f }, }
	},
	{ "UP", 2,{
	{ "ijeonMove_003", 0.3f },
	{ "ijeonMove_004", 0.3f }, }
	},
	{ "DATTACK", 4,{
	{ "ijeonAtk_001", 0.3f },
	{ "ijeonAtk_002", 0.1f },
	{ "ijeonAtk_003", 0.1f } ,
	{ "ijeonAtk_004", 0.3f } }
	},
	{ "UATTACK", 4,{
	{ "ijeonAtk_005", 0.3f },
	{ "ijeonAtk_006", 0.1f },
	{ "ijeonAtk_007", 0.1f } ,
	{ "ijeonAtk_008", 0.3f } }
	},
	{ "RATTACK", 4,{
	{ "ijeonAtk_013", 0.3f },
	{ "ijeonAtk_014", 0.1f },
	{ "ijeonAtk_015", 0.1f } ,
	{ "ijeonAtk_016", 0.3f } }
	},
	{ "LATTACK", 4,{
	{ "ijeonAtk_009", 0.3f },
	{ "ijeonAtk_010", 0.1f },
	{ "ijeonAtk_011", 0.1f } ,
	{ "ijeonAtk_012", 0.3f } }
	},
	{ "DEAD", 2,{
	{ "ijeonMove_010", 0.3f },
	{ "ijeonMove_011", 0.3f }, }
	},
	{ "HIT", 1,{ { "ijeonSpc_004", 0.5f },
	}
	},
	{ "POWER", 1,{ { "ijeonSpc_005", 0.5f },
	}
	},
	};
	mpPlayer->SetCamp(JoJoGun::CampType::PLAYER);
	mpPlayer->SetClassType(JoJoGun::ClassType::ARCHER);
	mpPlayer->SetName(L"이전");
	((MActor*)mpPlayer)->Init(Ianim, 11, E_SORTID_SECOND);
	mpPlayer->Init(500.0f, XMFLOAT2(341.0f, 145.0f), E_SORTID_SECOND);
	mpPlayer->SetStaus(100, 10, 100, 255, 4, 3);

	//장비추가
	mpPlayer = MActorManager::Instance().Create<Player>(spriteBatch, pAllPSheet, spriteFont);
	Animation Janim[] = {
		{ "DOWN", 2,{
	{ "jangbiMove_001", 0.3f },
	{ "jangbiMove_002", 0.3f }, }
		},
	{ "LEFT", 2,{
	{ "jangbiMove_005", 0.3f },
	{ "jangbiMove_006", 0.3f }, }
	},
	{ "RIGHT", 2,{
	{ "jangbiMove_012", 0.3f },
	{ "jangbiMove_013", 0.3f }, }
	},
	{ "UP", 2,{
	{ "jangbiMove_003", 0.3f },
	{ "jangbiMove_004", 0.3f }, }
	},
	{ "DATTACK", 4,{
	{ "jangbiAtk_001", 0.3f },
	{ "jangbiAtk_002", 0.1f },
	{ "jangbiAtk_003", 0.1f } ,
	{ "jangbiAtk_004", 0.3f } }
	},
	{ "UATTACK", 4,{
	{ "jangbiAtk_005", 0.3f },
	{ "jangbiAtk_006", 0.1f },
	{ "jangbiAtk_007", 0.1f } ,
	{ "jangbiAtk_008", 0.3f } }
	},
	{ "RATTACK", 4,{
	{ "jangbiAtk_013", 0.3f },
	{ "jangbiAtk_014", 0.1f },
	{ "jangbiAtk_015", 0.1f } ,
	{ "jangbiAtk_016", 0.3f } }
	},
	{ "LATTACK", 4,{
	{ "jangbiAtk_009", 0.3f },
	{ "jangbiAtk_010", 0.1f },
	{ "jangbiAtk_011", 0.1f } ,
	{ "jangbiAtk_012", 0.3f } }
	},
	{ "DEAD", 2,{
	{ "jangbiMove_010", 0.3f },
	{ "jangbiMove_011", 0.3f }, }
	},
	{ "HIT", 1,{ { "jangbiSpc_004", 0.5f },
	}
	},
	{ "POWER", 1,{ { "jangbiSpc_005", 0.5f },
	}
	},
	};
	mpPlayer->SetCamp(JoJoGun::CampType::PLAYER);
	mpPlayer->SetClassType(JoJoGun::ClassType::CAVALRY);
	mpPlayer->SetName(L"장비");
	((MActor*)mpPlayer)->Init(Janim, 11, E_SORTID_SECOND);
	mpPlayer->Init(500.0f, XMFLOAT2(391.0f, 145.0f), E_SORTID_SECOND);
	mpPlayer->SetStaus(100, 10, 100, 255, 10, 2);
	
	//순욱추가
	mpPlayer = MActorManager::Instance().Create<Player>(spriteBatch, pAllPSheet, spriteFont);
	Animation SGanim[] = {
		{ "DOWN", 2,{
	{ "sunugMove_001", 0.3f },
	{ "sunugMove_002", 0.3f }, }
		},
	{ "LEFT", 2,{
	{ "sunugMove_005", 0.3f },
	{ "sunugMove_006", 0.3f }, }
	},
	{ "RIGHT", 2,{
	{ "sunugMove_012", 0.3f },
	{ "sunugMove_013", 0.3f }, }
	},
	{ "UP", 2,{
	{ "sunugMove_003", 0.3f },
	{ "sunugMove_004", 0.3f }, }
	},
	{ "DATTACK", 4,{
	{ "sunugAtk_001", 0.3f },
	{ "sunugAtk_002", 0.1f },
	{ "sunugAtk_003", 0.1f } ,
	{ "sunugAtk_004", 0.3f } }
	},
	{ "UATTACK", 4,{
	{ "sunugAtk_005", 0.3f },
	{ "sunugAtk_006", 0.1f },
	{ "sunugAtk_007", 0.1f } ,
	{ "sunugAtk_008", 0.3f } }
	},
	{ "RATTACK", 4,{
	{ "sunugAtk_013", 0.3f },
	{ "sunugAtk_014", 0.1f },
	{ "sunugAtk_015", 0.1f } ,
	{ "sunugAtk_016", 0.3f } }
	},
	{ "LATTACK", 4,{
	{ "sunugAtk_009", 0.3f },
	{ "sunugAtk_010", 0.1f },
	{ "sunugAtk_011", 0.1f } ,
	{ "sunugAtk_012", 0.3f } }
	},
	{ "DEAD", 2,{
	{ "sunugMove_010", 0.3f },
	{ "sunugMove_011", 0.3f }, }
	},
	{ "HIT", 1,{ { "sunugSpc_004", 0.5f },
	}
	},
	{ "POWER", 1,{ { "sunugSpc_005", 0.5f },
	}
	},
	};
	mpPlayer->SetCamp(JoJoGun::CampType::PLAYER);
	mpPlayer->SetClassType(JoJoGun::ClassType::TACTICIAN);
	mpPlayer->SetName(L"순욱");
	((MActor*)mpPlayer)->Init(SGanim, 11, E_SORTID_SECOND);
	mpPlayer->Init(500.0f, XMFLOAT2(441.0f, 145.0f), E_SORTID_SECOND);
	mpPlayer->SetStaus(100, 10, 100, 255, 10, 2);

	//순유추가
	mpPlayer = MActorManager::Instance().Create<Player>(spriteBatch, pAllPSheet, spriteFont);
	Animation SUanim[] = {
		{ "DOWN", 2,{
	{ "sunyuMove_001", 0.3f },
	{ "sunyuMove_002", 0.3f }, }
		},
	{ "LEFT", 2,{
	{ "sunyuMove_005", 0.3f },
	{ "sunyuMove_006", 0.3f }, }
	},
	{ "RIGHT", 2,{
	{ "sunyuMove_012", 0.3f },
	{ "sunyuMove_013", 0.3f }, }
	},
	{ "UP", 2,{
	{ "sunyuMove_003", 0.3f },
	{ "sunyuMove_004", 0.3f }, }
	},
	{ "DATTACK", 4,{
	{ "sunyuAtk_001", 0.3f },
	{ "sunyuAtk_002", 0.1f },
	{ "sunyuAtk_003", 0.1f } ,
	{ "sunyuAtk_004", 0.3f } }
	},
	{ "UATTACK", 4,{
	{ "sunyuAtk_005", 0.3f },
	{ "sunyuAtk_006", 0.1f },
	{ "sunyuAtk_007", 0.1f } ,
	{ "sunyuAtk_008", 0.3f } }
	},
	{ "RATTACK", 4,{
	{ "sunyuAtk_013", 0.3f },
	{ "sunyuAtk_014", 0.1f },
	{ "sunyuAtk_015", 0.1f } ,
	{ "sunyuAtk_016", 0.3f } }
	},
	{ "LATTACK", 4,{
	{ "sunyuAtk_009", 0.3f },
	{ "sunyuAtk_010", 0.1f },
	{ "sunyuAtk_011", 0.1f } ,
	{ "sunyuAtk_012", 0.3f } }
	},
	{ "DEAD", 2,{
	{ "sunyuMove_010", 0.3f },
	{ "sunyuMove_011", 0.3f }, }
	},
	{ "HIT", 1,{ { "sunyuSpc_004", 0.5f },
	}
	},
	{ "POWER", 1,{ { "sunyuSpc_005", 0.5f },
	}
	},
	};
	mpPlayer->SetCamp(JoJoGun::CampType::PLAYER);
	mpPlayer->SetClassType(JoJoGun::ClassType::SUPPORTER);
	mpPlayer->SetName(L"순유");
	((MActor*)mpPlayer)->Init(SUanim, 11, E_SORTID_SECOND);
	mpPlayer->Init(500.0f, XMFLOAT2(491.0f, 145.0f), E_SORTID_SECOND);
	mpPlayer->SetStaus(100, 10, 100, 255, 10, 2);

	//유비추가
	mpPlayer = MActorManager::Instance().Create<Player>(spriteBatch, pAllPSheet, spriteFont);
	Animation Yanim[] = {
		{ "DOWN", 2,{
	{ "yubiMove_001", 0.3f },
	{ "yubiMove_002", 0.3f }, }
		},
	{ "LEFT", 2,{
	{ "yubiMove_005", 0.3f },
	{ "yubiMove_006", 0.3f }, }
	},
	{ "RIGHT", 2,{
	{ "yubiMove_012", 0.3f },
	{ "yubiMove_013", 0.3f }, }
	},
	{ "UP", 2,{
	{ "yubiMove_003", 0.3f },
	{ "yubiMove_004", 0.3f }, }
	},
	{ "DATTACK", 4,{
	{ "yubiAtk_001", 0.3f },
	{ "yubiAtk_002", 0.1f },
	{ "yubiAtk_003", 0.1f } ,
	{ "yubiAtk_004", 0.3f } }
	},
	{ "UATTACK", 4,{
	{ "yubiAtk_005", 0.3f },
	{ "yubiAtk_006", 0.1f },
	{ "yubiAtk_007", 0.1f } ,
	{ "yubiAtk_008", 0.3f } }
	},
	{ "RATTACK", 4,{
	{ "yubiAtk_013", 0.3f },
	{ "yubiAtk_014", 0.1f },
	{ "yubiAtk_015", 0.1f } ,
	{ "yubiAtk_016", 0.3f } }
	},
	{ "LATTACK", 4,{
	{ "yubiAtk_009", 0.3f },
	{ "yubiAtk_010", 0.1f },
	{ "yubiAtk_011", 0.1f } ,
	{ "yubiAtk_012", 0.3f } }
	},
	{ "DEAD", 2,{
	{ "yubiMove_010", 0.3f },
	{ "yubiMove_011", 0.3f }, }
	},
	{ "HIT", 1,{ { "yubiSpc_004", 0.5f },
	}
	},
	{ "POWER", 1,{ { "yubiSpc_005", 0.5f },
	}
	},
	};
	mpPlayer->SetCamp(JoJoGun::CampType::PLAYER);
	mpPlayer->SetClassType(JoJoGun::ClassType::LORD);
	mpPlayer->SetName(L"유비");
	((MActor*)mpPlayer)->Init(Yanim, 11, E_SORTID_SECOND);
	mpPlayer->Init(500.0f, XMFLOAT2(541.0f, 145.0f), E_SORTID_SECOND);
	mpPlayer->SetStaus(100, 100, 100, 255, 10, 2);

	auto pTexture2 = ResourceManager::Instance().GetShaderResource(L"Images\\\Enemy\\Hwang\\Hwang.png");//L"Images\\\Enemy\\FotManSprites.png"
	auto pSheet2 = ResourceManager::Instance().GetSpriteSheet(L"Images\\\Enemy\\Hwang\\Hwang.xml", pTexture2);//L"Images\\\Enemy\\FotManSprites.xml"

	mpEnemy = MActorManager::Instance().Create<Enemy>(spriteBatch, pSheet2, spriteFont);
	Animation Hanim[] = {
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
	{ "hwangMove_011", 0.3f }, }
	},
	{ "HIT", 1,{ { "hwangSpc_004", 0.5f },
	}
	},
	{ "POWER", 1,{ { "hwangSpc_005", 0.5f },
	}
	},
	};
	mpEnemy->SetCamp(JoJoGun::CampType::ENEMY);
	mpEnemy->SetClassType(JoJoGun::ClassType::NOMAL);
	mpEnemy->SetName(L"황건적");
	((MActor*)mpEnemy)->Init(Hanim, 11, E_SORTID_SECOND);
	mpEnemy->Init(500.0f, XMFLOAT2(260.0f, 500.0f), E_SORTID_SECOND);
	mpEnemy->SetStaus(100, 10, 100, 255, 4, 2);
	mpEnemy->SetAI(40.0f, 500.0f, 100.0f, 0.3f, 0.8f);

	mpEnemy = MActorManager::Instance().Create<Enemy>(spriteBatch, pSheet2, spriteFont);
	mpEnemy->SetCamp(JoJoGun::CampType::ENEMY);
	mpEnemy->SetClassType(JoJoGun::ClassType::NOMAL);
	mpEnemy->SetName(L"황건적");
	((MActor*)mpEnemy)->Init(Hanim, 11, E_SORTID_SECOND);
	mpEnemy->Init(500.0f, XMFLOAT2(260.0f, 548.0f), E_SORTID_SECOND);
	mpEnemy->SetStaus(100, 10, 100, 255, 4, 2);
	mpEnemy->SetAI(40.0f, 500.0f, 100.0f, 0.3f, 0.8f);
	
	mpEnemy = MActorManager::Instance().Create<Enemy>(spriteBatch, pSheet2, spriteFont);
	mpEnemy->SetCamp(JoJoGun::CampType::ENEMY);
	mpEnemy->SetClassType(JoJoGun::ClassType::NOMAL);
	mpEnemy->SetName(L"황건적");
	((MActor*)mpEnemy)->Init(Hanim, 11, E_SORTID_SECOND);
	mpEnemy->Init(500.0f, XMFLOAT2(452.0f, 692.0f), E_SORTID_SECOND);
	mpEnemy->SetStaus(100, 10, 100, 255, 4, 2);
	mpEnemy->SetAI(40.0f, 500.0f, 100.0f, 0.3f, 0.8f);
	
	mpEnemy = MActorManager::Instance().Create<Enemy>(spriteBatch, pSheet2, spriteFont);
	mpEnemy->SetCamp(JoJoGun::CampType::ENEMY);
	mpEnemy->SetClassType(JoJoGun::ClassType::NOMAL);
	mpEnemy->SetName(L"황건적");
	((MActor*)mpEnemy)->Init(Hanim, 11, E_SORTID_SECOND);
	mpEnemy->Init(500.0f, XMFLOAT2(500, 692), E_SORTID_SECOND);
	mpEnemy->SetStaus(100, 10, 100, 255, 4, 2);
	mpEnemy->SetAI(40.0f, 500.0f, 100.0f, 0.3f, 0.8f);

	mpEnemy = MActorManager::Instance().Create<Enemy>(spriteBatch, pSheet2, spriteFont);
	mpEnemy->SetCamp(JoJoGun::CampType::ENEMY);
	mpEnemy->SetClassType(JoJoGun::ClassType::NOMAL);
	mpEnemy->SetName(L"황건적");
	((MActor*)mpEnemy)->Init(Hanim, 11, E_SORTID_SECOND);
	mpEnemy->Init(500.0f, XMFLOAT2(644, 596), E_SORTID_SECOND);
	mpEnemy->SetStaus(100, 10, 100, 255, 4, 2);
	mpEnemy->SetAI(40.0f, 500.0f, 100.0f, 0.3f, 0.8f);

	mpEnemy = MActorManager::Instance().Create<Enemy>(spriteBatch, pSheet2, spriteFont);
	mpEnemy->SetCamp(JoJoGun::CampType::ENEMY);
	mpEnemy->SetClassType(JoJoGun::ClassType::NOMAL);
	mpEnemy->SetName(L"황건적");
	((MActor*)mpEnemy)->Init(Hanim, 11, E_SORTID_SECOND);
	mpEnemy->Init(500.0f, XMFLOAT2(644, 644), E_SORTID_SECOND);
	mpEnemy->SetStaus(100, 10, 100, 255, 4, 2);
	mpEnemy->SetAI(40.0f, 500.0f, 100.0f, 0.3f, 0.8f);

	mpEnemy = MActorManager::Instance().Create<Enemy>(spriteBatch, pSheet2, spriteFont);
	mpEnemy->SetCamp(JoJoGun::CampType::ENEMY);
	mpEnemy->SetClassType(JoJoGun::ClassType::NOMAL);
	mpEnemy->SetName(L"황건적");
	((MActor*)mpEnemy)->Init(Hanim, 11, E_SORTID_SECOND);
	mpEnemy->Init(500.0f, XMFLOAT2(644, 356), E_SORTID_SECOND);
	mpEnemy->SetStaus(100, 10, 100, 255, 4, 2);
	mpEnemy->SetAI(40.0f, 500.0f, 100.0f, 0.3f, 0.8f);

	mpEnemy = MActorManager::Instance().Create<Enemy>(spriteBatch, pSheet2, spriteFont);
	mpEnemy->SetCamp(JoJoGun::CampType::ENEMY);
	mpEnemy->SetClassType(JoJoGun::ClassType::NOMAL);
	mpEnemy->SetName(L"황건적");
	((MActor*)mpEnemy)->Init(Hanim, 11, E_SORTID_SECOND);
	mpEnemy->Init(500.0f, XMFLOAT2(308, 356), E_SORTID_SECOND);
	mpEnemy->SetStaus(100, 10, 100, 255, 4, 2);
	mpEnemy->SetAI(40.0f, 500.0f, 100.0f, 0.3f, 0.8f);

	auto pJbTexture2 = ResourceManager::Instance().GetShaderResource(L"Images\\\Enemy\\Jangbo\\Jangbo.png");
	auto pJbSheet2 = ResourceManager::Instance().GetSpriteSheet(L"Images\\\Enemy\\Jangbo\\Jangbo.xml", pJbTexture2);

	//장보 캐릭터 추가
	mpEnemy = MActorManager::Instance().Create<Enemy>(spriteBatch, pJbSheet2, spriteFont);
	Animation Jbanim[] = {
		{ "DOWN", 2,{ { "jangboMove_001", 0.3f },
	{ "jangboMove_002", 0.3f }, }
		},
	{ "LEFT", 2,{ { "jangboMove_005", 0.3f },
	{ "jangboMove_006", 0.3f }, }
	},
	{ "RIGHT", 2,{ { "jangboMove_012", 0.3f },
	{ "jangboMove_013", 0.3f }, }
	},
	{ "UP", 2,{ { "jangboMove_003", 0.3f },
	{ "jangboMove_004", 0.3f }, }
	},
	{ "DATTACK", 4,{ { "jangboAtk_001", 0.3f },
	{ "jangboAtk_002", 0.1f },
	{ "jangboAtk_003", 0.1f } ,
	{ "jangboAtk_004", 0.3f } }
	},
	{ "UATTACK", 4,{ { "jangboAtk_005", 0.3f },
	{ "jangboAtk_006", 0.1f },
	{ "jangboAtk_007", 0.1f } ,
	{ "jangboAtk_008", 0.3f } }
	},
	{ "RATTACK", 4,{ { "jangboAtk_013", 0.3f },
	{ "jangboAtk_014", 0.1f },
	{ "jangboAtk_015", 0.1f } ,
	{ "jangboAtk_016", 0.3f } }
	},
	{ "LATTACK", 4,{ { "jangboAtk_009", 0.3f },
	{ "jangboAtk_010", 0.1f },
	{ "jangboAtk_011", 0.1f } ,
	{ "jangboAtk_012", 0.3f } }
	},
	{ "DEAD", 2,{ { "jangboMove_010", 0.3f },
	{ "jangboMove_011", 0.3f }, }
	},
	{ "HIT", 1,{ { "jangboSpc_004", 0.5f },
	}
	},
	{ "POWER", 1,{ { "jangboSpc_005", 0.5f },
	}
	},
	};
	mpEnemy->SetCamp(JoJoGun::CampType::ENEMY);
	mpEnemy->SetClassType(JoJoGun::ClassType::NOMAL);
	mpEnemy->SetName(L"장보");
	((MActor*)mpEnemy)->Init(Jbanim, 11, E_SORTID_SECOND);
	mpEnemy->Init(500.0f, XMFLOAT2(452.0f, 548.0f), E_SORTID_SECOND);
	mpEnemy->SetStaus(100, 10, 100, 255, 4, 2);
	mpEnemy->SetAI(40.0f, 500.0f, 100.0f, 0.3f, 0.8f);

	//장량 캐릭터 추가
	auto pJlTexture = ResourceManager::Instance().GetShaderResource(L"Images\\\Enemy\\Janglyang\\Janglyang.png");
	auto pJlSheet = ResourceManager::Instance().GetSpriteSheet(L"Images\\\Enemy\\Janglyang\\Janglyang.xml", pJlTexture);

	mpEnemy = MActorManager::Instance().Create<Enemy>(spriteBatch, pJlSheet, spriteFont);
	Animation Jlanim[] = {
		{ "DOWN", 2,{ { "janglyangMove_001", 0.3f },
	{ "janglyangMove_002", 0.3f }, }
		},
	{ "LEFT", 2,{ { "janglyangMove_005", 0.3f },
	{ "janglyangMove_006", 0.3f }, }
	},
	{ "RIGHT", 2,{ { "janglyangMove_012", 0.3f },
	{ "janglyangMove_013", 0.3f }, }
	},
	{ "UP", 2,{ { "janglyangMove_003", 0.3f },
	{ "janglyangMove_004", 0.3f }, }
	},
	{ "DATTACK", 4,{ { "janglyangAtk_001", 0.3f },
	{ "janglyangAtk_002", 0.1f },
	{ "janglyangAtk_003", 0.1f } ,
	{ "janglyangAtk_004", 0.3f } }
	},
	{ "UATTACK", 4,{ 
	{ "janglyangAtk_005", 0.3f },
	{ "janglyangAtk_006", 0.1f },
	{ "janglyangAtk_007", 0.1f } ,
	{ "janglyangAtk_008", 0.3f } }
	},
	{ "RATTACK", 4,{ 
	{ "janglyangAtk_013", 0.3f },
	{ "janglyangAtk_014", 0.1f },
	{ "janglyangAtk_015", 0.1f } ,
	{ "janglyangAtk_016", 0.3f } }
	},
	{ "LATTACK", 4,{ 
	{ "janglyangAtk_009", 0.3f },
	{ "janglyangAtk_010", 0.1f },
	{ "janglyangAtk_011", 0.1f } ,
	{ "janglyangAtk_012", 0.3f } }
	},
	{ "DEAD", 2,{ 
	{ "janglyangMove_010", 0.3f },
	{ "janglyangMove_011", 0.3f }, }
	},
	{ "HIT", 1,{ { "janglyangSpc_004", 0.5f },
	}
	},
	{ "POWER", 1,{ { "janglyangSpc_005", 0.5f },
	}
	},
	};
	mpEnemy->SetCamp(JoJoGun::CampType::ENEMY);
	mpEnemy->SetClassType(JoJoGun::ClassType::NOMAL);
	mpEnemy->SetName(L"장량");
	((MActor*)mpEnemy)->Init(Jlanim, 11, E_SORTID_SECOND);
	mpEnemy->Init(500.0f, XMFLOAT2(500.0f, 548.0f), E_SORTID_SECOND);
	mpEnemy->SetStaus(100, 10, 100, 255, 4, 2);
	mpEnemy->SetAI(40.0f, 500.0f, 100.0f, 0.3f, 0.8f);

	// 사운드 생성
	FMOD_SOUND* pSound = NULL;
	FMOD_System_CreateSound(MActorManager::Instance().GetFMODSystem(), "Sound\\07-AudioTrack 07.mp3", FMOD_DEFAULT | FMOD_LOOP_NORMAL, 0, &pSound);
	MActorManager::Instance().GetVecFMODSound()->push_back(pSound);
	// 채널
	//MActorManager::Instance().GetVecFMODChannal()->resize(2);

	// 사운드 재생
	FMOD_System_PlaySound(MActorManager::Instance().GetFMODSystem(), pSound, 0, 0, &(*MActorManager::Instance().GetVecFMODChannal())[0]);
	//
	//1
	pSound = NULL;
	FMOD_System_CreateSound(MActorManager::Instance().GetFMODSystem(), "Sound/Se_m_01fire1.wav", FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &pSound);
	MActorManager::Instance().GetVecFMODSound()->push_back(pSound);
	//2
	pSound = NULL;
	FMOD_System_CreateSound(MActorManager::Instance().GetFMODSystem(), "Sound/Se_m_04wind.wav", FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &pSound);
	MActorManager::Instance().GetVecFMODSound()->push_back(pSound);
	//3
	pSound = NULL;
	FMOD_System_CreateSound(MActorManager::Instance().GetFMODSystem(), "Sound/Se_m_12firebird.wav", FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &pSound);
	MActorManager::Instance().GetVecFMODSound()->push_back(pSound);
	//4
	pSound = NULL;
	FMOD_System_CreateSound(MActorManager::Instance().GetFMODSystem(), "Sound/Se_m_15becho.wav", FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &pSound);
	MActorManager::Instance().GetVecFMODSound()->push_back(pSound);
	//5
	pSound = NULL;
	FMOD_System_CreateSound(MActorManager::Instance().GetFMODSystem(), "Sound/Se_m_16fDragon.wav", FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &pSound);
	MActorManager::Instance().GetVecFMODSound()->push_back(pSound);
	//6
	pSound = NULL;
	FMOD_System_CreateSound(MActorManager::Instance().GetFMODSystem(), "Sound/Se_m_25heal.wav", FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &pSound);
	MActorManager::Instance().GetVecFMODSound()->push_back(pSound);
	//7
	pSound = NULL;
	FMOD_System_CreateSound(MActorManager::Instance().GetFMODSystem(), "Sound/Se00click.wav", FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &pSound);
	MActorManager::Instance().GetVecFMODSound()->push_back(pSound);
	//8
	pSound = NULL;
	FMOD_System_CreateSound(MActorManager::Instance().GetFMODSystem(), "Sound/Se05pincer.wav", FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &pSound);
	MActorManager::Instance().GetVecFMODSound()->push_back(pSound);
	//9
	pSound = NULL;
	FMOD_System_CreateSound(MActorManager::Instance().GetFMODSystem(), "Sound/Se14item.wav", FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &pSound);
	MActorManager::Instance().GetVecFMODSound()->push_back(pSound);
	//10
	pSound = NULL;
	FMOD_System_CreateSound(MActorManager::Instance().GetFMODSystem(), "Sound/Se23walk.wav", FMOD_DEFAULT | FMOD_LOOP_NORMAL, 0, &pSound);
	MActorManager::Instance().GetVecFMODSound()->push_back(pSound);
	//11
	pSound = NULL;
	FMOD_System_CreateSound(MActorManager::Instance().GetFMODSystem(), "Sound/Se24raid.wav", FMOD_DEFAULT | FMOD_LOOP_NORMAL, 0, &pSound);
	MActorManager::Instance().GetVecFMODSound()->push_back(pSound);
	//12
	pSound = NULL;
	FMOD_System_CreateSound(MActorManager::Instance().GetFMODSystem(), "Sound/Se34sword.wav", FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &pSound);
	MActorManager::Instance().GetVecFMODSound()->push_back(pSound);
	//13
	pSound = NULL;
	FMOD_System_CreateSound(MActorManager::Instance().GetFMODSystem(), "Sound/Se35hit.wav", FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &pSound);
	MActorManager::Instance().GetVecFMODSound()->push_back(pSound);
	//14
	pSound = NULL;
	FMOD_System_CreateSound(MActorManager::Instance().GetFMODSystem(), "Sound/Se36Hhit.wav", FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &pSound);
	MActorManager::Instance().GetVecFMODSound()->push_back(pSound);
	//15
	pSound = NULL;
	FMOD_System_CreateSound(MActorManager::Instance().GetFMODSystem(), "Sound/Se37autor.wav", FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &pSound);
	MActorManager::Instance().GetVecFMODSound()->push_back(pSound);
	//16
	pSound = NULL;
	FMOD_System_CreateSound(MActorManager::Instance().GetFMODSystem(), "Sound/Se39skilstart.wav", FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &pSound);
	MActorManager::Instance().GetVecFMODSound()->push_back(pSound);
	//17
	pSound = NULL;
	FMOD_System_CreateSound(MActorManager::Instance().GetFMODSystem(), "Sound/Se16dead.wav", FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &pSound);
	MActorManager::Instance().GetVecFMODSound()->push_back(pSound);
	//18
	pSound = NULL;
	FMOD_System_CreateSound(MActorManager::Instance().GetFMODSystem(), "Sound/Se01cencel.wav", FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &pSound);
	MActorManager::Instance().GetVecFMODSound()->push_back(pSound);

	
	return S_OK;
}

E_SCENE Stage::Update(float dt)
{
	//MActorManager::Instance().Update(dt);

	//mpPlayer->SetPosition(mpWorld->GetmCameraPosition().x, mpWorld->GetmCameraPosition().y);
	//mpPlayer->SetCamerapos(mpWorld->GetmCameraPosition().x, mpWorld->GetmCameraPosition().y);

	//E_SCENE eResult = mpPlayer->GetScene();
	auto state = Keyboard::Get().GetState();

	if (state.D1)
		return E_SCENE_LOGO;
	else if (state.D2)
		return E_SCENE_STAGE;
	else if (state.D3)
		return E_SCENE_EVENT0;
	else if (state.D4)
		return E_SCENE_END;

	E_SCENE eResult = MActorManager::Instance().Update(dt);
	if (eResult > E_SCENE_NONPASS)
		return eResult;


	return E_SCENE_NONPASS;
}

void Stage::Render()
{
	MActorManager::Instance().Draw();
}

void Stage::Release()
{
	//// 사운드 해제
	//for (auto p : MFramework::m_vecSound)
	//{
	//	FMOD_Sound_Release(p);
	//}

}
