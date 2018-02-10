// directxjojo.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "directxjojo.h"
#include "GameMain.h"
#include <stdio.h>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	////콘솔창 띄우기
	//AllocConsole();
	//freopen("CONOUT$", "wt", stdout);
	////CONOUT$ - console 창
	////wt - 텍스트 쓰기 모드
	////stdout - 출력될 파일 포인터(모니터로 지정)

	//printf("hello DEBUG\n");

	GameMain main;

	main.InitWindow(hInstance);

	return (int)main.GameLoop();
}