#include <iostream>
#include"ADeque.h"
#include <deque>
using namespace std;

int main()
{
	ADeque<int> a({6,3,4,5});
	
	a.pop_back();
	a.pop_back();
	a.pop_back();
	a.pop_front();

	a.push_back(1);
	a.push_front(33);
	a.push_back(11);
	cout << a.size() << endl <<a ;
	return 0;
}