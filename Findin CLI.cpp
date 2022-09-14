#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>

const std::string DEFAULT_COLORS_TERMINAL_INSTRUCTION = "\033[0m";
const std::string GREEN_COLORS_TERMINAL_INSTRUCTION = "\033[32m";
const std::string RED_COLORS_TERMINAL_INSTRUCTION = "\033[31m";
const std::string YELLOW_COLORS_TERMINAL_INSTRUCTION = "\033[33m";

static bool ignore_case_toggled = false;

const std::string* trim_start(const std::string& input) {
    std::string *output = new std::string;

    bool in_initial_whitespaces = true;

    for (const auto& ch : input) {
        if (in_initial_whitespaces && (ch == '\t' || ch == '\n' || ch == '\r' || ch == ' ')) {
            continue;
        }

        in_initial_whitespaces = false;
        output->push_back(ch);
    }

    return output;
}

const std::string* get_string_modified_by_options(const std::string& trimmed_line) {
    std::string *uppercase_line = new std::string;

    for (const auto& ch : trimmed_line) uppercase_line->push_back(toupper(ch));

    return uppercase_line;
}

const std::string* get_same_string(const std::string& trimmed_line) {
    return &trimmed_line;
}

int main(int argc, char **argv)
{
    size_t total_files_searched = 0;
    size_t total_occurrences = 0;

    if (argc == 1) {
        std::cout << "Unable to search without extension.\n";
        exit(1);
    }

    if (argc == 2) {
        std::cout << "Unable to search without string to look for.\n";
        exit(1);
    }

    const std::string extension = argv[1];
    std::string search = argv[2];

    const std::string*(*get_modified_line_for_comparison)(const std::string&);

    if (argc == 4) {
        std::string option = "-i";
        if (option == "-i") {
            ignore_case_toggled = true;
            for (char& ch : search) ch = toupper(ch);
        }

        get_modified_line_for_comparison = get_string_modified_by_options;
    }
    else {
        get_modified_line_for_comparison = get_same_string;
    }

    const std::filesystem::path current_path = std::filesystem::current_path();

    for (const auto& entry : std::filesystem::recursive_directory_iterator(current_path)) {
        if (entry.path().extension() != extension) {
            continue;
        }

        std::ifstream current_file(entry.path());
        
        std::string line;

        size_t line_count = 0;

        while (current_file) {
            std::getline(current_file, line);

            const std::string* trimmed_line = trim_start(line);

            const std::string* line_modified_for_comparison = get_modified_line_for_comparison(*trimmed_line);

            size_t index = 0;

            bool first_occurrence_in_line = true;

            if (line_modified_for_comparison->find(search, index) != std::string::npos) {
                std::cout << "[" << entry.path() << "] on line " << line_count << ": ";
                size_t inner_index = 0;

                while (inner_index <= line_modified_for_comparison->length()) {
                    if ((index = line_modified_for_comparison->find(search, index)) != std::string::npos) {
                        if (inner_index == index) {
                            const std::string& highlight_color = first_occurrence_in_line ? GREEN_COLORS_TERMINAL_INSTRUCTION : YELLOW_COLORS_TERMINAL_INSTRUCTION;
                            std::cout << highlight_color;
                        }
                        else if ((inner_index - index) == search.length()) {
                            std::cout << DEFAULT_COLORS_TERMINAL_INSTRUCTION;
                            total_occurrences++;
                            index += search.length();
                            first_occurrence_in_line = false;
                        }
                    }

                    if (inner_index < line_modified_for_comparison->length()) {
                        std::cout << (*trimmed_line)[inner_index];
                    }

                    inner_index++;
                }
                
                std::cout << "\n\n";
            }

            std::cout << DEFAULT_COLORS_TERMINAL_INSTRUCTION;

            line_count++;

            delete line_modified_for_comparison;
            delete trimmed_line;
        }

        total_files_searched++;
    }

    if (total_occurrences == 0) {
        std::cout << RED_COLORS_TERMINAL_INSTRUCTION << total_occurrences << " occurrences found in " << total_files_searched << " files.\n";
    }
    else {
        std::cout << total_occurrences << " occurrences found in " << total_files_searched << " files.\n";
    }

    std::cout << DEFAULT_COLORS_TERMINAL_INSTRUCTION;
}