#pragma once
#include "LDeque.h"

template <class U>
class Model
{
	LDeque<U> deque;
	LDeque<U> srcDeque;
	vector<U> parseString(const string& s);
	bool isStringOK(const string&) const;
public:
	Model();
	Model(const LDeque<U>&, const LDeque<U>&);
	void setFirst(const LDeque<U>&);
	void setFirst(const string&);
	void setSecond(const LDeque<U>&);
	void setSecond(const string&);

	void clear();
	void push_back_string(const string&);
	void push_front_string(const string&);
	U pop_front();
	U pop_back();

	string firstToString() const;
	string secondToString() const;
	void swapDeques();
};

template <class U>
vector<U> Model<U>::parseString(const string& s)
{
	vector<U> vect;
	istringstream oss(s);
	while (!oss.eof())
	{
		U elem;
		oss >> elem;
		vect.push_back(elem);
	}
	return vect;
}

template <class U>
bool Model<U>::isStringOK(const string& s) const
{
	if (s.empty()) return false;
	for (char c : s)
		if (!isdigit(c) && c != ' ') return false;
	return true;
}

template <class U>
Model<U>::Model()
{
	deque = LDeque<U>();
	srcDeque = LDeque<U>();
}

template <class U>
Model<U>::Model(const LDeque<U>& ld1, const LDeque<U>& ld2)
{
	deque = ld1;
	srcDeque = ld2;
}

template <class U>
void Model<U>::setFirst(const LDeque<U>& ld)
{
	deque = ld;
}

template<class U>
inline void Model<U>::setFirst(const string & s)
{
	deque = LDeque<U>(s);
}

template<class U>
inline void Model<U>::setSecond(const string & s)
{
	srcDeque = LDeque<U>(s);
}


template <class U>
void Model<U>::setSecond(const LDeque<U>& ld)
{
	srcDeque= ld;
}

template <class U>
void Model<U>::clear()
{
	deque.clear();
}

template <class U>
void Model<U>::push_back_string(const string& s)
{
	auto vect = parseString(s);
	for (auto c : vect)
		deque.push_back(c);
}

template <class U>
void Model<U>::push_front_string(const string& s)
{
	auto vect = parseString(s);
	for (auto c : vect)
		deque.push_front(c);
}

template <class U>
U Model<U>::pop_front()
{
	return deque.pop_front();
}

template <class U>
U Model<U>::pop_back()
{
	return deque.pop_back();
}

template <class U>
string Model<U>::firstToString() const
{
	return deque.toString();
}

template <class U>
string Model<U>::secondToString() const
{
	return srcDeque.toString();
}

template <class U>
void Model<U>::swapDeques()
{
	deque.swap(srcDeque);
}