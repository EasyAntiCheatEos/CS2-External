#include "cheat/moneyhack.hpp"

#include <thread>

int __stdcall main(
	HINSTANCE instance,
	HINSTANCE previousInstance,
	PWSTR arguments,
	int commandShow)
{
	
	return moneyhack::entry();
}
