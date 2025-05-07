#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "bstree.h"
#include "types.h"
#include "vertex.h"
#include "search_queue.h"
#include "file_utils.h"

struct _SearchQueue {
  BSTree *data;
};

SearchQueue *search_queue_new(P_ele_print print_ele, P_ele_cmp cmp_ele){
  SearchQueue *queue = malloc(sizeof(SearchQueue));
  if (!queue) {
    return NULL;
  }
  queue->data = tree_init(print_ele, cmp_ele);
  if (!queue->data) {
    free(queue);
    return NULL;
  }
  return queue;
}

void search_queue_free(SearchQueue *q) {
  if (!q) {
    return;
  }
  tree_destroy(q->data);
  free(q);
}

Bool search_queue_isEmpty(const SearchQueue *q) {
  if (!q) {
    return TRUE;
  }
  return tree_isEmpty(q->data);
}

Status search_queue_push(SearchQueue *q, void *ele) {
  if (!q || !ele) {
    return ERROR;
  }
  return tree_insert(q->data, ele);
}

void *search_queue_pop(SearchQueue *q) {
  void *min = NULL;
  if (!q || search_queue_isEmpty(q)) {
    return NULL;
  }
  min = tree_find_min(q->data);
  if (!min) {
    return NULL;
  }
  if (tree_remove(q->data, min) == ERROR) {
    return NULL;
  }
  return min;
}

void *search_queue_getFront(const SearchQueue *q) {
  if (!q || search_queue_isEmpty(q)) {
    return NULL;
  }
  return tree_find_min(q->data);
}

void *search_queue_getBack(const SearchQueue *q) {
  if (!q || search_queue_isEmpty(q)) {
    return NULL;
  }
  return tree_find_max(q->data);
}

size_t search_queue_size(const SearchQueue *q) {
  if (!q) {
    return 0;
  }
  return tree_size(q->data);
}

int search_queue_print(FILE *fp, const SearchQueue *q) {
  if (!fp || !q) {
    return -1;
  }
  return tree_inOrder(fp, q->data);
}

void *search_queue_popBack(SearchQueue *q) {
  void *max = NULL;
  if (!q || search_queue_isEmpty(q)) {
    return NULL;
  }
  max = tree_find_max(q->data);
  if (!max) {
    return NULL;
  }
  if (tree_remove(q->data, max) == ERROR) {
    return NULL;
  }
  return max;
}