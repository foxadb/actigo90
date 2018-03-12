class Stock {

    _id: string;
    symbol: string;
    name: string;
    currency: string;
    spots: Array<string>;

    constructor(json: any) {
        this._id = json._id;
        this.symbol = json.symbol;
        this.name = json.name;
        this.currency = json.currency;
        this.spots = json.spots;
    }

}

export default Stock;
