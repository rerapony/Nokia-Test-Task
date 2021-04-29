# Nokia Test Task: CSV Reader

Reads, evaluates and prints csv file to console.

#### Build:
```sh
mkdir build && cd build && cmake ..
cmake --build .
```
#### Test run:
```sh
cd build
cp -r ../test/data .
./nokia_csv_test
```

#### Example usage:
```sh
cd build
./nokia_csv_reader ../table.csv
```