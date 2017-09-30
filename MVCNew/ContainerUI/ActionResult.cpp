#include "ActionResult.h"

ActionResult::ActionResult()
{
	UpdateFirst = false;
	UpdateSecond = false;
	First = "";
	Second = "";
	Error = false;
	ErrorMessage = "";
}

ActionResult::ActionResult(bool f, bool s, const string& s1, const string& s2, bool error, const string& errMsg)
{
	UpdateFirst = f;
	UpdateSecond = s;
	
	if (UpdateFirst)
		First = s1;

	if (UpdateSecond)
		Second = s2;

	Error = error;
	ErrorMessage = errMsg;
}
