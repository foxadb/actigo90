const DeltaService = require('../services/delta.service');

exports.getDeltas = async function (req, res) {
    // Check the existence of the query parameters, If the exists doesn't exists assign a default value
    let page = req.query.page ? +req.query.page : 1;
    let limit = req.query.limit ? +req.query.limit : 30;

    try {
        let deltas = await DeltaService.getDeltas({}, page, limit);
        return res.status(200).json({ status: 200, data: deltas, message: 'Successfully deltaes received' });
    } catch (error) {
        return res.status(400).json({ status: 400, message: error.message });
    }
};

exports.getDelta = async function (req, res) {
    // Delta ID
    let id = req.params.id;

    try {
        let delta = await DeltaService.getDelta(id);
        return res.status(200).json({ status: 200, data: delta, message: 'Successfully delta received' });
    } catch (error) {
        return res.status(404).json({ status: 404, message: error.message });
    }
};

exports.getStockDeltas = async function (req, res) {
    // Stock ID
    let id = req.params.id;

    // Check the existence of the query parameters, If the exists doesn't exists assign a default value
    let page = req.query.page ? +req.query.page : 1;
    let limit = req.query.limit ? +req.query.limit : 30;

    try {
        let deltas = await DeltaService.getDeltas({ stock: id }, page, limit);
        return res.status(200).json({ status: 200, data: deltas, message: 'Successfully delta received' });
    } catch (error) {
        return res.status(404).json({ status: 404, message: error.message });
    }
};

exports.createDelta = async function (req, res) {
    let delta = {
        stock: req.body.stock,
        date: req.body.date,
        delta: req.body.delta
    };

    try {
        let createdDelta = await DeltaService.createDelta(delta);

        // Return success result
        return res.status(201).json({ status: 201, data: createdDelta, message: 'Successfully created delta' });
    } catch (error) {
        return res.status(400).json({ status: 400, message: error.message });
    }
};

exports.updateDelta = async function (req, res) {
    // Delta ID
    let id = req.params.id;

    // Id is necessary for the update
    if (!id) {
        return res.status(400).json({ status: 400, message: 'Id must be present' });
    }

    let delta = {
        id,
        date: req.body.date,
        delta: req.body.delta
    };

    try {
        // Update the delta
        let updatedDelta = await DeltaService.updateDelta(delta);

        // Return success result
        return res.status(200).json({ status: 200, data: updatedDelta, message: 'Successfully updated delta' });
    } catch (error) {
        return res.status(403).json({ status: 403, message: error.message });
    }
};

exports.deleteDelta = async function (req, res) {
    // Delta ID
    let id = req.params.id;

    try {
        // Delete the delta
        await DeltaService.deleteDelta(id);

        // Return the result
        return res.status(204).json({ status: 204, message: 'Successfully delta deleted' });
    } catch (error) {
        return res.status(403).json({ status: 403, message: error.message });
    }
};