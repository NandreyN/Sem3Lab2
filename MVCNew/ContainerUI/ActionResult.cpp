#include "ActionResult.h"

ActionResult::ActionResult()
{
	UpdateFirst = false;
	UpdateSecond = false;
	First = "";
	Second = "";
}

ActionResult::ActionResult(bool f, bool s, const string& s1, const string& s2)
{
	UpdateFirst = f;
	UpdateSecond = s;
	
	if (UpdateFirst)
		First = s1;

	if (UpdateSecond)
		Second = s2;
}
