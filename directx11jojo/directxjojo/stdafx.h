// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>
#include <wrl.h>
#include <map>
#include <vector>
#include <sstream>
#include <list>
#include <algorithm>
#include <stack>
#include <queue>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <d3d11.h>
#include <SimpleMath.h>
#include <SpriteBatch.h>
#include <Keyboard.h>
#include <Mouse.h>
#include <WICTextureLoader.h>
#include <SpriteFont.h>

//사운드 추가
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

#include "vld.h"

#pragma comment (lib, "d3d11.lib")

#pragma comment(lib, "dsound.lib") 
#pragma comment(lib, "dxguid.lib") 
#pragma comment(lib, "winmm.lib") 

//fmod 사운드 추가
#include "inc\\fmod.h"

#pragma comment(lib, "lib\\fmod_vc.lib")
#pragma comment(lib, "lib\\fmod64_vc.lib")
//#pragma comment(lib, "lib\\fmod64_vc.lib")
//#pragma comment(lib, "fmodL_vc.lib")
//#pragma comment(lib, "fmodL64_vc.lib")


//#pragma comment(lib, "libogg_static.lib")
//#pragma comment(lib, "libvorbis_static.lib")
//#pragma comment(lib, "libvorbisfile_static.lib")

//#pragma comment(lib, "dinput8.lib")
//#include <codec.h>

//#include <dinput.h>

//#include <vorbisfile.h>

using namespace std;
using namespace Microsoft::WRL;
using namespace DirectX;
using namespace DirectX::SimpleMath;

#include "dxerr.h"
#include "Defines.h"
#include "commonMacroFunction.h"

#include "MFramework.h"
#include "SpriteSheet.h"
#include "MActor.h"
#include "MActorManager.h"
#include "EventEditorMgr.h"
#include "ResourceManager.h"
//#include "Projectile.h"
#include "MFSM.h"
#include "Timer.h"
#include "SceneMgr.h"
#include "ScrollMgr.h"
//#include "ySortMgr.h"
#include "EnemyState.h"
#include "ProductionState.h"

#include "World.h"
#include "GameMain.h"
#include "Player.h"
#include "MoveBox.h"


//#include "ProgresiveBar.h"