#include <string>
#include <iostream>
#include "csv_reader.hpp"


int main(int argc,  char **argv) {
    if (argc <= 1) {
        std::cerr << "Argument required: csv file" << std::endl;
        return 1;
    } else if (argc > 2) {
        std::cerr << "Too many arguments" << std::endl;
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