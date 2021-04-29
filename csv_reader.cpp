#include <regex>
#include <fstream>
#include <sstream>
#include <iostream>
#include "csv_reader.hpp"


bool is_integer(const std::string& s){
    const std::regex base_regex("-?[[:digit:]]+");

    return std::regex_match(s, base_regex);
}

bool is_op(char ch) {
    return (ch == '+' || ch == '-' || ch == '/' || ch == '*');
}

bool is_expression(const std::string& s) {
    return std::any_of(s.begin(), s.end(),
                       [](char ch){ return is_op(ch); });
}


CSVReader::CSVReader(char d): delimiter(d) {}

void CSVReader::read_csv(const std::string& filename) {
    clear();

    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    std::string line, token;

    if (file.good()) {
        std::getline(file, line);
        std::stringstream ss(line);

        // skipping empty cell
        std::getline(ss, token, delimiter);

        size_t index = 0;
        while (std::getline(ss, token, delimiter)) {
            colnames.push_back(token);
            colname_to_index[token] = index;
            index++;
        }
    }

    int rownum;
    size_t index = 0;
    // read data, line by line
    while (std::getline(file, line)) {
        parsed_table.emplace_back();

        std::stringstream ss(line);
        if (ss >> rownum) {
            rownums.push_back(rownum);
            rownum_to_index[rownum] = index;

            if(ss.peek() == ',') {
                ss.ignore();
            } else {
                throw std::invalid_argument("Invalid table format");
            }

            while (std::getline(ss, token, delimiter)) {
                parsed_table[index].push_back(token);
            }

            if (parsed_table[index].size() != colnames.size()) {
                throw std::invalid_argument("Invalid table format");
            }
            index++;
        } else {
            throw std::invalid_argument("Invalid index");
        }
    }

    file.close();
}

void CSVReader::evaluate_table() {
    evaluated = std::vector<std::vector<bool>>(rownums.size(), std::vector<bool>(colnames.size(), false));
    eval_table = std::vector<std::vector<int>>(rownums.size(), std::vector<int>(colnames.size()));

    for (size_t i = 0; i < rownums.size(); ++i) {
        for (size_t j = 0; j < colnames.size(); ++j) {
            if (!evaluated[i][j]) {
                evaluate_cell(i, j);
            }
        }
    }
}

void CSVReader::print_raw() {
    print_table(parsed_table);
}

void CSVReader::print_evaluated() {
    print_table(eval_table);
}


void CSVReader::clear() {
    colnames.clear();
    rownums.clear();

    colname_to_index.clear();
    rownum_to_index.clear();

    parsed_table.clear();
    eval_table.clear();
    evaluated.clear();
}

template<typename T>
void CSVReader::print_table(std::vector<std::vector<T>> table) {
    if (!table.empty()) {
        print_line(colnames);
        for (size_t i = 0; i < rownums.size(); ++i) {
            std::cout << rownums[i];
            print_line(table[i]);
        }
    }
}

template<typename T>
void CSVReader::print_line(std::vector<T> line) {
    for (auto& elem: line) {
        std::cout << delimiter << elem;
    }
    std::cout << std::endl;
}

void CSVReader::evaluate_cell(size_t i, size_t j) {
    std::string raw_cell = parsed_table[i][j];
    if (is_integer(raw_cell)) {
        evaluated[i][j] = true;
        eval_table[i][j] = std::atoi(raw_cell.c_str());
    } else if (is_expression(raw_cell)) {
        evaluate_expr(raw_cell, i, j);
    } else {
        throw std::invalid_argument("Invalid cell value");
    }
}

void CSVReader::evaluate_expr(const std::string& expr, size_t i, size_t j) {
    std::string left, right;
    char op;

    if (expr[0] != '=') {
        throw std::invalid_argument("Invalid cell format");
    }

    for (auto& ch : expr) {
        if (is_op(ch)) {
            op = ch;
            break;
        }
    }

    size_t op_pos = expr.find(op);
    left = expr.substr(1, op_pos-1),
            right = expr.substr(op_pos+1);

    int l_res = is_integer(left) ? std::atoi(left.c_str()) : evaluate_cell_ref(left);
    int r_res = is_integer(right) ? std::atoi(right.c_str()) : evaluate_cell_ref(right);
    int result;

    switch (op) {
        case '+':
            result = l_res + r_res;
            break;
        case '-':
            result = l_res - r_res;
            break;
        case '/':
            if (r_res == 0) {
                throw std::logic_error("Can't divide by 0");
            }
            result = l_res / r_res;
            break;
        case '*':
            result = l_res * r_res;
            break;
        default:
            throw std::invalid_argument("Unsupported operation");
    }

    evaluated[i][j] = true;
    eval_table[i][j] = result;
}

int CSVReader::evaluate_cell_ref(const std::string& cell_name) {
    size_t i = cell_name.size() - 1;

    for (; i >= 0; --i) {
        if (!is_integer(cell_name.substr(i))) {
            break;
        }
    }

    if (i == cell_name.size() - 1) {
        throw std::invalid_argument("Wrong cell name format");
    }

    std::string name = cell_name.substr(0, i + 1);
    size_t index = atoi(cell_name.substr(i + 1).c_str());

    if (colname_to_index.find(name) == colname_to_index.end() ||
        rownum_to_index.find(index) == rownum_to_index.end()) {
        throw std::invalid_argument("Invalid cell name");
    }

    evaluate_cell(rownum_to_index[index], colname_to_index[name]);

    return eval_table[rownum_to_index[index]][colname_to_index[name]];
}

std::vector <std::vector<int>> CSVReader::get_evaluated() {
    return eval_table;
}
