#include <iostream>
#include <assert.h>
#include "Node.hpp"
#include "NFA.hpp"

NFA* Node::tree_to_nfa()
{
	switch (type)
	{
		case CHR:
			return &build_nfa_basic(data);
		case ALTER:
		{
			assert(left != NULL && right != NULL);
			NFA left_nfa = *left->tree_to_nfa();
			NFA right_nfa = *right->tree_to_nfa();
			return &build_nfa_alter(&left_nfa, &right_nfa);
		}
		case STAR:
		{
			assert((left == NULL) ^ (right == NULL)); //left XOR right. Only one may be undefined. Both can't be defined.
			if (left != NULL)
			{
				NFA nfa = *left->tree_to_nfa();
				return &build_nfa_star(&nfa);
			}
			else {
				NFA nfa = *right->tree_to_nfa();
				return &build_nfa_star(&nfa);
			}
		}
		case PLUS:
		{
			assert((left == NULL) ^ (right == NULL));
			if (left != NULL)
			{
				NFA nfa = *left->tree_to_nfa();
				return &build_nfa_plus(&nfa);
			}
			else {
				NFA nfa = *right->tree_to_nfa();
				return &build_nfa_plus(&nfa);
			}
		}
		case CONCAT:
		{
			assert(left != NULL && right != NULL);
			NFA left_nfa = *left->tree_to_nfa();
			NFA right_nfa = *right->tree_to_nfa();
			return &build_nfa_concat(&left_nfa, &right_nfa);
		}
		case QUESTION:
		{
			assert((left == NULL) ^ (right == NULL));
			if (left != NULL)
			{
				NFA nfa = *left->tree_to_nfa();
				return &build_nfa_question(&nfa);
			}
			else
			{
				NFA nfa = *right->tree_to_nfa();
				return &build_nfa_question(&nfa);
			}
		}
		default:
			assert(0);
	}
}

Node* Node::copy()
{
	Node* newNode = new Node(type, data);
	if (left != NULL)
	{
		newNode->left = left->copy();
	}
	if (right != NULL)
	{
		newNode->right = right->copy();
	}
	return newNode;
}

Node::~Node() {}