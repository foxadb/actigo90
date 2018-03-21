const app = require('../../app');
const request = require('supertest');

const userRoute = '/api/user';
const deltaRoute = '/api/delta';
const stockRoute = '/api/stock';

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

it('Create a Stock', function (done) {
    const stock = {
        symbol: '^STOCK1',
        name: 'Stock 1',
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

it('GET', function (done) {
    request(app)
        .get(deltaRoute)
        .set('Authorization', token)
        .expect('Content-Type', /json/)
        .expect(200, done);
});

var deltaId;

it('POST', function (done) {
    const body = {
        stock: stockId,
        date: new Date(2017, 1, 1),
        delta: 0.143
    };

    request(app)
        .post(deltaRoute)
        .set('Authorization', token)
        .send(body)
        .expect('Content-Type', /json/)
        .expect(function (res) {
            deltaId = res.body.data._id;
        })
        .expect(201, done);
});

it('POST an already existing date', function (done) {
    const body = {
        stock: stockId,
        date: new Date(2017, 1, 1),
        delta: 0.718
    };

    request(app)
        .post(deltaRoute)
        .set('Authorization', token)
        .send(body)
        .expect('Content-Type', /json/)
        .expect(400, done);
});

it('PUT', function (done) {
    const body = {
        delta: 0.592
    };

    request(app)
        .put(`${deltaRoute}/${deltaId}`)
        .set('Authorization', token)
        .send(body)
        .expect('Content-Type', /json/)
        .expect(200, done);
});

it('GET/:id', function (done) {
    request(app)
        .get(`${deltaRoute}/${deltaId}`)
        .set('Authorization', token)
        .expect('Content-Type', /json/)
        .expect(200, done);
});

it('DELETE', function (done) {
    request(app)
        .delete(`${deltaRoute}/${deltaId}`)
        .set('Authorization', token)
        .expect(204, done);
});

it('Delete the test stock', function (done) {
    request(app)
        .delete(`${stockRoute}/${stockId}`)
        .set('Authorization', token)
        .expect(204, done);
});