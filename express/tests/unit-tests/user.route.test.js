const app = require('../../app');
const request = require('supertest');

const userRoute = '/api/user';

var token;

it('Login Success', function (done) {
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

it('Login Fail: wrong password', function (done) {
    let body = {
        username: 'admin',
        password: 'wrongpassword'
    };

    request(app)
        .post(`${userRoute}/login`)
        .send(body)
        .expect('Content-Type', /json/)
        .expect(401, done);
});

it('Login Fail: unknown username', function (done) {
    let body = {
        username: 'unknown',
        password: 'password'
    };

    request(app)
        .post(`${userRoute}/login`)
        .send(body)
        .expect('Content-Type', /json/)
        .expect(401, done);
});

it('Get Users', function (done) {
    request(app)
        .get(userRoute)
        .set('Authorization', token)
        .expect('Content-Type', /json/)
        .expect(200, done);
});

var userId;

it('Register a user', function (done) {
    let body = {
        username: 'newuser',
        password: 'password',
        role: 'user'
    };

    request(app)
        .post(`${userRoute}/register`)
        .set('Authorization', token)
        .send(body)
        .expect('Content-Type', /json/)
        .expect(function (res) {
            userId = res.body.data._id;
        })
        .expect(201, done);
});

it('Register an admin', function (done) {
    let body = {
        username: 'newadmin',
        password: 'password',
        role: 'admin'
    };

    request(app)
        .post(`${userRoute}/register`)
        .set('Authorization', token)
        .send(body)
        .expect('Content-Type', /json/)
        .expect(function (res) {
            userId = res.body.data._id;
        })
        .expect(201, done);
});

it('Register a user w/o Auth: should fail', function (done) {
    let body = {
        username: 'nouser',
        password: 'nopassword',
        role: 'user'
    };

    request(app)
        .post(`${userRoute}/register`)
        .send(body)
        .expect('Content-Type', /json/)
        .expect(401, done);
});

it('Register an admin w/o Auth: should fail', function (done) {
    let body = {
        username: 'nouser',
        password: 'nopassword',
        role: 'admin'
    };

    request(app)
        .post(`${userRoute}/register`)
        .send(body)
        .expect('Content-Type', /json/)
        .expect(401, done);
});

it('Already existing username: should fail', function (done) {
    let body = {
        username: 'newuser',
        password: 'password',
        role: 'user'
    };

    request(app)
        .post(`${userRoute}/register`)
        .set('Authorization', token)
        .send(body)
        .expect('Content-Type', /json/)
        .expect(403, done);
});

it('Update', function (done) {
    let body = {
        username: 'newuser',
        password: 'newpassword',
        role: 'admin'
    };

    request(app)
        .put(`${userRoute}/${userId}`)
        .set('Authorization', token)
        .send(body)
        .expect('Content-Type', /json/)
        .expect(200, done);
});

it('Update w/o Auth: should fail', function (done) {
    let body = {
        role: 'user'
    };

    request(app)
        .put(`${userRoute}/${userId}`)
        .send(body)
        .expect('Content-Type', /json/)
        .expect(401, done);
});

it('Delete unknown user: should fail', function (done) {
    request(app)
        .delete(`${userRoute}/314159265359`)
        .set('Authorization', token)
        .expect(403, done);
});

it('Delete w/o Auth: should fail', function (done) {
    request(app)
        .delete(`${userRoute}/${userId}`)
        .expect(401, done);
});

it('Delete w Auth', function (done) {
    request(app)
        .delete(`${userRoute}/${userId}`)
        .set('Authorization', token)
        .expect(204, done);
});