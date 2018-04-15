#pragma once
class ySortMgr final
{

public:
	static ySortMgr& Instance()
	{
		static ySortMgr instance;
		return instance;
	}
private:
	ySortMgr();
	ySortMgr(ySortMgr const&);
	void operator = (ySortMgr const&);
public:
	virtual ~ySortMgr();
private:
	//list<unique_ptr <MActor>> mActors[E_SORTID_END];
public:
	void AddSortObject(unique_ptr<MActor> &pActor);
	void RenderObject();
	//	list<unique_ptr <MActor>> GetSortActors(int sortnum = 0) { return mActors[sortnum]; }
};

