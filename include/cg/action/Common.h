/**
 * Zillians
 * Copyright (C) 2011-2012 Zillians.com
 * For more information see http://www.zillians.com
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
			{

#define BEGIN_TEMPLATED_ACTION(name, ...) \
		template<__VA_ARGS__> \
		struct name \
		{ \
			template<typename ParserAttribute, typename ParserContext> \
			void operator()(ParserAttribute& parser_attribute, ParserContext& context, bool& passed) const \
			{

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

namespace zillians { namespace cg { namespace action { namespace detail {

template<typename T>
struct is_fusion_vector : boost::is_base_of<boost::fusion::sequence_base<T>, T>
{ };

template<typename T>
struct is_std_vector : boost::mpl::false_
{ };

template<typename _Tp, typename _Alloc>
struct is_std_vector<std::vector<_Tp, _Alloc>> : boost::mpl::true_
{ };

template<int N, typename Attribute, bool IsFusionVector>
struct attribute_accessor_impl;

template<int N, typename Attribute>
struct attribute_accessor_impl<N, Attribute, true>
{
	typedef typename boost::add_reference<
	            typename boost::mpl::at_c<typename Attribute::types, N>::type
	        >::type result_type;

	static result_type get(Attribute& attribute)
	{
		return boost::fusion::at_c<N>(attribute);
	}
};

template<int N, typename Attribute>
struct attribute_accessor_impl<N, Attribute, false>
{
	typedef typename boost::add_reference<Attribute>::type result_type;
	static result_type get(Attribute& attribute)
	{
		BOOST_MPL_ASSERT(( boost::mpl::bool_<N == 0> ));
		return attribute;
	}
};

template<int N, typename Attribute>
struct attribute_accessor
{
	typedef typename attribute_accessor_impl<N, Attribute, is_fusion_vector<Attribute>::value>::result_type result_type;
	static result_type get(Attribute& attribute)
	{
		return attribute_accessor_impl<N, Attribute, is_fusion_vector<Attribute>::value>::get(attribute);
	}
};

} } } }

#endif /* ZILLIANS_CG_ACTION_COMMON_H_ */
