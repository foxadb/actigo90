const mongoose = require('mongoose');
const mongoosePaginate = require('mongoose-paginate');

const PriceSchema = new mongoose.Schema({
    date: { type: Date, required: true, unique: true },
    actigo: { type: Number, default: 0 },
    hedging: { type: Number, default: 0 }
}, { timestamps: true });

PriceSchema.methods.toDto = function () {
    return {
        id: this._id,
        date: this.date,
        actigo: this.actigopo,
        hedging: this.hedging
    };
};

PriceSchema.plugin(mongoosePaginate);
const Price = mongoose.model('Price', PriceSchema);

module.exports = Price;