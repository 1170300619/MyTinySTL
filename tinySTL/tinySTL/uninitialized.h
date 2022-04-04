#pragma once
#ifndef MYTINYSTL_UNINITIALIZED_H_
#define MYTINYSTL_UNINITIALIZED_H_

#include "construct.h"
#include "iterator.h"
#include "type_traits.h"
#include "util.h"

namespace mystl
{
	/**************************************************************************/
	// uninitialized_copy
	// 把 [first, last) 上的内容复制到以 result 为起始处的空间，返回复制结束的位置
	/**************************************************************************/
	template <class InputIter, class ForwardIter>
	ForwardIter unchecked_uninit_copy(InputIter first, InputIter last, ForwardIter result, std::true_type)
	{
		
	}
}







#endif MYTINYSTL_UNINITIALIZED_H_