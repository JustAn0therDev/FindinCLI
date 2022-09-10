#include <iostream>
#include <filesystem>
#include <fstream>
#include <stdlib.h>
#include <string>

int main(int argc, char **argv)
{
    // TODO: get the current path in the future
    //std::filesystem::path current_path = std::filesystem::current_path();

    std::filesystem::path current_path = std::filesystem::path("D:\\repos");

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

    for (const auto& entry : std::filesystem::recursive_directory_iterator(current_path)) {
        if (entry.path().extension() != extension) {
            continue;
        }

        std::ifstream myfile(entry.path());
        
        std::string line;

        size_t line_count = 0;

        // TODO: Maybe highlight the substring encountered with
        // a different color?

        while (myfile) {
            std::getline(myfile, line);

            size_t index = 0;

            bool first_occurrence_in_line = true;
            
            while ((index = line.find(search, index)) != std::string::npos) {
                if (first_occurrence_in_line) {
                    std::cout << "[" << entry.path() << "] on line " << line_count << ": " << line << '\n';
                    first_occurrence_in_line = false;
                }

                total_occurrences++;
                index += search.length();
            }

            line_count++;
        }

        total_files_searched++;
    }

    std::cout << total_occurrences << " occurrences found in " << total_files_searched << " files.\n";
}