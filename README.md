# Actigo90 Pricer

## Online Preview

An online preview is available here:
[Actigo 90](https://www.actigo90.trade)

Use the following user credentials:
- username: actigoguest
- password: actigoguest123

*This guest account have read-only access to the web app preview.*

## Dependencies

### NodeJS

This project requires nodejs >= 8.11.x.

The web app uses ExpressJS and Angular 5.

### Pricer

- C++11
- cmake >= 3.6.3
- gcc >= 7.2

## Pricer

### PNL

This project requires [PNL](https://github.com/pnlnum/pnl)
You have to clone and build this library.

Then set the `PNL_DIR` in the root `CMakeLists.txt`
This directory is the PNL build directory containing the lib file.

### Build

```
mkdir build
cd build
cmake ..
make
```

### Run (after running Node server)

For this part, you need to retreive prices on Yahoo Finance with the Angular
Client. Please skip this part until Express server and Angular client are
running. The reason is the pricer needs the historical data and the server is
used to collect them.

In the build directory, run the pricer with
```
./test/portfolioValue [date in epoch] [rebalancing frequency] [Monte Carlo samples number]
```
The computation might take a while. Use small value of Monte Carlo Samples
number firstly (such as 5000).

You have to set the date in epoch time. You can use the following converter:
[Epoch Converter](https://www.epochconverter.com)

*Note: the Angular Client fully automite the convertion for friendly usage.*

Example: Pricing everyday until 2015-12-31 (= 1451520000 in epoch)
```
./test/portfolioValue 1451520000 1 5000
```
Please run with small value at first to test the performance (e.g. 5000)

### Unit tests

Run the unit tests with
- `make runQuoteTests` for quote library test
- `make runPricerTests` for pricer test

### Documentation

Build the documentation using Doxygen with
```
make doc
```

## Express server

### Install dependencies and run

```
npm install (only first time)
npm start
```

### Usage

URL to access the API: `http://localhost:3000/api`

#### Authentication

Use ``api/user`` route for authentication as it is required to use most of POST/PUT/DELETE requests.

Authentication is based on [JSON Web Tokens](https://jwt.io). Use it as *Bearer Authentication Token* in your request header.

#### HTTP request examples

Get stock id `509637a41d5q7az697d1417az5` data
```
GET http://localhost:3000/api/stock/509637a41d5q7az697d1417az5
```

Get all spots data
```
GET http://localhost:3000/api/spot
```

Sign in
```
POST http://localhost:3000/api/user/login

HEADERS
Content-Type: application/json

BODY
{
    "username": "admin",
    "password": "password"
}
```

Create a new stock
```
POST http://localhost:3000/api/stock

HEADERS
Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6I...
Content-Type: application/json

BODY
{
    "symbol": "^STOXX50E",
    "name": "Euro Stoxx 50",
    "currency": "EUR"
}
```

Edit name of an existing stock
```
PUT http://localhost:3000/api/stock/501w47a69a5q71979d5q417edw

HEADERS
Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6I...
Content-Type: application/json

BODY
{
    "name": "New name"
}
```

Delete a spot
```
DELETE http://localhost:3000/api/spot/5a6f272493605913dbd0aa3c

HEADERS
Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6I...
```

#### Roles

- ``user``: have access to all API excepting ``user``
- ``admin``: have access to all API

### Unit tests

You can run unit tests in a test server instance using the following script:
```
npm run unit-tests
```

### Load tests

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
