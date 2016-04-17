#include <iostream>
#include <vector>
#include <string>
#include "FSM.hpp"
#include "NFA.hpp"
#include "State.hpp"
#include "Transition.hpp"
#include "Node.hpp"
#include "Tree.h"

int main()
{
	for (int i = 0; i < 256; ++i)
	{
		std::cout << i << ": " << (char)i << std::endl;
	}

	build_tree_from_expression("(a|b\\))*abb");

	Node* concat1 = new Node(CONCAT);
	Node* b1 = new Node(CHR, 'b');
	concat1->right = b1;
	Node* concat2 = new Node(CONCAT);
	concat1->left = concat2;
	Node* b2 = new Node(CHR, 'b');
	concat2->right = b2;
	Node* concat3 = new Node(CONCAT);
	concat2->left = concat3;
	Node* a1 = new Node(CHR, 'a');
	concat3->right = a1;
	Node* star1 = new Node(QUESTION);
	concat3->left = star1;
	Node* alter1 = new Node(ALTER);
	star1->left = alter1;
	Node* a2 = new Node(CHR, 'a');
	alter1->left = a2;
	Node* b3 = new Node(CHR, 'b');
	alter1->right = b3;

	Tree* tree = new Tree(concat1);

	/*
	NFA a = build_nfa_basic('a');
	NFA b = build_nfa_basic('b');
	NFA alt = build_nfa_alter(&a, &b);
	NFA str = build_nfa_star(&alt);
	NFA sa = build_nfa_concat(&str, &a);
	NFA sab = build_nfa_concat(&sa, &b);
	NFA sabb = build_nfa_concat(&sab, &b);
	*/
	NFA fin = *tree->root->tree_to_nfa();
	std::cout << "  ";
	if (fin.state_count >= 10)
		std::cout << " ";
	for (int x = 0; x < fin.state_count; ++x) {
		std::cout << x << " ";
		if (x < 10 && fin.state_count >= 10)
			std::cout << " ";
	}
	std::cout << std::endl;
	for (int y = 0; y < fin.state_count; ++y)
	{
		std::cout << y << " ";
		if (fin.state_count >= 10 && y < 10)
			std::cout << " ";
		for (int x = 0; x < fin.state_count; ++x)
		{
			std::cout << fin.get_char(x, y) << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "Start: " << fin.start << std::endl << "End: " << fin.end << std::endl;
	while (1);
}