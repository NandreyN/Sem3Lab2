#pragma once
#include <windows.h>
#include <string>
#include <map>
#include <memory>
#include "Controller.h"
#pragma region Defines
#define IDC_CCONTENT 1001
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
#pragma endregion

/*
View class manipulates only output to user
via special methods
*/
class View
{
private:
	HWND _handle;
	std::map<std::string, int> _commandDictionary;
	std::shared_ptr<Controller> _controllerPtr;
	void update(const ActionResult&);
	string getInput(HWND hwnd, int elementID) const;
	void UIUpdate(std::string commandStr, int controlID, std::string data);
public:
	LRESULT mainProc(HWND hwmd, UINT message, WPARAM wParam, LPARAM lParam);
	BOOL dlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	View(HINSTANCE hinstance, HINSTANCE prevHinstance, LPSTR lpCmdLine, int nCmdShow);
	int run();
};

inline int View::run()
{
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

map<HWND, View*> windowMap;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	std::map<HWND, View *>::iterator it = windowMap.find(hWnd);
	if (it != windowMap.end())
		return it->second->mainProc(hWnd, message, wParam, lParam);
	return DefWindowProc(hWnd, message, wParam, lParam);;
}

LRESULT CALLBACK DlgCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	std::map<HWND, View *>::iterator it = windowMap.find(hWnd);
	if (it != windowMap.end())
		return it->second->dlgProc(hWnd, message, wParam, lParam);
	return 0;
}

LRESULT View::mainProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND dlgHWND;
	switch (message)
	{
	case WM_CREATE:
		dlgHWND = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), hwnd, (DLGPROC)DlgCallback);
		windowMap[dlgHWND] = this;
		ShowWindow(dlgHWND, SW_SHOW);
		SendMessage(dlgHWND, WM_INITDIALOG, 0, 0);
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

inline void View::update(const ActionResult &ar)
{
	if (ar.UpdateFirst) UIUpdate("SetWindowText", IDC_CCONTENT, ar.First.data());
	if (ar.UpdateSecond) UIUpdate("SetWindowText", IDC_TEXT, ar.Second.data());
}

string View::getInput(HWND hwnd, int elementID) const
{
	char buffer[1024];
	GetWindowText(GetDlgItem(hwnd, elementID), buffer, 1024);
	return string(buffer);
}

BOOL View::dlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static ActionResult ar;
	switch (message)
	{
	case WM_INITDIALOG:
		_handle = hwnd;
		_controllerPtr = std::shared_ptr<Controller>(new Controller());
		ar = _controllerPtr->GetModelState();
		update(ar);
		break;
	case WM_COMMAND:
		if (HIWORD(wParam) == EN_SETFOCUS && LOWORD(wParam) == IDC_INPUTINIT)
		{
			SetWindowText(GetDlgItem(_handle, IDC_INPUTINIT), "");
		}

		if (HIWORD(wParam) == EN_SETFOCUS && LOWORD(wParam) == IDC_INPUTPUSHFRONT)
		{
			SetWindowText(GetDlgItem(_handle, IDC_INPUTPUSHFRONT), "");
		}

		if (HIWORD(wParam) == EN_SETFOCUS && LOWORD(wParam) == IDC_INPUTPUSHBACK)
		{
			SetWindowText(GetDlgItem(_handle, IDC_INPUTPUSHBACK), "");
		}

		switch (LOWORD(wParam))
		{
		case IDC_INITBUTTON:
		{
			ar = _controllerPtr->OnInitButtonClick(getInput(_handle, IDC_INPUTINIT));
			break;
		}

		case IDC_PUSHF:
		{
			ar = _controllerPtr->OnPushFrontButtonClick(getInput(_handle, IDC_PUSHF));
			break;
		}

		case IDC_PUSHB:
		{
			ar = _controllerPtr->OnPushBackButtonClick(getInput(_handle, IDC_PUSHB));
			break;
		}
		case IDC_POPB:
		{
			ar = _controllerPtr->OnPopBackButtonClick();
			break;
		}

		case IDC_POPF:
		{
			ar = _controllerPtr->OnPopFrontButtonClick();
			break;
		}

		case IDC_CLEARBUTTON:
		{
			ar = _controllerPtr->OnClearButtonClick();
			break;
		}

		case IDC_SWAPBUTTON:
		{
			ar = _controllerPtr->OnSwapButtonClick();
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
			ar = _controllerPtr->OnVisitorButtonClick();
			break;
		}
		
		}
		update(ar);
		break;
	}
	

	return FALSE;
}

inline View::View(HINSTANCE hinstance, HINSTANCE prevHinstance, LPSTR lpCmdLine, int nCmdShow)
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
		throw "Exception";
	}
	windowMap[hwnd] = this;
	SendMessage(hwnd, WM_CREATE, 0, 0);

	_commandDictionary["SetWindowText"] = 0;
	_commandDictionary["DisableWindow"] = 1;
	_commandDictionary["EnableWindow"] = 2;
	_commandDictionary["UpdateDequeStateMessage"] = 3;
	_commandDictionary["MessageBox"] = 4;
}

inline void View::UIUpdate(std::string commandStr, int controlID, std::string data)
{
	if (_commandDictionary.find(commandStr) == _commandDictionary.end()) return;
	switch (_commandDictionary[commandStr])
	{
	case 0:
		SetWindowText(GetDlgItem(_handle, controlID), data.data());
		break;
	case 1:
		EnableWindow(GetDlgItem(_handle, controlID), false);
		break;
	case 2:
		EnableWindow(GetDlgItem(_handle, controlID), true);
		break;
	case 3:
		UIUpdate("SetWindowText", IDC_CCONTENT, data);
		break;
	case 4:
		MessageBox(_handle, data.data(), "Info", MB_OK);
		break;
	}
}
