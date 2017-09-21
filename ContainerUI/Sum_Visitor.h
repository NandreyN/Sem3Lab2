#pragma once
#include "Base_Visitor.h"
#include "LDeque.h"
#include "ADeque.h"

template <class T>
class Sum_Visitor : public Base_Visitor<T>
{
	T sum;
public:
	Sum_Visitor() { sum = 0; };
	T getSum() const { return sum; };
	void visit(LDeque<T>& d) override { sum = 0;for (auto it = d.begin(); it != d.end(); ++it) { sum += *it; } }//for (auto it = d.begin(); it != d.end(); ++it) { sum += *it; } };
	void visit(ADeque<T>& d) override { sum = 0; for (auto i = 0; i < d.size(); i++) { sum += d[i]; } }
};
