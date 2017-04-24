#pragma once

class ReferenceCounted
{
public:
	ReferenceCounted() { nReferences = 0; }
	~ReferenceCounted();

	int nReferences;

private:

	ReferenceCounted(const ReferenceCounted &);
	ReferenceCounted &operator=(const ReferenceCounted &);
};
