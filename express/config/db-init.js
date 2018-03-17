const StockService = require('../services/stock.service');

// Stocks initialization
StockService.createStock({
    symbol: '^STOXX50E',
    name: 'Euro Stoxx 50',
    currency: 'EUR'
}).then(
    stock => console.log('Euro Stoxx 50 initialized'),
    error => { }
);
StockService.createStock({
    symbol: '^GSPC',
    name: 'S&P 500',
    currency: 'USD'
}).then(
    stock => console.log('S&P 500 initialized'),
    error => { }
);
StockService.createStock({
    symbol: '^AXJO',
    name: 'S&P ASX/200',
    currency: 'AUD'
}).then(
    stock => console.log('S&P ASX/200 initialized'),
    error => { }
);
StockService.createStock({
    symbol: 'EURUSD=X',
    name: 'EUR/USD Forex',
    currency: 'USD'
}).then(
    stock => console.log('EUR/USD Forex initialized'),
    error => { }
);
StockService.createStock({
    symbol: 'EURAUD=X',
    name: 'EUR/AUD Forex',
    currency: 'AUD'
}).then(
    stock => console.log('EUR/AUD Forex initialized'),
    error => { }
);