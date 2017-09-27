#include <windows.h>
#include <memory>
#include "View.h"
#include "Model.h"
using namespace std;

class Controller
{
	HWND _handle;
	std::shared_ptr<View> _viewPtr;
	std::shared_ptr<Model<int>> _modelPtr;
	
	string getInput(HWND, int) const;

	template<class T>
	string initListToString(const std::initializer_list<T>& lst);
	string trim(const string&);
public:
	Controller(HINSTANCE hinstance, HINSTANCE prevHinstance, LPSTR lpCmdLine, int nCmdShow);
	int run();
	LRESULT mainProc(HWND hwmd,UINT message, WPARAM wParam, LPARAM lParam);
	BOOL dlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};

template <class T>
string Controller::initListToString(const std::initializer_list<T>& lst)
{
	ostringstream oss("");
	for (const auto& elem : lst)
		oss << elem << " ";
	return oss.str();
}