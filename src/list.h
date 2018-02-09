#ifndef LIST_H
#define LIST_H

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

bool empty(const List* list);
size_t size(const List* list);
Node* front(List* list);
Node* back(List* list);
void push_front(List* list, void* value);
void push_back(List* list, void* value);
void pop_front(List* list);
void pop_back(List* list);
void insert(List* list, void* value, int position);
void erase(List* list, int position);
void remove(List* list, void* value);
void clear(List* list);

#endif
