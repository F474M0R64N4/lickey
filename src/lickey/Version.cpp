#include "stdafx.h"
#include "Version.h"


namespace lickey
{
	Version::Version()
		: version(1) // first version
	{
	}


	Version& Version::GetInstance()
	{
		static Version obj;
		return obj;
	}


	Version::~Version()
	{
	}

}
