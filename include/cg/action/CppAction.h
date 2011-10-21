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

			// ignore any useless code block before first useful code block
			// this is done to allow spaces between prologue block and signature block
			if(!_local(0) || !_local(1))
			{
				std::wstring dummy = code;
				boost::trim(dummy);
				if(dummy.length() == 0)
					return;
			}

			if(!_local(0)) { detail::generateDefaultPrologue(); _local(0) = true; }
			if(!_local(1)) { detail::generateDefaultSignature(); detail::generateDefaultCodeGenBegin(); _local(1) = true; }

			std::wstringstream& buffer = GeneratorContext::instance()->buffer;
			buffer << L"\tstd::wcout <<\n\t\"";
			for(int i = 0; i < code.length(); ++i)
			{
				wchar_t c = code.at(i);
				if(c == L'\n')
				{
					buffer << L"\\n\"\n\t\"";
				}
				else if(c == L'\"')
				{
					buffer << L"\\\"";
				}
				else if(c == L'\'')
				{
					buffer << L"\\'";
				}
				else if(c == L'\\')
				{
					buffer << L"\\\\";
				}
				else
				{
					buffer << c;
				}
			}
			buffer << L"\";\n";
		}
		END_ACTION

		BEGIN_ACTION(on_comment_block)
		{
			std::wstring& comment = _param(0);

			const static std::wstring code_generation_tag (L"[[[code");
			const static std::wstring code_generation_include_tag (L"[[[code_include");
			const static std::wstring prologue_tag        (L"[[[prologue");
			const static std::wstring epilogue_tag        (L"[[[epilogue");
			const static std::wstring signature_tag       (L"[[[signature");

			if(comment.compare(0, code_generation_tag.length(), code_generation_tag) == 0)
			{
				if(!_local(0)) { detail::generateDefaultPrologue(); _local(0) = true; }
				if(!_local(1)) { detail::generateDefaultSignature(); detail::generateDefaultCodeGenBegin(); _local(1) = true; }

				if(!_local(2))
				{
					if(comment.compare(0, code_generation_include_tag.length(), code_generation_include_tag) == 0)
					{
						std::wstring s(comment.c_str() + code_generation_include_tag.length());
						boost::trim(s);
						boost::filesystem::path p(s);

						std::ifstream in;
						boost::filesystem::path in_p;
						if(p.is_absolute())
						{
							in_p = p;
							in.open(p.c_str(), std::ios_base::in);
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
					else
					{
						GeneratorContext::instance()->buffer << (comment.c_str() + code_generation_tag.length()) << std::endl;
					}
				}
				else
				{
					// throw out some warning here
					std::cerr << "Warn: epilogue has been emitted, the epilogue block is ignored" << std::endl;
				}
			}
			else if(comment.compare(0, signature_tag.length(), signature_tag) == 0)
			{
				if(!_local(0)) { detail::generateDefaultPrologue(); _local(0) = true; }
				if(!_local(1))
				{
					std::wstring signature(comment.c_str() + signature_tag.length());
					boost::trim(signature);

					GeneratorContext::instance()->buffer << signature << std::endl;
					detail::generateDefaultCodeGenBegin();
					_local(1) = true;
				}
				else
				{
					// throw out some warning here
					std::cerr << "Warn: signautre has been emitted, the signautre block is ignored" << std::endl;
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
					// throw out some warning here
					std::cerr << "Warn: prologue has been emitted, the prologue block is ignored" << std::endl;
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
					// throw out some warning here
					std::cerr << "Warn: epilogue has been emitted, the epilogue block is ignored" << std::endl;
				}
			}
		}
		END_ACTION
	};
};

} } }

#endif /* ZILLIANS_CG_ACTION_CPPACTION_H_ */
