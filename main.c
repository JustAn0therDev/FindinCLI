#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "main_functions.h"
#include "file_information.h"
#include "file_information_node.h"

// TODO: A string should not have a max size; instead,
// any variable memory allocation should use realloc.
const int MAX_STRING_SIZE = 2048;

int main(void) {
    struct file_information_node *list = get_file_information_linked_list("D:/repos/Findin CLI", ".txt");

    int idx = 0;

    while (list->next != NULL) {
        printf("Navigating! Item: %i\n", idx);
        idx++;
        
        print_formatted_file_content(list->value->path, list->value->file_content);
        list = list->next;
    }

    free_file_information_list_from_top_to_bottom(list);
    
    return EXIT_SUCCESS;
}

// Gets the file's content by its path.
// The content is malloc'ed so it needs to be freed.
// If the function does not manage to either find the file
// or read its content, it'll return NULL.
char* get_file_content(char* filepath) {
    FILE* file = fopen(filepath, "r");

    if (!file) {
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    size_t length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(length);

    if (!buffer) {
        return NULL;
    }

    length = fread(buffer, 1, length, file);

    buffer[length] = '\0';

    fclose(file);

    return buffer;
}

// TODO: Update this documentation containing stuff like "with X extension and so on"
// This method returns a doubly linked list containing all paths and
// file content of each ".txt" file.
struct file_information_node *get_file_information_linked_list(char* directory, char *file_extension) {
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    struct file_information_node *first_node = (struct file_information_node*)malloc(sizeof(struct file_information_node));
    struct file_information_node *current_node = NULL;

    first_node->previous = NULL;
    current_node = first_node;

    char path[2048];

    sprintf(path, "%s\\*.txt", directory, file_extension);

    if ((hFind = FindFirstFile(path, &fdFile)) == INVALID_HANDLE_VALUE) {
        printf("Path not found [%s]\n", directory);
        return NULL;
    }

    do {
        if (strcmp(fdFile.cFileName, ".") != 0 && strcmp(fdFile.cFileName, "..") != 0) {
            sprintf(path, "%s\\%s", directory, fdFile.cFileName);

            if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                printf("Directory:  %s\n", path);
            } else {
                char *file_content = get_file_content(path);

                if (file_content != NULL) {
                    current_node->value = (struct file_information*)malloc(sizeof(struct file_information));
                    
                    current_node->value->path = (char*)malloc(sizeof(char) * MAX_STRING_SIZE);
                    current_node->value->file_content = (char*)malloc(sizeof(char) * MAX_STRING_SIZE);

                    strcpy(current_node->value->path, path);
                    strcpy(current_node->value->file_content, file_content);
                    
                    current_node->next = (struct file_information_node*)malloc(sizeof(struct file_information_node));;
                    struct file_information_node *temp_node = current_node;
                    current_node = current_node->next;
                    current_node->previous = temp_node;
                }
            }
        }
    } while (FindNextFile(hFind, &fdFile));

    FindClose(hFind);

    current_node->next = NULL;

    return first_node;
}

void print_formatted_file_content(const char *path, const char *content) {
    char buffer[4000];
    sprintf(buffer, "[%s]: \n%s", path, content);
    printf("%s\n", buffer);
}
