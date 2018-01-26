# Actigo90 Pricer

## Dependencies

- C++11
- cmake >= 3.6.3
- gcc >= 7.2

### PNL

This project requires [PNL](https://github.com/pnlnum/pnl)
You have to clone and build this library.

Then set the `PNL_DIR` in the root `CMakeLists.txt`
This directory is the PNL build directory containing the lib file.

## Build

```
mkdir build
cd build
cmake ..
make
```

## Run

In the build directory, run the pricer with
```
./test/pricerActigo [samples number]
```
The computation might take a while. Please run with small value at first to test the performance (e.g. 100)

## Unit tests

Run the unit tests with
- `make runQuoteTests` for quote library test
- `make runPricerTests` for pricer test

## Documentation

Build the documentation using Doxygen with
```
make doc
```
