#pragma once
template<class U>
class LDeque;

template<class U>
class ADeque;

template<class T>
class Base_Visitor
{
public:
	virtual void visit(LDeque<T>&) = 0;
	virtual void visit(ADeque<T>&) = 0;

	virtual ~Base_Visitor() = default;
};