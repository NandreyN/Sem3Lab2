#include <iostream>
#include "LDeque.h"

using namespace std;
int main()
{	
	LDeque<int> deque({1,2,3}); 
	deque.emplace(deque.end(),0,5,7);
	cout << deque << endl;
	return 0;
}