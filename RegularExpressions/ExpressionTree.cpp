#include "ExpressionTree.hpp"
#include <stack>
#include <assert.h>

#define MIN_CHAR 32
#define MAX_CHAR 126

Expression_Tree::Expression_Tree(std::string Regex)
{
	std::string::iterator i = Regex.begin();
	std::stack<tree_item*> item_stack;
	std::stack<tree_item*> braces_stack;
	std::stack<tree_item*> brackets_stack;
	int braces_comma_count = 0;
	bool inBraces = false;
	bool inBrackets = false;
	while (i != Regex.end())
	{
		char c = *i;
		if (inBraces)
		{
			if (c - '0' >= 0 && c - '0' <= 9)
			{
				braces_stack.push(new tree_item(FUNC, c));
			}
			else if (c == ',' && braces_comma_count == 0) {
				braces_stack.push(new tree_item(FUNC, c));
				braces_comma_count++;
			}
			else if (c == '}' && braces_stack.size() > 1)
			{
				int min = -1;
				int max = -1;
				int magnitude = 1;
				if (braces_stack.top()->character != ',')
				{
					max = 0;
				}
				while (braces_stack.size() > 1 && braces_stack.top()->character != ',')
				{
					max += (braces_stack.top()->character - '0') * magnitude;
					magnitude *= 10;
					braces_stack.pop();
				}
				braces_stack.pop();
				if (braces_stack.empty())
				{
					min = max;
				}
				else {
					min = 0;
					magnitude = 1;
					while (braces_stack.size() > 1)
					{
						min += (braces_stack.top()->character - '0') * magnitude;
						magnitude *= 10;
						braces_stack.pop();
					}
				}
				tree_item* top = item_stack.top();
				item_stack.pop();
				
				for (int i = 0; i < min; ++i)
				{
					item_stack.push(top->copy());
				}
				if (max == -1)
				{
					item_stack.push(top->copy());
					Node* newNode = new Node(STAR);
					newNode->left = item_stack.top()->node;
					item_stack.top()->node = newNode;
				}
				else {
					for (int i = min; i < max; ++i)
					{
						item_stack.push(top->copy());
						Node* newNode = new Node(QUESTION);
						newNode->left = item_stack.top()->node;
						item_stack.top()->node = newNode;
					}
				}

				braces_comma_count = 0;
				inBraces = false;
			}
			else {
				braces_comma_count = 0;
				inBraces = false;
				while (!braces_stack.empty())
				{
					i--;
					braces_stack.pop();
				}
				item_stack.push(new tree_item(NODE, '{'));
			}
		}
		else if (inBrackets)
		{
			if (c == '\\')
			{
				assert(i != Regex.end());
				i++;
				c = *i;
				brackets_stack.push(new tree_item(NODE, c));
			}
			else if (c == '-')
			{
				brackets_stack.push(new tree_item(FUNC, c));
			}
			else if (c != ']')
			{
				brackets_stack.push(new tree_item(NODE, c));
			}
			else if (c == ']' && brackets_stack.size() > 1)
			{
				std::stack<char> char_stack;
				while (brackets_stack.size() > 1)
				{
					if (brackets_stack.top()->type == NODE)
					{
						char_stack.push(brackets_stack.top()->character);
						brackets_stack.pop();
					}
					else {
						assert(brackets_stack.top()->character == '-');
						brackets_stack.pop();
						int a = char_stack.top();
						char_stack.pop();
						int b = brackets_stack.top()->character;
						brackets_stack.pop();
						int min = a;
						int max = b;
						if (a > b)
						{
							min = b;
							max = a;
						}
						for (int i = min; i <= max; ++i)
						{
							char_stack.push((char)i);
						}
					}
				}
				brackets_stack.pop();
				std::stack<tree_item*> new_stack;
				new_stack.push(new tree_item(NODE, char_stack.top()));
				char_stack.pop();
				while (!char_stack.empty())
				{
					new_stack.push(new tree_item(FUNC, '|'));
					new_stack.push(new tree_item(NODE, char_stack.top()));
					char_stack.pop();
				}
				evaluate_stack(&new_stack);
				item_stack.push(new_stack.top());
				inBrackets = false;
			}
			else {
				inBrackets = false;
				while (!brackets_stack.empty())
				{
					i--;
					brackets_stack.pop();
				}
				item_stack.push(new tree_item(NODE, '['));
			}
		}
		else {
			switch (c) {
				case '(': {
					item_stack.push(new tree_item(FUNC, c));
					break;
				}
				case ')': {
					evaluate_stack_paren(&item_stack);
					break;
				}
				case '{': {
					inBraces = true;
					braces_stack.push(new tree_item(FUNC, c));
					break;
				}
				case '[': {
					inBrackets = true;
					brackets_stack.push(new tree_item(FUNC, c));
					break;
				}
				case '*': {
					item_stack.push(new tree_item(FUNC, c));
					break;
				}
				case '?': {
					item_stack.push(new tree_item(FUNC, c));
					break;
				}
				case '+': {
					item_stack.push(new tree_item(FUNC, c));
					break;
				}
				case '|': {
					item_stack.push(new tree_item(FUNC, c));
					break;
				}
				case '.': {
					std::stack<tree_item*> dotStack;
					dotStack.push(new tree_item(NODE, (char)MIN_CHAR));
					for (int i = MIN_CHAR+1; i < MAX_CHAR; ++i)
					{
						dotStack.push(new tree_item(FUNC, '|'));
						dotStack.push(new tree_item(NODE, (char)i));
					}
					evaluate_stack_alter(&dotStack);
					item_stack.push(dotStack.top());
				}
				default:
				{
					item_stack.push(new tree_item(NODE, c));
					break;
				}
			}
		}
		i++;
	}
	evaluate_stack(&item_stack);
	root = item_stack.top()->node;
}

