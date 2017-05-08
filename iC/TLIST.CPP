static char *tlist_id = 
	"@(#)Copyright (C) gaoguangtao 2008-2050   tlist.cpp	Ver2.06";
/* ========================================================================
	Project  Name			: Win32 Lightweight  Class Library Test
	Module Name				: List Class
	Create					: 2008-02-20(Sat)
    Update					: 2008-02-14(Wed)
	Copyright				: gaoguangtao
	Reference				: 
	======================================================================== */

#include "tlib.h"

/*
	TList class
*/
TList::TList(void)
{
	top.prior = top.next = &top;
}

void TList::AddObj(TListObj * obj)
{
	obj->prior = top.prior;
	top.prior->next = obj;
	obj->next = &top;
	top.prior = obj;
}

void TList::DelObj(TListObj * obj)
{
	if (obj->next)
		obj->next->prior = obj->prior;
	if (obj->prior)
		obj->prior->next = obj->next;
	obj->next = obj->prior = NULL;
}

TListObj* TList::TopObj(void)
{
	return	top.next == &top ? NULL : top.next;
}

TListObj* TList::NextObj(TListObj *obj)
{
	return	obj->next == &top ? NULL : obj->next;
}


/*
	TRecycleList class
*/
TRecycleList::TRecycleList(int init_cnt, int size)
{
	data = new char [init_cnt * size];
	memset(data, 0, init_cnt * size);

	for (int cnt=0; cnt < init_cnt; cnt++) {
		TListObj *obj = (TListObj *)(data + cnt * size);
		list[FREE_LIST].AddObj(obj);
	}
}

TRecycleList::~TRecycleList()
{
	delete [] data;
}

TListObj *TRecycleList::GetObj(int list_type)
{
	TListObj	*obj = list[list_type].TopObj();

	if (obj)
		list[list_type].DelObj(obj);

	return	obj;
}

void TRecycleList::PutObj(int list_type, TListObj *obj)
{
	list[list_type].AddObj(obj);
}

