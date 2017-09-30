#include "Controller.h"
#include "LDeque.h"
#include <map> 
#include "Sum_Visitor.h"

bool isStringOK(const string& s)
{
	if (s.empty()) return false;
	for (char c : s)
		if (!isdigit(c) && c != ' ') return false;
	return true;
}

string Controller::trim(const string & str)
{
	size_t first = str.find_first_not_of(' ');
	if (string::npos == first)
	{
		return str;
	}
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));

}

Controller::Controller()
{
	_modelPtr = shared_ptr<Model<int>>(new Model<int>(LDeque<int>{1, 2, 3, 4, 5}, LDeque<int>{6, 7, 8}));
}

ActionResult Controller::GetModelState() const
{
	return ActionResult(true, true, _modelPtr->firstToString(), _modelPtr->secondToString());
}

ActionResult Controller::OnPushBackButtonClick(const string& str)
{
	if (!isStringOK(str)) return ActionResult(false, false, "", "");
	_modelPtr->push_back_string(str);
	return ActionResult(true, false, _modelPtr->firstToString(), "");
}

ActionResult  Controller::OnPushFrontButtonClick(const string & str)
{
	if (!isStringOK(str)) return ActionResult(false, false, "", "");
	_modelPtr->push_front_string(str);
	return ActionResult(true, false, _modelPtr->firstToString(), "");
}


ActionResult Controller::OnClearButtonClick()
{
	_modelPtr->clear();
	return ActionResult(true, false, _modelPtr->firstToString(), "");
}

ActionResult Controller::OnInitButtonClick(const string& inp)
{
	if (!isStringOK(inp)) return ActionResult(false, false, "", "");
	_modelPtr->setFirst(trim(inp));

	return ActionResult(true, false, _modelPtr->firstToString(), "");
}

ActionResult Controller::OnPopBackButtonClick()
{
	try
	{
		_modelPtr->pop_back();
	}
	catch (std::out_of_range ex)
	{
		return  ActionResult(true, false, _modelPtr->firstToString(), "", true, "Empty Deque");
	}
	return ActionResult(true, false, _modelPtr->firstToString(), "");
}

ActionResult Controller::OnPopFrontButtonClick()
{
	try
	{
		_modelPtr->pop_front();
	}
	catch (std::out_of_range ex)
	{
		return  ActionResult(true, false, _modelPtr->firstToString(), "", true, "Empty Deque");
	}
	return ActionResult(true, false, _modelPtr->firstToString(), "");
}

ActionResult Controller::OnSwapButtonClick()
{
	_modelPtr->swapDeques();
	return ActionResult(true, true, _modelPtr->firstToString(), _modelPtr->secondToString());
}

ActionResult Controller::OnVisitorButtonClick()
{
	std::initializer_list<int> ld_init = { 1,2,2,0,7,8,3,5 };
	std::initializer_list<int> ad_init = { 10,10,12,11 };

	Sum_Visitor<int> v;
	LDeque<int> ld(ld_init);
	ADeque<int> ad(ad_init);

	string visitor_text_string = "LDeque init: " + initListToString(ld_init) + " \r\nADeque init: " + initListToString(ad_init) + "\r\n";

	ld.accept(v);
	int l = v.getSum();
	ad.accept(v);
	int a = v.getSum();

	string messageBoxText = "for LDeque : " + to_string(l) + "\r\n" + "for ADeque : " + to_string(a) + "\r\n";

	return ActionResult(false, false, "", "");
}



