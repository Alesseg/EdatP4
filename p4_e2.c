#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "search_queue.h"
#include "types.h"
#include "file_utils.h"

#define LINE_LEN 1024

void search_queue_free_all(SearchQueue *sq, void free_elem(void *elem)) {
  if(sq) {
    while(search_queue_isEmpty(sq) == FALSE) {
      free_elem(search_queue_pop(sq));
    }
    search_queue_free(sq);
  }
}

int main(int argc, char *argv[])
{
  FILE *in = NULL, *out = NULL;
  SearchQueue *q = NULL;
  char line[LINE_LEN];
  char *item = NULL;

  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  in = fopen(argv[1], "r");
  if (!in)
  {
    perror("Couldn't open input file");
    exit(EXIT_FAILURE);
  }

  out = fopen(argv[2], "w");
  if (!out)
  {
    perror("Couldn't open output file");
    fclose(in);
    exit(EXIT_FAILURE);
  }

  q = search_queue_new(string_print, string_cmp);
  if (!q)
  {
    fprintf(stderr, "Couldn't create queue\n");
    fclose(in);
    fclose(out);
    exit(EXIT_FAILURE);
  }

  while (read_line(in, line) != 0)
  {
    item = str2str(line);
    if (!item)
    {
      fprintf(stderr, "Error copying line\n");
      break;
    }

    if(search_queue_contains(q, item) == FALSE) {
      if (search_queue_push(q, item) == ERROR)
      {
        fprintf(stderr, "Error adding to queue\n");
        search_queue_free_all(q, string_free);
        string_free(item);
        fclose(in);
        fclose(out);
        exit(EXIT_FAILURE);
      }
    } else {
      string_free(item);
    }

  }
  fclose(in);

  search_queue_print(out, q);

  search_queue_free_all(q, string_free);
  fclose(out);

  exit(EXIT_SUCCESS);
}