void evaluate_stack(std::stack<tree_item*>* stack)
{
	evaluate_stack_repetition(stack);
	evaluate_stack_char_sequence(stack);
	evaluate_stack_alter(stack);
	return;
}

void evaluate_stack_paren(std::stack<tree_item*>* stack)
{
	std::stack<tree_item*> paren_stack;
	while (stack->top()->type == NODE || stack->top()->character != '(')
	{
		paren_stack.push(stack->top());
		stack->pop();
		assert(!stack->empty());
	}
	stack->pop();
	std::stack<tree_item*> flip_paren_stack;
	while (!paren_stack.empty())
	{
		flip_paren_stack.push(paren_stack.top());
		paren_stack.pop();
	}
	evaluate_stack(&flip_paren_stack);
	stack->push(flip_paren_stack.top());
}

void evaluate_stack_repetition(std::stack<tree_item*>* stack)
{
	if (stack->size() == 1)
		return;
	std::stack<tree_item*> new_stack;
	while (!stack->empty())
	{
		if (stack->top()->type == FUNC)
		{
			switch (stack->top()->character)
			{
				case '*':
				{
					stack->pop();
					Node* newNode = new Node(STAR);
					newNode->left = stack->top()->node;
					stack->top()->node = newNode;
					stack->top()->character = '\0';
					break;
				}
				case '?':
				{
					stack->pop();
					Node* newNode = new Node(QUESTION);
					newNode->left = stack->top()->node;
					stack->top()->node = newNode;
					stack->top()->character = '\0';
					break;
				}
				case '+':
				{
					stack->pop();
					Node* newNode = new Node(PLUS);
					newNode->left = stack->top()->node;
					stack->top()->node = newNode;
					stack->top()->character = '\0';
					break;
				}
			}
		}
		new_stack.push(stack->top());
		stack->pop();
	}
	while (!new_stack.empty())
	{
		stack->push(new_stack.top());
		new_stack.pop();
	}
	return;
}

void evaluate_stack_char_sequence(std::stack<tree_item*>* stack)
{
	if (stack->size() == 0)
	{
		return;
	}
	tree_item* top = stack->top();
	stack->pop();
	if (top->type == FUNC)
	{
		evaluate_stack_char_sequence(stack);
		stack->push(top);
		return;
	}
	else
	{
		std::stack<tree_item*> new_stack;
		new_stack.push(top);
		while (!stack->empty() && stack->top()->type == NODE)
		{
			new_stack.push(stack->top());
			stack->pop();
		}
		
		std::stack<Node*> flip_new_stack_nodes;
		while (!new_stack.empty())
		{
			flip_new_stack_nodes.push(new_stack.top()->node);
			new_stack.pop();
		}
		
		collapse_concat_stack(&flip_new_stack_nodes);
		evaluate_stack_char_sequence(stack);
		stack->push(new tree_item(NODE));
		stack->top()->node = flip_new_stack_nodes.top();
	}
}

void collapse_concat_stack(std::stack<Node*>* stack)
{
	if (stack->size() <= 1)
		return;
	std::stack<Node*> newStack;
	while (stack->size() >= 2)
	{
		Node* right = stack->top();
		stack->pop();
		Node* left = stack->top();
		stack->pop();
		Node* newNode = new Node(CONCAT);
		newNode->left = left;
		newNode->right = right;
		newStack.push(newNode);
	}
	if (stack->size() == 1)
	{
		newStack.push(stack->top());
		stack->pop();
	}
	std::stack<Node*> flip_newStack;
	while (!newStack.empty())
	{
		flip_newStack.push(newStack.top());
		newStack.pop();
	}
	collapse_concat_stack(&flip_newStack);
	assert(flip_newStack.size() == 1);
	stack->push(flip_newStack.top());
	return;
}

void evaluate_stack_alter(std::stack<tree_item*>* stack)
{
	//At this point, due to order of operations, the stack should look like [node, |, node, |, node] with no functions other than '|', and no sequences of nodes.
	if (stack->size() <= 1)
		return;
	std::stack<tree_item*> flip_newStack;
	while (!stack->empty())
	{
		if (stack->top()->type == FUNC)
		{
			//assert(stack->top()->character == '|');
			stack->pop();
		}
		else {
			flip_newStack.push(stack->top());
			stack->pop();
		}
	}
	std::stack<tree_item*> newStack;
	while (flip_newStack.size() >= 2)
	{
		tree_item* left = flip_newStack.top();
		flip_newStack.pop();
		tree_item* right = flip_newStack.top();
		flip_newStack.pop();
		Node* newNode = new Node(ALTER);
		newNode->left = left->node;
		newNode->right = right->node;
		newStack.push(new tree_item(NODE));
		newStack.top()->node = newNode;
	}
	if (flip_newStack.size() == 1)
	{
		newStack.push(flip_newStack.top());
		flip_newStack.pop(); //Unnecessary
	}
	evaluate_stack_alter(&newStack);
	assert(newStack.size() == 1);
	stack->push(newStack.top());
}