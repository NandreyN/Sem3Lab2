#pragma once
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
using namespace std;


template<class Type>
class LDeque
{
	struct Node
	{
		Node()
		{
			value = 0;
			next = nullptr;
			prev = nullptr;
		}
		Node(const Type& t)
		{
			value = t;
			next = nullptr;
			prev = nullptr;
		}
		Type value;
		Node* next;
		Node* prev;
	};

	class const_iterator;

	class NodeGetter
	{
	public:
		static Node* getNode(const_iterator& ci) { return ci.nd; };
	};

	class const_iterator
	{
		Node* nd;
		typedef const_iterator  self_type;
		typedef Type value;
		typedef const Type& const_reference;
		typedef Type *pointer;
		typedef int difference_type;
		typedef std::forward_iterator_tag const_iterator_category;
	public:
		// Constructor init list:
		const_iterator(Node* p) : nd(p) {}
		// opeartors overload 
		self_type operator++() { self_type src = *this; nd = nd->next; return src; };
		self_type operator++(int) { nd = nd->next; return *this; };
		self_type operator=(const self_type& it) { nd = it->nd; return *this; }
		const_reference operator*() { return nd->value; }
		pointer operator->() { return &nd->value; }
		bool operator==(const self_type& rhs) { return nd == rhs.nd; };
		bool operator!=(const self_type& rhs) { return nd != rhs.nd; }
		friend Node* NodeGetter::getNode(const_iterator&);
	};

public:
	LDeque();
	LDeque(std::initializer_list<Type>); // ok
	LDeque(LDeque&&); // ok
	LDeque(const LDeque&);// ok
	LDeque(const string&); // ok
	~LDeque(); // ok

	LDeque& operator=(LDeque&&); // ok
	LDeque& operator=(const LDeque&);// ok
	LDeque& operator+=(const LDeque&);// ok

	LDeque operator+(const LDeque&);//ok

	template<class U>
	friend ostream& operator<<(ostream& o, const LDeque<U>&); // ok

	template<class U>
	friend istream& operator >> (istream& i, LDeque<U>&); // ok

	string getInternalInfo() const;

	bool operator==(const LDeque&) const;//ok
	bool operator!=(const LDeque&) const;//ok

	const_iterator  begin() const; // ok
	const_iterator  end() const; // ok
	string toString() const; // ok

	void push_back(const Type&); // ok
	void push_front(const Type&); // ok
	Type pop_front();//ok
	Type pop_back();//ok
	Type front() const;//ok
	Type back() const;//ok

	void swap(LDeque<Type>&); // ok

	template<class ... Args>
	const_iterator  emplace(const_iterator  position, Args&& ... args);

	void clear();//ok
	size_t size() const; // ok
	bool isEmpty() const; // ok
private:
	NodeGetter ng;
	Node* _head;
	Node* _tail;
	int  _length;
};

template <class Type>
LDeque<Type>::LDeque() : _length(0)
{
}

template <class Type>
LDeque<Type>::LDeque(std::initializer_list<Type> ilist) : _length(ilist.size())
{
	_head = new Node();
	Node* headCopy = _head;

	for (auto t : ilist)
	{
		headCopy->value = t;

		Node* newHead = new Node();
		headCopy->next = newHead;
		newHead->prev = headCopy;

		headCopy = headCopy->next;
	}
	_tail = headCopy;
}

template <class Type>
LDeque<Type>::LDeque(LDeque&& ld)
{
	operator=(ld);
}

template <class Type>
LDeque<Type>::LDeque(const LDeque& ld)
{
	_head = new Node();
	Node* headCopy = _head;
	for (auto it = ld.begin(); it != ld.end(); ++it)
	{
		headCopy->value = *it;
		Node* newHead = new Node();
		headCopy->next = newHead;
		newHead->prev = headCopy;

		headCopy = headCopy->next;
	}
	_tail = headCopy; // >????
	_length = ld.size();
}

template <class Type>
LDeque<Type>::LDeque(const string& s)
{
	istringstream iss(s);
	vector<Type> values;

	Type t{};
	while (!iss.eof())
	{
		iss >> t;
		values.push_back(t);
	}
	_length = 0;
	for (auto val : values)
		push_back(val);
}

template <class Type>
LDeque<Type>::~LDeque()
{
	if (!isEmpty()) clear();
}

template <class Type>
LDeque<Type>& LDeque<Type>::operator=(LDeque&& ld)
{
	if (*this != ld)
	{
		for (auto it = ld.begin(); it != ld.end(); ++it)
			push_back(*it);
		ld.clear();
	}
	return *this;
}

template <class Type>
LDeque<Type>& LDeque<Type>::operator=(const LDeque& ld)
{
	*this = LDeque(ld);
	return *this;
}

template <class Type>
LDeque<Type>& LDeque<Type>::operator+=(const LDeque& ld)
{
	for (auto it = ld.begin(); it != ld.end(); ++it)
		push_back(*it);
	return *this;
}

