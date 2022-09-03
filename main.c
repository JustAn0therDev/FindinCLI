#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "main_functions.h"
#include "file_search_utils.h"
#include "file_information_node.h"
#include "file_information.h"
#include "result.h"

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
    struct result *result = get_file_information_linked_list("D:\\repos\\PokemonAdventureGame\\PokemonAdventureGame\\Pokemon", extension, search);

    if (result->list == NULL) {
        printf("No matches found for '%s'\n", search);
        free(search);
        return EXIT_SUCCESS;
    }

    while (result->list->next != NULL) {
        print_formatted_file_match(result->list->file_information);
        result->list = result->list->next;
    }

    printf("\n%i occurrences found in %i files\n", result->total_occurrences, result->total_files);

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
    result->total_files = 0;
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
            int same_file_flag = 0;

            if (fdFile.dwFileAttributes ^ FILE_ATTRIBUTE_DIRECTORY) {
                char *file_content = get_file_content(path);

                if (file_content != NULL) {

                    int line_number = 1;
                    char *token = strtok(file_content, "\n");

                    // TODO: implement my own strtok in file_search_utils that reads
                    // empty lines so that the program doesn't skip'em.
                    // TODO 2: implement a trim(const char* input) function that removes any leading or trailing whitespace
                    do {
                        int occurrences = get_number_of_substring_occurrences(token, search);

                        if (occurrences > 0) {
                            if (!same_file_flag) {
                                result->total_files++;
                                same_file_flag = 1;
                            }
                            
                            result->total_occurrences += occurrences;
                            current_node->file_information = (struct file_information*)malloc(sizeof(struct file_information));
                            
                            current_node->file_information->path = (char*)malloc(sizeof(char) * strlen(path));
                            current_node->file_information->line_content = (char*)malloc(sizeof(char) * strlen(token));
                            
                            strcpy(current_node->file_information->path, path);
                            strcpy(current_node->file_information->line_content, token);
                            current_node->file_information->line_number = line_number;
                            current_node->next = (struct file_information_node*)malloc(sizeof(struct file_information_node));;
                            struct file_information_node *temp_node = current_node;
                            current_node = current_node->next;
                            current_node->previous = temp_node;
                        }
                        line_number++;
                    } while (token = strtok(NULL, "\n"));
                }
            }
        }
    } while (FindNextFile(hFind, &fdFile));

    FindClose(hFind);
    
    if (result->list->file_information == NULL) {
        free(result->list);
        result->list = NULL;
    } else {
        current_node->next = NULL;
    }

    return result;
}

void print_formatted_file_match(struct file_information *file_information) {
    if (file_information == NULL) return;
    
    printf("[%s] line %i: %s\n", 
            file_information->path, 
            file_information->line_number,
            file_information->line_content
            );
}
