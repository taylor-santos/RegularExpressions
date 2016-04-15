#include <iostream>
#include <string>
#include "FSM.hpp"
#include "State.hpp"
#include "Transition.hpp"

int main()
{
	FSM* Regex = new FSM();
	State* state_0 = new State(0);
	State* state_1 = new State(1);
	State* state_2 = new State(2);
	State* state_3 = new State(3);
	state_0->add_transition(new Transition(new char('b'), state_0));
	state_0->add_transition(new Transition(new char('a'), state_1));
	state_1->add_transition(new Transition(new char('a'), state_1));
	state_1->add_transition(new Transition(new char('b'), state_2));
	state_2->add_transition(new Transition(new char('a'), state_1));
	state_2->add_transition(new Transition(new char('b'), state_3));
	state_3->add_transition(new Transition(new char('b'), state_0));
	state_3->add_transition(new Transition(new char('a'), state_1));
	Regex->set_start(state_0);
	Regex->set_final(state_3);
	if (Regex->recognize("ababababababbbababbababbabaabb"))
	{
		std::cout << "Yes.\n";
	}
	else {
		std::cout << "No.\n";
	}
	while (1);
}