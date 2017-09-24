#include <windows.h>
#include "View.h"

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prevHinstance, LPSTR lpCmdLine, int nCmdShow)
{
	View view(hinstance, prevHinstance, lpCmdLine, nCmdShow);
	view.run();
}