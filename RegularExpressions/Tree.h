#ifndef TREE_H
#define TREE_H

#include <string>

class Tree {
	public:
		class Node* root;

		Tree(class Node* root_in) : root(root_in) {}
};

Tree build_tree_from_expression(std::string str);

#endif
