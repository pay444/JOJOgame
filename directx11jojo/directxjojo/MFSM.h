#pragma once

class MFSM;

class MState
{
protected:
	MFSM *mpFSM;
public:
	virtual ~MState() {};
	virtual void Enter() = 0;
	virtual void Execute(float dt) = 0;
	virtual void Exit() = 0;

public:
	void SetFSM(MFSM* pFSM) { this->mpFSM = pFSM; }
};

class MFSM
{
private:
	MState* mpCurrent;
	map<int, unique_ptr<MState>> mStateMap;
	MActor* mpOwner;
	int mStateID;
public:
	void SetOwner(MActor* pOwner) { this->mpOwner = pOwner; }
	MActor* GetOwner() { return mpOwner; }
	int GetStateID() { return mStateID; }

public:
	template<class T>
	void AddState(int stateID);
	void ChangeState(int stateID);
	void Update(float dt);
};

template<class T>
inline void MFSM::AddState(int stateID)
{
	auto result = mStateMap.insert( pair<int, unique_ptr<MState>>(stateID, nullptr));

	//�ߺ��Ǵ°��� ���ٶ�¶�
	if (result.second == true)
	{
		//�������� �Ѿ
		result.first->second = move(unique_ptr<MState>(new T()));
		result.first->second->SetFSM(this);
	}
	//�̹� Ű������ ���ִٴ¶�
	else
	{
		DXTRACE_ERR_MSGBOX(L"State already exist!", 0);
	}
}
