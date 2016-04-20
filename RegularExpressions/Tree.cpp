#include "Tree.hpp"
#include "Node.hpp"
#include "NFA.hpp"
#include <string>
#include <stack>
#include <queue>
#include <iostream>
#include <assert.h>

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
			case '{':
			{
				brace_depth++;
				treeStack.push(new stack_item(FUNC, str[i]));
				break;
			}
			case '}':
			{
				assert(paren_depth > 0);
				collapse_stack_strings(&treeStack);
				evaluate_stack_paren(&treeStack);
				paren_depth--;
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
				treeStack.push(new stack_item(FUNC, str[i]));
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
			case ',':
			{
				if (brace_depth > 0)
					treeStack.push(new stack_item(FUNC, str[i]));
				else
					treeStack.push(new stack_item(NODE, str[i]));
				break;
			}
			default:
			{
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
