#include "stdafx.h"
#include "SoundMgr.h"


CSoundMgr::CSoundMgr()
	:m_pDS(NULL)
{
}

CSoundMgr::~CSoundMgr()
{
	this->Release();
}

HRESULT CSoundMgr::InitSoundDevice()
{
	if (FAILED(DirectSoundCreate8(NULL, &m_pDS, NULL)))
	{
		MessageBox(NULL, L"DirectSoundCreate8() Failed !", NULL, NULL);
		return E_FAIL;
	}
	m_pDS->Initialize(NULL);
	m_pDS->SetCooperativeLevel(g_hWnd, DSSCL_NORMAL);
	/*
	협조레벨 설정
	1. DSSCL_EXCLUSIVE
	사운드카드를 독점하는 협력단계이다.
	먼저 첫번째 인자로 온 윈도우가 활성화 중일때만 오디오가 재생된다.
	그 다음으로 전면 사운드 버퍼의 제어권이 프로그래머에게 주어진다.

	2. DSSCL_NORMAL
	일반적인 단계로, 다른 응용프로그램과 함께 쓰기 좋다.
	윈도우가 활성화일 때만 오디오가 재생되며, 전면 버퍼를 제어할 수 없다.

	3. DSSCL_PRIORITY
	윈도우가 비활성화일 때도 출력되는 점을 제외하면, DSSCL_EXCLUSIVE와
	동일하다.

	4. DSSCL_WRITEPRIMARY
	전면 버퍼의 모든 제어권을 프로그래머에게 준다.
	따라서 재생, 정지 등의 모든 기능을 프로그래머가 정의해야 한다.
	이는 주로 자신만의 오이오 엔진을 만들때 많이 쓰인다.
	*/
	/*
	사운드 버퍼
	1. 전면버퍼(Primary Buffer)
	전면 버퍼는 이름 그대로 사운드카드에 전달되어 스피커에 출력될 오디오
	데이터가 담긴 버퍼이다. 모든 재생될 오디오는 이 전면 버퍼에서 섞여서
	하나의 오디오로 바뀐후 스피커로 출력된다. 
	이 버퍼를 제어하는 것은 사실상 컴퓨터의 모든 오디오를 제어하는 것이다.
	따라서 이 기능은 독점 협력 레벨이 아닌 DSSCL_NORMAL에서는 사용할 수 없다.
	전면버퍼는 1개밖에 존재할 수 없다.

	2. 보조 버퍼(Secondary Buffer)
	컴퓨터에서 재생될 모든 오디오가 여기에 담긴다.
	이 보조 버퍼는 여러개가 존재 할 수 있다.
	따라서, DirectSound에서는 여러개의 오디오를 동시에 재생할 수 있다.
	이후로 보조 버퍼는 또 다시 2개의 종류 나뉜다.

	2-1. 정적 버퍼(static Buffer)
	정적 버퍼는 고정된 크기를 지닌 버퍼이다.
	대부분의 버퍼는 정적버퍼이다.

	2-2. 스트리밍 버퍼(Streaming Buffer)
	오디오가 재생하기엔 너무 큰 경우, 버퍼에 전부 읽어들이고
	재생하는 정적 버퍼와는 다르게 읽어들이면서 재생하는 버퍼이다.
	스트리밍 버퍼는 특별한 메서드가 필요하다(멀티 쓰레드)
	*/

	return S_OK;
}

