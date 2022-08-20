#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

// TODO: study arrays in C and why writing to an array of strings could overwrite other indexes.

void list_files_in_directory(char* diretory, char* file_extension);
char* get_file_content(char* filepath);

int MAX_PATH_SIZE = 2048;

int main(void) {
    list_files_in_directory("D:/repos/Findin CLI", ".txt");

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

// This method prints out the paths and their content to STDOUT.
// It takes a directory that cannot be NULL and a file_extension that,
// if NULL, a wildcard search is done.
void list_file_content_in_stdout(char* directory, char *file_extension) {
    // TODO: make this function return an array of structs in heap that contains the
    // file path and its content. Print it out using another function.

    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    char path[MAX_PATH_SIZE];

    sprintf(path, "%s\\*.txt", directory, file_extension);

    if ((hFind = FindFirstFile(path, &fdFile)) == INVALID_HANDLE_VALUE) {
        printf("Path not found [%s]\n", directory);
        return;
    }

    do {
        if (strcmp(fdFile.cFileName, ".") != 0 && strcmp(fdFile.cFileName, "..") != 0) {
            sprintf(path, "%s\\%s", directory, fdFile.cFileName);

            if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                printf("Directory:  %s\n", path);
            } else {
                char *file_content = get_file_content(path);

                if (file_content != NULL) {
                    printf("[%s]: \n%s\n", path, file_content);
                    free(file_content);
                }
            }
        }
    } while (FindNextFile(hFind, &fdFile));

    FindClose(hFind);
}
