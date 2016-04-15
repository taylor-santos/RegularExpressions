#ifndef TRANSITION_H
#define TRANSITION_H

class Transition {
	public:
		bool epsilon;
		char condition;
		class State* state;
		Transition* next;
		Transition(char* condition_in, class State* state_in);
		~Transition();
};
#endif