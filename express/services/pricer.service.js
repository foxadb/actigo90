const exec = require('child_process').exec;

exports.actigoDelta = function (date, callback) {
    const pricer = exec(`~/Projets3A/peps/pricer/build/test/computeActigoDelta ${date}`, { timeout: 10000 });

    let json;
    pricer.stdout.on('data', (data) => {
        const result = data.split('\n');
        json = {
            euroStoxx50: Number(result[0]),
            snp500: Number(result[1]),
            snp200: Number(result[2]),
            eurUsd: Number(result[3]),
            eurAud: Number(result[4]),
            price: Number(result[5])
        };
    });

    pricer.stderr.on('data', (data) => {
        console.error(`stderr: ${data}`);
    });

    pricer.on('close', (code) => {
        console.log(`computeActigoDelta process exited with code ${code}`);
        callback(json);
    });
};