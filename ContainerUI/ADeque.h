#pragma once
#include <vector>
#include <sstream>
#include <algorithm>
#include <assert.h>

const size_t initialSize = 6;

using namespace std;

template<class DataType>
class ADeque
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

	template<class U>
	friend ostream& operator<<(ostream& o, const ADeque<U>&); // OK

	template<class U>
	friend istream& operator >> (istream& i, ADeque<U>&); // OK

	string getInternalInfo() const;

	bool operator==(const ADeque&);

	iterator begin() const;//OK
	iterator end() const;//OK

	string toString();

	void push_back(const DataType&); // OK
	void push_front(const DataType&); // OK
	DataType pop_front(); // OK
	DataType pop_back(); // OK
	void swap(ADeque<DataType>&); // OK


								  // EMPLACE

	void clear();//OK
	size_t size() const; // ok
	bool isEmpty() const; // ok
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

		for (size_t i = 0; i < ad.size(); i++)
			_deqData[i] = ad._deqData[i];

		_headIdx = ad._headIdx;
		_tailIdx = ad._tailIdx;

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
	string info = "";
	info = "Space reserved : " + to_string(_sz) + "\r\n" +
		"Head idx : " + to_string(_headIdx) + "\r\n" +
		"Tail idx : " + to_string(_tailIdx) + "\r\n" +
		"Current size : " + to_string(size()) + "\r\n";
	return info;
}

template <class DataType>
bool ADeque<DataType>::operator==(const ADeque& ad)
{
	if (_sz != ad.size()) return false;

	for (size_t i = 0; i < _sz; i++)
		if (_deqData[i] != ad._deqData[i]) return false;
	return true;
}

template <class DataType>
typename ADeque<DataType>::iterator ADeque<DataType>::begin() const
{
	return iterator(_deqData + _headIdx);
}

template <class DataType>
typename ADeque<DataType>::iterator ADeque<DataType>::end() const
{
	return iterator(_deqData + _tailIdx);
}

template <class DataType>
string ADeque<DataType>::toString()
{
	if (isEmpty()) return "";
	ostringstream oss;
	for (size_t i = _headIdx; i <= _tailIdx; i++)
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
void ADeque<DataType>::swap(ADeque<DataType>& other)
{
	ADeque<DataType> tmp = move(other);
	other = move(*this)
		*this = move(tmp);
}

/*template <class DataType>
template <DataType, class ... Args>
void ADeque<DataType>::emplace(typename BDeque<DataType>::const_iterator it, Args&&... a)
{
size_t insertElemCnt = sizeof...(a);
vector<DataType> v;

auto bgn = begin();
size_t i = 0;
while (bgn != it)
{
v.push_back(*it);
++bgn;
i++;
}

v.push_back(a...);

while (it != end())
{
v.push_back(*it);
++it;
i++;
}

if (getSpaceBack() + getSpaceFront() < insertElemCnt) enlarge();

copy(v.begin(), v.end(), _deqData);
}*/

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
		size_t i = _headIdx;
		for (; i <= _tailIdx; i++)
			d.push_back(_deqData[i]);
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