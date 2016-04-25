#ifndef EXPRESSIONTREE_H
#define EXPRESSIONTREE_H

#include "Node.hpp"
#include <string>
#include <stack>

typedef enum { NODE, FUNC } item_type;

class Expression_Tree {
	public:
		Node* root;

		Expression_Tree(std::string Regex);
};

class tree_item {
	public:
		item_type type;
		char character;
		Node* node;

		tree_item* copy()
		{
			tree_item* new_item = new tree_item(type, character);
			new_item->node = node->copy();
			return new_item;
		}

		tree_item(item_type type_in, char character_in = '\0') :type(type_in), character(character_in) 
		{
			if (type == NODE)
			{
				node = new Node(CHR, character);
			}
		}
};

void evaluate_stack(std::stack<tree_item*>* stack);

void evaluate_stack_paren(std::stack<tree_item*>* stack);

void evaluate_stack_repetition(std::stack<tree_item*>* stack);

void evaluate_stack_char_sequence(std::stack<tree_item*>* stack);
void collapse_concat_stack(std::stack<Node*>* stack);

void evaluate_stack_alter(std::stack<tree_item*>* stack);

#endif
