#include "Tree.h"
#include "Node.hpp"
#include <string>
#include <stack>
#include <iostream>

typedef enum{NODE,FUNC} item_type;

class stack_item {
	public:
		item_type type;
		Node* node;
		char function;
		stack_item(item_type type_in, char c)
		{
			if (type_in == NODE)
			{
				type = NODE;
				node = new Node(CHR, c);
			}
			else if (type_in == FUNC)
			{
				type = FUNC;
				function = c;
			}
		}
};

bool is_special(char c)
{
	if (c == '\\' ||
		c == '^' ||
		c == '$' ||
		c == '{' ||
		c == '}' ||
		c == '[' ||
		c == ']' ||
		c == '(' ||
		c == ')' ||
		c == '<' ||
		c == '>' ||
		c == '.' ||
		c == '*' ||
		c == '+' ||
		c == '?' ||
		c == '|' ||
		c == '-' ||
		c == ',' ||
		c == '&') {
		return true;
	}
	return false;
}

Tree build_tree_from_expression(std::string str)
{
	std::string::iterator i = str.begin();
	std::stack<stack_item*> charStack;
	while (i != str.end())
	{
		if (!is_special(*i))
		{
			charStack.push(new stack_item(NODE, *i));
		}
		else if (*i == '\\')
		{
			i++;
			if (*i - '0' >= 0 && *i - '0' <= 9)
			{
				charStack.push(new stack_item(FUNC, *i));
			}
			else {
				charStack.push(new stack_item(NODE, *i));
			}
		}else
		{
			charStack.push(new stack_item(FUNC, *i));
		}
		i++;
	}
	return NULL;
}