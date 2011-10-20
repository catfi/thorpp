/**
 * Zillians
 * Copyright (C) 2011-2012 Zillians.com
 * For more information see http://www.zillians.com
 */

#include "cg/Prerequisite.h"

int process_java_source(const std::string& to, const std::vector<std::string>& sources)
{
	if(to == "cpp")
	{
		return 0;
	}

	std::cerr << "unrecognized target language" << std::endl;
	return -1;
}
