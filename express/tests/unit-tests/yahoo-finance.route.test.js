const app = require('../../app');
const request = require('supertest');

const stockRoute = '/api/stock';
const yahooFinanceRoute = '/api/yahoo-finance';

var stockId;

it('Create Dow Jones stock', function (done) {
    const stock = {
        symbol: '^DJI',
        name: 'Dow Jones Industrial Average',
        currency: 'USD'
    };

    request(app)
        .post(stockRoute)
        .send(stock)
        .expect('Content-Type', /json/)
        .expect(function (res) {
            stockId = res.body.data._id;
        })
        .expect(201, done);
});

it('POST: Download historical spots', function (done) {
    const body = {
        stock: stockId,
        period1: 1483228800, // Jan 1, 2017
        period2: 1514764800, // Jan 1, 2018
        interval: '1d'
    };

    request(app)
        .post(yahooFinanceRoute)
        .send(body)
        .expect('Content-Type', /json/)
        .expect(201, done);
});

it('Delete Dow Jones stock', function (done) {
    request(app)
        .delete(`${stockRoute}/${stockId}`)
        .expect(204, done);
});