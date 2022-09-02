#include "result.h"
#include "file_information_node.h"
#ifndef MAIN_FUNCTIONS_H
#define MAIN_FUNCTIONS_H

struct result *get_file_information_linked_list(char* diretory, char* extension, char* content_to_look_for);
char* get_file_content(char* filepath);
void print_formatted_file_match(struct file_information *file_information);

#endif // MAIN_FUNCTIONS_H