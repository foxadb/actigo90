const express = require('express');
const router = express.Router();

const pricer = require('./pricer.route');

router.use('/pricer', pricer);

module.exports = router;