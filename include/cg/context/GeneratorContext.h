/**
 * Zillians
 * Copyright (C) 2011-2012 Zillians.com
 * For more information see http://www.zillians.com
 */

#ifndef ZILLIANS_CG_CONTEXT_GENERATORCONTEXT_H_
#define ZILLIANS_CG_CONTEXT_GENERATORCONTEXT_H_

#include "cg/Singleton.h"

namespace zillians { namespace cg { namespace context {

struct GeneratorContext : Singleton<GeneratorContext, SingletonInitialization::automatic>
{
	std::string source;
	std::wstringstream buffer;

	struct
	{
		struct {
			bool capture_cout_or_wcout;
		} cpp;
	} settings;
};

} } }

#endif /* ZILLIANS_CG_CONTEXT_GENERATORCONTEXT_H_ */
