const app = require('../../app');
const request = require('supertest');

const spotRoute = '/api/spot';
const stockRoute = '/api/stock';

describe('Spot API', function () {

    var stockId;

    it('Create a Stock', function (done) {
        const stock = {
            symbol: '^STOCK1',
            name: 'Stock 1',
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

    it('GET', function (done) {
        request(app)
            .get(spotRoute)
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
            .send(body)
            .expect('Content-Type', /json/)
            .expect(200, done);
    });

    it('GET/:id', function (done) {
        request(app)
            .get(`${spotRoute}/${spotId}`)
            .expect('Content-Type', /json/)
            .expect(200, done);
    });

    it('DELETE', function (done) {
        request(app)
            .delete(`${spotRoute}/${spotId}`)
            .expect(204, done);
    });

    it('Delete the temp stock', function (done) {
        request(app)
            .delete(`${stockRoute}/${stockId}`)
            .expect(204, done);
    });

});