#pragma once
template <typename T>
class Action
{
public:
	typedef void(*func)(T arg1);
	Action(func f)
	{
		_function = f;
	}

	void Invoke(T arg)
	{
		_function(arg);
	}
private:
	func _function;
};
