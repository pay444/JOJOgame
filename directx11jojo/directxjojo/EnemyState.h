#pragma once

class IdleState :public SGAState
{
public:
	virtual ~IdleState() {};
	virtual void Enter();
	virtual void Execute(float dt);
	virtual void Exit();
};

class ChaseState :public SGAState
{
	float mfElapsedTime;
public:
	virtual ~ChaseState() {};
	virtual void Enter();
	virtual void Execute(float dt);
	virtual void Exit();
};

class AttackState :public SGAState
{
	float mfElapsedTime;
public:
	virtual ~AttackState() {};
	virtual void Enter();
	virtual void Execute(float dt);
	virtual void Exit();
};