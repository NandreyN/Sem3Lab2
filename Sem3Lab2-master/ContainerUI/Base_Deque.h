#pragma once
#include "Base_Visitor.h"

template<class Type>
class Base_Deque
{
public:
	virtual ~Base_Deque() = default;
	virtual void accept(Base_Visitor<Type>&) = 0;
};
