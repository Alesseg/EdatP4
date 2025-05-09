#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "bstree.h"
#include "types.h"
#include "vertex.h"
#include "search_queue.h"
#include "file_utils.h"

/**
 * @brief Calculate the median of a search queue
 * 
 * @param sq a pointer to the search queue
 * @param size size of the search queue, number of elements
 * @return median of the search queue, or -1 in case of error
 */
float search_queue_median(SearchQueue *sq, int size);

void search_queue_free_all(SearchQueue *sq, void free_elem(void *elem)) {
  if(sq) {
    while(search_queue_isEmpty(sq) == FALSE) {
      free_elem(search_queue_pop(sq));
    }
    search_queue_free(sq);
  }
}

int main(int argc, char const *argv[]) {
  FILE *f = NULL;
  float *grade = NULL, mean = 0, median = 0, elem = 0, *lowest[3];
  int i = 0, max = 0, size = 0;
  SearchQueue *sq = NULL;


  if(argc < 2)
  {
    printf("Too few arguments.\n");
    exit(EXIT_FAILURE);
  }

  /* Opens the file */
  if(!(f = fopen(argv[1], "r"))) {
    printf("Error while opening the file.\n");
    exit(EXIT_FAILURE);
  }

  /* Create a new search queue */
  if(!(sq = search_queue_new(float_print, float_cmp))) {
    printf("Erro while creating a new search queue.\n");
    fclose(f);
    exit(EXIT_FAILURE);
  }

  /* Read the grades from the file and get the size and mean of the search queue*/
  if(fscanf(f, "%d", &size) != 1) {
    printf("Error reading grades.\n");
    search_queue_free_all(sq, float_free);
    fclose(f);
    exit(EXIT_FAILURE);
  }
  for(i = 0; i < size ;i++) {
    if(fscanf(f, "%f", &elem) != 1) {
    printf("Error reading grades.\n");
    search_queue_free_all(sq, float_free);
    fclose(f);
    exit(EXIT_FAILURE);
  }
    grade = float_init(elem);
    if(!grade) {
      printf("Error reading grades.\n");
      search_queue_free_all(sq, float_free);
      fclose(f);
      exit(EXIT_FAILURE);
    }
    if(search_queue_contains(sq, grade) == FALSE) {
      if (search_queue_push(sq, grade) == ERROR)
      {
        printf("Error reading grades.\n");
        search_queue_free_all(sq, float_free);
        fclose(f);
        exit(EXIT_FAILURE);
      }
    } else {
      float_free(grade);
    }
    mean += elem;
  }
  mean /= size;
  fclose(f);

  /* Print grades in order */
  fprintf(stdout, "Ordered grades: ");
  if(search_queue_print(stdout, sq) < 0) {
    printf("Error printing ordered grades.\n");
    search_queue_free_all(sq, float_free);
    exit(EXIT_FAILURE);
  }

  /* Print the mean */
  fprintf(stdout, "\nMean: %.2f\n", mean);
  /* Print the median */
  fprintf(stdout, "\nMedian: ");
  median = search_queue_median(sq, size);
  if(median < 0) {
    printf("Error calculating median.\n");
    search_queue_free_all(sq, float_free);
    exit(EXIT_FAILURE);
  }
  fprintf(stdout, "%.2f\n", median);

  /* Print the 3 lowest and highest grades */
  max = 3;
  if(max > size)
    max = size;
  fprintf(stdout, "\n");
  fprintf(stdout, "Lowest grades: ");
  for(i = 0; i < max ;i++) {
    if(!(grade = search_queue_pop(sq))) {
      printf("\nError extracting grade form the search queue.\n");
      search_queue_free_all(sq, float_free);
      exit(EXIT_FAILURE);
    }
    lowest[i] = grade;
    fprintf(stdout, "%.2f ", *lowest[i]);
  }
  fprintf(stdout, "\n");
  /* Insert the lowest grades */
  for(i = 0; i < max ; i++) {
    if(search_queue_push(sq, lowest[i]) == ERROR) {
      printf("\nError pushing grade to the search queue.\n");
      search_queue_free_all(sq, float_free);
      exit(EXIT_FAILURE);
    }
  }

  fprintf(stdout, "\n");
  fprintf(stdout,"Highest grades:");
  for(i = 0; i < max ;i++) {
    if(!(grade = (float *)search_queue_popBack(sq))) {
      printf("\nError extracting grade form the search queue.\n");
      search_queue_free_all(sq, float_free);
      exit(EXIT_FAILURE);
    }
    fprintf(stdout, " %.2f", *grade);
    float_free(grade);
  }
  fprintf(stdout, "\n");


  /* Free all the allocated memory */
  search_queue_free_all(sq, float_free);

  exit(EXIT_SUCCESS);
}


float search_queue_median(SearchQueue *sq, int size) {
  SearchQueue *aux_sq = NULL;
  void *elem = NULL, *imp = NULL;
  int i = 0;
  float median = -1;

  /* Create and auxiliar search queue */
  if(!(aux_sq = search_queue_new(float_print, float_cmp))) {
    return median;
  }

  /* Extract half of the elements form the original search queue and push them to the auxiliary one */
  for(i = 0; i < size/2 ;i++) {
    if(!(elem = search_queue_pop(sq))) {
      search_queue_free_all(aux_sq, float_free);
      return median;
    }
    if(search_queue_push(aux_sq, elem) == ERROR) {
      search_queue_free_all(aux_sq, float_free);
      if(elem) float_free(elem);
      return median;
    }
  }
  /* Calculate the median */
  if(size%2)
  {  
    if(!(imp = search_queue_pop(sq))) {
        search_queue_free_all(aux_sq, float_free);
        return median;
    }
    median = (*(float *)elem + *(float *)imp)/2;
  } else {
    median = *(float *)elem;
  }

  /* Empty the auxiliar search queue and restore the original one */
  if(imp) {
    if(search_queue_push(sq, imp) == ERROR) {
      search_queue_free_all(aux_sq, float_free);
      float_free(imp);
      return -1;
    }
  }
  if(search_queue_push(sq, elem) == ERROR) {
    search_queue_free_all(aux_sq, float_free);
    float_free(elem);
    return -1;
  }
  while(search_queue_isEmpty(aux_sq) == FALSE) {
    if(!(elem = search_queue_pop(aux_sq))) {
      search_queue_free_all(aux_sq, float_free);
      return -1;
    }
    if(search_queue_push(sq, elem) == ERROR) {
      search_queue_free_all(aux_sq, float_free);
      float_free(elem);
      return -1;
    }
  }

  /* Free the memory of the auxiliar search queue */
  search_queue_free_all(aux_sq, float_free);

  return median;
}