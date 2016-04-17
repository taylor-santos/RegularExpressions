#ifndef NODE_H
#define NODE_H

typedef enum {CHR, STAR, DOT, QUESTION, ALTER, CONCAT} node_type;

class Node {
	public:
		node_type type;
		char data;
		Node* left;
		Node* right;

		class NFA* tree_to_nfa();

		Node(node_type type_in, char c = '\0') : type(type_in), data(c) {}
		~Node();
};

#endif