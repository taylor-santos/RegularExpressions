#include "DFA.hpp"
#include <algorithm>

DFA_state* collapse_nfa_state(NFA* nfa, int state)
{
	std::vector<int> states;
	states.push_back(state);
	int i = 0;
	bool accepting = false;
	if (nfa->end == state)
		accepting = true;
	while (i < states.size())
	{
		std::vector<char>::iterator j = (*(nfa->transition_table))[states[i]]->begin();
		while (j != (*(nfa->transition_table))[states[i]]->end())
		{
			if (*j == (char)eps)
			{
				int id = j - (*(nfa->transition_table))[states[i]]->begin();
				if (std::find(states.begin(), states.end(), id) == states.end())
				{
					states.push_back(id);
					if (nfa->end == id)
						accepting = true;
				}
			}
			j++;
		}
		i++;
	}
	std::sort(states.begin(), states.end());
	DFA_state* newState = new DFA_state(states);
	newState->accepting = accepting;
	return newState;
}

DFA::DFA(NFA* nfa)
{
	//std::vector<DFA_state*> states;
	states.push_back(collapse_nfa_state(nfa,nfa->start));
	int i = 0;
	while (i < states.size())
	{
		std::vector<int>::iterator j = states[i]->ids.begin();
		while (j != states[i]->ids.end())
		{
			std::vector<char>::iterator k = (*(nfa->transition_table))[*j]->begin();
			while (k != (*(nfa->transition_table))[*j]->end())
			{
				if (*k != '\0' && *k != (char)eps)
				{
					int dest = k - (*(nfa->transition_table))[*j]->begin();
					states[i]->addTransition(*k, dest);
				}
				k++;
			}
			j++;
		}
		std::vector<DFA_transition*>::iterator l = states[i]->transitions.begin();
		while (l != states[i]->transitions.end())
		{
			std::sort((*l)->destinations.begin(), (*l)->destinations.end());
			bool foundState = false;
			for (int m = 0; m < states.size(); ++m)
			{
				if ((*l)->destinations == states[m]->ids)
				{
					(*l)->dest_state = states[m];
					foundState = true;
					break;
				}
			}
			if (!foundState)
			{
				DFA_state* newState = new DFA_state((*l)->destinations);
				newState = collapse_nfa_state(nfa, (newState->ids[0]));
				(*l)->dest_state = newState;
				states.push_back(newState);
			}
			l++;
		}

		i++;
	}
}

void DFA_state::addTransition(char c, int dest)
{
	std::vector<class DFA_transition*>::iterator i = transitions.begin();
	while (i != transitions.end())
	{
		if ((*i)->condition == c)
		{
			if (find((*i)->destinations.begin(), (*i)->destinations.end(), dest) == (*i)->destinations.end())
			{
				(*i)->destinations.push_back(dest);
			}
			return;
		}
		std::sort((*i)->destinations.begin(), (*i)->destinations.end());
		i++;
	}
	DFA_transition* t = new DFA_transition();
	t->condition = c;
	t->destinations.push_back(dest);
	transitions.push_back(t);
}