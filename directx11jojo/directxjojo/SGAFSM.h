#pragma once

class SGAFSM;

class SGAState
{
protected:
	SGAFSM *mpFSM;

public:
	virtual ~SGAState() {};
	virtual void Enter() = 0;
	virtual void Execute(float dt) = 0;
	virtual void Exit() = 0;

public:
	void SetFSM(SGAFSM* pFSM) { this->mpFSM = pFSM; }
};

class SGAFSM
{
private:
	SGAState* mpCurrent;
	map<int, unique_ptr<SGAState>> mStateMap;
	SGAActor* mpOwner;

public:
	void SetOwner(SGAActor* pOwner) { this->mpOwner = pOwner; }
	SGAActor* GetOwner() { return mpOwner; }

public:
	template<class T>
	void AddState(int stateID);
	void ChangeState(int stateID);
	void Update(float dt);
};

template<class T>
inline void SGAFSM::AddState(int stateID)
{
	auto result = mStateMap.insert( pair<int, unique_ptr<SGAState>>(stateID, nullptr));

	//중복되는것이 없다라는뜻
	if (result.second == true)
	{
		//소유권이 넘어감
		result.first->second = move(unique_ptr<SGAState>(new T()));
		result.first->second->SetFSM(this);
	}
	//이미 키값으로 들어가있다는뜻
	else
	{
		DXTRACE_ERR_MSGBOX(L"State already exist!", 0);
	}
}