HRESULT CSoundMgr::LoadFile(char* path_)
{
	LPDIRECTSOUNDBUFFER pTemp = NULL;

	FILE* f;
	fopen_s(&f, path_, "rb");

	if (!f)
	{
		MessageBox(NULL, L"CSoundMgr::LoadFile() Failed !", NULL, NULL);
		return E_FAIL;
	}

	// Ogg 음원 파일을 읽기 위한
	// 정보가 담긴 구조체 선언
	OggVorbis_File vf;
	// 바이너리 -> OggVorbis_File 구조체 형식으로 변환
	ov_open(f, &vf, NULL, 0);

	// ogg 음원의 정보 얻기
	vorbis_info* vi = ov_info(&vf, -1);

	WAVEFORMATEX wfm;
	wfm.cbSize = sizeof(wfm);
	wfm.nChannels = vi->channels;
	wfm.wBitsPerSample = 16;
	wfm.nSamplesPerSec = vi->rate;
	wfm.nAvgBytesPerSec = wfm.nSamplesPerSec * wfm.nChannels * 2;
	wfm.nBlockAlign = 2 * wfm.nChannels;
	wfm.wFormatTag = 1;

	// 음원파일의 크기 = 시간 * 채널 * 비트수(16) * 주파수
	// Ogg음원 파일은 비트수가 16이다.
	DWORD BUFSIZE = DWORD(ov_pcm_total(&vf, -1) * vi->channels * 2);

	DSBUFFERDESC desc;
	desc.dwSize = sizeof(desc);
	desc.dwFlags =
		DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN |
		DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPOSITIONNOTIFY |
		DSBCAPS_STATIC | DSBCAPS_GLOBALFOCUS;
	desc.lpwfxFormat = &wfm;
	desc.dwReserved = 0;
	desc.dwBufferBytes = BUFSIZE;
	// ogg음원 조사후 DSBUFFERDESC 구조체 채우기

	m_pDS->CreateSoundBuffer(&desc, &pTemp, NULL);
	DWORD size = BUFSIZE;
	DWORD pos = 0;
	int sec = 0;
	int ret = 1;
	char* buf;

	pTemp->Lock(0, size, (LPVOID*)&buf, &size, NULL, NULL,
		DSBLOCK_ENTIREBUFFER);
	// Lock메서드를 통해 사운드 디바이스의 메모리에 (buf)와 연결을 해 두었다.(전체버퍼)

	while (ret && pos < size)
	{
		// buf와 연결된 변수를 이용해서 ov_read()로 사운드 장치 메모리에 복사한다.
		ret = ov_read(&vf, buf + pos, size - pos, 0, 2, 1, &sec);
		// ret : 사운드장치에 쓴 Byte수
		// 그 Byte수 만큼 pos에 더하기
		pos += ret;
	}
	pTemp->Unlock(buf, size, NULL, NULL);

	// 사운드파일의 최종 크기(Byte)
	desc.dwBufferBytes = pos;

	soundFile* TmpFile = new soundFile;
	TmpFile->pDSB = pTemp;
	TmpFile->nCurSection = TmpFile->nLastSection = 0;
	TmpFile->actualsize = pos;
	m_SoundFileVec.push_back(TmpFile);

	ov_clear(&vf);
	fclose(f);
	buf = NULL;

	return S_OK;
}

bool CSoundMgr::isRightIndex(int index_)
{
	if (index_ < 0 || index_ >(int)m_SoundFileVec.size())
		return false;

	return true;
}

void CSoundMgr::Play(int index_, DWORD flags_)
{
	if (!isRightIndex(index_))
		return;

	m_SoundFileVec[index_]->pDSB->Play(0, 0, flags_);
}

void CSoundMgr::Stop(int index_)
{
	if (!isRightIndex(index_))
		return;

	m_SoundFileVec[index_]->pDSB->Stop();
}

void CSoundMgr::SetPan(int index_, long lpan_)
{
	if (!isRightIndex(index_))
		return;

	m_SoundFileVec[index_]->pDSB->SetPan(lpan_);
	/*
	SetPan의 인자는 DSBPAN_LEFT(-10,000) ~ DSBPAN_RIGHT(10,000)까지의
	값이다.
	기본으로 0, LEFT, RIGHT 제공한다.
	*/
}

void CSoundMgr::GetPan(int index_, long* plpan_)
{
	if (!isRightIndex(index_))
		return;

	m_SoundFileVec[index_]->pDSB->GetPan(plpan_);
	// 현재 세팅된 pan값을 얻어온다.
}

