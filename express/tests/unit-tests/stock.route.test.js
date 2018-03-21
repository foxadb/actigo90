const app = require('../../app');
const request = require('supertest');

const userRoute = '/api/user';
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

it('GET', function (done) {
    request(app)
        .get(stockRoute)
        .set('Authorization', token)
        .expect('Content-Type', /json/)
        .expect(200, done);
});

var stockId;

it('POST', function (done) {
    const body = {
        symbol: '^STOCK1',
        name: 'Stock 1',
        currency: 'AUD'
    };

    request(app)
        .post(stockRoute)
        .set('Authorization', token)
        .send(body)
        .expect('Content-Type', /json/)
        .expect(function (res) {
            stockId = res.body.data._id;
        })
        .expect(201, done);
});

it('POST an already existing stock', function (done) {
    const body = {
        symbol: '^STOCK1',
        name: 'Other Stock',
        currency: 'EUR'
    };

    request(app)
        .post(stockRoute)
        .set('Authorization', token)
        .send(body)
        .expect('Content-Type', /json/)
        .expect(400, done);
});

it('PUT', function (done) {
    const body = {
        name: 'Updated Stock 1',
        currency: 'USD'
    };

    request(app)
        .put(`${stockRoute}/${stockId}`)
        .set('Authorization', token)
        .send(body)
        .expect('Content-Type', /json/)
        .expect(200, done);
});

it('GET/:id', function (done) {
    request(app)
        .get(`${stockRoute}/${stockId}`)
        .set('Authorization', token)
        .expect('Content-Type', /json/)
        .expect(200, done);
});

it('DELETE', function (done) {
    request(app)
        .delete(`${stockRoute}/${stockId}`)
        .set('Authorization', token)
        .expect(204, done);
});