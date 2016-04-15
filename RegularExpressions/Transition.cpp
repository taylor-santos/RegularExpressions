#include "Transition.hpp"

Transition::Transition(char* condition_in, class State* state_in)
{
	if (condition_in == '\0')
	{
		epsilon = true;
	}
	else {
		condition = *condition_in;
		epsilon = false;
	}
		
	state = state_in;
}

Transition::~Transition() {}