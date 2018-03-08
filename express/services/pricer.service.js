const exec = require('child_process').exec;

exports.getPricing = function (callback) {
    const pricer = exec('~/Projets3A/peps/build/test/pricerActigo 200', { timeout: 10000 });

    let json;
    pricer.stdout.on('data', (data) => {
        json = data.split('\n');
    });

    pricer.stderr.on('data', (data) => {
        console.error(`stderr: ${data}`);
    });

    pricer.on('close', (code) => {
        console.log(`pricerActigo process exited with code ${code}`);
        callback(json);
    });
};