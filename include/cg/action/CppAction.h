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

static void escapeToBuffer(std::wstringstream& buffer, const std::wstring& str, std::wstring::size_type from = 0, std::wstring::size_type to = std::wstring::npos)
{
	buffer << L"\tstd::wcout <<\n\t\t\"";
	{
		for(auto i = from; i < to && i < str.length(); ++i)
		{
			wchar_t c = str.at(i);
			switch(c)
			{
			case L'\n':
			case L'\r':
				buffer << L"\\n\"\n\t\t\""; break;
			case L'\"':
				buffer << L"\\\""; break;
			case L'\'':
				buffer << L"\\'"; break;
			case L'\\':
				buffer << L"\\\\"; break;
			default:
				buffer << c; break;
			}
		}
	}
	buffer << L"\";\n";
}

static void generateDefaultEntry()
{
	GeneratorContext* ctx = GeneratorContext::instance();

	ctx->state.transforms.push_back([=]{
		ctx->buffer_composed <<
				L"int main(int argc, char** argv) {\n";

		ctx->buffer_composed << ctx->buffer_driver.str();

		ctx->buffer_composed <<
				L"\treturn 0;"
				L"}\n";
	});
}

static void generateDefaultGlobal()
{
	GeneratorContext* ctx = GeneratorContext::instance();

	ctx->state.transforms.push_back([=]{
		ctx->buffer_composed <<
				L"#include <iostream>\n"
				L"\n";
	});
}

static void generateInclude(std::wstring& filename, bool static_or_dynamic)
{
	filename = filename.substr(1, filename.length() - 2);
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
		if(static_or_dynamic)
		{
			std::wstringstream ss; ss << in.rdbuf();
			escapeToBuffer(GeneratorContext::instance()->buffer_driver, ss.str());
		}
		else
		{
			GeneratorContext::instance()->buffer_driver << in.rdbuf();
		}
	}
	else
	{
		std::wcerr << "Error: failed to open include file: " << in_p << std::endl;
	}
}

static void generateOnEntryTag(GeneratorTag* tag, GeneratorBlock* block)
{
	if(tag->options)
	{
		if(tag->options->opcode == GeneratorOptions::OpCode::Auto)
		{
			detail::generateDefaultEntry();
		}
		else
		{
			std::wcerr << L"Error: invalid opcode \"" << GeneratorOptions::OpCode::toString(tag->options->opcode) << L"\" for entry tag, ignored" << std::endl;
		}
	}
	else
	{
		GeneratorContext* ctx = GeneratorContext::instance();
		std::wstring& block_ref = block->block;
		ctx->state.transforms.push_back([=]{
			ctx->buffer_composed << block_ref;
		});
	}
}

static void generateOnDriverTag(GeneratorTag* tag, GeneratorBlock* block)
{
	GeneratorContext* ctx = GeneratorContext::instance();
	ctx->state.transforms.push_back([=]{
		ctx->buffer_composed << ctx->buffer_driver.str();
	});

	if(tag->options)
	{
		std::wcerr << L"Error: invalid opcode \"" << GeneratorOptions::OpCode::toString(tag->options->opcode) << L"\" for driver tag, ignored" << std::endl;
	}

	if(block)
	{
		boost::trim(block->block);
		if(block->block.length() > 0)
			std::wcerr << L"Warn: code enclosed by driver tag is ignored" << std::endl;
	}
}

static void generateOnCodeTag(GeneratorTag* tag, GeneratorBlock* block)
{
	if(tag->options)
	{
		if(tag->options->opcode == GeneratorOptions::OpCode::Include)
		{
			for(auto i = tag->options->list->data.begin(); i != tag->options->list->data.end(); ++i)
			{
				if((*i)->key == L"file")
				{
					detail::generateInclude((*i)->value, false);
				}
			}
		}
		else
		{
			std::wcerr << L"Error: invalid opcode \"" << GeneratorOptions::OpCode::toString(tag->options->opcode) << L"\" for code tag, ignored" << std::endl;
		}
	}

	if(block)
	{
		GeneratorContext::instance()->buffer_driver << block->block;
	}
}

static void generateOnGenTag(GeneratorTag* tag, GeneratorBlock* block)
{
	GeneratorContext* ctx = GeneratorContext::instance();

	if(tag->options)
	{
		if(tag->options->opcode == GeneratorOptions::OpCode::Replace)
		{
			if(!block) return;

			std::wstringstream& buffer = ctx->buffer_driver;
			std::wstring& block_str = block->block;

			std::wstring::size_type prev_pos = 0;
			while(prev_pos < block_str.length())
			{
				std::wstring::size_type next_pos = std::wstring::npos;
				std::wstring matched_key;
				std::wstring matched_value;

				for(auto i = tag->options->list->data.begin(); i != tag->options->list->data.end(); ++i)
				{
					std::wstringstream ss; ss << L"${" << (*i)->key << L"}";
					std::wstring::size_type t = block_str.find(ss.str(), prev_pos);
					if(t != std::wstring::npos && t < next_pos)
					{
						next_pos = t;
						matched_key = ss.str();
						matched_value = (*i)->value;
					}
				}

				detail::escapeToBuffer(buffer, block_str, prev_pos, next_pos);

				if(next_pos == std::wstring::npos)
					break;

				buffer << L"\tstd::wcout << " << matched_value << L";\n";

				prev_pos = next_pos + matched_key.length();
			}
		}
		else if(tag->options->opcode == GeneratorOptions::OpCode::Include)
		{
			for(auto i = tag->options->list->data.begin(); i != tag->options->list->data.end(); ++i)
			{
				if((*i)->key == L"file")
				{
					detail::generateInclude((*i)->value, true);
				}
			}

			if(block)
			{
				boost::trim(block->block);
				if(block->block.length() > 0)
					std::wcerr << L"Warn: code enclosed by driver tag is ignored" << std::endl;
			}
		}
		else
		{
			std::wcerr << L"Error: invalid opcode \"" << GeneratorOptions::OpCode::toString(tag->options->opcode) << L"\" for gen tag, ignored" << std::endl;
		}
	}
	else
	{
		if(!block) return;

		detail::escapeToBuffer(ctx->buffer_driver, block->block);
	}
}

}

