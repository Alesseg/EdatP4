#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "search_queue.h"
#include "types.h"
#include "file_utils.h"

int main(int argc, char *argv[]) {
  FILE *fin = NULL, *fout = NULL;
  SearchQueue *sq = NULL;
  void *str = NULL;

  if (argc != 3) {
    fprintf(stderr, "Uso: %s <input_file> <output_file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  fin = fopen(argv[1], "r");
  if (!fin) {
    perror("Error al abrir el fichero de entrada");
    return EXIT_FAILURE;
  }

  fout = fopen(argv[2], "w");
  if (!fout) {
    perror("Error al abrir el fichero de salida");
    fclose(fin);
    return EXIT_FAILURE;
  }

  // Crear la cola de búsqueda con funciones para strings
  sq = search_queue_new(string_print, string_cmp);
  if (!sq) {
    fprintf(stderr, "Error al crear la SearchQueue\n");
    fclose(fin);
    fclose(fout);
    return EXIT_FAILURE;
  }

  // Leer línea a línea e insertar en la cola
  while ((str = string_init()) != NULL) {
    if (string_read(fin, str) == ERROR) {
      string_free(str);
      break;
    }

    if (search_queue_push(sq, str) == ERROR) {
      fprintf(stderr, "Error al insertar en la cola\n");
      string_free(str);
      break;
      }
    }

    // Extraer en orden e imprimir
  while (!search_queue_isEmpty(sq)) {
    str = search_queue_pop(sq);
    if (str) {
      string_print(fout, str);
      fprintf(fout, "\n");
      string_free(str); // Libera cada string después de usar
      }
    }

  // Liberar recursos
  search_queue_free(sq);
  fclose(fin);
  fclose(fout);

  return EXIT_SUCCESS;
}
