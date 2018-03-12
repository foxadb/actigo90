const mongoose = require('mongoose');
const mongoosePaginate = require('mongoose-paginate');

var StockSchema = new mongoose.Schema({
    symbol: { type: String, maxlength: 24, required: true, unique: true },
    name: { type: String, maxlength: 80 },
    currency: { type: String, enum: ['AUD', 'EUR', 'USD'] },
    spots: [{ type: mongoose.Schema.Types.ObjectId, ref: 'Spot' }]
}, { timestamps: true });

StockSchema.methods.toDto = function () {
    return {
        id: this._id,
        symbol: this.symbol,
        name: this.name,
        spots: this.spots.length < 10 ? this.spots : this.spots.length
    };
};

StockSchema.plugin(mongoosePaginate);
const Stock = mongoose.model('Stock', StockSchema);

module.exports = Stock;