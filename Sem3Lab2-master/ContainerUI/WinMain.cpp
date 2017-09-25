#include "Controller.h"

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prevHinstance, LPSTR lpCmdLine, int nCmdShow)
{
	shared_ptr<Controller> controllerPtr(new Controller(hinstance, prevHinstance, lpCmdLine, nCmdShow));
	controllerPtr->run();
}