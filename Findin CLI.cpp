#include <iostream>
#include <filesystem>
#include <fstream>
#include <stdlib.h>
#include <string>

const std::string DEFAULT_COLORS_TERMINAL_INSTRUCTION = "\033[0m";
const std::string GREEN_COLORS_TERMINAL_INSTRUCTION = "\033[32m";
const std::string RED_COLORS_TERMINAL_INSTRUCTION = "\033[31m";
const std::string YELLOW_COLORS_TERMINAL_INSTRUCTION = "\033[33m";

std::string trimstart(std::string input) {
    std::string output;

    bool in_initial_whitespaces = true;

    for (const auto& ch : input) {
        if (in_initial_whitespaces && (ch == '\t' || ch == '\n' || ch == '\r' || ch == ' ')) {
            continue;
        }
        else {
            in_initial_whitespaces = false;
            output.push_back(ch);
        }
    }

    return output;
}

void write_occurrence_to_stdout(std::string& occurrence, std::string& substr, size_t occ_index) {
    size_t idx = 0;
    for (const auto& ch : occurrence) {
        if (idx == occ_index) {
            std::cout << GREEN_COLORS_TERMINAL_INSTRUCTION;
        }
        else if ((idx - occ_index) == substr.length()) {
            std::cout << DEFAULT_COLORS_TERMINAL_INSTRUCTION;
        }

        std::cout << ch;
        idx++;
    }
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

    std::string extension = argv[1];
    std::string search = argv[2];

    std::filesystem::path current_path = std::filesystem::current_path();

    for (const auto& entry : std::filesystem::recursive_directory_iterator(current_path)) {
        if (entry.path().extension() != extension) {
            continue;
        }

        std::ifstream myfile(entry.path());
        
        std::string line;

        size_t line_count = 0;

        while (myfile) {
            std::getline(myfile, line);

            std::string trimmed_line = trimstart(line);

            size_t index = 0;

            bool first_occurrence_in_line = true;

            if (trimmed_line.find(search, index) != std::string::npos) {
                std::cout << "[" << entry.path() << "] on line " << line_count << ": ";
                size_t outer_index = 0;

                while (outer_index <= trimmed_line.length()) {
                    if ((index = trimmed_line.find(search, index)) != std::string::npos) {
                        if (outer_index == index) {
                            const std::string& highlight_color = first_occurrence_in_line ? GREEN_COLORS_TERMINAL_INSTRUCTION : YELLOW_COLORS_TERMINAL_INSTRUCTION;
                            std::cout << highlight_color;
                        }
                        else if ((outer_index - index) == search.length()) {
                            std::cout << DEFAULT_COLORS_TERMINAL_INSTRUCTION;
                            total_occurrences++;
                            index += search.length();
                            first_occurrence_in_line = false;
                        }
                    }

                    if (outer_index < trimmed_line.length()) {
                        std::cout << trimmed_line[outer_index];
                    }

                    outer_index++;
                }
                
                std::cout << "\n";
            }

            std::cout << DEFAULT_COLORS_TERMINAL_INSTRUCTION;

            line_count++;
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