const request = require('request');

const SpotService = require('./spot.service');
const StockService = require('./stock.service');

exports.getCrumbCookie = async function (symbol) {
    // Yahoo Finance URL
    const url = `https://finance.yahoo.com/quote/${symbol}/?p=${symbol}`;

    // Cookie jar
    const cookieJar = request.jar();

    // HTTP Request to Yahoo Finance
    return new Promise(resolve => {
        request({ url: url, jar: cookieJar }, function (error, response, body) {
            if (error) {
                // Print the status code and error if one occurred
                throw Error(error);
            }

            //// Extract Crumb value
            const crumbStartIndex = body.search('CrumbStore') + 22;
            const crumbEndIndex = crumbStartIndex + body.substring(crumbStartIndex).search('"');

            const crumb = body.substring(crumbStartIndex, crumbEndIndex);

            //// Extract Cookie value
            let cookie = String(cookieJar.getCookies(url)[0]);
            const cookieStartIndex = cookie.search('B=');
            const cookieEndIndex = cookie.search(';');
            cookie = cookie.substring(cookieStartIndex, cookieEndIndex);

            resolve({
                crumb: crumb,
                cookie: cookie
            });
        });
    });
};

exports.downloadSpotsCsv = async function (symbol, period1, period2, interval, crumb, cookie) {
    // Yahoo Finance URL
    const url = 'https://query1.finance.yahoo.com/v7/finance/download/'
        + symbol
        + '?period1=' + period1
        + '&period2=' + period2
        + '&interval=' + interval
        + '&events=history'
        + '&crumb=' + crumb;

    // Set cookie jar value
    const cookieJar = request.jar();
    const cookieValue = request.cookie(cookie);
    cookieJar.setCookie(cookieValue, url);

    // HTTP Request to Yahoo Finance
    return new Promise(resolve => {
        request({ url: url, jar: cookieJar }, function (error, response, body) {
            if (error) {
                // Print the status code and error if one occurred
                throw Error(error);
            }

            // Resolve the request promise
            resolve(body);
        });
    });
};

exports.parseSpotsCsv = async function (stockId, csv) {
    // Split CSV lines
    let lines = csv.split(RegExp('\\n'));

    // Remove header line and last empty line
    lines.shift();
    lines.pop();

    try {
        if (lines) {
            for (let i = 0; i < lines.length; ++i) {
                let line = lines[i];

                // Split line
                let lineArray = line.split(RegExp(','));

                // Create the new spot
                SpotService.createSpot({
                    stock: stockId,
                    date: lineArray[0],
                    price: lineArray[4]
                }).then(function (spot) {
                    // Add the spot to the stock spots list
                    StockService.addSpot(stockId, spot._id);
                }, function (error) {
                    // do nothing
                });
            }
        }
    } catch (error) {
        throw Error('Error while parsing spots CSV file');
    }    
};