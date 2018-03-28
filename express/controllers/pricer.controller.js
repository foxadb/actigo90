const PricerService = require('../services/pricer.service');

exports.actigoDelta = function (req, res) {
    // Get date
    const date = req.body.date;

    try {
        PricerService.actigoDelta(date, pricing => {
            res.status(200).json({ status: 200, data: pricing, message: 'Pricing data successfully received' });
        });
    } catch (error) {
        res.status(400).json({ status: 400, message: error.message });
    }
};

exports.rebalance = function (req, res) {
    // Get pricer parameters
    const date = req.body.date;
    const samples = req.body.samples;

    try {
        PricerService.rebalance(date, samples, code => {
            res.status(200).json({ status: 200, data: code, message: 'Rebalancing portfolio successfully done' });
        });
    } catch (error) {
        res.status(400).json({ status: 400, message: error.message });
    }
};

exports.hedging = function (req, res) {
    // Get pricer parameters
    const date = req.body.date;
    const frequency = req.body.frequency;
    const samples = req.body.samples;

    try {
        PricerService.hedging(date, frequency, samples, code => {
            res.status(200).json({ status: 200, data: code, message: 'Hedging portfolio successfully computed' });
        });
    } catch (error) {
        res.status(400).json({ status: 400, message: error.message });
    }
};