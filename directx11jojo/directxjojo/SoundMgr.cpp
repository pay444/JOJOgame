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
	�������� ����
	1. DSSCL_EXCLUSIVE
	����ī�带 �����ϴ� ���´ܰ��̴�.
	���� ù��° ���ڷ� �� �����찡 Ȱ��ȭ ���϶��� ������� ����ȴ�.
	�� �������� ���� ���� ������ ������� ���α׷��ӿ��� �־�����.

	2. DSSCL_NORMAL
	�Ϲ����� �ܰ��, �ٸ� �������α׷��� �Բ� ���� ����.
	�����찡 Ȱ��ȭ�� ���� ������� ����Ǹ�, ���� ���۸� ������ �� ����.

	3. DSSCL_PRIORITY
	�����찡 ��Ȱ��ȭ�� ���� ��µǴ� ���� �����ϸ�, DSSCL_EXCLUSIVE��
	�����ϴ�.

	4. DSSCL_WRITEPRIMARY
	���� ������ ��� ������� ���α׷��ӿ��� �ش�.
	���� ���, ���� ���� ��� ����� ���α׷��Ӱ� �����ؾ� �Ѵ�.
	�̴� �ַ� �ڽŸ��� ���̿� ������ ���鶧 ���� ���δ�.
	*/
	/*
	���� ����
	1. �������(Primary Buffer)
	���� ���۴� �̸� �״�� ����ī�忡 ���޵Ǿ� ����Ŀ�� ��µ� �����
	�����Ͱ� ��� �����̴�. ��� ����� ������� �� ���� ���ۿ��� ������
	�ϳ��� ������� �ٲ��� ����Ŀ�� ��µȴ�. 
	�� ���۸� �����ϴ� ���� ��ǻ� ��ǻ���� ��� ������� �����ϴ� ���̴�.
	���� �� ����� ���� ���� ������ �ƴ� DSSCL_NORMAL������ ����� �� ����.
	������۴� 1���ۿ� ������ �� ����.

	2. ���� ����(Secondary Buffer)
	��ǻ�Ϳ��� ����� ��� ������� ���⿡ ����.
	�� ���� ���۴� �������� ���� �� �� �ִ�.
	����, DirectSound������ �������� ������� ���ÿ� ����� �� �ִ�.
	���ķ� ���� ���۴� �� �ٽ� 2���� ���� ������.

	2-1. ���� ����(static Buffer)
	���� ���۴� ������ ũ�⸦ ���� �����̴�.
	��κ��� ���۴� ���������̴�.

	2-2. ��Ʈ���� ����(Streaming Buffer)
	������� ����ϱ⿣ �ʹ� ū ���, ���ۿ� ���� �о���̰�
	����ϴ� ���� ���ۿʹ� �ٸ��� �о���̸鼭 ����ϴ� �����̴�.
	��Ʈ���� ���۴� Ư���� �޼��尡 �ʿ��ϴ�(��Ƽ ������)
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

	// Ogg ���� ������ �б� ����
	// ������ ��� ����ü ����
	OggVorbis_File vf;
	// ���̳ʸ� -> OggVorbis_File ����ü �������� ��ȯ
	ov_open(f, &vf, NULL, 0);

	// ogg ������ ���� ���
	vorbis_info* vi = ov_info(&vf, -1);

	WAVEFORMATEX wfm;
	wfm.cbSize = sizeof(wfm);
	wfm.nChannels = vi->channels;
	wfm.wBitsPerSample = 16;
	wfm.nSamplesPerSec = vi->rate;
	wfm.nAvgBytesPerSec = wfm.nSamplesPerSec * wfm.nChannels * 2;
	wfm.nBlockAlign = 2 * wfm.nChannels;
	wfm.wFormatTag = 1;

	// ���������� ũ�� = �ð� * ä�� * ��Ʈ��(16) * ���ļ�
	// Ogg���� ������ ��Ʈ���� 16�̴�.
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
	// ogg���� ������ DSBUFFERDESC ����ü ä���

	m_pDS->CreateSoundBuffer(&desc, &pTemp, NULL);
	DWORD size = BUFSIZE;
	DWORD pos = 0;
	int sec = 0;
	int ret = 1;
	char* buf;

	pTemp->Lock(0, size, (LPVOID*)&buf, &size, NULL, NULL,
		DSBLOCK_ENTIREBUFFER);
	// Lock�޼��带 ���� ���� ����̽��� �޸𸮿� (buf)�� ������ �� �ξ���.(��ü����)

	while (ret && pos < size)
	{
		// buf�� ����� ������ �̿��ؼ� ov_read()�� ���� ��ġ �޸𸮿� �����Ѵ�.
		ret = ov_read(&vf, buf + pos, size - pos, 0, 2, 1, &sec);
		// ret : ������ġ�� �� Byte��
		// �� Byte�� ��ŭ pos�� ���ϱ�
		pos += ret;
	}
	pTemp->Unlock(buf, size, NULL, NULL);

	// ���������� ���� ũ��(Byte)
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
	SetPan�� ���ڴ� DSBPAN_LEFT(-10,000) ~ DSBPAN_RIGHT(10,000)������
	���̴�.
	�⺻���� 0, LEFT, RIGHT �����Ѵ�.
	*/
}

void CSoundMgr::GetPan(int index_, long* plpan_)
{
	if (!isRightIndex(index_))
		return;

	m_SoundFileVec[index_]->pDSB->GetPan(plpan_);
	// ���� ���õ� pan���� ���´�.
}

void CSoundMgr::SetVolumn(int index_, long lVolume_)
{
	if (!isRightIndex(index_))
		return;

	m_SoundFileVec[index_]->pDSB->SetVolume(lVolume_);
	/*
	������ DSBVOLUME_MIN(-10,000) ~ DSBVOLUME_MAX(0)������ ���� ������.
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
	������� ��ġ�� Get~ �� ���� �� �ִ�.
	���⼭�� ��ġ�� �ʴ����� �ƴ϶� Byte�����̴�.
	pdwCurrentPlayerCursor_, pdwCurrentWriteCursor_�� ���� ���� Ŀ���� ��ġ�� ���Ѵ�.
	��⸦ ������ �ʴ� ���� 0(NULL)�� �� �� �ִ�.
	*/
}

void CSoundMgr::SetFrequency(int index_, DWORD dwFrequncy_)
{
	if (!isRightIndex(index_))
		return;

	m_SoundFileVec[index_]->pDSB->SetFrequency(dwFrequncy_);
	/*
	�����, �� ���ļ��� �����ϴ� ���� ����ӵ��� �ٲٴ� ���̴�.
	���ļ��� 22KHz�� �Ҹ��� 11KHz�� ����ϸ� 2��� ������ �Ҹ��� �鸰��.
	���� ���̳���. 44Khz�� �ٲٸ� 2�� ��������, ���� �ö󰣴�.
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
	���� �ܰ谡 ��������, �� DSSCL_NORMAL�� �ƴϾ��� ����,
	���� ���°� ������ ��� �� �� ������ ���۰� ��� �սǵȴ�.
	�̶��� ���¸� ���� �ν�Ʈ ��� �Ѵ�.
	�̶��� Restore() �޼��带 ȣ���Ͽ� ���۸� ���� ���־���Ѵ�.

	2. DSBSTATUS_LOOPING
	���� ���۰� �ݺ� ��������� ���Ѵ�.

	3. DSBSTATUS_PLAYING
	���� ���۰� ������̴�, �� ���� �������� ������ �����̴�.
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
