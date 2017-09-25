#include "Controller.h"
#define IDD_DIALOG1 101
#define IDC_CCONTENT 1001
#define IDC_INITBUTTON 1011
#define IDC_CLEARBUTTON 1012
#define IDC_GSIZE 1008
#define IDC_PUSHF 1005
#define IDC_PUSHB 1004
#define IDC_POPB 1007
#define IDC_POPF 1006
#define IDC_OUPUT 1009
#define IDC_INPUTPUSHBACK 1002
#define IDC_INPUTPUSHFRONT 1003
#define IDC_INPUTINIT 1010
#define IDC_TEXT 1013
#define IDC_SWAPBUTTON 1014
#define IDC_VISITORSUM 1015
#define IDC_VISITORTEXT 1016
#include "LDeque.h"
#include <map> 
#include "Sum_Visitor.h"

map<HWND, Controller*> windowMap;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	std::map<HWND, Controller *>::iterator it = windowMap.find(hWnd);
	if (it != windowMap.end())
		return it->second->mainProc(hWnd, message, wParam, lParam);
	return DefWindowProc(hWnd, message, wParam, lParam);;
}

LRESULT CALLBACK DlgCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	std::map<HWND, Controller *>::iterator it = windowMap.find(hWnd);
	if (it != windowMap.end())
		return it->second->dlgProc(hWnd, message, wParam, lParam);
	return 0;
}

vector<int> Controller::parseString(const string& s)
{
	vector<int> vect;
	istringstream oss(s);
	while (!oss.eof())
	{
		int elem;
		oss >> elem;
		vect.push_back(elem);
	}
	return vect;

}

bool Controller::isStringOK(const string& s) const
{
	if (s.empty()) return false;
	for (char c : s)
		if (!isdigit(c) && c != ' ') return false;
	return true;
}

string Controller::getInput(HWND hwnd, int elementID) const
{
	char buffer[1024];
	GetWindowText(GetDlgItem(hwnd, elementID), buffer, 1024);
	return string(buffer);
}

Controller::Controller(HINSTANCE hinstance, HINSTANCE prevHinstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wndclass;
	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hinstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = "UICONTAINER";
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wndclass))
	{
		throw "";
	}

	HWND hwnd;
	hwnd = CreateWindow(
		(LPCSTR)"UICONTAINER",
		(LPCSTR)"UICONTAINER",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		NULL,
		NULL,
		hinstance,
		NULL);


	if (!hwnd || hwnd == NULL)
	{
		throw "fuck";
	}
	_handle = hwnd;
	windowMap[_handle] = this;
	SendMessage(hwnd, WM_CREATE, 0, 0);
}

int Controller::run()
{
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT Controller::mainProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND dlgHWND;
	switch (message)
	{
	case WM_CREATE:
		dlgHWND = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), hwnd, (DLGPROC)DlgCallback);
		windowMap[dlgHWND] = this;
		ShowWindow(dlgHWND, SW_SHOW);
		SendMessage(dlgHWND, WM_INITDIALOG,0,0);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return FALSE;
}

