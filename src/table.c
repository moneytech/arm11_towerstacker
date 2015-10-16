#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"
#include <assert.h>

//this table ADT is a list

struct table_elem *table_alloc_elem(void){
  struct table_elem *elem = malloc(sizeof(struct table_elem));
  if(elem == NULL){
    perror("table_alloc_elem");
    exit(EXIT_FAILURE);
  }
  return elem;
}

void table_elem_free(struct table_elem *elem){
  free(elem);
}

void table_init(struct table *tPtr){
  tPtr -> header = table_alloc_elem();
  tPtr -> footer = table_alloc_elem();
  tPtr -> header -> value = NULL;
  tPtr -> footer -> value = NULL;
  tPtr -> header -> next = tPtr -> footer;
  tPtr -> header -> prev = NULL;
  tPtr -> footer -> next = NULL;
  tPtr -> footer -> prev = tPtr -> header;

}

// --------------------------------- iterator -----------------------------//
//typedef struct table_elem* table_iter;

table_iter table_begin(struct table *tPtr){
  return tPtr->header->next;
} 

table_iter table_end(struct table *tPtr){
  return tPtr->footer;
}

table_iter table_iter_next(table_iter iter){
  assert(table_is_internal(iter));
  return iter -> next; 
}

int table_is_internal(table_iter iter){
  return iter -> next != NULL && iter -> prev != NULL;
}
//-------------------------------------------------------------------------//

void table_insert(struct table *tPtr, table_iter iter, char *value, int addr){
  struct table_elem *newElem = table_alloc_elem();
  newElem -> next = iter;
  newElem -> prev = iter -> prev;
  newElem -> value = value;
  newElem -> addr = addr;

  iter -> prev -> next = newElem; 
  iter -> prev = newElem;
}

void table_insert_front(struct table *tPtr, char *value, int addr){
  table_insert(tPtr, table_begin(tPtr), value, addr);  
}

void table_destroy(struct table *tPtr){
  struct table_elem *elem = tPtr -> header;
  while(elem != NULL){
    struct table_elem *next = elem -> next;
    table_elem_free(elem);
    elem = next;
  }
}

int table_get_addr(struct table *tPtr, char *value){
  table_iter current = table_begin(tPtr);
//	printf("current label: %s\n", current->value);
//	printf(" value: %s\n", value);
  while(strcmp(current -> value, value) != 0){
    current = table_iter_next(current);
    if(current == tPtr -> footer){
      perror("table_get_address: getting address of a non existing value");
      exit(EXIT_FAILURE);
    }
  } 
  return current -> addr;
}

