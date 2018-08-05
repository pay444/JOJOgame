#pragma once
#include "Default.h"

class CSoundMgr
{
	DECLARE_SINGLETON(CSoundMgr);
private:
	LPDIRECTSOUND8 m_pDS;
	vector<soundFile*> m_SoundFileVec;
public:
	HRESULT InitSoundDevice();		// 사운드 장치 로드
	HRESULT LoadFile(char* path_);	// 경로로부터 Ogg파일 로드
	bool isRightIndex(int index_);	// 벡터의 범위내에 있냐?
	void Play(int index_, DWORD flags_);	// 재생
	void Stop(int index_);					// 정지
	void SetPan(int index_, long lpan_);	// 좌우 밸런스 조절
	void GetPan(int index_, long* plpan_);	// 좌우 밸런스값을 얻어온다
	void SetVolumn(int index_, long lVolume_);	// 볼륨 설정
	void GetVolumn(int index_, long* plVolum_);	// 볼륨값 얻어오기
	void SetCurrentPosition(int index_, DWORD dwNewPostion_);	// 재생위치 변경
	void GetCurrentPosition(int index_,
		DWORD* pdwCurrentPlayerCursor_,
		DWORD* pdwCurrentWriteCursor_);// 현재 재생위치
	void SetFrequency(int index_, DWORD dwFrequncy_);	// 재생속도
	void GetFrequency(int index_, DWORD* pdwFrequncy_);	// 현재 재생속도 얻어오기
	void GetStatus(int index_, DWORD* pdwStatus_);
	unsigned int GetSize(int index_);
	void Release();
private:
	CSoundMgr();
public:
	~CSoundMgr();
};