BOOL Controller::dlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
		_handle = hwnd;
		_viewPtr = std::shared_ptr<View>(new View(_handle));
		_modelPtr = std::shared_ptr<Model<int>>(new Model<int>());
		_modelPtr->srcDeque = LDeque<int>{6,7,8};
		_modelPtr->deque = LDeque<int>{1,2,3,4,5};
		_viewPtr->UIUpdate("UpdateDequeStateMessage", IDC_CCONTENT, _modelPtr->deque.toString());
		_viewPtr->UIUpdate("SetWindowText", IDC_TEXT, _modelPtr->srcDeque.toString());
		break;
	case WM_COMMAND:
		if (HIWORD(wParam) == EN_SETFOCUS && LOWORD(wParam) == IDC_INPUTINIT)
		{
			_viewPtr->UIUpdate("SetWindowText", IDC_INPUTINIT, "");
		}

		if (HIWORD(wParam) == EN_SETFOCUS && LOWORD(wParam) == IDC_INPUTPUSHFRONT)
		{
			_viewPtr->UIUpdate("SetWindowText", IDC_INPUTPUSHFRONT, "");
		}

		if (HIWORD(wParam) == EN_SETFOCUS && LOWORD(wParam) == IDC_INPUTPUSHBACK)
		{
			_viewPtr->UIUpdate("SetWindowText", IDC_INPUTPUSHBACK, "");
		}

		switch (LOWORD(wParam))
		{
			case IDC_INITBUTTON:
			{
				string inp = getInput(_handle, IDC_INPUTINIT);
				if (!isStringOK(inp)) break;
				_modelPtr->deque = LDeque<int>(inp);
				_viewPtr->UIUpdate("DisableWindow", IDC_INPUTINIT, "");
				_viewPtr->UIUpdate("DisableWindow", IDC_INITBUTTON, "");
				_viewPtr->UIUpdate("UpdateDequeStateMessage", IDC_CCONTENT, _modelPtr->deque.toString());
				break;
			}

			case IDC_PUSHF:
			{
				string inp = getInput(_handle, IDC_INPUTPUSHFRONT);
				if (!isStringOK(inp)) break;

				vector<int> data = parseString(inp);
				for (auto& elem : data)
					_modelPtr->deque.push_front(elem);

				_viewPtr->UIUpdate("UpdateDequeStateMessage", IDC_CCONTENT, _modelPtr->deque.toString());
				break;
			}

			case IDC_PUSHB:
			{
				string inp = getInput(_handle, IDC_INPUTPUSHFRONT);
				if (!isStringOK(inp)) break;

				vector<int> data = parseString(inp);
				for (auto& elem : data)
					_modelPtr->deque.push_back(elem);

				_viewPtr->UIUpdate("UpdateDequeStateMessage", IDC_CCONTENT, _modelPtr->deque.toString());
				break;
			}
			case IDC_POPB:
			{
				try
				{
					_modelPtr->deque.pop_back();
				}
				catch (std::out_of_range ex)
				{
					_viewPtr->UIUpdate("SetWindowText", IDC_OUPUT, string(ex.what()));
				}
				_viewPtr->UIUpdate("UpdateDequeStateMessage", IDC_CCONTENT, _modelPtr->deque.toString());
				break;
			}

			case IDC_POPF:
			{
				try
				{
					_modelPtr->deque.pop_front();
				}
				catch (std::out_of_range ex)
				{
					_viewPtr->UIUpdate("SetWindowText", IDC_OUPUT, string(ex.what()));
				}
				_viewPtr->UIUpdate("UpdateDequeStateMessage", IDC_CCONTENT, _modelPtr->deque.toString());
				break;
			}

			case IDC_CLEARBUTTON:
			{
				_modelPtr->deque.clear();
				_viewPtr->UIUpdate("UpdateDequeStateMessage", IDC_CCONTENT, _modelPtr->deque.toString());
				break;
			}

			case IDC_SWAPBUTTON:
			{
				_modelPtr->deque.swap(_modelPtr->srcDeque);
				_viewPtr->UIUpdate("UpdateDequeStateMessage", IDC_CCONTENT, _modelPtr->deque.toString());
				_viewPtr->UIUpdate("SetWindowText", IDC_TEXT, _modelPtr->srcDeque.toString());
				break;
			}

			case IDOK:
			{
				SendMessage(GetParent(_handle), WM_CLOSE, 0, 0);
				EndDialog(_handle, 0);
				break;
			}

			case IDC_VISITORSUM:
			{
				std::initializer_list<int> ld_init = { 1,2,2,0,7,8,3,5 };
				std::initializer_list<int> ad_init = { 10,10,12,11 };

				Sum_Visitor<int> v;
				LDeque<int> ld(ld_init);
				ADeque<int> ad(ad_init);

				string visitor_text_string = "LDeque init: " + initListToString(ld_init) + " \r\nADeque init: " + initListToString(ad_init) + "\r\n";
				_viewPtr->UIUpdate("SetWindowText", IDC_VISITORTEXT, visitor_text_string);

				ld.accept(v);
				int l = v.getSum();
				ad.accept(v);
				int a = v.getSum();

				string messageBoxText = "for LDeque : " + to_string(l) + "\r\n" + "for ADeque : " + to_string(a) + "\r\n";
				_viewPtr->UIUpdate("MessageBox", 0, messageBoxText);

				break;
			}
		}
	
		break;
	}


	return FALSE;
}
