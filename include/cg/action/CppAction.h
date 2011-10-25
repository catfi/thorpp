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

using namespace zillians::cg::context;

//#define CTX GeneratorContext::instance()

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
		DEFINE_ATTRIBUTES(GeneratorOptions::OpCode)
		DEFINE_LOCALS()

		BEGIN_ACTION(init_replace)
		{
			_result = GeneratorOptions::OpCode::Replace;
		}
		END_ACTION

		BEGIN_ACTION(init_include)
		{
			_result = GeneratorOptions::OpCode::Include;
		}
		END_ACTION
	};

	struct parameter
	{
		DEFINE_ATTRIBUTES(GeneratorParameter*)
		DEFINE_LOCALS()

		BEGIN_ACTION(init)
		{
			_result = new GeneratorParameter(_param(0), _param(1));
		}
		END_ACTION
	};

	struct parameter_list
	{
		DEFINE_ATTRIBUTES(GeneratorParameterList*)
		DEFINE_LOCALS()

		BEGIN_ACTION(init)
		{
			_result = new GeneratorParameterList();
		}
		END_ACTION

		BEGIN_ACTION(append)
		{
			_result->data.push_back(_param(0));
		}
		END_ACTION
	};

	struct options
	{
		DEFINE_ATTRIBUTES(GeneratorOptions*)
		DEFINE_LOCALS()

		BEGIN_ACTION(init)
		{
			_result = new GeneratorOptions(_param(0), _param(1));
		}
		END_ACTION
	};

	struct block
	{
		DEFINE_ATTRIBUTES(GeneratorBlock*)
		DEFINE_LOCALS()

		BEGIN_ACTION(init)
		{
			_result = new GeneratorBlock(_param(0));
		}
		END_ACTION
	};

	struct tag_global
	{
		DEFINE_ATTRIBUTES(GeneratorTag*)
		DEFINE_LOCALS()

		BEGIN_ACTION(init)
		{
			if(_param(0).is_initialized())
				_result = new GeneratorTag(L"global", *_param(0), _param(1).is_initialized());
			else
				_result = new GeneratorTag(L"global", NULL, _param(1).is_initialized());
		}
		END_ACTION
	};

	struct tag_default_entry
	{
		DEFINE_ATTRIBUTES(GeneratorTag*)
		DEFINE_LOCALS()

		BEGIN_ACTION(init)
		{
			if(_param(0).is_initialized())
				_result = new GeneratorTag(L"default_entry", *_param(0), _param(1).is_initialized());
			else
				_result = new GeneratorTag(L"default_entry", NULL, _param(1).is_initialized());
		}
		END_ACTION
	};

	struct tag_entry
	{
		DEFINE_ATTRIBUTES(GeneratorTag*)
		DEFINE_LOCALS()

		BEGIN_ACTION(init)
		{
			if(_param(0).is_initialized())
				_result = new GeneratorTag(L"entry", *_param(0), _param(1).is_initialized());
			else
				_result = new GeneratorTag(L"entry", NULL, _param(1).is_initialized());
		}
		END_ACTION
	};

	struct tag_driver
	{
		DEFINE_ATTRIBUTES(GeneratorTag*)
		DEFINE_LOCALS()

		BEGIN_ACTION(init)
		{
			if(_param(0).is_initialized())
				_result = new GeneratorTag(L"driver", *_param(0), _param(1).is_initialized());
			else
				_result = new GeneratorTag(L"driver", NULL, _param(1).is_initialized());
		}
		END_ACTION
	};

	struct tag_gen
	{
		DEFINE_ATTRIBUTES(GeneratorTag*)
		DEFINE_LOCALS()

		BEGIN_ACTION(init)
		{
			if(_param(0).is_initialized())
				_result = new GeneratorTag(L"gen", *_param(0), _param(1).is_initialized());
			else
				_result = new GeneratorTag(L"gen", NULL, _param(1).is_initialized());
		}
		END_ACTION
	};

	struct tag_code
	{
		DEFINE_ATTRIBUTES(GeneratorTag*)
		DEFINE_LOCALS()

		BEGIN_ACTION(init)
		{
			if(_param(0).is_initialized())
				_result = new GeneratorTag(L"code", *_param(0), _param(1).is_initialized());
			else
				_result = new GeneratorTag(L"code", NULL, _param(1).is_initialized());
		}
		END_ACTION
	};

	struct tag_close
	{
		DEFINE_ATTRIBUTES(GeneratorTag*)
		DEFINE_LOCALS()

		BEGIN_ACTION(init)
		{
			_result = new GeneratorTag(L"close", NULL);
		}
		END_ACTION
	};

	struct start
	{
		DEFINE_ATTRIBUTES(void)
		DEFINE_LOCALS()

		BEGIN_ACTION(init)
		{
			GeneratorContext* ctx = GeneratorContext::instance();
			ctx->state.prologue_generated = false;
			ctx->state.signature_generated = false;
			ctx->state.epilogue_generated = false;
		}
		END_ACTION

		BEGIN_ACTION(fini)
		{
			GeneratorContext* ctx = GeneratorContext::instance();

			if(!ctx->state.prologue_generated)  { detail::generateDefaultPrologue(); ctx->state.prologue_generated = true; }
			if(!ctx->state.signature_generated) { detail::generateDefaultSignature(); detail::generateDefaultCodeGenBegin(); ctx->state.signature_generated = true; }
			if(!ctx->state.epilogue_generated)  { detail::generateDefaultCodeGenEnd(); detail::generateDefaultEpilogue(); ctx->state.epilogue_generated = true; }

			if(ctx->state.tag_stack.size() != 0)
			{
				// TODO unmatched opening/closing tags, the code generation is not complete
			}
		}
		END_ACTION

		BEGIN_ACTION(on_block)
		{
//			std::wcout << _param(0)->data << std::endl;
		}
		END_ACTION

		BEGIN_ACTION(on_tag_global)
		{

		}
		END_ACTION

		BEGIN_ACTION(on_tag_default_entry)
		{

		}
		END_ACTION

		BEGIN_ACTION(on_tag_entry)
		{

		}
		END_ACTION

		BEGIN_ACTION(on_tag_driver)
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

		BEGIN_ACTION(on_tag_close)
		{
			GeneratorContext* ctx = GeneratorContext::instance();
			if(ctx->state.tag_stack.size() > 0)
			{
				ctx->state.tag_stack.pop();
			}
			else
			{
				// TODO unmatched opening/closing tags, the code generation is not complete
			}
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
