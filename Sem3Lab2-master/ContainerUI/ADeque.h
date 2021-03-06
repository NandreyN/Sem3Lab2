#pragma once
#include <vector>
#include <sstream>
#include <algorithm>
#include <assert.h>
#include <queue>
#include "Base_Deque.h"

const size_t initialSize = 6;

using namespace std;

template<class DataType>
class ADeque : public Base_Deque<DataType>
{
public:

	class iterator
	{
	public:
		typedef iterator self_type;
		typedef DataType value;
		typedef DataType &reference;
		typedef DataType *pointer;
		typedef int difference_type;
		typedef std::forward_iterator_tag iterator_category;

		// Constructor init list:
		iterator(pointer p) : _ptr(p) {}
		// opeartors overload 
		self_type operator++() { self_type src = *this; ++_ptr; return src; };
		self_type operator++(int) { ++_ptr; return *this; };

		reference operator*() { return *_ptr; }
		pointer operator->() { return _ptr; }
		bool operator==(const self_type& rhs) { return _ptr == rhs._ptr; };
		bool operator!=(const self_type& rhs) { return _ptr != rhs._ptr; }
	private:
		pointer _ptr;
	};

	ADeque();//OK
	ADeque(std::initializer_list<DataType>); // OK
	ADeque(ADeque&&);//OK
	ADeque(const ADeque&);//OK
	ADeque(const string&);
	~ADeque();

	ADeque& operator=(ADeque&&);//OK
	ADeque& operator=(const ADeque&);//OK
	ADeque& operator+=(const ADeque&);

	friend ADeque operator+(const ADeque&, const ADeque&);

	template<class U>
	friend ostream& operator<<(ostream& o, const ADeque<U>&); // OK

	template<class U>
	friend istream& operator >> (istream& i, ADeque<U>&); // OK

	string getInternalInfo() const;

	bool operator==(const ADeque&) const;
	DataType operator[](int idx)const ;

	iterator begin() ;//OK
	iterator end() ;//OK

	string toString() const;

	void push_back(const DataType&); // OK
	void push_front(const DataType&); // OK
	DataType pop_front(); // OK
	DataType pop_back(); // OK
	DataType front() const;
	DataType back() const;

	void swap(ADeque<DataType>&); // OK
	
	// emplace
	template<class ... Args>
	void emplace(iterator position, Args&& ... args);

	void clear();//OK
	size_t size() const; // ok
	bool isEmpty() const; // ok

	void accept(Base_Visitor<DataType>&) override;
private:
	bool enoughSingleSpaceBack(int);
	bool enoughSingleSpaceFront(int);
	size_t getSpaceFront();
	size_t getSpaceBack();

	void enlarge();
	DataType* _deqData;
	size_t _sz, _headIdx, _tailIdx;
};



template <class DataType>
ADeque<DataType>::ADeque() :_deqData(new DataType[initialSize]), _sz(initialSize), _headIdx(_sz / 2 + 1), _tailIdx(_sz / 2)
{}

template <class DataType>
ADeque<DataType>::ADeque(std::initializer_list<DataType> initList) : _deqData(new DataType[initialSize]), _sz(initList.size())
{
	size_t idx = 0;
	_headIdx = 0;
	_tailIdx = _sz - 1;
	for (auto in : initList)
	{
		_deqData[idx] = in;
		idx++;
	}
}

template <class DataType>
ADeque<DataType>::ADeque(ADeque&& ad)
{
	*this = std::move(ad);
}

template <class DataType>
ADeque<DataType>::ADeque(const ADeque& ad)
{
	operator=(ad);
}

template <class DataType>
ADeque<DataType>::ADeque(const string& s)
{
	istringstream iss(s);
	vector<DataType> values;

	DataType t{};
	while (!iss.eof())
	{
		iss >> t;
		values.push_back(t);
	}

	_sz = values.size();
	_deqData = new DataType[_sz];
	_headIdx = 0;
	_tailIdx = _sz - 1;

	size_t idx = 0;
	for (DataType elem : values)
	{
		_deqData[idx] = elem;
		idx++;
	}
}

template <class DataType>
ADeque<DataType>::~ADeque()
{
	if (_deqData != nullptr)
		delete[] _deqData;
}

template <class DataType>
ADeque<DataType>& ADeque<DataType>::operator=(ADeque&& ad)
{
	if (this != &ad)
	{
		if (_sz == 0)
		{
			_deqData = new DataType[ad._sz];
			_sz = ad._sz;
		}

		_headIdx = ad._headIdx;
		_tailIdx = ad._tailIdx;

		for (size_t i = _headIdx; i <= _tailIdx; i++)
			_deqData[i] = ad._deqData[i];

		delete[] ad._deqData;
		ad._deqData = nullptr;
		ad._sz = 0;
		ad._headIdx = 0;
		ad._tailIdx = 0;
	}
	return *this;
}

template <class DataType>
ADeque<DataType>& ADeque<DataType>::operator=(const ADeque& ad)
{
	_deqData = new DataType[ad._sz];
	_sz = ad._sz;
	_headIdx = ad._headIdx;
	_tailIdx = ad._tailIdx;
	copy(ad._deqData, ad._deqData + _sz, _deqData);

	return *this;
}

template <class DataType>
ADeque<DataType>& ADeque<DataType>::operator+=(const ADeque& ad)
{
	for (auto it = ad.begin(); it != ad.end(); ++it)
		push_back(*it);
	return *this;
}

template <class T>
ostream& operator<<(ostream& o, const ADeque<T>& ad)
{
	if (ad.isEmpty()) return o;
	for (size_t i = ad._headIdx; i <= ad._tailIdx; i++)
		o << ad._deqData[i] << " ";
	return o;
}

