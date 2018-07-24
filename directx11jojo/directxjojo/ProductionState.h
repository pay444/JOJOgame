#pragma once
class P_IdleState :public MState
{
public:
	virtual ~P_IdleState() {};
	virtual void Enter();
	virtual void Execute(float dt);
	virtual void Exit();
};

class P_MoveState :public MState
{
public:
	virtual ~P_MoveState() {};
	virtual void Enter();
	virtual void Execute(float dt);
	virtual void Exit();
};