/**
 * Zillians
 * Copyright (C) 2011-2012 Zillians.com
 * For more information see http://www.zillians.com
 */

#ifndef ZILLIANS_CG_CONTEXT_GENERATORCONTEXT_H_
#define ZILLIANS_CG_CONTEXT_GENERATORCONTEXT_H_

#include "cg/Singleton.h"

namespace zillians { namespace cg { namespace context {

struct GeneratorParameter
{
	GeneratorParameter(const std::wstring& key, const std::wstring& value) : key(key), value(value)
	{ }

	std::wstring key;
	std::wstring value;
};

struct GeneratorParameterList
{
	GeneratorParameterList()
	{ }

	~GeneratorParameterList()
	{
		for(auto i = data.begin(); i != data.end(); ++i)
			delete *i;
		data.clear();
	}

	std::vector<GeneratorParameter*> data;
};

struct GeneratorOptions
{
	enum class OpCode : unsigned int
	{
		Replace,
		Include,
	};

	GeneratorOptions(OpCode opcode, GeneratorParameterList* list) : opcode(opcode), list(list)
	{ }

	~GeneratorOptions()
	{ if(list) delete list; }

	OpCode opcode;
	GeneratorParameterList* list;
};

struct GeneratorTag
{
	GeneratorTag(const std::wstring& tag, GeneratorOptions* options, bool closed = false) : tag(tag), options(options), closed(closed)
	{ }

	~GeneratorTag()
	{ if(options) delete options; }

	std::wstring tag;
	GeneratorOptions* options;
	bool closed;
};

struct GeneratorBlock
{
	GeneratorBlock(const std::wstring& block) : block(block)
	{ }

	std::wstring block;
};

struct GeneratorContext : Singleton<GeneratorContext, SingletonInitialization::automatic>
{
	std::string source;
	std::wstringstream buffer;

	struct
	{
		bool prologue_generated;
		bool signature_generated;
		bool epilogue_generated;
		std::stack<GeneratorTag*> tag_stack;
	} state;

	struct
	{
		struct {
			bool capture_cout_or_wcout;
		} cpp;
	} settings;
};

} } }

#endif /* ZILLIANS_CG_CONTEXT_GENERATORCONTEXT_H_ */
