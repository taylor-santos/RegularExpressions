#include "Tree.hpp"
#include "Node.hpp"
#include "NFA.hpp"
#include <string>
#include <stack>
#include <queue>
#include <iostream>
#include <assert.h>
#include <algorithm>

#define MIN_CHAR 32
#define MAX_CHAR 126

typedef enum{NODE,FUNC} item_type;

class stack_item {
	public:
		item_type type;
		Node* node;
		char function;

		stack_item* copy()
		{
			stack_item* newItem = new stack_item(FUNC, function);
			newItem->type = type;
			newItem->node = node->copy();
			newItem->function = function;
			return newItem;
		}

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

char control_character(char c)
{
	switch (c)
	{
		case 'a':
			return (char)7;
		case 'b':
			return (char)8;
		case 'e':
			return (char)27;
		case 'f':
			return (char)12;
		case 'n':
			return (char)10;
		case 'r':
			return (char)13;
		case 't':
			return (char)9;
		case 'v':
			return (char)11;
		default:
			return c;
	}
}

void evaluate_stack(std::stack<stack_item*>* stack);
void collapse_stack_strings(std::stack<stack_item*>* stack);
void evaluate_stack_paren(std::stack<stack_item*>* stack);
void evaluate_stack_braces(std::stack<stack_item*>* stack);
void evaluate_stack_brackets(std::stack<stack_item*>* stack);

void evaluate_stack(std::stack<stack_item*>* stack)
{
	if (stack->size() <= 1)
		return;
	stack_item* top_item = stack->top();
	stack->pop();
	if (top_item->type == NODE)
	{
		assert(stack->top()->type == FUNC);
		if (stack->top()->function == '|')
		{
			stack->pop();
			collapse_stack_strings(stack);
			evaluate_stack(stack);
			Node* newNode = new Node(ALTER);
			newNode->left = stack->top()->node;
			newNode->right = top_item->node;
			stack->top()->node = newNode;
			return;
		}
		else {
			evaluate_stack(stack);
			Node* newNode = new Node(CONCAT);
			newNode->left = stack->top()->node;
			newNode->right = top_item->node;
			stack->top()->node = newNode;
			return;
		}
	}
	else if (top_item->type == FUNC)
	{
		assert(stack->top()->type == NODE);
		if (top_item->function == '*')
		{
			Node* newNode = new Node(STAR);
			newNode->left = stack->top()->node;
			stack->top()->node = newNode;
			return;
		}
		else if (top_item->function == '+')
		{
			Node* newNode = new Node(PLUS);
			newNode->left = stack->top()->node;
			stack->top()->node = newNode;
			return;
		}
		else if (top_item->function == '?')
		{
			Node* newNode = new Node(QUESTION);
			newNode->left = stack->top()->node;
			stack->top()->node = newNode;
			return;
		}
	}
}

void collapse_stack_strings(std::stack<stack_item*>* stack)
{
	if (stack->size() <= 1)
		return;
	stack_item* top_item = stack->top();
	if (top_item->type == NODE)
	{
		stack->pop();
		if (stack->top()->type == NODE)
		{
			collapse_stack_strings(stack);
			Node* newNode = new Node(CONCAT);
			newNode->left = stack->top()->node;
			newNode->right = top_item->node;
			stack->top()->node = newNode;
			return;
		}
		else if (stack->top()->type == FUNC)
		{
			stack_item* func_item = stack->top();
			stack->pop();
			collapse_stack_strings(stack);
			stack->push(func_item);
			stack->push(top_item);
			return;
		}
	}
	else {
		stack->pop();
		collapse_stack_strings(stack);
		stack->push(top_item);
		return;
	}
}

void evaluate_stack_paren(std::stack<stack_item*>* stack)
{
	std::stack<stack_item*> stack1;
	while (stack->top()->type == CHR || stack->top()->function != '(')
	{

		stack1.push(stack->top());
		stack->pop();
		assert(!stack->empty());
	}
	std::stack<stack_item*> itemStack;
	while (!stack1.empty())
	{
		itemStack.push(stack1.top());
		stack1.pop();
	}
	while (itemStack.size() > 1)
	{
		evaluate_stack(&itemStack);
	}
	stack->pop();
	stack->push(itemStack.top());
}

void evaluate_stack_braces(std::stack<stack_item*>* stack)
{
	std::stack<stack_item*> stack1;
	while (stack->top()->function != '{')
	{
		stack1.push(stack->top());
		stack->pop();
		assert(!stack->empty());
	}
	std::stack<stack_item*> itemStack;
	while (!stack1.empty())
	{
		itemStack.push(stack1.top());
		stack1.pop();
	}
	int min = -1;
	int max = -1;
	int magnitude = 1;
	assert(!itemStack.empty());
	if (itemStack.top()->function != ',')
	{
		max = 0;
	}
	while (!itemStack.empty() && itemStack.top()->function != ',')
	{
		max += (itemStack.top()->function - '0') * magnitude;
		magnitude *= 10;
		itemStack.pop();
	}
	if (itemStack.empty())
	{
		min = max;
	}
	else {
		assert(itemStack.top()->function == ',');
		itemStack.pop();
		min = 0;
		magnitude = 1;
		while (!itemStack.empty())
		{
			min += (itemStack.top()->function - '0') * magnitude;
			magnitude *= 10;
			itemStack.pop();
		}
	}
	stack->pop();
	stack_item* prev_item = stack->top();
	stack->pop();
	assert(min != -1);
	assert(max == -1 || max >= min);
	for (int i = 0; i < min; ++i)
	{
		stack->push(prev_item->copy());
	}
	if (max == -1)
	{
		stack->push(prev_item->copy());
		Node* newNode = new Node(STAR);
		newNode->left = stack->top()->node;
		stack->top()->node = newNode;
	}
	else {
		for (int i = min; i < max; ++i)
		{
			stack->push(prev_item->copy());
			Node* newNode = new Node(QUESTION);
			newNode->left = stack->top()->node;
			stack->top()->node = newNode;
		}
	}
	return;
}

void evaluate_stack_brackets(std::stack<stack_item*>* stack)
{
	std::stack<stack_item*> stack1;
	while (stack->top()->function != '[')
	{
		stack1.push(stack->top());
		stack->pop();
		assert(!stack->empty());
	}
	std::stack<stack_item*> itemStack;
	while (!stack1.empty())
	{
		itemStack.push(stack1.top());
		stack1.pop();
	}
	std::stack<char> charStack;
	while (!itemStack.empty())
	{
		assert(itemStack.top()->type != FUNC || itemStack.top()->function == '-');
		if (itemStack.top()->type == NODE)
		{
			assert(itemStack.top()->node->type == CHR);
			charStack.push(itemStack.top()->node->data);
			itemStack.pop();
		}
		else if (itemStack.top()->function == '-')
		{
			itemStack.pop();
			assert(itemStack.top()->node->type == CHR);
			char firstChar = itemStack.top()->node->data;
			itemStack.pop();
			char lastChar = charStack.top();
			charStack.pop();
			int a = std::min((int)firstChar, (int)lastChar);
			int b = std::max((int)firstChar, (int)lastChar);
			for (int i = a; i <= b; ++i)
			{
				charStack.push((char)i);
			}
		}
	}
	stack->pop();
	while (!charStack.empty())
	{
		stack->push(new stack_item(NODE, charStack.top()));
		stack->push(new stack_item(FUNC, '|'));
		charStack.pop();
	}
	stack->pop(); //Remove trailing '|'
	evaluate_stack(stack);
}

Tree* build_tree_from_expression(std::string str)
{
	int i = 0;
	std::stack<stack_item*> treeStack;
	int paren_depth = 0;
	int bracket_depth = 0;
	int brace_depth = 0;
	while (i < str.length())
	{
		switch (str[i])
		{
			case '\\':
			{
				i++;
				assert(i < str.length());
				if (str[i] - '0' >= 0 && str[i] - '0' <= 9)
				{
					treeStack.push(new stack_item(FUNC, str[i]));
				}
				else {
					treeStack.push(new stack_item(NODE, control_character(str[i])));
				}
				break;
			}
			case '(':
			{
				paren_depth++;
				treeStack.push(new stack_item(FUNC, str[i]));
				break;
			}
			case ')':
			{
				assert(paren_depth > 0);
				collapse_stack_strings(&treeStack);
				evaluate_stack_paren(&treeStack);
				paren_depth--;
				break;
			}
			case '[':
			{
				bracket_depth++;
				treeStack.push(new stack_item(FUNC, str[i]));
				break;
			}
			case ']':
			{
				assert(bracket_depth > 0);
				evaluate_stack_brackets(&treeStack);
				collapse_stack_strings(&treeStack);
				bracket_depth--;
				break;
			}
			case '{':
			{
				brace_depth++;
				treeStack.push(new stack_item(FUNC, str[i]));
				break;
			}
			case '}':
			{
				assert(brace_depth > 0);
				evaluate_stack_braces(&treeStack);
				collapse_stack_strings(&treeStack);
				brace_depth--;
				break;
			}
			case '^':
			{
				treeStack.push(new stack_item(FUNC, str[i]));
				break;
			}
			case '$':
			{
				treeStack.push(new stack_item(FUNC, str[i]));
				break;
			}
			case '.':
			{
				treeStack.push(new stack_item(FUNC, '['));
				treeStack.push(new stack_item(NODE, (char)MIN_CHAR));
				treeStack.push(new stack_item(FUNC, '-'));
				treeStack.push(new stack_item(NODE, (char)MAX_CHAR));
				evaluate_stack_brackets(&treeStack);
				collapse_stack_strings(&treeStack);
				//treeStack.push(new stack_item(FUNC, str[i]));
				break;
			}
			case '*':
			{
				treeStack.push(new stack_item(FUNC, str[i]));
				break;
			}
			case '+':
			{
				treeStack.push(new stack_item(FUNC, str[i]));
				break;
			}
			case '?':
			{
				treeStack.push(new stack_item(FUNC, str[i]));
				break;
			}
			case '|':
			{
				treeStack.push(new stack_item(FUNC, str[i]));
				break;
			}
			case '-':
			{
				if (bracket_depth > 0)
					treeStack.push(new stack_item(FUNC, str[i]));
				else
					treeStack.push(new stack_item(NODE, str[i]));
				break;
			}
			default:
			{
				if (brace_depth > 0)
				{
					treeStack.push(new stack_item(FUNC, str[i]));
					assert(str[i] == ',' || (str[i] - '0' >= 0 && str[i] - '0' <= 9));
				}else
					treeStack.push(new stack_item(NODE, str[i]));
				break;
			}
		}
		i++;
	}
	collapse_stack_strings(&treeStack);
	evaluate_stack(&treeStack);
	return new Tree(treeStack.top()->node);
}
