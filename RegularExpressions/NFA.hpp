#include <vector>

#ifndef NFA_H
#define NFA_H

class NFA {
		std::vector<std::vector<char>*>* transition_table;
	public:
		int state_count;
		int start;
		int end;
		void append();
		void shift(int count);
		void fill(NFA* other);
		void set_char(int from, int to, char c);
		const char get_char(int from, int to);
		NFA(int states);
		~NFA();
};

NFA build_nfa_basic(char c);
NFA build_nfa_alter(NFA* nfa1, NFA* nfa2);
NFA build_nfa_star(NFA* nfa);
NFA build_nfa_concat(NFA* nfa1, NFA* nfa2);

#endif