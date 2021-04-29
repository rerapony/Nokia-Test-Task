#include <gtest/gtest.h>
#include "../src/csv_reader.hpp"

class CSVReaderTest : public ::testing::Test {
public:
    CSVReaderTest(): reader(result) {}

    void read_csv(const std::string& filename) {
        try {
            reader.read_csv(filename);
            reader.evaluate_table();
            reader.print_evaluated();
        } catch (const std::exception& e) {
            exception_message = e.what();
        }
    }

protected:
    void SetUp() override {
        result.clear();
        exception_message = "";
    }

    std::ostringstream result;
    std::string exception_message;

private:
    CSVReader reader;
};

TEST_F(CSVReaderTest, TestCorrectTable) {
    read_csv("data/correct_table.csv");

    ASSERT_EQ(exception_message, "");
    ASSERT_EQ(result.str(), ",A,B,Cell\n"
                            "1,1,0,1\n"
                            "2,2,6,0\n"
                            "30,0,1,5\n");
}

TEST_F(CSVReaderTest, TestInvalidFile) {
    read_csv("data/wrong_table0.csv");

    ASSERT_EQ(exception_message, "Could not open file");
}

TEST_F(CSVReaderTest, TestWrongIndex) {
    read_csv("data/wrong_table1.csv");

    ASSERT_EQ(exception_message, "Invalid cell name");
}

TEST_F(CSVReaderTest, TestWrongCellName) {
    read_csv("data/wrong_table2.csv");

    ASSERT_EQ(exception_message, "Invalid cell name");
}

TEST_F(CSVReaderTest, TestWrongOp) {
    read_csv("data/wrong_table3.csv");

    ASSERT_EQ(exception_message, "Invalid cell value");
}

TEST_F(CSVReaderTest, TestWrongCellFormat) {
    read_csv("data/wrong_table4.csv");

    ASSERT_EQ(exception_message, "Invalid cell format");
}

TEST_F(CSVReaderTest, TestWrongTableFormat) {
    read_csv("data/wrong_table5.csv");

    ASSERT_EQ(exception_message, "Invalid table format");
}

TEST_F(CSVReaderTest, TestWrongCellValue) {
    read_csv("data/wrong_table7.csv");

    ASSERT_EQ(exception_message, "Invalid cell value");
}

TEST_F(CSVReaderTest, TestZeroDivison) {
    read_csv("data/wrong_table8.csv");

    ASSERT_EQ(exception_message, "Can't divide by 0");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}