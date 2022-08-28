#include <string.h>

int get_number_of_char_occurrences(char *source, char char_to_look_for) {
	int idx = 0;
	int occurrences = 0;
	char current_char = *(source + idx);

	while (current_char != '\0') {
		if (current_char == char_to_look_for) {
			occurrences++;
		}

		idx++;
		current_char = *(source + idx);
	}

	return occurrences;
}

int get_number_of_substring_occurrences(char *source, char* substring) {
	int occurrences = 0;
	char *str_ptr = source;
	char *current_line = NULL;
	int line = 1;

	while (*str_ptr != '\0') {
		char *from_found_substring = strstr(str_ptr, substring);
		
		if (from_found_substring != NULL) {
			occurrences++;
			str_ptr = from_found_substring;
			str_ptr += 1;
			continue;
		}
		
		str_ptr += 1;
	}

	return occurrences;
}