
#undef UNICODE
#include <windows.h>
//#include "LDeque.h"
#include "LDeque.h"
#include  <math.h>
#include <vector>
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


using namespace std;
void updateDequeStateMessage(HWND&, LDeque<int>&);
BOOL InitWnd1(HINSTANCE hinstance);
BOOL InitInstance1(HINSTANCE hinstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK CContainerProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
bool isNormal(const string&);
vector<int> parseString(const string&);

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prevHinstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	if (!InitWnd1(hinstance))
	{
		MessageBox(NULL, "Unable to Init App", "Error", MB_OK);
		return FALSE;
	}

	if (!InitInstance1(hinstance, nCmdShow))
	{
		MessageBox(NULL, "Unable to Init Instance", "Error", MB_OK);
		return FALSE;
	}

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

BOOL InitWnd1(HINSTANCE hinstance)
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
	wndclass.lpszClassName = "UI";
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wndclass))
	{
		MessageBox(NULL, "Cannot register class", "Error", MB_OK);
		return FALSE;
	}

	return TRUE;
}

BOOL CALLBACK CContainerProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
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

	static const string initString = "1 2 3 4 5";
	static const string inputPushBackString = "Data to push back:";
	static const string inputPushFrontString = "Data to push front:";
	static const string cContentString = "Contatiner data: ";
	static const string outputString = "Output: ";
	static string anotherQString = "D: 6 7 8";

	switch (message)
	{
	case WM_INITDIALOG:
		inputInit = GetDlgItem(hwndDlg, IDC_INPUTINIT);
		inputPushBack = GetDlgItem(hwndDlg, IDC_INPUTPUSHBACK);
		inputPushFront = GetDlgItem(hwndDlg, IDC_INPUTPUSHFRONT);
		cContent = GetDlgItem(hwndDlg, IDC_CCONTENT);
		output = GetDlgItem(hwndDlg, IDC_OUPUT);
		inputInitButton = GetDlgItem(hwndDlg, IDC_INITBUTTON);
		clearButton = GetDlgItem(hwndDlg, IDC_CLEARBUTTON);
		text = GetDlgItem(hwndDlg, IDC_TEXT);

		SendMessageA(output, EM_REPLACESEL, 0, (LPARAM)(outputString.data()));
		SendMessageA(inputInit, EM_REPLACESEL, 0, (LPARAM)(initString.data()));
		SendMessageA(inputPushBack, EM_REPLACESEL, 0, (LPARAM)(inputPushBackString.data()));
		SendMessageA(inputPushFront, EM_REPLACESEL, 0, (LPARAM)(inputPushFrontString.data()));
		SendMessageA(cContent, EM_REPLACESEL, 0, (LPARAM)(cContentString.data()));
		SetWindowText(text,anotherQString.data());

		anotherDeque = LDeque<int>({6,7,8});
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
			GetWindowText(inputInit,buffer,1024);
			string s(buffer);
			if (!isNormal(s)) break;

			deque = LDeque<int>(s);
			SetWindowText(inputInit, 0);
			EnableWindow(inputInit, false);
			EnableWindow(inputInitButton, false);

			SetWindowText(cContent, deque.toString().data());
			updateDequeStateMessage(output,deque);
			break;
		}
		case IDC_PUSHF:
		{
			GetWindowText(inputPushFront, buffer, 1024);
			string s(buffer);
			if (!isNormal(s)) break;

			auto vect = parseString(s);
			for (auto c : vect)
				deque.push_front(c);

			s = deque.toString();
			SetWindowText(cContent, deque.toString().data());
			updateDequeStateMessage(output, deque);
			break;
		}

		case IDC_PUSHB:
		{
			GetWindowText(inputPushBack, buffer, 1024);
			string s(buffer);
			if (!isNormal(s)) break;

			auto vect = parseString(s);
			for (auto c : vect)
				deque.push_back(c);

			s = deque.toString();
			SetWindowText(cContent, deque.toString().data());
			updateDequeStateMessage(output, deque);
			break;
		}

		case IDC_POPB:
		{
			try
			{
				deque.pop_back();
			}
			catch(std::out_of_range ex)
			{
				SetWindowText(output, ex.what());
			}
			SetWindowText(cContent, deque.toString().data());
			updateDequeStateMessage(output, deque);
			break;
		}

		case IDC_POPF:
		{
			try
			{
				deque.pop_front();
			}
			catch (std::out_of_range ex)
			{
				SetWindowText(output, ex.what());
			}
			SetWindowText(cContent, deque.toString().data());
			updateDequeStateMessage(output, deque);
			break;
		}

		case IDC_CLEARBUTTON:
		{
			deque.clear();
			SetWindowText(cContent, deque.toString().data());
			updateDequeStateMessage(output, deque);
			break;
		}

		case IDC_SWAPBUTTON:
		{
			deque.swap(anotherDeque);
			updateDequeStateMessage(output, deque);
			SetWindowText(cContent, deque.toString().data());
			SetWindowText(text, anotherDeque.toString().data());
			break;
		}
		case IDOK:
			SendMessage(GetParent(hwndDlg), WM_CLOSE, 0, 0);
			EndDialog(hwndDlg, 0);
			break;
		}
		break;
	}
	return FALSE;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static int x, y;
	static HDC hdc;
	PAINTSTRUCT ps;
	static RECT r;

	static HWND hwndDialog;
	switch (message)
	{
	case WM_CREATE:
		DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), hwnd, CContainerProc);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
	}
	return FALSE;
}

BOOL InitInstance1(HINSTANCE hinstance, int nCmdShow)
{
	HWND hwnd;
	hwnd = CreateWindow(
		"UI",
		"UI",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		NULL,
		NULL,
		hinstance,
		NULL);


	if (!hwnd)
		return FALSE;
	//ShowWindow(hwnd, nCmdShow);
	//UpdateWindow(hwnd);
	return TRUE;
}

bool isNormal(const string& s)
{
	if (s.empty()) return false;
	for (char c : s)
		if (!isdigit(c) && c != ' ') return false;
	return true;
}

vector<int> parseString(const string& s)
{
	vector<int> vect;
	istringstream oss(s);
	while(!oss.eof())
	{
		int elem;
		oss >> elem;
		vect.push_back(elem);
	}
	return vect;
}
void updateDequeStateMessage(HWND& hwnd, LDeque<int>& d)
{
	SetWindowText(hwnd, d.getInternalInfo().data());
}