const app = require('../../app');
const request = require('supertest');

const userRoute = '/api/user';
const priceRoute = '/api/price';

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
        .get(priceRoute)
        .set('Authorization', token)
        .expect('Content-Type', /json/)
        .expect(200, done);
});

var priceId;

it('POST', function (done) {
    const body = {
        date: new Date(2019, 3, 16),
        portfolio: 0.81,
        hedging: 0.79
    };

    request(app)
        .post(priceRoute)
        .set('Authorization', token)
        .send(body)
        .expect('Content-Type', /json/)
        .expect(function (res) {
            priceId = res.body.data._id;
        })
        .expect(201, done);
});

it('POST an already existing date', function (done) {
    const body = {
        date: new Date(2019, 3, 16),
        portfolio: 0.88,
        hedging: 0.84
    };

    request(app)
        .post(priceRoute)
        .set('Authorization', token)
        .send(body)
        .expect('Content-Type', /json/)
        .expect(400, done);
});

it('PUT', function (done) {
    const body = {
        portfolio: 0.73
    };

    request(app)
        .put(`${priceRoute}/${priceId}`)
        .set('Authorization', token)
        .send(body)
        .expect('Content-Type', /json/)
        .expect(200, done);
});

it('GET/:id', function (done) {
    request(app)
        .get(`${priceRoute}/${priceId}`)
        .set('Authorization', token)
        .expect('Content-Type', /json/)
        .expect(200, done);
});

it('DELETE', function (done) {
    request(app)
        .delete(`${priceRoute}/${priceId}`)
        .set('Authorization', token)
        .expect(204, done);
});