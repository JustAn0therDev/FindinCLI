#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "file_information_node.h"
#include "file_information.h"

void free_file_information_node(struct file_information_node *list) {
    assert(list != NULL);

    if (list->file_information != NULL) {
        free(list->file_information->path);
        free(list->file_information->line_content);
        free(list->file_information);
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