void CSoundMgr::SetVolumn(int index_, long lVolume_)
{
	if (!isRightIndex(index_))
		return;

	m_SoundFileVec[index_]->pDSB->SetVolume(lVolume_);
	/*
	볼륨은 DSBVOLUME_MIN(-10,000) ~ DSBVOLUME_MAX(0)까지의 값을 가진다.
	*/
}

void CSoundMgr::GetVolumn(int index_, long* plVolum_)
{
	if (!isRightIndex(index_))
		return;
	m_SoundFileVec[index_]->pDSB->GetVolume(plVolum_);
}

void CSoundMgr::SetCurrentPosition(int index_, DWORD dwNewPostion_)
{
	if (!isRightIndex(index_))
		return;

	m_SoundFileVec[index_]->pDSB->SetCurrentPosition(dwNewPostion_);
}

void CSoundMgr::GetCurrentPosition(int index_, DWORD* pdwCurrentPlayerCursor_, DWORD* pdwCurrentWriteCursor_)
{
	if (!isRightIndex(index_))
		return;

	m_SoundFileVec[index_]->pDSB->GetCurrentPosition(
		pdwCurrentPlayerCursor_,
		pdwCurrentWriteCursor_);
	/*
	재생중인 위치는 Get~ 로 얻을 수 있다.
	여기서의 위치는 초단위가 아니라 Byte단위이다.
	pdwCurrentPlayerCursor_, pdwCurrentWriteCursor_는 현재 쓰기 커서의 위치를 말한다.
	얻기를 원하지 않는 값은 0(NULL)을 줄 수 있다.
	*/
}

void CSoundMgr::SetFrequency(int index_, DWORD dwFrequncy_)
{
	if (!isRightIndex(index_))
		return;

	m_SoundFileVec[index_]->pDSB->SetFrequency(dwFrequncy_);
	/*
	재생률, 즉 주파수를 설정하는 것은 재생속도를 바꾸는 것이다.
	주파수가 22KHz인 소리를 11KHz로 재생하면 2배로 느리게 소리가 들린다.
	낮은 음이난다. 44Khz로 바꾸면 2배 빨라지고, 음이 올라간다.
	*/
}

void CSoundMgr::GetFrequency(int index_, DWORD* pdwFrequncy_)
{
	if (!isRightIndex(index_))
		return;

	m_SoundFileVec[index_]->pDSB->GetFrequency(pdwFrequncy_);
}

void CSoundMgr::GetStatus(int index_, DWORD* pdwStatus_)
{
	if (!isRightIndex(index_))
		return;

	m_SoundFileVec[index_]->pDSB->GetStatus(pdwStatus_);

	if ((*pdwStatus_) == DSBSTATUS_BUFFERLOST)
		m_SoundFileVec[index_]->pDSB->Restore();

	/*
	1. DSBSTATUS_BUFFERLOST
	협력 단계가 독점중인, 즉 DSSCL_NORMAL이 아니었던 경우는,
	독점 상태가 해제될 경우 그 때 생성된 버퍼가 모두 손실된다.
	이때의 상태를 버퍼 로스트 라고 한다.
	이때는 Restore() 메서드를 호출하여 버퍼를 복구 해주어야한다.

	2. DSBSTATUS_LOOPING
	현재 버퍼가 반복 재생중임을 말한다.

	3. DSBSTATUS_PLAYING
	현재 버퍼가 재생중이다, 이 값이 없을경우는 중지된 상태이다.
	*/
}

unsigned int CSoundMgr::GetSize(int index_)
{
	if (!isRightIndex(index_))
		return 0;

	return m_SoundFileVec[index_]->actualsize;	// Byte !
}

void CSoundMgr::Release()
{
	for (vector<soundFile*>::iterator iter = m_SoundFileVec.begin();
		iter != m_SoundFileVec.end(); ++iter)
	{
		SAFE_RELEASE((*iter)->pDSB);
		SAFE_DELETE(*iter);
	}
	m_SoundFileVec.clear();
	SAFE_RELEASE(m_pDS);
}