template <class T>
istream& operator >> (istream& i, ADeque<T>& ad)
{
	string s;
	getline(i, s);
	ad = ADeque<T>(s);
	return i;
}

template <class DataType>
string ADeque<DataType>::getInternalInfo() const
{
	string info =
		"Space reserved : " + to_string(_sz) + "\r\n" +
		"Head idx : " + to_string(_headIdx) + "\r\n" +
		"Tail idx : " + to_string(_tailIdx) + "\r\n" +
		"Current size : " + to_string(size()) + "\r\n";
	return info;
}

template <class DataType>
bool ADeque<DataType>::operator==(const ADeque& ad) const
{
	if (_sz != ad.size()) return false;

	for (size_t i = 0; i < _sz; i++)
		if (_deqData[i] != ad._deqData[i]) return false;
	return true;
}

template <class DataType>
DataType ADeque<DataType>::operator[](int idx) const
{
	return _deqData[_headIdx + idx];
}

template <class DataType>
typename ADeque<DataType>::iterator ADeque<DataType>::begin() 
{
	return iterator(_deqData + _headIdx);
}

template <class DataType>
typename ADeque<DataType>::iterator ADeque<DataType>::end()
{
	return iterator(_deqData + _tailIdx);
}

template <class DataType>
string ADeque<DataType>::toString() const
{
	if (isEmpty()) return "";
	ostringstream oss;
	for (auto i = _headIdx; i <= _tailIdx;i++)
		oss << _deqData[i] << " ";
	return "-->" + oss.str() + "<--";
}

template <class DataType>
void ADeque<DataType>::push_back(const DataType& val)
{
	if (!enoughSingleSpaceBack(1))
		enlarge();

	_tailIdx++;
	_deqData[_tailIdx] = val;
}

template <class DataType>
void ADeque<DataType>::push_front(const DataType& val)
{
	if (!enoughSingleSpaceFront(1))
		enlarge();

	_headIdx--;
	_deqData[_headIdx] = val;
}

template <class DataType>
DataType ADeque<DataType>::pop_front()
{
	if (isEmpty())
		throw std::out_of_range("pop_front out of range");
	auto it = begin();
	_headIdx++;
	return *it;
}

template <class DataType>
DataType ADeque<DataType>::pop_back()
{
	if (isEmpty())
		throw std::out_of_range("pop_back out of range");
	auto it = end();
	if (_tailIdx == 0) clear();
	_tailIdx--;
	return *it;
}

template <class DataType>
DataType ADeque<DataType>::front() const
{
	return *begin();
}

template <class DataType>
DataType ADeque<DataType>::back() const
{
	return *end();
}

template <class DataType>
void ADeque<DataType>::swap(ADeque<DataType>& other)
{
	ADeque<DataType> tmp;
	tmp = move(other);
	other = move(*this);
	*this = move(tmp);
}

template <class DataType>
template <class ... Args>
void ADeque<DataType>::emplace(iterator position, Args&&... args)
{
	queue<DataType> tail, params;
	// "Tail was copyed"
	while (position != end)
	{
		tail.push(*position);
		++position;
	}

	int a[] = { ((void)(params.push(args)), 1) ... };
	int totalSize = params.size() + tail.size();

	while (!enoughSingleSpaceBack(totalSize) && !enoughSingleSpaceFront(totalSize))
		enlarge();

	while (!params.empty())
		push_back(params.pop());

	while (!tail.empty())
		push_back(tail.pop());

}


template <class DataType>
void ADeque<DataType>::clear()
{
	_sz = 0;
	delete[] _deqData;
	_deqData = nullptr;
	_headIdx = 0;
	_tailIdx = 0;
}

template <class DataType>
size_t ADeque<DataType>::size() const
{
	if (_sz == 0 || _headIdx > _tailIdx) return 0;
	auto s = abs(int(_headIdx - _tailIdx)) + 1;
	return 	s;
}

template <class DataType>
bool ADeque<DataType>::isEmpty() const
{
	return size() == 0 ? true : false;
}

template <class DataType>
void ADeque<DataType>::accept(Base_Visitor<DataType>& visitor)
{
	visitor.visit(*this);
}

template <class DataType>
bool ADeque<DataType>::enoughSingleSpaceBack(int x)
{
	return (int(_tailIdx) + x < _sz) ? true : false;
}

template <class DataType>
bool ADeque<DataType>::enoughSingleSpaceFront(int x)
{
	return (int(_headIdx) - x >= 0) ? true : false;
}

template <class DataType>
size_t ADeque<DataType>::getSpaceFront()
{
	return _headIdx;
}

template <class DataType>
size_t ADeque<DataType>::getSpaceBack()
{
	return _sz - _tailIdx;
}

template <class DataType>
void ADeque<DataType>::enlarge()
{
	size_t newSize = _sz * 3;
	if (newSize == 0)
		newSize = initialSize;
	vector<DataType> d;

	if (_sz != 0)
	{
		for(auto it = begin(); it != end(); ++it)
			d.push_back(*it);
	}

	if (_deqData != nullptr)
		delete[] _deqData;
	_sz = newSize;
	_deqData = new DataType[_sz];

	newSize = (_sz / 2) - (d.size() / 2);

	_headIdx = newSize + 1;
	_tailIdx = newSize;

	for (auto elem : d)
		push_back(elem);
	//cout << "Enlarged!" << endl;
}

template<class U>
ADeque<U> operator+(const ADeque<U>& lhs, const ADeque<U>& rhs)
{
	ADeque<U> sum(lhs);
	sum += rhs;
	return sum;
}