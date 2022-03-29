#pragma once
#ifndef MYTINYSTL_ITERATOR_H_
#define MYTINYSTL_ITERATOR_H_

#include <cstddef>
#include "type_traits.h"

namespace mystl
{

	//五种迭代器类型
	struct input_iterator_tag { };
	struct output_iterator_tag { };
	struct forward_iterator_tag : public input_iterator_tag { };
	struct bidirectional_iterator_tag : public forward_iterator_tag { };
	struct random_access_iterator_tag : public bidirectional_iterator_tag { };

	//iterator 模板
	template <class Category, class T, class Distance = ptrdiff_t,
		class Pointer = T *, class Reference = T &>
	struct iterator
	{
		typedef Category iterator_category;
		typedef T value_type;
		typedef Pointer pointer;
		typedef Reference reference;
		typedef Distance difference_type;
	};

	//iterator traits

	//判断一个类型T是否定义了iteartor_category
	template <class T>
	struct has_iterator_cat
	{
	private:
		//为了和char的大小区别开，two的大小为2
		struct two
		{
			char a;
			char b;
		};
		template <class U>
		static two test(...);
		template <class U>
		static char test(typename U::iterator_category * = 0);

	public:
		//如果T定义了iterator_category类型，则下面调用返回char类型的test
		//此时sizeof返回值和sizeof(char)相等，则value为true，否则为false
		static const bool value = sizeof(test<T>(0)) == sizeof(char);
	};
	
	template <class Iterator, bool>
	struct iterator_traits_impl{ };

	template <class Iterator>
	struct iterator_traits_impl<Iterator, true>
	{
		typedef typename Iterator::iterator_category iterator_category;
		typedef typename Iterator::value_type value_type;
		typedef typename Iterator::pointer pointer;
		typedef typename Iterator::reference reference;
		typedef typename Iterator::difference_type difference_type;
	};

	//不能隐式转换为输入指针或者输出指针
	template <class Iterator, bool>
	struct iterator_traits_helper
	{
	};


	template <class Iterator>
	struct iterator_traits_helper<Iterator, true>
		: public iterator_traits_impl<Iterator,
		std::is_convertible<typename Iterator::iterator_category, input_iterator_tag>::value ||
		std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value>
	{
	};

	//萃取迭代器的特性
	template <class Iterator>
	struct iterator_traits
		: public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value>
	{
	};

	//针对原生指针的偏特化版本
	template <class T>
	struct iterator_traits<T *>
	{
		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef T *pointer;
		typedef T &reference;
		typedef ptrdiff_t difference_type;
	};

	//针对原生const指针的偏特化版本
	template <class T>
	struct iterator_traits<const T*>
	{
		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef const T *pointer;
		typedef const T &reference;
		typedef ptrdiff_t difference_type;
	};


	//T类型迭代器能隐式转换为U类型迭代器
	template <class T, class U, bool = has_iterator_cat<iterator_traits<T>>:value>
	struct has_iterator_cat_of
		: public m_bool_constant<std::is_convertible<
		typename iterator_traits<T>::iterator_category, U>::value>
	{
	};


	//T类型迭代器不能隐式转换为U类型迭代器
	template <class T, class U>
	struct has_iterator_cat_of<T, U, false> : public m_false_type
	{
	};

	//判断是否为input_iteartor类别迭代器
	template <class Iter>
	struct is_input_iterator : public has_iterator_cat_of<Iter, input_iterator_tag>
	{
	};

	//判断是否为output_iteartor类别迭代器
	template <class Iter>
	struct is_output_iterator : public has_iterator_cat_of<Iter, output_iterator_tag>
	{
	};

	//判断是否为forward_iteartor类别迭代器
	template <class Iter>
	struct is_forward_iteartor : public has_iterator_cat_of<Iter, forward_iterator_tag>
	{
	};

	//判断是否为bidirectional_iteartor类别迭代器
	template <class Iter>
	struct is_bidirectional_iterator : public has_iterator_cat_of<Iter, bidirectional_iterator_tag>
	{
	};

	//判断是否为random_access_iteartor类别迭代器
	template <class Iter>
	struct is_random_access_iterator : public has_iterator_cat_of<Iter, random_access_iterator_tag>
	{
	};

	//判断是否为迭代器，所有迭代器都是从input_iterator和output_iterator派生来的
	template <class Iterator>
	struct is_iterator : public m_bool_constant<is_input_iterator<Iterator>::value ||
		is_output_iterator<Iterator>::value>
	{
	};

	// 萃取某个迭代器的 category
	template <class Iterator>
	typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator &)
	{
		typedef typename iterator_traits<Iterator>::iterator_category Category;
		return Category();
	}


	//萃取某个迭代器的 distance_type
	template <class Iterator>
	typename iterator_traits<Iterator>::difference_type * distance_type(const Iterator &)
	{
		return static_cast<typename iterator_traits<Iterator>::difference_type *>(0);
	}
	

	//萃取某个迭代器的 value_type
	template <class Iterator>
	typename iterator_traits<Iterator>::value_type * value_type(const Iterator &)
	{
		return static_cast<typename iterator_traits<Iterator>::value_type *>(0);
	}

	// 以下函数用于计算迭代器间的距离

	// distance 的 input_iterator_tag 的版本
	template <class InputIterator>
	typename iterator_traits<InputIterator>::difference_type 
		distance_dispatch(InputIterator first, InputIterator last, input_iterator_tag)
	{
		typename iterator_traits<InputIterator>::difference_type n = 0;
		while (first != last)
		{
			++first;
			++n;
		}
		return n;
	}

	// distance 的 random_access_iterator_tag 的版本
	template <class RandomIter>
	typename iterator_traits<RandomIter>::difference_type 
		distance_dispatch(RandomIter first, RandomIter last, random_access_iterator_tag)
	{
		return last - first;
	}


	template <class InputIterator>
	typename iterator_traits<InputIterator>::difference_type
		distance(InputIterator first, InputIterator last)
	{
		return distance_dispatch(first.last, iterator_category(first));
	}


	// 以下函数用于让迭代器前进 n 个距离

	// advance 的 input_iterator_tag 的版本
	template <class InputIterator, class Distance>
	void advance_dispatch(InputIterator &i, Distance n, input_iterator_tag)
	{
		while (n--)
		{
			++i;
		}
	}


	// advance 的 bidirectional_iterator_tag 的版本
	template <class BidirectionalIterator, class Distance>
	void advance_dispatch(BidirectionalIterator &i, Distance n, bidirectional_iterator_tag)
	{
		if (n >= 0) {
			while (n--)
			{
				++i;
			}
		}
		else
		{
			while (n++)
			{
				--i;
			}
		}
	}

	// advance 的 random_iterator_tag 的版本
	template <class RandomIter, class Distance>
	void advance_dispatch(RandomIter &i, Distance n, random_access_iterator_tag)
	{
		i += n;
	}


	template <clss InputIterator, class Distance>
	void advance(InputIterator &i, Distance n) 
	{
		advance_dispatch(i, n, iterator_category(i));
	}

	/****************************************************************************/


	// 模板类 : reverse_iterator
	// 代表反向迭代器，使前进为后退，后退为前进


}









#endif // !MYTINYSTL_ITERATOR_H_
