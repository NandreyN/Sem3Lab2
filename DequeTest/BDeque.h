#pragma once
#include <initializer_list>
#include<string>
#include<iostream>

// Base deque class
using namespace std;
template<class DataType>
class BDeque
{
public:
	~BDeque();

	

	BDeque();
	BDeque(std::initializer_list<DataType>);
	BDeque(BDeque&&);
	BDeque(const BDeque&);

	//virtual BDeque& operator=(BDeque&&);
	virtual ostream& operator<<(ostream& o);
	virtual istream& operator >> (istream& i);
	//virtual BDeque& operator+(const BDeque&);
	//virtual BDeque& operator+=(const BDeque&);
	//virtual bool operator==(const BDeque&);
	//virtual bool operator!=(const BDeque&);

	virtual bool isEmpty();
	virtual size_t size();
	virtual void clear();
	virtual void push_back(const DataType&);
	virtual void push_front(const DataType&);

	virtual iterator begin();
	virtual iterator end();

	virtual DataType pop_front();
	virtual DataType pop_back();

	

private:
	virtual void enlarge() = 0;
	size_t _currentCapacity;
};