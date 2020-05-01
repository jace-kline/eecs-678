/** @file libpriqueue.h
 */

#ifndef LIBPRIQUEUE_H_
#define LIBPRIQUEUE_H_


/**
  Priqueue Data Structure
*/
typedef struct _node {
  void *item_ptr;
  struct _node *next;
} node;

typedef struct _priqueue_t
{
  node *front;
  int(*compare)(const void *, const void *);
} priqueue_t;

// Node functions
void node_init(node **n, void *ptr);
void node_destroy_solo(node *n);
void node_destroy_recursive(node* n);


// Priority Queue functions
void   priqueue_init     (priqueue_t *q, int (*comparer)(const void *, const void *));

int    priqueue_offer    (priqueue_t *q, void *ptr);
void * priqueue_peek     (priqueue_t *q);
void * priqueue_poll     (priqueue_t *q);
void * priqueue_at       (priqueue_t *q, int index);
int    priqueue_remove   (priqueue_t *q, void *ptr);
void * priqueue_remove_at(priqueue_t *q, int index);
int    priqueue_size     (priqueue_t *q);

void   priqueue_destroy  (priqueue_t *q);

#endif /* LIBPQUEUE_H_ */
