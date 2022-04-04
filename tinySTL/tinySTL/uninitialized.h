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
	// �� [first, last) �ϵ����ݸ��Ƶ��� result Ϊ��ʼ���Ŀռ䣬���ظ��ƽ�����λ��
	/**************************************************************************/
	template <class InputIter, class ForwardIter>
	ForwardIter unchecked_uninit_copy(InputIter first, InputIter last, ForwardIter result, std::true_type)
	{
		
	}
}







#endif MYTINYSTL_UNINITIALIZED_H_