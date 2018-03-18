const StockService = require('../services/stock.service');

// Stocks initialization
exports.stockInit = function (done) {
    let eurStoxx50 = StockService.createStock({
        symbol: '^STOXX50E',
        name: 'Euro Stoxx 50',
        currency: 'EUR'
    }).then(
        stock => console.log('Euro Stoxx 50 initialized'),
        error => console.log('Euro Stoxx 50 already initialized')
    );

    let snp500 = StockService.createStock({
        symbol: '^GSPC',
        name: 'S&P 500',
        currency: 'USD'
    }).then(
        stock => console.log('S&P 500 initialized'),
        error => console.log('S&P 500 already initialized')
    );

    let snp200 = StockService.createStock({
        symbol: '^AXJO',
        name: 'S&P ASX/200',
        currency: 'AUD'
    }).then(
        stock => console.log('S&P ASX/200 initialized'),
        error => console.log('S&P ASX/200 already initialized')
    );

    let eurUsd = StockService.createStock({
        symbol: 'EURUSD=X',
        name: 'EUR/USD Forex',
        currency: 'USD'
    }).then(
        stock => console.log('EUR/USD Forex initialized'),
        error => console.log('EUR/USD Forex already initialized')
    );

    let eurAud = StockService.createStock({
        symbol: 'EURAUD=X',
        name: 'EUR/AUD Forex',
        currency: 'AUD'
    }).then(
        stock => console.log('EUR/AUD Forex initialized'),
        error => console.log('EUR/AUD Forex already initialized')
    );

    // Wait until all stocks are created
    Promise.all([eurStoxx50, snp500, snp200, eurUsd, eurAud])
        .then(res => done(res));
};