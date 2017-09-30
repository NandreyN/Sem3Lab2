#pragma once
#include <memory>
#include "LDeque.h"
class ActionResult
{
	public:
		bool UpdateFirst;
		bool UpdateSecond;
		string First;
		string Second;
		string ErrorMessage;
		bool Error;

		ActionResult();
		ActionResult(bool, bool, const string&, const string&, bool = false,const string& = "");
};