template <class Type>
LDeque<Type> LDeque<Type>::operator+(const LDeque& ld)
{
	return *this += ld;
}

template <class Type>
string LDeque<Type>::getInternalInfo() const
{
	return "Length = " + to_string(_length) + "\r\n";
}

template <class Type>
bool LDeque<Type>::operator==(const LDeque& ld) const
{
	if (size() != ld.size()) return false;

	auto ldBgn = ld.begin();
	auto thisBegin = begin();

	while (ldBgn != ld.end() || thisBegin != end())
	{
		if (*ldBgn != *thisBegin) return false;
		++ldBgn;
		++thisBegin;
	}
	return true;
}

template <class Type>
bool LDeque<Type>::operator!=(const LDeque& ld) const
{
	return !operator==(ld);
}

template <class Type>
typename LDeque<Type>::const_iterator  LDeque<Type>::begin() const
{
	return const_iterator(_head);
}

template <class Type>
typename LDeque<Type>::const_iterator  LDeque<Type>::end() const
{
	return const_iterator(_tail);
}

template <class Type>
string LDeque<Type>::toString() const
{
	ostringstream oss;
	oss << *this;
	return oss.str();
}

template <class Type>
void LDeque<Type>::push_back(const Type& val)
{
	if (isEmpty())
	{
		_head = new Node(val);
		_length = 1;
		_tail = new Node;
		_head->next = _tail;
		_tail->prev = _head;
		return;
	}

	Node* lastValid = _tail->prev;
	Node* emptyNode = _tail;

	Node* actualnode = new Node(val);
	actualnode->next = emptyNode;
	actualnode->prev = lastValid;

	lastValid->next = actualnode;

	_tail = emptyNode;
	_tail->prev = actualnode;
}

template <class Type>
void LDeque<Type>::push_front(const Type& val)
{
	if (isEmpty())
	{
		_head = new Node(val);
		_length = 1;
		_tail = new Node;
		_head->next = _tail;
		_tail->prev = _head;
		return;
	}

	Node* newHead = new Node(val);
	newHead->next = _head;
	_head->prev = newHead;
	_head = newHead;
}

template <class Type>
Type LDeque<Type>::pop_front()
{
	if (isEmpty()) throw std::out_of_range("Deque is empty");
	Node* oldHead = _head;
	_head = _head->next;
	delete oldHead;
	_head->prev = nullptr;
	return *begin();
}

template <class Type>
Type LDeque<Type>::pop_back()
{
	if (isEmpty()) throw std::out_of_range("Deque is empty");
	Node* oldTail = _tail;
	_tail = _tail->prev;
	_tail->next = nullptr;
	delete oldTail;
	return *end();
}

template <class Type>
Type LDeque<Type>::front() const
{
	if (isEmpty()) throw std::out_of_range("empty deque");
	return *begin();
}

template <class Type>
Type LDeque<Type>::back() const
{
	if (isEmpty()) throw std::out_of_range("empty deque");
	return _tail->prev->value;
}

template <class Type>
void LDeque<Type>::swap(LDeque<Type>& other)
{
	LDeque<Type> tmp;
	tmp = move(other);
	other = move(*this);
	*this = move(tmp);
}

template <class Type>
template <class ... Args>
typename LDeque<Type>::const_iterator LDeque<Type>::emplace(const_iterator position, Args&&... args)
{
	int sz = sizeof...(args);
	Type arguments[] = { static_cast<Type>(args)... };

	Node* cpy = _head;
	Node* got = NodeGetter::getNode(position);
	if (got == _tail)
	{
		for (auto i = 0; i < sz; i++) push_back(arguments[i]);
		return ++got;
	}

	while (cpy != _tail && cpy != got)
		cpy = cpy->next;


	Node* targetNode = NodeGetter::getNode(position);
	Node* toReturn = NodeGetter::getNode(position);
	Node* concatNodeRight = targetNode->next;

	for (auto i = 0; i < sz; i++)
	{
		Node* n = new Node(arguments[i]);
		n->prev = targetNode;

		targetNode->next = n;
		targetNode = targetNode->next;
	}
	targetNode->next = concatNodeRight;
	concatNodeRight->prev = targetNode;

	return const_iterator(_head);
}

template <class Type>
void LDeque<Type>::clear()
{
	if (isEmpty()) return;

	do
	{
		Node* src = _head->next;
		delete _head;
		_head = src;
	} while (_head != _tail);

	delete _head;
	_length = 0;
}

template <class Type>
size_t LDeque<Type>::size() const
{
	return _length;
}

template <class Type>
bool LDeque<Type>::isEmpty() const
{
	return size() == 0 ? true : false;
}

template<class U>
ostream& operator<<(ostream& o, const LDeque<U>& ld)
{
	for (auto it = ld.begin(); it != ld.end(); ++it)
		o << *it << " ";
	return o;
}

template<class U>
istream& operator >> (istream& i, LDeque<U>& ld)
{
	string s;
	getline(i, s);
	ld = (s != "") ? LDeque<U>(s) : LDeque<U>();
	return i;
}
