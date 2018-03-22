class Price {

    _id: string;
    date: Date;
    actigo: number;
    hedging: number;

    constructor(json: any) {
        this._id = json._id;
        this.date = json.date;
        this.actigo = json.actigo;
        this.hedging = json.hedging;
    }

}

export default Price;
