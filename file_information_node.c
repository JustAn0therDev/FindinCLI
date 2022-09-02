#include <stdlib.h>
#include <stdio.h>

#ifndef FILE_INFORMATION_NODE_H

#include "file_information_node.h"

#endif

#include "file_information.h"

void free_file_information_node(struct file_information_node *list) {
    if (list->value != NULL) {
        free(list->value->path);
        free(list->value->file_content);
        free(list->value);
        free(list);
    }
}

void free_file_information_list_from_top_to_bottom(struct file_information_node *list) {
    while (list->previous != NULL) {
        struct file_information_node *tmp = list->previous;
        free_file_information_node(list);
        list = tmp;
    }

    free_file_information_node(list);
    free(list);
}
