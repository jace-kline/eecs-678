/** @file libpriqueue.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libpriqueue.h"

void node_init(node **n, void *ptr) {
  if(ptr != NULL) {
    *n = (node*)calloc(1,sizeof(node));
    void *tmp_ptr = calloc(1,sizeof(*ptr));
    memcpy(tmp_ptr, ptr, sizeof(*ptr));
    (*n)->item_ptr = tmp_ptr;
    (*n)->next = NULL;
  } else {
    perror("NULL pointer passed to node constructor");
  }
}

void node_destroy_solo(node* n) {
  if(n != NULL) {
    if(n->item_ptr != NULL) free(n->item_ptr);
    n->next = NULL;
    free(n);
    n = NULL;
  }
}

void node_destroy_recursive(node* n) {
  if(n != NULL) {
    node* next = n->next;
    node_destroy_solo(n);
    node_destroy_recursive(next);
  }
}
/**
  Initializes the priqueue_t data structure.
  
  Assumtions
    - You may assume this function will only be called once per instance of priqueue_t
    - You may assume this function will be the first function called using an instance of priqueue_t.
  @param q a pointer to an instance of the priqueue_t data structure
  @param comparer a function pointer that compares two elements.
  See also @ref comparer-page
 */
void priqueue_init(priqueue_t *q, int(*comparer)(const void *, const void *))
{
  q->front = NULL;
  q->compare = comparer;
}


/**
  Insert the specified element into this priority queue.

  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr a pointer to the data to be inserted into the priority queue
  @return The zero-based index where ptr is stored in the priority queue, where 0 indicates that ptr was stored at the front of the priority queue.
 */
int priqueue_offer(priqueue_t *q, void *ptr)
{
  if(q != NULL && ptr != NULL) {
    node *n = q->front;
    node *new_n;
    node_init(&new_n, ptr);

    if(n == NULL) {
      q->front = new_n;
      return 0;
    }

    for(int i = 1; ; i++) { 
      if(n->next == NULL) {
        n->next = new_n;
        return i;
      } 
      else if(q->compare(ptr, n->next->item_ptr) < 0) {
        new_n->next = n->next;
        n->next = new_n;
        return i;
      }
      n = n->next;
    }
  }
	return -1;
}


/**
  Retrieves, but does not remove, the head of this queue, returning NULL if
  this queue is empty.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @return pointer to element at the head of the queue
  @return NULL if the queue is empty
 */
void *priqueue_peek(priqueue_t *q)
{
	if(q != NULL && q->front != NULL) {
    void *tmp_ptr = malloc(sizeof(*q->front->item_ptr));
    memcpy(tmp_ptr, q->front->item_ptr, sizeof(*q->front->item_ptr));
    return tmp_ptr;
  }
  return NULL;
}


/**
  Retrieves and removes the head of this queue, or NULL if this queue
  is empty.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @return the head of this queue
  @return NULL if this queue is empty
 */
void *priqueue_poll(priqueue_t *q)
{
  if(q != NULL && q->front != NULL) {
      node *tmp = q->front;
      q->front = q->front->next;
      void *ret_ptr = tmp->item_ptr;
      tmp->item_ptr = NULL;
      node_destroy_solo(tmp);
      return ret_ptr;
  }
	return NULL;
}


/**
  Returns the element at the specified position in this list, or NULL if
  the queue does not contain an index'th element.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @param index position of retrieved element
  @return the index'th element in the queue
  @return NULL if the queue does not contain the index'th element
 */
void *priqueue_at(priqueue_t *q, int index)
{
  if(q != NULL && q->front != NULL) {
    node *n = q->front;
    for(int i = 0; ; i++) {
      if(n == NULL) break;
      else if(i == index) return n->item_ptr;
      n = n->next;
    }
  }
	return NULL;
}


/**
  Removes all instances of ptr from the queue. 
  
  This function should not use the comparer function, but check if the data contained in each element of the queue is equal (==) to ptr.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr address of element to be removed
  @return the number of entries removed
 */
int priqueue_remove(priqueue_t *q, void *ptr)
{
  if(q != NULL && q->front != NULL && ptr != NULL) {
    int count = 0;
    node *n = q->front;
    node *n_prev = n;
    while(1) {
      if(n == NULL) break;
      else if(memcmp(n->item_ptr, ptr, sizeof(*ptr)) == 0) {
        n_prev->next = n->next;
        node_destroy_solo(n);
        n = n_prev;
        count++;
      }
      n_prev = n;
      n = n->next;
    }
    return count;
  }
	return 0;
}


/**
  Removes the specified index from the queue, moving later elements up
  a spot in the queue to fill the gap.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @param index position of element to be removed
  @return the element removed from the queue
  @return NULL if the specified index does not exist
 */
void *priqueue_remove_at(priqueue_t *q, int index)
{
  if(q != NULL && q->front != NULL) {
    node *n = q->front;
    node *n_prev = n;
    for(int i = 0; ; i++) {
      if(n == NULL) return NULL;
      else if(i == index) {
        node *n_next = n->next;
        n_prev->next = n_next;
        void *ret_ptr = n->item_ptr;
        n->item_ptr = NULL;
        node_destroy_solo(n);
        if(i == 0) q->front = n_next;
        return ret_ptr;
      }
      n_prev = n;
      n = n->next;
    }
  }
	return NULL;
}


/**
  Return the number of elements in the queue.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @return the number of elements in the queue
 */
int priqueue_size(priqueue_t *q)
{
  if(q != NULL && q->front != NULL) {
    node *n = q->front;
    for(int i = 0; ; i++) {
      if(n == NULL) return i;
      n = n->next;
    }
  }
	return 0;
}


/**
  Destroys and frees all the memory associated with q.
  
  @param q a pointer to an instance of the priqueue_t data structure
 */
void priqueue_destroy(priqueue_t *q)
{
  if(q != NULL && q->front != NULL) node_destroy_recursive(q->front);
}
