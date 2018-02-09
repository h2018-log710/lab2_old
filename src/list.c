#include "list.h"
#include <stdlib.h>

bool empty(const List* list)
{
	if (size(list) == 0)
	{
		return true;
	}
	
	return false;
}

size_t size(const List* list)
{
	if (list)
	{
		return list->size;
	}
	
	return 0;
}

Node* front(List* list)
{
	if (list)
	{
		return list->head;
	}
	
	return NULL;
}

Node* back(List* list)
{	
	if (list)
	{
		return list->tail;
	}
	
	return NULL;
}

void push_front(List* list, void* value)
{
	if (list)
	{
		Node* new = malloc(sizeof(Node));
		
		new->value = value;
		new->previous = NULL;
		new->next = list->head;
		
		if (list->size > 0)
		{
			list->head->previous = new;
		}
		
		else
		{
			list->tail = new;
		}
		
		list->head = new;
		++list->size;
	}
}

void push_back(List* list, void* value)
{
	if (list)
	{
		Node* new = malloc(sizeof(Node));
		
		new->value = value;
		new->previous = list->tail;
		new->next = NULL;
		
		if (list->size > 0)
		{
			list->tail->next = new;
		}
		
		else
		{
			list->head = new;
		}
		
		list->tail = new;
		++list->size;
	}
}

void pop_front(List* list)
{
	if (list && list->size > 0)
	{
		Node* new = list->head->next;
		free(list->head->value); // Free the value.
		free(list->head); // Free the node.
		
		if (list->size > 1)
		{
			new->previous = NULL;
		}
		
		else
		{
			list->tail = new;
		}
		
		list->head = new;
		--list->size;
	}
}

void pop_back(List* list)
{
	if (list && list->size > 0)
	{
		Node* new = list->tail->previous;
		free(list->tail->value); // Free the value.
		free(list->tail); // Free the node.
		
		if (list->size == 1)
		{
			new->next = NULL;
		}
		
		else
		{
			list->head = new;
		}
		
		list->tail = new;
		--list->size;
	}
}

void insert(List* list, void* value, int position)
{
	// TODO
}

void erase(List* list, int position)
{
	// TODO
}

void remove(List* list, void* value)
{
	// TODO
}

void clear(List* list)
{
	if (list)
	{
		clear(list->head);
		
		list->size = 0;
		list->head = NULL;
		list->tail = NULL;
	}
}

static void clear(Node* node)
{
	if (node)
	{
		Node* next = node->next;
		free(node->value); // Free the value.
		free(node); // Free the node.
		clear(next);
	}
}
