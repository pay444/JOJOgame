#pragma once
#include "Default.h"

class CSoundMgr
{
	DECLARE_SINGLETON(CSoundMgr);
private:
	LPDIRECTSOUND8 m_pDS;
	vector<soundFile*> m_SoundFileVec;
public:
	HRESULT InitSoundDevice();		// ���� ��ġ �ε�
	HRESULT LoadFile(char* path_);	// ��ηκ��� Ogg���� �ε�
	bool isRightIndex(int index_);	// ������ �������� �ֳ�?
	void Play(int index_, DWORD flags_);	// ���
	void Stop(int index_);					// ����
	void SetPan(int index_, long lpan_);	// �¿� �뷱�� ����
	void GetPan(int index_, long* plpan_);	// �¿� �뷱������ ���´�
	void SetVolumn(int index_, long lVolume_);	// ���� ����
	void GetVolumn(int index_, long* plVolum_);	// ������ ������
	void SetCurrentPosition(int index_, DWORD dwNewPostion_);	// �����ġ ����
	void GetCurrentPosition(int index_,
		DWORD* pdwCurrentPlayerCursor_,
		DWORD* pdwCurrentWriteCursor_);// ���� �����ġ
	void SetFrequency(int index_, DWORD dwFrequncy_);	// ����ӵ�
	void GetFrequency(int index_, DWORD* pdwFrequncy_);	// ���� ����ӵ� ������
	void GetStatus(int index_, DWORD* pdwStatus_);
	unsigned int GetSize(int index_);
	void Release();
private:
	CSoundMgr();
public:
	~CSoundMgr();
};

