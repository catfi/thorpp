/**
 * Zillians
 * Copyright (C) 2011-2012 Zillians.com
 * For more information see http://www.zillians.com
 */

#ifndef ZILLIANS_CG_ACTION_CPPACTION_H_
#define ZILLIANS_CG_ACTION_CPPACTION_H_

#include "cg/action/Common.h"
#include "cg/context/GeneratorContext.h"

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
			<< L"\tss << __redirect_stream;" << std::endl
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

}

struct CppAction
{
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

		BEGIN_ACTION(on_code_block)
		{
			std::wstring& code = _param(0);
			std::wcout << "code: " << code << std::endl;
		}
		END_ACTION

		BEGIN_ACTION(on_comment_block)
		{
			std::wstring& comment = _param(0);

			const static std::wstring code_generation_tag (L"[[[code");
			const static std::wstring prologue_tag        (L"[[[prologue");
			const static std::wstring epilogue_tag        (L"[[[epilogue");
			const static std::wstring signature_tag       (L"[[[signature");

			if(comment.compare(0, code_generation_tag.length(), code_generation_tag) == 0)
			{
				if(!_local(0)) { detail::generateDefaultPrologue(); _local(0) = true; }
				if(!_local(1)) { detail::generateDefaultSignature(); detail::generateDefaultCodeGenBegin(); _local(1) = true; }

				if(!_local(2))
				{
					GeneratorContext::instance()->buffer << (comment.c_str() + code_generation_tag.length());
				}
				else
				{
					// epilogue has been emited, the code block is ignored, throw out some warning here
				}
			}
			else if(comment.compare(0, signature_tag.length(), signature_tag) == 0)
			{
				if(!_local(0)) { detail::generateDefaultPrologue(); _local(0) = true; }
				if(!_local(1))
				{
					GeneratorContext::instance()->buffer << (comment.c_str() + code_generation_tag.length());
					detail::generateDefaultCodeGenBegin();
				}
				else
				{
					// the signautre has been generated, the signature block is ignored
				}
			}
			else if(comment.compare(0, prologue_tag.length(), prologue_tag) == 0)
			{
				if(!_local(0) /*prologue_generated*/)
				{
					GeneratorContext::instance()->buffer << (comment.c_str() + prologue_tag.length()) << std::endl;
					detail::generateDefaultPrologue();
					_local(0) = true;
				}
				else
				{
					// the prologue has been generated, the prologue block is ignored
				}
			}
			else if(comment.compare(0, epilogue_tag.length(), epilogue_tag) == 0)
			{
				if(!_local(2) /*epilogue_generated*/)
				{
					detail::generateDefaultCodeGenEnd();
					GeneratorContext::instance()->buffer << (comment.c_str() + epilogue_tag.length()) << std::endl;
					_local(2) = true;
				}
				else
				{
					// the epilogue has been generated, the epilogue block is ignored
				}
			}
		}
		END_ACTION
	};
};

} } }

#endif /* ZILLIANS_CG_ACTION_CPPACTION_H_ */
