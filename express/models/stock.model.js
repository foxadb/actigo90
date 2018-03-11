const mongoose = require('mongoose');
const mongoosePaginate = require('mongoose-paginate');

var StockSchema = new mongoose.Schema({
    symbol: { type: String, maxlength: 24, required: true, unique: true },
    name: { type: String, maxlength: 80 },
    spots: [{ type: mongoose.Schema.Types.ObjectId, ref: 'Spot' }]
}, { timestamps: true });

StockSchema.methods.toDto = function () {
    return {
        id: this._id,
        symbol: this.symbol,
        name: this.name,
        spots: this.spots.map((spot) => { return spot.toDto(); })
    };
};

StockSchema.plugin(mongoosePaginate);
const Stock = mongoose.model('Stock', StockSchema);

module.exports = Stock;