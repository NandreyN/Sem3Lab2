#include <windows.h>
class View;
using namespace std;

class View
{
private:
	HWND _handle;
public:
	View(HINSTANCE hinstance, HINSTANCE prevHinstance, LPSTR lpCmdLine, int nCmdShow);
	int run();
	LRESULT mainProc(HWND hwmd,UINT message, WPARAM wParam, LPARAM lParam);
	BOOL dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
};
