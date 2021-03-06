#include "stdafx.h"
#include "MFSM.h"

MFSM::~MFSM()
{
	Release();
}

void MFSM::ChangeState(int stateID)
{
	if (this->mpCurrent != NULL)
		this->mpCurrent->Exit();

	auto result = mStateMap.find(stateID);

	if (result == mStateMap.cend())//��ã����
	{
		DXTRACE_ERR_MSGBOX(L"can not find stateID", 0);
		return;
	}
	
	this->mpCurrent = result->second.get(); 

	if (this->mpCurrent != NULL)
		this->mpCurrent->Enter();

	mStateID = stateID;
}

void MFSM::Update(float dt)
{
	if (this->mpCurrent != NULL)
		this->mpCurrent->Execute(dt);
}

void MFSM::Release()
{
	auto statemapiter = mStateMap.begin();
	while (statemapiter != mStateMap.end())
	{
		statemapiter->second.reset();
		statemapiter = mStateMap.erase(statemapiter);
	}
	mStateMap.clear();
}
