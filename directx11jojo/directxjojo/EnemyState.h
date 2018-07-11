#pragma once

class IdleState :public MState
{
public:
	virtual ~IdleState() {};
	virtual void Enter();
	virtual void Execute(float dt);
	virtual void Exit();
};

class ChaseState :public MState
{
	float mfElapsedTime;
public:
	virtual ~ChaseState() {};
	virtual void Enter();
	virtual void Execute(float dt);
	virtual void Exit();
};

class AttackState :public MState
{
	float mfElapsedTime;
public:
	virtual ~AttackState() {};
	virtual void Enter();
	virtual void Execute(float dt);
	virtual void Exit();
};