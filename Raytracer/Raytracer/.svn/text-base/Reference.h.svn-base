#pragma once

#include "windows.h"

template <class T> class Reference
{
public:
	Reference(T *p = NULL)
	{
		ptr = p;
		if (ptr) ++ptr->nReferences;
	}

	Reference(const Reference<T> &r)
	{
		ptr = r.ptr;
		if (ptr) ++ptr->nReference;
	}

	Reference &operator=(const Reference<T> &r)
	{
		if (r.ptr) r.ptr->nReferences++;
		if (ptr && --ptr->nReferences == 0) delete ptr;
		ptr = r.ptr;
		return *this;
	}

	Reference &operator=(T *p)
	{
		if (p) p->nReferences++;
		if (ptr && --ptr->nReferences == 0) delete ptr;
		ptr = p;
		return *this;
	}

	T *operator->() { return ptr; }
	const T *operator->() const { return ptr; }
	operator bool() const { return ptr != NULL; }
	bool operator<(const Reference<T> &t2) const
	{
		return ptr < t2.ptr;
	}

	~Reference()
	{
		if (ptr && --ptr->nReferences == 0)
		{
			delete ptr;
		}
	}

private:
	T *ptr;
};
