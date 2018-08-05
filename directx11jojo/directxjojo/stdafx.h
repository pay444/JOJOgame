// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>
#include <wrl.h>
#include <map>
#include <vector>
#include <sstream>
#include <list>
#include <algorithm>
#include <stack>
#include <queue>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include <d3d11.h>
#include <SimpleMath.h>
#include <SpriteBatch.h>
#include <Keyboard.h>
#include <Mouse.h>
#include <WICTextureLoader.h>
#include <SpriteFont.h>

//���� �߰�
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

#include "vld.h"

#pragma comment (lib, "d3d11.lib")

#pragma comment(lib, "dsound.lib") 
#pragma comment(lib, "dxguid.lib") 
#pragma comment(lib, "winmm.lib") 

//fmod ���� �߰�
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