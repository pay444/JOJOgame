#include "stdafx.h"
#include "SGAFSM.h"

void SGAFSM::ChangeState(int stateID)
{
	if (this->mpCurrent != NULL)
		this->mpCurrent->Exit();

	auto result = mStateMap.find(stateID);

	if (result == mStateMap.cend())//못찾은거
	{
		DXTRACE_ERR_MSGBOX(L"can not find stateID", 0);
		return;
	}
	
	this->mpCurrent = result->second.get(); 

	if (this->mpCurrent != NULL)
		this->mpCurrent->Enter();

	mStateID = stateID;
}

void SGAFSM::Update(float dt)
{
	if (this->mpCurrent != NULL)
		this->mpCurrent->Execute(dt);
}
