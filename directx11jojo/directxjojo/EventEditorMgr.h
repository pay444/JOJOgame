#pragma once
class EventEditorMgr
{
public:
	//���۷��� ǥ��
	static EventEditorMgr& Instance()
	{
		static EventEditorMgr instance;
		return instance;
	}
private:
	EventEditorMgr();
	EventEditorMgr(EventEditorMgr const&);
	void operator = (EventEditorMgr const&);
public:
	~EventEditorMgr();
public:
	list<unique_ptr<MActor>>* mpActors;
	vector<wstring> mVecEvents;
	int mOrder;					//�ൿ�� ���°����
	float mfTimer;				//�̺�Ʈ ��ǲ Ÿ�̸�
	bool misMoveTalkFlag;		//�� �÷��װ� �ö󰡸� ������Ʈ�� �ڵ� �ൿ�� ����
public:
	void Init();
	E_SCENE Update(float dt);					//���� ������������ Update();
	void Release();
	void Draw();
public:
	void EventCheck();
public:
	void SetActors(list<unique_ptr<MActor>>* pactor) { mpActors = pactor; }
	list<unique_ptr<MActor>>* GetActors() { return mpActors; }

	void SetOrder(int order) { mOrder = order; }
	int GetOrder() { return mOrder; }


};

