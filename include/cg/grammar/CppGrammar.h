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

template <typename Iterator, typename SA>
struct CppGrammar : qi::grammar<Iterator, typename SA::start::attribute_type, typename SA::start::local_type >
{
	CppGrammar() : CppGrammar::base_type(start)
	{
		code_block %= qi::lexeme[+(unicode::char_ - L"/*")];
		code_block.name("code_block");

		comment_block %= qi::lexeme[L"/*" > *(unicode::char_ - L"*/") > L"*/"];
		comment_block.name("comment_block");

		start = qi::eps [ typename SA::start::init() ]
				> *( code_block [ typename SA::start::on_code_block() ]
				   | comment_block [ typename SA::start::on_comment_block() ] )
				> qi::eoi [ typename SA::start::fini() ];

		start.name("cpp_source");

//		debug(code_block);
//		debug(comment_block);
//		debug(start);
	}

	qi::rule<Iterator, std::wstring() > code_block;
	qi::rule<Iterator, std::wstring() > comment_block;

	DECL_RULE(start);
};

} } }

#endif /* ZILLIANS_CG_GRAMMAR_CPPGRAMMAR_H_ */
