#include <iostream>
#include <vector>
#include <string>
#include "FSM.hpp"
#include "NFA.hpp"
#include "State.hpp"
#include "Transition.hpp"

int main()
{
	NFA a = build_nfa_basic('a');
	NFA b = build_nfa_basic('b');
	NFA alt = build_nfa_alter(&a, &b);
	NFA str = build_nfa_star(&alt);
	NFA sa = build_nfa_concat(&str, &b);
	NFA sab = build_nfa_concat(&sa, &b);
	NFA sabb = build_nfa_concat(&sab, &b);

	NFA fin = sabb;
	std::cout << "   ";
	for (int x = 0; x < fin.state_count; ++x) {
		std::cout << x << " ";
	}
	std::cout << std::endl;
	for (int y = 0; y < fin.state_count; ++y)
	{
		std::cout << y << "  ";
		for (int x = 0; x < fin.state_count; ++x)
		{
			std::cout << fin.get_char(x, y) << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "Start: " << fin.start << std::endl << "End: " << fin.end << std::endl;
	while (1);
}