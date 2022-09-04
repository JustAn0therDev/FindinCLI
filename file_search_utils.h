#ifndef FILE_SEARCH_UTILS_H
#define FILE_SEARCH_UTILS_H

// Gets the number of occurrences of a char in a source string.
int get_number_of_char_occurrences(char *source, char char_to_look_for);

// Gets the number of occurrences of a substring in a source string.
int get_number_of_substring_occurrences(char *source, char* substring);

// Trims the leading whitespace characters of a string, including '\t', '\r' and '\n' characters.
char* trimstart(const char* input);

#endif // FILE_SEARCH_UTILS_H
