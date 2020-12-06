#include <iostream>
#include <string>

// Node structure contains pointer for the
// previous object and it's own value
int size = 0;
Node* up = nullptr;

struct Node
{
	Node* prev;
	int number;
};
void push(int value)
{
	// creating pointer for the element we want to push
	Node* new_element = new Node;
	// updating the new element fields
	new_element->prev = up;
	new_element->number = value;
	// updating top element pointer
	up = new_element;
	std::cout << "ok" << std::endl;
	// updating size variable since we add one new element
	size++;
}

void pop()
{
	// if we don't have any top element we can't return anything
	if (up == nullptr)
		std::cout << "error" << std::endl;
	else
	{
		// printing top value in the stack
		std::cout << up->number << std::endl;
		// changing top element pointer to previous
		Node* log = up;
		up = up->prev;
		// freeing waste data
		delete log;
		// updating size value since we remove one element
		size--;
	}
}


void back()
{
	// if we don't have any top element we can't return anything
	if (up == nullptr)
		std::cout << "error" << std::endl;
	// just printing value of the last element
	else
		std::cout << up->number << std::endl;
}

void get_size()
{
	std::cout << size << std::endl;
}

void exit()
{
	std::cout << "bye" << std::endl;
	exit(0);
}


void clear()
{
	// deleting the top element while it is possible
	while (up != nullptr)
	{
		// deleting the top element operation
		Node* log = up;
		up = up->prev;
		// freeing waste data
		delete log;
	}
	std::cout << "ok" << std::endl;
	size = 0;
}

void stack()
{
	std::string command = "";// up is a name for the top object in the stack
	Node* up = nullptr;
	// size variable is an easy way to control
	// number of containing elements
	int size = 0;
	// reading command and processing it while 
	// the command is not "exit"
	while (command != "exit")
	{
		std::cin >> command;
		// pushing element
		if (command == "push")
		{
			int value;
			std::cin >> value;
			push(value);
		}
		// removing top element
		else if (command == "pop")
		{
			pop();
		}
		// printing last element
		else if (command == "back")
		{
			back();
		}
		// printing stack size
		else if (command == "size")
			get_size();
		// clearing the whole stack
		else if (command == "clear")
		{
			clear();
		}
		// exiting
		else if (command == "exit")
		{
			exit();
		}
	}
}

// I hope it's not a problem that i've emplaced the whole program in just one function 
int main()
{
	stack();
}
