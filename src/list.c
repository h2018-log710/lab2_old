#include "list.h"
#include <stdlib.h>

bool list_empty(const List* list)
{
	if (list_size(list) == 0)
	{
		return true;
	}
	
	return false;
}

size_t list_size(const List* list)
{
	if (list)
	{
		return list->size;
	}
	
	return 0;
}

Node* list_front(List* list)
{
	if (list)
	{
		return list->head;
	}
	
	return NULL;
}

Node* list_back(List* list)
{	
	if (list)
	{
		return list->tail;
	}
	
	return NULL;
}

void list_push_front(List* list, void* value)
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

void list_push_back(List* list, void* value)
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

void list_pop_front(List* list)
{
	if (list && list->size > 0)
	{
		Node* new = list->head->next;
		free(list->head->value);
		free(list->head);
		
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

void list_pop_back(List* list)
{
	if (list && list->size > 0)
	{
		Node* new = list->tail->previous;
		free(list->tail->value);
		free(list->tail);
		
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

void list_insert(List* list, void* value, int position)
{
	// TODO
}

void list_erase(List* list, int position)
{
	// TODO
}

void list_remove(List* list, Node* node)
{
	if (list && node)
	{
		Node* previous = node->previous;
		Node* next = node->next;
		
		free(node->value);
		free(node);
		
		previous->next = next;
		next->previous = previous;
	}
}

static void list_clear_nodes(Node* node)
{
	if (node)
	{
		Node* next = node->next;
		free(node->value);
		free(node);
		list_clear_nodes(next);
	}
}

void list_clear(List* list)
{
	if (list)
	{
		list_clear_nodes(list->head);
		
		list->size = 0;
		list->head = NULL;
		list->tail = NULL;
	}
}
