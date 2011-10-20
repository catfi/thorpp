/**
 * Zillians
 * Copyright (C) 2011-2012 Zillians.com
 * For more information see http://www.zillians.com
 */

#ifndef ZILLIANS_SINGLETON_H_
#define ZILLIANS_SINGLETON_H_

#include "cg/Prerequisite.h"
#include <boost/assert.hpp>

namespace zillians {

/**
 * Template parameter for Singleton class
 *
 * For manual singleton initialization, you have to make instance of the singleton class
 * (by calling new operator) as well as destroy the instance of it (by calling delete
 * operator). This is inspired by OGRE to allow program to control the object construction
 * order and destruction order manually.
 *
 * On the other hand, for automatic singleton initialization, the instance is created
 * if necessary whenever the Singleton::instance() is called, and is destroyed when
 * program exit.
 *
 * @see Singleton
 */
struct SingletonInitialization
{
	enum type
	{
		manual		= 0,
		automatic	= 1
	};
};


/**
 * Simple implementation of Singleton Pattern
 *
 * Inspired by OGRE, the Singleton class can be created once, and access through instance() method
 * NOTE: have potential problem when linking against dynamic library (nothing said that...)
 * NOTE: this is NOT thread-safe because the Singleton object can be created in two different threads in the same time
 */
template <class T, SingletonInitialization::type Init = SingletonInitialization::manual>
class Singleton
{
	template<typename V, SingletonInitialization::type F>
	struct CreateDelegate;

	template<typename V>
	struct CreateDelegate<V, SingletonInitialization::automatic>
	{
		static void create(V** p)
		{
			*p = new V;
		}
	};

	template<typename V>
	struct CreateDelegate<V, SingletonInitialization::manual>
	{
		static void create(V** p)
		{ }
	};

public:
	Singleton()
	{
		BOOST_ASSERT(!mInstance);

#if defined( _MSC_VER ) && _MSC_VER < 1200
		int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
		mInstance = (T*)((int)this + offset);
#else
		mInstance = static_cast<T*>(this);
#endif
	}

	~Singleton()
	{
		if(Init)
		{
			delete mInstance; mInstance = NULL;
		}
	}

public:
	static T* instance()
	{
		if(Init == SingletonInitialization::automatic && !mInstance)
		{
			CreateDelegate<T, Init>::create(&mInstance);
			//mInstance = new T;
		}

		return mInstance;
	}

private:
	static T* mInstance;
};

template <typename T, SingletonInitialization::type Init> T* Singleton<T, Init>::mInstance = NULL;

}

#endif/*ZILLIANS_SINGLETON_H_*/
