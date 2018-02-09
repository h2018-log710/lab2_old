#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct Node
{
	void* value;
    struct Node* previous;
    struct Node* next;
} Node;

typedef struct List
{
	size_t size;
	struct Node* head;
	struct Node* tail;
} List;

bool list_empty(const List* list);
size_t list_size(const List* list);
Node* list_front(List* list);
Node* list_back(List* list);
void list_push_front(List* list, void* value);
void list_push_back(List* list, void* value);
void list_pop_front(List* list);
void list_pop_back(List* list);
void list_insert(List* list, void* value, int position);
void list_erase(List* list, int position);
void list_remove(List* list, Node* node);
void list_clear(List* list);

#endif
