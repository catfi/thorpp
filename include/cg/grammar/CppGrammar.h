/**
 * Zillians MMO
 * Copyright (C) 2007-2011 Zillians.com, Inc.
 * For more information see http://www.zillians.com
 *
 * Zillians MMO is the library and runtime for massive multiplayer online game
 * development in utility computing model, which runs as a service for every
 * developer to build their virtual world running on our GPU-assisted machines.
 *
 * This is a close source library intended to be used solely within Zillians.com
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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

#define DECL_RULE(x) qi::rule<Iterator, typename SA::x::attribute_type, detail::WhiteSpace<Iterator>, typename SA::x::local_type> x

namespace zillians { namespace cg { namespace grammar {

template <typename Iterator>
struct NonWhiteSpace : qi::grammar<Iterator>
{

};

template <typename Iterator, typename SA>
struct CppGrammar : qi::grammar<Iterator>
{
	CppGrammar() : CppGrammar::base_type(start)
	{
//		comment_c_style = qi::lexeme[L"/*" > *(unicode::char_ - L"*/") > L"*/"];
//		comment_c_style.name("comment_in_c_style");
//
//		comment_cpp_style = qi::lexeme[L"//" > *(unicode::char_ - qi::eol) > qi::eol];
//		comment_cpp_style.name("comment_in_cpp_style");
//
		start
			= unicode::space    // tab/space/cr/lf
//			| comment_c_style   // c-style comment "/* */"
//			| comment_cpp_style // cpp-style comment "//"
			;

		start.name("cpp_source");
	}


	DECL_RULE(start);

	qi::rule<Iterator> comment_c_style;
	qi::rule<Iterator> comment_cpp_style;
};

} } }

#endif /* ZILLIANS_CG_GRAMMAR_CPPGRAMMAR_H_ */
