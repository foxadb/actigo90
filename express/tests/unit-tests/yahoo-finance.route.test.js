const app = require('../../app');
const request = require('supertest');

const userRoute = '/api/user';
const stockRoute = '/api/stock';
const yahooFinanceRoute = '/api/yahoo-finance';

var token;

it('AUTHENTICATION', function (done) {
    let body = {
        username: 'admin',
        password: 'password'
    };

    request(app)
        .post(`${userRoute}/login`)
        .send(body)
        .expect('Content-Type', /json/)
        .expect(function (res) {
            token = 'Bearer ' + res.body.token;
        })
        .expect(200, done);
});

var stockId;

it('Create Dow Jones stock', function (done) {
    const stock = {
        symbol: '^DJI',
        name: 'Dow Jones Industrial Average',
        currency: 'USD'
    };

    request(app)
        .post(stockRoute)
        .set('Authorization', token)
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
        .set('Authorization', token)
        .send(body)
        .expect('Content-Type', /json/)
        .expect(201, done);
});

it('Delete Dow Jones stock', function (done) {
    request(app)
        .delete(`${stockRoute}/${stockId}`)
        .set('Authorization', token)
        .expect(204, done);
});