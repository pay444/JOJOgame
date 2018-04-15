#include "stdafx.h"
#include "ySortMgr.h"


ySortMgr::ySortMgr()
{
}


ySortMgr::~ySortMgr()
{
}

void ySortMgr::AddSortObject(unique_ptr<MActor> &pActor)
{
	//E_SORTID eID = pActor->GetSortID();

	//mActors[eID].push_back(std::move(pActor));

}

void ySortMgr::RenderObject()
{
	//for (int i =0; i < E_SORTID_END; ++i)
	//{
	//	for (auto &actor : mActors[i])
	//		actor->Draw();
	//}
	//
}
