#pragma once
#include "struct.h"

const int JOJOTILESX = 48;
const int JOJOTILESY = 48;

class Player;

#define DEFAULT_SCREEN_WIDTH			960
#define DEFAULT_SCREEN_HEIGHT			576
#define STARTSCENE_SCERRN_WIDTH			640
#define STARTSCENE_SCERRN_HEIGHT		400

enum E_SCENE
{
	E_SCENE_NONPASS = -1,
	E_SCENE_LOGO,
	E_SCENE_STAGE,
	E_SCENE_EVENT0
};

enum E_SORTID
{
	E_SORTID_FIRST,
	E_SORTID_SECOND,
	E_SORTID_THIRD,
	E_SORTID_FOURTH,
	E_SORTID_END,
};



namespace DirectX
{
	struct FrameInfo
	{
		string	FrameName;
		float	DelayPerFrame;
	};

	struct Animation
	{
		string				Name;
		int					TotalFrames;
		vector<FrameInfo>	FrameInfos;
	};
}

namespace JoJoGun
{
	enum CampType
	{
		None = 0,
		PLAYER,
		MONSTER,
		NPC
	};

	enum ClassType
	{
		NOMAL = 0,
		MONARCH
	};

	enum EnemyState
	{
		Enemy_Idle,
		Enemy_Chase,
		Enemy_Attack
	};

	enum ProductionState
	{
		Production_Idle,
		Production_Move,
		Production_Talk
	};

	static Animation BulletAnim[3] = {
		{ "RED", 4,{
			{ "bullet11", 0.3f },
			{ "bullet12", 0.3f },
			{ "bullet13", 0.3f },
			{ "bullet14", 0.3f }, }
		},
		{ "BLUE", 4,{
			{ "bullet21", 0.3f },
			{ "bullet22", 0.3f },
			{ "bullet23", 0.3f },
			{ "bullet24", 0.3f }, }
		},
		{ "YELLOW", 4,{
			{ "bullet31", 0.3f },
			{ "bullet32", 0.3f },
			{ "bullet33", 0.3f },
			{ "bullet34", 0.3f }, }
		},
	};

	class Blackboard final
	{
	public:
		static Blackboard& Instance()
		{
			static Blackboard instance;
			return instance;
		}
	private:
		Blackboard(){}
		Blackboard(Blackboard const&){}
		void operator = (Blackboard const&) {}

	public:
		~Blackboard() {}
	protected:
		Player* mpPlayer;

	public:
		void SetPlayer(Player *pPlayer) { mpPlayer = pPlayer; };
		Player* GetPlayer() { return mpPlayer; }
	};

		
}
