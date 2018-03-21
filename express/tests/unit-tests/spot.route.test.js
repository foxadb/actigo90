const app = require('../../app');
const request = require('supertest');

const userRoute = '/api/user';
const spotRoute = '/api/spot';
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
        .get(spotRoute)
        .set('Authorization', token)
        .expect('Content-Type', /json/)
        .expect(200, done);
});

var spotId;

it('POST', function (done) {
    const body = {
        stock: stockId,
        date: new Date(2017, 1, 1),
        price: 42
    };

    request(app)
        .post(spotRoute)
        .set('Authorization', token)
        .send(body)
        .expect('Content-Type', /json/)
        .expect(function (res) {
            spotId = res.body.data._id;
        })
        .expect(201, done);
});

it('POST an already existing date', function (done) {
    const body = {
        stock: stockId,
        date: new Date(2017, 1, 1),
        price: 13
    };

    request(app)
        .post(spotRoute)
        .set('Authorization', token)
        .send(body)
        .expect('Content-Type', /json/)
        .expect(400, done);
});

it('PUT', function (done) {
    const body = {
        price: 349
    };

    request(app)
        .put(`${spotRoute}/${spotId}`)
        .set('Authorization', token)
        .send(body)
        .expect('Content-Type', /json/)
        .expect(200, done);
});

it('GET/:id', function (done) {
    request(app)
        .get(`${spotRoute}/${spotId}`)
        .set('Authorization', token)
        .expect('Content-Type', /json/)
        .expect(200, done);
});

it('DELETE', function (done) {
    request(app)
        .delete(`${spotRoute}/${spotId}`)
        .set('Authorization', token)
        .expect(204, done);
});

it('Delete the test stock', function (done) {
    request(app)
        .delete(`${stockRoute}/${stockId}`)
        .set('Authorization', token)
        .expect(204, done);
});