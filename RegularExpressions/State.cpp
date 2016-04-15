#include <iostream>
#include "State.hpp"
#include "Transition.hpp"

State::State(int id_in) {
	id = id_in;
}

void State::add_transition(Transition* t)
{
	if (transitions == NULL)
	{
		transitions = t;
		return;
	}
	Transition* TWalker = transitions;
	while (TWalker->next != NULL)
	{
		if (TWalker->condition == t->condition)
		{
			return;
		}
		TWalker = TWalker->next;
	}
	TWalker->next = t;
	return;
}

State* State::move(char* c)
{
	Transition* TWalker = transitions;
	while (TWalker != NULL)
	{
		if (TWalker->condition == *c)
		{
			std::cout << "Transition from State " << id << " to State " << TWalker->state->id << " with '" << *c << "'" << std::endl;
			return TWalker->state;
		}
		TWalker = TWalker->next;
	}
	return NULL;
}

State::~State() {}