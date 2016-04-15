#include "FSM.hpp"
#include "State.hpp"

FSM::FSM(){}

void FSM::set_start(class State* start_in)
{
	start = start_in;
}
void FSM::set_final(class State* final_in)
{
	final = final_in;
}

bool FSM::recognize(std::string str)
{
	if (str == "")
		return false;
	State* state = start;
	std::string::const_iterator i = str.begin();
	while (i != str.end())
	{
		char input = *i;
		state = state->move(&input);
		if (state == NULL)
			return false;
		++i;
	}
	if (state == final)
	{
		return true;
	}
	return false;
}

FSM::~FSM(){}