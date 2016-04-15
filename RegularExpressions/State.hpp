#include "Transition.hpp"

#ifndef STATE_H
#define STATE_H

class State {
	int id;
	Transition* transitions;
	
	public:
		State* move(char* c);
		void add_transition(Transition* t);
		State(int id_in);
		~State();
};
#endif
