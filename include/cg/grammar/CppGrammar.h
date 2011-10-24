/**
 * Zillians
 * Copyright (C) 2011-2012 Zillians.com
 * For more information see http://www.zillians.com
 */

#ifndef ZILLIANS_CG_GRAMMAR_CPPGRAMMAR_H_
#define ZILLIANS_CG_GRAMMAR_CPPGRAMMAR_H_

#include "cg/PrerequisiteBoost.h"

namespace qi = boost::spirit::qi;
namespace unicode = boost::spirit::unicode;
using boost::spirit::repository::distinct;
using boost::spirit::ascii::no_case;
using boost::spirit::repository::qi::iter_pos;
using boost::spirit::omit;

#define DECL_RULE(x) qi::rule<Iterator, typename SA::x::attribute_type, typename SA::x::local_type> x

namespace zillians { namespace cg { namespace grammar {

namespace detail {

template <typename Iterator, typename SA>
struct Identifier : qi::grammar<Iterator, std::wstring()>
{
	Identifier() : Identifier::base_type(start)
	{
		start %= qi::lexeme[ ((unicode::alpha | L'_') > *(unicode::alnum | L'_')) ];

		debug(start);
	}

	qi::rule<Iterator, std::wstring()> keyword;
	qi::rule<Iterator, std::wstring()> start;
};

}

template <typename Iterator, typename SA>
struct CppGrammar : qi::grammar<Iterator, typename SA::start::attribute_type, typename SA::start::local_type >
{
	CppGrammar() : CppGrammar::base_type(start)
	{
		opcode = qi::lit(L"replace") [ typename SA::opcode::init_replace() ]
		       | qi::lit(L"include") [ typename SA::opcode::init_include() ];

		parameter = (IDENTIFIER > L'=' > IDENTIFIER) [ typename SA::parameter::init() ];
		parameter_list %= qi::eps [typename SA::parameter_list::init() ]
				       >> parameter [ typename SA::parameter_list::append() ]
				       >> *(L"," > parameter [ typename SA::parameter_list::append() ] );

		options = (qi::eps >> L"(" > opcode > L":" > parameter_list > L")") [ typename SA::options::init() ];
		block = qi::lexeme[+(unicode::char_ - (qi::lit(L"//[[[") | qi::lit(L"//]]]")))] [ typename SA::block::init() ];

		tag_gen = (qi::lit(L"//[[[gen") > -options) [ typename SA::tag_gen::init() ];
		tag_code = (qi::lit(L"//[[[code") > -options) [ typename SA::tag_code::init() ];
		tag_close = qi::lit(L"//]]]") [ typename SA::tag_close::init() ];

		start = qi::eps [ typename SA::start::init() ]
				> *( block [ typename SA::start::on_block() ]
				   | tag_gen [ typename SA::start::on_tag_gen() ]
				   | tag_code [ typename SA::start::on_tag_code() ]
				   | tag_close [ typename SA::start::on_tag_close() ] )
				> qi::eoi [ typename SA::start::fini() ];

		opcode.name("opcode");
		parameter.name("parameter");
		parameter_list.name("parameter_list");
		options.name("options");
		block.name("block");
		tag_gen.name("tag_gen");
		tag_code.name("tag_code");
		tag_close.name("tag_close");
		start.name("start");

		debug(opcode);
		debug(parameter);
		debug(parameter_list);
		debug(options);
		debug(block);
		debug(tag_gen);
		debug(tag_code);
		debug(tag_close);
		debug(start);
	}

	detail::Identifier<Iterator, SA> IDENTIFIER;

	DECL_RULE(opcode);
	DECL_RULE(parameter);
	DECL_RULE(parameter_list);
	DECL_RULE(options);
	DECL_RULE(block);
	DECL_RULE(tag_gen);
	DECL_RULE(tag_code);
	DECL_RULE(tag_close);
	DECL_RULE(start);
};

} } }

#endif /* ZILLIANS_CG_GRAMMAR_CPPGRAMMAR_H_ */
