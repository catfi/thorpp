/**
 * Zillians
 * Copyright (C) 2011-2012 Zillians.com
 * For more information see http://www.zillians.com
 */

#ifndef ZILLIANS_CG_ACTION_CPPACTION_H_
#define ZILLIANS_CG_ACTION_CPPACTION_H_

#include "cg/action/Common.h"
#include "cg/context/GeneratorContext.h"
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

using zillians::cg::context::GeneratorContext;

namespace zillians { namespace cg { namespace action {

namespace detail {

void generateDefaultSignature()
{
	GeneratorContext::instance()->buffer << L"void generate(std::wstringstream& ss)" << std::endl;
}

void generateDefaultCodeGenBegin()
{
	GeneratorContext::instance()->buffer
			<< L"{" << std::endl
			<< L"\tstd::wstringstream __redirect_stream;" << std::endl
			<< L"\tstd::wstreambuf* __old_buf = std::wcout.rdbuf(__redirect_stream.rdbuf());" << std::endl;
}

void generateDefaultCodeGenEnd()
{
	GeneratorContext::instance()->buffer
			<< std::endl
			<< L"\tstd::wcout.rdbuf(__old_buf);" << std::endl
			<< L"\tss << __redirect_stream.str();" << std::endl
			<< L"}" << std::endl;
}

void generateDefaultPrologue()
{
	GeneratorContext::instance()->buffer
			<< L"#include <iostream>" << std::endl
			<< L"#include <sstream>" << std::endl
			<< std::endl;
}

void generateDefaultEpilogue()
{

}

void generateInclude(std::wstring& filename)
{
	boost::filesystem::path p(filename);

	std::wifstream in;
	boost::filesystem::path in_p;
	if(p.is_absolute())
	{
		in_p = p;
		in.open(p.c_str(), std::ios_base::in | std::ios_base::binary);
	}
	else
	{
		boost::filesystem::path relative_p(GeneratorContext::instance()->source);
		relative_p = relative_p.parent_path();
		if(relative_p.empty())
		{
			relative_p = boost::filesystem::current_path();
		}
		relative_p /= p;

		in_p = relative_p;
		in.open(relative_p.c_str());
	}

	if(in.good())
	{
		GeneratorContext::instance()->buffer << in.rdbuf();
	}
	else
	{
		std::cerr << "Error: failed to open include file: " << in_p << std::endl;
	}
}

}

struct CppAction
{
	struct opcode
	{
		DEFINE_ATTRIBUTES(opcode*)
		DEFINE_LOCALS()

		BEGIN_ACTION(init_replace)
		{
			_result = new opcode(L"replace");
		}
		END_ACTION

		BEGIN_ACTION(init_include)
		{
			_result = new opcode(L"include");
		}
		END_ACTION

		opcode(const std::wstring& s) : op(s)
		{ }

		std::wstring op;
	};

	struct parameter
	{
		DEFINE_ATTRIBUTES(parameter*)
		DEFINE_LOCALS()

		BEGIN_ACTION(init)
		{
			_result = new parameter(_param(0), _param(1));
		}
		END_ACTION

		parameter(const std::wstring& k, const std::wstring& v) : key(k), value(v)
		{ }

		std::wstring key;
		std::wstring value;
	};

	struct parameter_list
	{
		DEFINE_ATTRIBUTES(parameter_list*)
		DEFINE_LOCALS()

		BEGIN_ACTION(init)
		{
			_result = new parameter_list();
		}
		END_ACTION

		BEGIN_ACTION(append)
		{
			_result->all.push_back(_param(0));
		}
		END_ACTION

		std::vector<parameter*> all;
	};

	struct options
	{
		DEFINE_ATTRIBUTES(options*)
		DEFINE_LOCALS()

		BEGIN_ACTION(init)
		{
			_result = new options(_param(0), _param(1));
		}
		END_ACTION

		options(opcode* a, parameter_list* l) : op(a), list(l)
		{ }

		opcode* op;
		parameter_list* list;
	};

	struct block
	{
		DEFINE_ATTRIBUTES(block*)
		DEFINE_LOCALS()

		BEGIN_ACTION(init)
		{

		}
		END_ACTION
	};

	struct tag_gen
	{
		DEFINE_ATTRIBUTES(tag_gen*)
		DEFINE_LOCALS()

		BEGIN_ACTION(init)
		{
			_result = new tag_gen();
		}
		END_ACTION
	};

	struct tag_code
	{
		DEFINE_ATTRIBUTES(tag_code*)
		DEFINE_LOCALS()

		BEGIN_ACTION(init)
		{
			_result = new tag_code();
		}
		END_ACTION
	};

	struct start
	{
		DEFINE_ATTRIBUTES(void)
		DEFINE_LOCALS(bool /*prologue_generated*/, bool /*signature_generated*/, bool /*epilogue_generated*/)

		BEGIN_ACTION(init)
		{
			_local(0) = false;
			_local(1) = false;
			_local(2) = false;
		}
		END_ACTION

		BEGIN_ACTION(fini)
		{
			if(!_local(0)) { detail::generateDefaultPrologue(); _local(0) = true; }
			if(!_local(1)) { detail::generateDefaultSignature(); detail::generateDefaultCodeGenBegin(); _local(1) = true; }
			if(!_local(2)) { detail::generateDefaultCodeGenEnd(); detail::generateDefaultEpilogue(); _local(2) = true; }
		}
		END_ACTION

		BEGIN_ACTION(on_block)
		{

		}
		END_ACTION

		BEGIN_ACTION(on_tag_gen)
		{

		}
		END_ACTION

