#pragma once
#include <windows.h>
#include <string>
#include <map>

#define IDC_CCONTENT 1001

/*
View class manipulates only output to user
via special methods
*/
class View
{
	HWND _dlgHwnd;
	std::map<std::string, int> _commandDictionary;

public:
	View(HWND);
	void UIUpdate(std::string commandStr, int controlID, std::string data);
};

inline View::View(HWND hwnd)
{
	_dlgHwnd = hwnd;
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
		SetWindowText(GetDlgItem(_dlgHwnd, controlID), data.data());
		break;
	case 1:
		EnableWindow(GetDlgItem(_dlgHwnd, controlID), false);
		break;
	case 2:
		EnableWindow(GetDlgItem(_dlgHwnd, controlID), true);
		break;
	case 3:
		UIUpdate("SetWindowText", IDC_CCONTENT, data);
		break;
	case 4:
		MessageBox(_dlgHwnd, data.data(), "Info", MB_OK);
		break;
	}
}