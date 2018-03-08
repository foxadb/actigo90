const PricerService = require('../services/pricer.service');

exports.getPricing = function (req, res) {
    try {
        PricerService.getPricing(pricing => {
            res.status(200).json({ status: 200, data: pricing, message: 'Pricing data successfully received' });
        });
    } catch (error) {
        res.status(400).json({ status: 400, message: error.message });
    }
};