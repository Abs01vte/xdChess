#include "list.h"
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

struct node* makeNode(void){
  struct node* newNode = malloc(sizeof(struct node));
  if(newNode == NULL){
    return NULL;
  }
  *newNode = (struct node){.data = NULL, .next = NULL, .prev = NULL};
  return newNode;
}
struct linkedList* makeList(size_t dataLen){
  struct linkedList* list = malloc(sizeof(struct linkedList));
  if(list == NULL){
    return NULL;
  }
  *list = (struct linkedList){.size = 0, .dataLen = dataLen, .first = NULL, .last = NULL};
  return list;
}
void addNode(struct linkedList* list, struct node* node){
  list->size++;
  struct node* tempNode = list->last;
  if(list->first == NULL && list->last == NULL){
    list->first = node;
    list->last = node;
  }
  else{
    list->last = node;
    node->prev = tempNode;
    tempNode->next = node;
  }
}
void destroyList(struct linkedList* list){
  struct node* curNode = list->first;
  for(int i = 0; i < list->size; i++){
    struct node* tempNode = curNode->next;
    free(curNode->data);
    free(curNode);
    curNode = tempNode;
  }
  free(list);
}
void addList(const void* item, struct linkedList* list){
  if(item == NULL){
    return;
  }
  if(list == NULL){
    return;
  }
  struct node* node = makeNode();
  if(node == NULL){
    return;
  }
  void* newItem = malloc(list->dataLen);
  if(newItem == NULL){
    return;
  }
  memcpy(newItem, item, list->dataLen);
  node->data = newItem;
  addNode(list, node);
}
void printList(struct linkedList* list){
  struct node* curNode = list->first;
  for(int i = 0; i < list->size; i++){
    printf("%c ", *((char*)curNode->data));
    curNode = curNode->next;
  }
}
