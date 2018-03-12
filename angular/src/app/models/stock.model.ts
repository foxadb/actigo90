class Stock {

    _id: string;
    symbol: string;
    name: string;
    spots: Array<string>;

    constructor(json: any) {
        this._id = json._id;
        this.symbol = json.symbol;
        this.name = json.name;
        this.spots = json.spots;
    }

}

export default Stock;
