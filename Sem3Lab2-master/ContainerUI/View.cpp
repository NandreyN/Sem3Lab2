#include "View.h"
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

map<HWND, View*> windowMap;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	std::map<HWND, View *>::iterator it = windowMap.find(hWnd);
	if (it != windowMap.end())
		return it->second->mainProc(hWnd,message, wParam, lParam);
	return DefWindowProc(hWnd, message, wParam, lParam);;
}

LRESULT CALLBACK DlgCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	std::map<HWND, View *>::iterator it = windowMap.find(hWnd);
	if (it != windowMap.end())
		return it->second->mainProc(hWnd,message, wParam, lParam);
	return 0;
}

View::View(HINSTANCE hinstance, HINSTANCE prevHinstance, LPSTR lpCmdLine, int nCmdShow)
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
	wndclass.lpszClassName = (LPCWSTR)"UICONTAINER";
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wndclass))
	{
		throw "";
	}

	HWND hwnd;
	hwnd = CreateWindow(
		(LPCWSTR)"UICONTAINER",
		(LPCWSTR)"UICONTAINER",
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

int View::run()
{
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT View::mainProc(HWND hwnd,UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), hwnd, (DLGPROC)DlgCallback);
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

BOOL View::dlgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	char buffer[1024];
	static LDeque<int> deque;
	static LDeque<int> anotherDeque;
	static HWND inputInit;
	static HWND inputInitButton;
	static HWND inputPushBack;
	static HWND inputPushFront;
	static HWND cContent;
	static HWND output;
	static HWND clearButton;
	static HWND text;
	static HWND swapButton;
	static HWND visitorTextInit;

	static const string initString = "1 2 3 4 5";
	static const string inputPushBackString = "Data to push back:";
	static const string inputPushFrontString = "Data to push front:";
	static const string cContentString = "Contatiner data: ";
	static const string outputString = "Output: ";
	static string anotherQString = "D: 6 7 8";
	static string visitor_text_string = "";
	static string messageBoxText = "";
	int b = 0;
	switch (message)
	{
	case WM_INITDIALOG:
		b = 8;
		break;
	case WM_COMMAND:
		if (HIWORD(wParam) == EN_SETFOCUS && LOWORD(wParam) == IDC_INPUTINIT)
		{
			SetWindowText(inputInit, 0);
		}

		if (HIWORD(wParam) == EN_SETFOCUS && LOWORD(wParam) == IDC_INPUTPUSHFRONT)
		{
			SetWindowText(inputPushFront, 0);
		}

		if (HIWORD(wParam) == EN_SETFOCUS && LOWORD(wParam) == IDC_INPUTPUSHBACK)
		{
			SetWindowText(inputPushBack, 0);
		}

		switch (LOWORD(wParam))
		{
		case IDC_INITBUTTON:
		{

			break;
		}
		case IDC_PUSHF:
		{

			break;
		}

		case IDC_PUSHB:
		{

			break;
		}

		case IDC_POPB:
		{

			break;
		}

		case IDC_POPF:
		{

			break;
		}

		case IDC_CLEARBUTTON:
		{

			break;
		}

		case IDC_SWAPBUTTON:
		{

			break;
		}
		case IDOK:

			break;

		case IDC_VISITORSUM:
		{

			break;
		}
		}

		break;


	}
	return FALSE;
}