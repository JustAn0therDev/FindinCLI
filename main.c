#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "main_functions.h"
#include "file_search_utils.h"
#include "file_information_node.h"
#include "file_information.h"
#include "result.h"

// TODO: A string should not have a max size; instead,
// any variable memory allocation should use realloc.

int main(int argc, char *argv[]) {
    // The extension must have a search pattern, with the following format:
    // ".txt", ".c", ".cs" etc.
    char *extension = NULL;
    // The search can have any size and contain any type of character (TODO: change most char readings for wchar_t).
    char *search = NULL;

    if (argc == 1) {
        printf("Cannot invoke program without an extension argument, e.g. '.txt'.\n");
        return EXIT_FAILURE;
    }

    if (argc == 2) {
        printf("Cannot invoke program without search argument.\n");
        return EXIT_FAILURE;
    }

    extension = (char*)malloc(sizeof(char) * (strlen(argv[1]) + 1));
    strcpy(extension, "*");
    strcat(extension, argv[1]);

    search = (char*)malloc(sizeof(char) * strlen(argv[2]));
    strcpy(search, argv[2]);

    // TODO: get current directory
    struct result *result = get_file_information_linked_list("D:\\repos\\PokemonAdventureGame\\PokemonAdventureGame\\Trainers", extension, search);

    int idx = 0;

    if (result == NULL) {
        printf("No matches found for '%s'\n", search);
        free(search);
        return EXIT_SUCCESS;
    }

    while (result->list->next != NULL) {
        print_formatted_file_match(result->list->value);
        result->list = result->list->next;
        idx++;
    }

    printf("%i occurrences found in %i files\n", result->total_occurrences, idx);

    free_file_information_list_from_top_to_bottom(result->list);

    free(extension);
    free(search);
    free(result);
    
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

    char *buffer = (char*)calloc(sizeof(char), length);

    if (!buffer) {
        return NULL;
    }

    length = fread(buffer, 1, length, file);

    buffer[length] = '\0';

    fclose(file);

    return buffer;
}

// This method returns a result struct containing the total occurrences found and a
// doubly linked list containing all paths and file content of each file.
// It accepts the following arguments: 
// Directory to look for (char*), the file extension to look for (char*) and the search (char*)
struct result *get_file_information_linked_list(char* directory, char *extension, char* search) {
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    struct result *result = (struct result*)malloc(sizeof(struct result));
    result->total_occurrences = 0;
    result->list = (struct file_information_node*)malloc(sizeof(struct file_information_node));
    
    struct file_information_node *current_node = NULL;

    result->list->previous = NULL;
    current_node = result->list;

    char path[2048];

    sprintf(path, "%s\\%s", directory, extension);

    if ((hFind = FindFirstFile(path, &fdFile)) == INVALID_HANDLE_VALUE) {
        printf("Path not found [%s]\n", directory);
        return NULL;
    }

    do {
        if (strcmp(fdFile.cFileName, ".") != 0 && strcmp(fdFile.cFileName, "..") != 0) {
            sprintf(path, "%s\\%s", directory, fdFile.cFileName);

            if (fdFile.dwFileAttributes ^ FILE_ATTRIBUTE_DIRECTORY) {
                char *file_content = get_file_content(path);

                if (file_content != NULL) {
                    // TODO: this should return more information or the search should be made 
                    // for each line (strtok?)
                    int occurrences = get_number_of_substring_occurrences(file_content, search);

                    if (occurrences > 0) {
                        result->total_occurrences += occurrences;
                        current_node->value = (struct file_information*)malloc(sizeof(struct file_information));
                        
                        current_node->value->path = (char*)malloc(sizeof(char) * strlen(path));
                        current_node->value->file_content = (char*)malloc(sizeof(char) * strlen(file_content));
                        
                        strcpy(current_node->value->path, path);
                        strcpy(current_node->value->file_content, file_content);
                        
                        current_node->next = (struct file_information_node*)malloc(sizeof(struct file_information_node));;
                        struct file_information_node *temp_node = current_node;
                        current_node = current_node->next;
                        current_node->previous = temp_node;
                    }
                }
            }
        }
    } while (FindNextFile(hFind, &fdFile));

    FindClose(hFind);
    
    if (result->list->value == NULL) {
        free(result->list);
        result->list = NULL;
    } else {
        current_node->next = NULL;
    }

    return result;
}

void print_formatted_file_match(struct file_information *file_information) {
    if (file_information == NULL) return;
    
    printf("[%s]\n%s", 
            file_information->path, 
            file_information->file_content
            );
}
