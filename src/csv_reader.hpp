#pragma once

#include <vector>
#include <map>

class CSVReader {
    char delimiter;
    std::vector<std::string> colnames;
    std::vector<size_t> rownums;

    std::map<std::string, size_t> colname_to_index;
    std::map<size_t, size_t> rownum_to_index;

    std::vector<std::vector<std::string>> parsed_table;
    std::vector<std::vector<int>> eval_table;
    std::vector<std::vector<bool>> evaluated;

    std::ostream& out_stream;

public:
    explicit CSVReader(std::ostream& stream = std::cout, char d=',');

    void read_csv(const std::string& filename);

    void evaluate_table();

    void print_raw();

    void print_evaluated();

    std::vector<std::vector<int>> get_evaluated();

private:
    void clear();

    template<typename T>
    void print_table(std::vector<std::vector<T>> table);

    template<typename T>
    void print_line(std::vector<T> line);

    void evaluate_cell(size_t i, size_t j);

    void evaluate_expr(const std::string& expr, size_t i, size_t j);

    int evaluate_cell_ref(const std::string& cell_name);
};

