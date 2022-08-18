#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

char* get_file_content(char* filepath);
void list_txt_files_in_directory(char* diretory, char* list_of_files[5]);

int main(void) {
    char* list_of_files[5];

    list_of_files[0] = NULL;
    list_of_files[1] = NULL;
    list_of_files[2] = NULL;
    list_of_files[3] = NULL;
    list_of_files[4] = NULL;
    
    list_txt_files_in_directory("D:/repos/Findin CLI", list_of_files);

    for (int i = 0; i < 5; i++) {
        char* file_content_buffer = get_file_content(list_of_files[i]);

        if (!file_content_buffer) continue;

        printf("[%s]: %s\n", list_of_files[i], file_content_buffer);
        
        free(file_content_buffer);
    }

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

void list_txt_files_in_directory(char* directory, char* list_of_files[5]) {
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    char path[2048];

    int idx = 0;

    sprintf(path, "%s\\*.txt", directory);

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
                list_of_files[idx] = path;
                idx++;
            }
        }

    } while (FindNextFile(hFind, &fdFile));

    FindClose(hFind);
}
