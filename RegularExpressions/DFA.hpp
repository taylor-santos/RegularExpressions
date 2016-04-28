#ifndef DFA_H
#define DFA_H

#include <vector>
#include "NFA.hpp"

class DFA {
	std::vector<class DFA_state*> states;
	public:
		DFA(NFA* nfa);
};

class DFA_state {
	public:
		std::vector<class DFA_transition*> transitions;
		bool accepting = false;
		std::vector<int> ids;
		void addTransition(char c, int dest);
		DFA_state(std::vector<int> ids_in) : ids(ids_in) {}
};

class DFA_transition {
	
	public:
		char condition;
		std::vector<int> destinations;
		DFA_state* dest_state;
};

DFA_state* collapse_nfa_state(NFA* nfa, int state);

#endif
