# Actigo90 Pricer

## Dependencies

### Express & Angular

- npm >= 5.6.1
- nodejs >= 9.x

### Pricer

- C++11
- cmake >= 3.6.3
- gcc >= 7.2

## Quick Start

### Pricer

#### PNL

This project requires [PNL](https://github.com/pnlnum/pnl)
You have to clone and build this library.

Then set the `PNL_DIR` in the root `CMakeLists.txt`
This directory is the PNL build directory containing the lib file.

#### Build

```
mkdir build
cd build
cmake ..
make
```

#### Run (after running Node server)

For this part, you need to retreive prices on Yahoo Finance with the Angular
Client. Please skip this part until Express server and Angular client are
running. The reason is the pricer needs the historical data and the server is
used to collect them.

In the build directory, run the pricer with
```
./test/portfolioValue [date in epoch] [rebalancing frequency] [MC samples number]
```
The computation might take a while.

Example: Pricing everyday until 2015-12-31 (= 1451520000 in epoch)
```
./test/portfolioValue 1451520000 1 5000
```
Please run with small value at first to test the performance (e.g. 5000)

#### Unit tests

Run the unit tests with
- `make runQuoteTests` for quote library test
- `make runPricerTests` for pricer test

#### Documentation

Build the documentation using Doxygen with
```
make doc
```

### Express server

#### Install dependencies and run

```
npm install (only first time)
npm start
```

#### Usage

URL to access the API: `http://localhost:3000/api`

#### Unit tests

You can run unit tests in test mode using the following script:
```
npm run unit-tests
```

#### Load tests

Load tests require to run the server with `npm run test-server` first.
Then call the script for load tests.
```
npm run load-tests
```

### Angular client

#### Install dependencies and run

```
npm install (only first time)
npm start
```

#### Usage

URL to access the web client: `http://localhost:4200`
