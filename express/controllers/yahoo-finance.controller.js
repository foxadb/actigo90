const YahooFinanceService = require('../services/yahoo-finance.service');
const StockService = require('../services/stock.service');

exports.downloadHistoricalSpots = async function (req, res) {
    const stockId = req.body.stock;
    const period1 = req.body.period1;
    const period2 = req.body.period2;
    const interval = req.body.interval;

    try {
        // Get stock
        let stock = await StockService.getStock(stockId);
        
        // Retreive crumb and cookie
        let crumbCookie = await YahooFinanceService.getCrumbCookie(stock.symbol);
        
        // Download Spots CSV
        let csv = await YahooFinanceService.downloadSpotsCsv(
            stock.symbol, period1, period2, interval, crumbCookie.crumb, crumbCookie.cookie);
        
        // Parse Spots CSV into the database
        await YahooFinanceService.parseSpotsCsv(stockId, csv);

        // Send HTTP result
        res.status(201).json({ status: 201, message: 'Historical spots successfully received' });
    } catch (error) {
        res.status(400).json({ status: 400, message: error.message });
    }
};