struct CppAction
{
	struct opcode
	{
		DEFINE_ATTRIBUTES(GeneratorOptions::OpCode::type)
		DEFINE_LOCALS()

		BEGIN_TEMPLATED_ACTION(init, GeneratorOptions::OpCode::type Op)
		{
			_result = Op;
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
			if(_param(1).is_initialized())
				_result = new GeneratorOptions(_param(0), *_param(1));
			else
				_result = new GeneratorOptions(_param(0), NULL);

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
			ctx->state.global_generated = false;

			// append the outer-most generate tag
			ctx->state.tag_stack.push(new GeneratorTag(L"gen", NULL));
		}
		END_ACTION

		BEGIN_ACTION(fini)
		{
			GeneratorContext* ctx = GeneratorContext::instance();

			if(!ctx->state.global_generated)
			{
				detail::generateDefaultGlobal();
				detail::generateDefaultEntry();
				ctx->state.global_generated = true;
			}

			if(ctx->state.tag_stack.size() != 0)
			{
				// apply all transforms
				for(auto i = ctx->state.transforms.begin(); i != ctx->state.transforms.end(); ++i)
					(*i)();
			}
			else
			{
				// unmatched opening/closing tags, the code generation is not complete
				std::wcerr << L"Error: unmatched opening/closing tags, the code generation is not complete" << std::endl;
			}
		}
		END_ACTION

		BEGIN_ACTION(on_tag_global)
		{
			GeneratorContext* ctx = GeneratorContext::instance();
			if(!_param(0)->closed)
				ctx->state.tag_stack.push(_param(0));
			else
				std::wcerr << L"Warn: useless global tag" << std::endl;
		}
		END_ACTION

		BEGIN_ACTION(on_tag_entry)
		{
			if(_param(0)->closed)
				detail::generateOnEntryTag(_param(0), NULL);
			else
				GeneratorContext::instance()->state.tag_stack.push(_param(0));
		}
		END_ACTION

		BEGIN_ACTION(on_tag_driver)
		{
			if(_param(0)->closed)
				detail::generateOnDriverTag(_param(0), NULL);
			else
				GeneratorContext::instance()->state.tag_stack.push(_param(0));
		}
		END_ACTION

		BEGIN_ACTION(on_tag_gen)
		{
			if(_param(0)->closed)
				detail::generateOnGenTag(_param(0), NULL);
			else
				GeneratorContext::instance()->state.tag_stack.push(_param(0));
		}
		END_ACTION

		BEGIN_ACTION(on_tag_code)
		{
			if(_param(0)->closed)
				detail::generateOnCodeTag(_param(0), NULL);
			else
				GeneratorContext::instance()->state.tag_stack.push(_param(0));
		}
		END_ACTION

		BEGIN_ACTION(on_tag_close)
		{
			GeneratorContext* ctx = GeneratorContext::instance();
			if(ctx->state.tag_stack.size() > 0)
			{
				if(ctx->state.tag_stack.top()->tag == L"global")
				{
					ctx->state.global_generated = true;
				}

				ctx->state.tag_stack.pop();
			}
			else
			{
				// unmatched opening/closing tags, the code generation is not complete
				std::wcerr << L"Error: unmatched opening/closing tags, the code generation is not complete" << std::endl;
			}
		}
		END_ACTION

		BEGIN_ACTION(on_block)
		{
			GeneratorContext* ctx = GeneratorContext::instance();

			if(ctx->state.tag_stack.size() == 0)
			{
				std::wcerr << L"Error: unmatched opening/closing tags, the code generation is not complete" << std::endl;
				return;
			}

			if(ctx->state.tag_stack.top()->tag == L"global")
			{
				if(!ctx->state.global_generated)
				{
					GeneratorTag* tag = ctx->state.tag_stack.top();
					if(tag->options)
					{
						std::wcerr << L"Error: invalid opcode \"" << GeneratorOptions::OpCode::toString(tag->options->opcode) << L"\" for global tag, ignored" << std::endl;
					}

					std::wstring& block_ref = _param(0)->block;
					ctx->state.transforms.push_back([=]{
						ctx->buffer_composed << block_ref;
					});
				}
			}
			else if(ctx->state.tag_stack.top()->tag == L"entry")
			{
				detail::generateOnEntryTag(ctx->state.tag_stack.top(), _param(0));
			}
			else if(ctx->state.tag_stack.top()->tag == L"driver")
			{
				detail::generateOnDriverTag(ctx->state.tag_stack.top(), _param(0));
			}
			else if(ctx->state.tag_stack.top()->tag == L"code")
			{
				detail::generateOnCodeTag(ctx->state.tag_stack.top(), _param(0));
			}
			else if(ctx->state.tag_stack.top()->tag == L"gen")
			{
				detail::generateOnGenTag(ctx->state.tag_stack.top(), _param(0));
			}
			else
			{
				std::wcerr << L"Error: invalid tag found: " << ctx->state.tag_stack.top()->tag << std::endl;
			}
		}
		END_ACTION
	};
};

} } }

#endif /* ZILLIANS_CG_ACTION_CPPACTION_H_ */
