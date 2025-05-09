#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "search_queue.h"
#include "types.h"
#include "file_utils.h"

#define LINE_LEN 1024

int main(int argc, char *argv[]) {
    FILE *in = NULL, *out = NULL;
    SearchQueue *q = NULL;
    char line[LINE_LEN];
    void *item = NULL;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    in = fopen(argv[1], "r");
    if (!in) {
        perror("Couldn't open input file");
        return EXIT_FAILURE;
    }

    out = fopen(argv[2], "w");
    if (!out) {
        perror("Couldn't open output file");
        fclose(in);
        return EXIT_FAILURE;
    }

    q = search_queue_new(string_print, string_cmp);
    if (!q) {
        fprintf(stderr, "Couldn't create queue\n");
        fclose(in);
        fclose(out);
        return EXIT_FAILURE;
    }

    while (read_line(in, line) != 0) {
        item = str2str(line);
        if (!item) {
            fprintf(stderr, "Error copying line\n");
            break;
        }

        if (search_queue_push(q, item) == ERROR) {
            fprintf(stderr, "Error adding to queue\n");
            free(item);
            break;
        }
    }

    while (!search_queue_isEmpty(q)) {
        item = search_queue_pop(q);
        if (item) {
            string_print(out, item);
            fprintf(out, "\n");
            free(item);
        }
    }

    search_queue_free(q);
    fclose(in);
    fclose(out);

    return EXIT_SUCCESS;
}
