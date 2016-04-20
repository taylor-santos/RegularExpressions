#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>
#include "FSM.hpp"
#include "NFA.hpp"
#include "State.hpp"
#include "Transition.hpp"
#include "Node.hpp"
#include "Tree.hpp"

HANDLE hConsole;

int main()
{
	//system("Color F0");
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int whiteColor = 15;
	int greyColor = 8;
	SetConsoleTextAttribute(hConsole, whiteColor);
	Tree* newTree = build_tree_from_expression("(ABC|DEF|G*)+ABB");
	/*
	Node* star1 = new Node(PLUS);
	Node* a1 = new Node(CHR, 'a');
	star1->left = a1;

	Tree* tree = new Tree(star1);
	*/
	/*
	NFA a = build_nfa_basic('a');
	NFA b = build_nfa_basic('b');
	NFA alt = build_nfa_alter(&a, &b);
	NFA str = build_nfa_star(&alt);
	NFA sa = build_nfa_concat(&str, &a);
	NFA sab = build_nfa_concat(&sa, &b);
	NFA sabb = build_nfa_concat(&sab, &b);
	*/
	NFA fin = *newTree->root->tree_to_nfa();

	SetConsoleTextAttribute(hConsole, whiteColor);
	std::cout << " ";
	if (fin.state_count >= 10)
		std::cout << " ";
	for (int x = 0; x < fin.state_count; ++x) {
		std::cout << x << " ";
		if (x < 10 && fin.state_count >= 10)
			std::cout << " ";
	}
	SetConsoleTextAttribute(hConsole, greyColor);
	std::cout << std::endl;
	for (int y = 0; y < fin.state_count; ++y)
	{
		SetConsoleTextAttribute(hConsole, whiteColor);
		std::cout << y;
		SetConsoleTextAttribute(hConsole, greyColor);
		if (fin.state_count >= 10 && y < 10)
			std::cout << " ";
		for (int x = 0; x < fin.state_count; ++x)
		{
			if (fin.get_char(x, y) == '\0')
				std::cout << (char)197 << (char)196;
			else {
				SetConsoleTextAttribute(hConsole, whiteColor);
				std::cout << fin.get_char(x, y);
				SetConsoleTextAttribute(hConsole, 8);
				if (x < fin.state_count - 1)
					std::cout << (char)196;
			}
				
			if (fin.state_count >= 10 && x < fin.state_count-1)
				std::cout << (char)196;
		}
		std::cout << std::endl;
		if (y < fin.state_count - 1)
		{
			for (int x = 0; x < fin.state_count; ++x)
			{
				if (fin.state_count >= 10)
					std::cout << " ";
				std::cout << " " << (char)179;

			}
		}
		std::cout << std::endl;
	}
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << "Start: " << fin.start << std::endl << "End: " << fin.end << std::endl << *fin.regex << std::endl;
	while (1);
}