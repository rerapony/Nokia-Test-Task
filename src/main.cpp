#include <string>
#include <iostream>
#include "csv_reader.hpp"

bool check_extension(const std::string& filename, const std::string& extension = "csv") {
    size_t i = filename.find_last_of('.');

    if (i == std::string::npos) {
        return false;
    }

    return (filename.substr(i+1) == extension);
}

int main(int argc,  char **argv) {
    if (argc <= 1) {
        std::cerr << "Argument required: csv file" << std::endl;
        return 1;
    } else if (argc > 2) {
        std::cerr << "Too many arguments" << std::endl;
        return 1;
    }

    if (!check_extension(argv[1])) {
        std::cerr << "Wrong file extension: expected .csv" << std::endl;
        return 1;
    }

    try {
        CSVReader reader;
        reader.read_csv(argv[1]);
        reader.evaluate_table();
        reader.print_evaluated();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}