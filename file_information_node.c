#include <stdlib.h>
#include <stdio.h>
#include "file_information_node.h"
#include "file_information.h"

void free_file_information_node(struct file_information_node *list) {
    free(list->value->path);
    free(list->value->file_content);
    free(list->value);
}

void free_file_information_list_from_top_to_bottom(struct file_information_node *list) {
    int idx = 0;

    while (list->previous != NULL) {
        struct file_information_node *tmp = list->previous;

        free_file_information_node(list);

        list = tmp;

        idx++;
    }

    free_file_information_node(list);
    free(list);
}