		BEGIN_ACTION(on_tag_code)
		{

		}
		END_ACTION

//		BEGIN_ACTION(on_code_block)
//		{
//			std::wstring& code = _param(0);
//
//			// ignore any useless code block before first useful code block
//			// this is done to allow spaces between prologue block and signature block
//			if(!_local(0) || !_local(1))
//			{
//				std::wstring dummy = code;
//				boost::trim(dummy);
//				if(dummy.length() == 0)
//					return;
//			}
//
//			if(!_local(0)) { detail::generateDefaultPrologue(); _local(0) = true; }
//			if(!_local(1)) { detail::generateDefaultSignature(); detail::generateDefaultCodeGenBegin(); _local(1) = true; }
//
//			std::wstringstream& buffer = GeneratorContext::instance()->buffer;
//			buffer << L"\tstd::wcout <<\n\t\t\"";
//			for(int i = 0; i < code.length(); ++i)
//			{
//				wchar_t c = code.at(i);
//				if(c == L'\n' || c == L'\r')
//				{
//					buffer << L"\\n\"\n\t\t\"";
//				}
//				else if(c == L'\"')
//				{
//					buffer << L"\\\"";
//				}
//				else if(c == L'\'')
//				{
//					buffer << L"\\'";
//				}
//				else if(c == L'\\')
//				{
//					buffer << L"\\\\";
//				}
//				else
//				{
//					buffer << c;
//				}
//			}
//			buffer << L"\";\n";
//		}
//		END_ACTION
//
//		BEGIN_ACTION(on_comment_block)
//		{
//			std::wstring& comment = _param(0);
//
//			const static std::wstring code_generation_tag (L"[[[code");
//			const static std::wstring prologue_tag        (L"[[[prologue");
//			const static std::wstring epilogue_tag        (L"[[[epilogue");
//			const static std::wstring signature_tag       (L"[[[signature");
//
//			const static std::wstring code_generation_include_tag (L"[[[code_include");
//			const static std::wstring prologue_include_tag        (L"[[[prologue_include");
//			const static std::wstring epilogue_include_tag        (L"[[[epilogue_include");
//			const static std::wstring signature_include_tag       (L"[[[signature_include");
//
//			if(comment.compare(0, code_generation_tag.length(), code_generation_tag) == 0)
//			{
//				if(!_local(0)) { detail::generateDefaultPrologue(); _local(0) = true; }
//				if(!_local(1)) { detail::generateDefaultSignature(); detail::generateDefaultCodeGenBegin(); _local(1) = true; }
//
//				if(!_local(2))
//				{
//					if(comment.compare(0, code_generation_include_tag.length(), code_generation_include_tag) == 0)
//					{
//						std::wstring s(comment.c_str() + code_generation_include_tag.length());
//						boost::trim(s);
//						detail::generateInclude(s);
//					}
//					else
//					{
//						GeneratorContext::instance()->buffer << comment.substr(code_generation_tag.length()) << std::endl;
//					}
//				}
//				else
//				{
//					// throw out some warning here
//					std::cerr << "Warn: epilogue has been emitted, the epilogue block is ignored" << std::endl;
//				}
//			}
//			else if(comment.compare(0, signature_tag.length(), signature_tag) == 0)
//			{
//				if(!_local(0)) { detail::generateDefaultPrologue(); _local(0) = true; }
//				if(!_local(1))
//				{
//					if(comment.compare(0, signature_include_tag.length(), signature_include_tag) == 0)
//					{
//						std::wstring s(comment.c_str() + signature_include_tag.length());
//						boost::trim(s);
//						detail::generateInclude(s);
//					}
//					else
//					{
//						std::wstring signature(comment.c_str() + signature_tag.length());
//						boost::trim(signature);
//
//						GeneratorContext::instance()->buffer << signature << std::endl;
//						detail::generateDefaultCodeGenBegin();
//					}
//					_local(1) = true;
//				}
//				else
//				{
//					// throw out some warning here
//					std::cerr << "Warn: signautre has been emitted, the signautre block is ignored" << std::endl;
//				}
//			}
//			else if(comment.compare(0, prologue_tag.length(), prologue_tag) == 0)
//			{
//				if(!_local(0) /*prologue_generated*/)
//				{
//					if(comment.compare(0, prologue_include_tag.length(), prologue_include_tag) == 0)
//					{
//						std::wstring s(comment.c_str() + prologue_include_tag.length());
//						boost::trim(s);
//						detail::generateInclude(s);
//					}
//					else
//					{
//						GeneratorContext::instance()->buffer << comment.substr(prologue_tag.length()) << std::endl;
//						detail::generateDefaultPrologue();
//					}
//					_local(0) = true;
//				}
//				else
//				{
//					// throw out some warning here
//					std::cerr << "Warn: prologue has been emitted, the prologue block is ignored" << std::endl;
//				}
//			}
//			else if(comment.compare(0, epilogue_tag.length(), epilogue_tag) == 0)
//			{
//				if(!_local(2) /*epilogue_generated*/)
//				{
//					if(comment.compare(0, epilogue_include_tag.length(), epilogue_include_tag) == 0)
//					{
//						std::wstring s(comment.c_str() + epilogue_include_tag.length());
//						boost::trim(s);
//						detail::generateDefaultCodeGenEnd();
//						detail::generateInclude(s);
//					}
//					else
//					{
//						detail::generateDefaultCodeGenEnd();
//						GeneratorContext::instance()->buffer << comment.substr(epilogue_tag.length()) << std::endl;
//					}
//					_local(2) = true;
//				}
//				else
//				{
//					// throw out some warning here
//					std::cerr << "Warn: epilogue has been emitted, the epilogue block is ignored" << std::endl;
//				}
//			}
//		}
//		END_ACTION
	};
};

} } }

#endif /* ZILLIANS_CG_ACTION_CPPACTION_H_ */
