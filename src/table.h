#ifndef TABLE_H
#define TABLE_H

struct table_elem{
  char *value; // tha string Lable
  int addr; // address of that lable in the instruction
  struct table_elem *prev;
  struct table_elem *next; 
};

struct table{
  struct table_elem *header;
  struct table_elem *footer; 
};

typedef struct table_elem* table_iter;

void table_elem_free(struct table_elem *elem);
void table_init(struct table *tPtr);
void table_insert(struct table *tPtr, table_iter iter, char *value, int addr);
table_iter table_begin(struct table *tPtr);
table_iter table_end(struct table *tPtr);
table_iter table_iter_next(table_iter iter);
int table_is_internal(table_iter iter);
void table_insert_front(struct table *tPtr, char *value, int addr);
void table_destroy(struct table *tPtr);
int table_get_addr(struct table *tPtr, char *value);
#endif
