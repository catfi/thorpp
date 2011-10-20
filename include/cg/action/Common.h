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

#ifndef ZILLIANS_CG_ACTION_COMMON_H_
#define ZILLIANS_CG_ACTION_COMMON_H_

#include "cg/PrerequisiteBoost.h"
#include <boost/mpl/bool.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/add_reference.hpp>

#define DEFINE_ATTRIBUTES(return_type, ...) typedef return_type (attribute_type) (__VA_ARGS__);
#define DEFINE_LOCALS(...) typedef boost::spirit::locals<__VA_ARGS__> local_type;
#define DEFINE_ACTION(name) \
		struct name \
		{ \
			template<typename Attribute, typename Context> \
			void operator()(Attribute& parser_attribute, Context& context, bool& passed) const; \
		};

#define BEGIN_ACTION(name) \
		struct name \
		{ \
			template<typename ParserAttribute, typename ParserContext> \
			void operator()(ParserAttribute& parser_attribute, ParserContext& context, bool& passed) const \
			{ \
				if(!getParserContext().enable_semantic_action) \
					return;

#define BEGIN_TEMPLATED_ACTION(name, ...) \
		template<__VA_ARGS__> \
		struct name \
		{ \
			template<typename ParserAttribute, typename ParserContext> \
			void operator()(ParserAttribute& parser_attribute, ParserContext& context, bool& passed) const \
			{ \
				if(!getParserContext().enable_semantic_action) \
					return;

#define END_ACTION \
			} \
		};

#define _result   boost::fusion::at_c<0>(context.attributes)
#define _result_t decltype(boost::fusion::at_c<0>(context.attributes))

#define _param(i)   detail::attribute_accessor<i, ParserAttribute>::get(parser_attribute)
#define _param_t(i) typename detail::attribute_accessor<i, ParserAttribute>::result_type

#define _aux_param(i)   boost::fusion::at_c<i+1>(context.attributes)
#define _aux_param_t(i) decltype(boost::fusion::at_c<i+1>(context.attributes))

#define _local(i)   boost::fusion::at_c<i>(context.locals)
#define _local_t(i) decltype(boost::fusion::at_c<i>(context.locals))

#endif /* ZILLIANS_CG_ACTION_COMMON_H_ */
