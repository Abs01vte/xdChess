#ifndef LIST_H
#define LIST_H
#include <stdint.h>
#include <stddef.h>
struct node {
  void* data;
  struct node* next;
  struct node* prev;
};
struct linkedList {
  size_t size;
  size_t dataLen;
  struct node* first;
  struct node* last;
};
struct linkedList* makeList(size_t dataLen);
void destroyList(struct linkedList* list);
void *addList(const void* item, struct linkedList* list);
void printList(struct linkedList* list);

#endif
