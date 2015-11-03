// vim: set ts=4 sw=4 et ai :

var Gpio = require('onoff').Gpio;
var fs = require('fs');

var pwmPin = 'P9_14';
var pwmDir = '/sys/class/pwm/pwm3'
var pinIn = 'P9_12';
var pinInNum = 60;
var pinInDir = '/sys/class/gpio/gpio60'

count = 0;
state = 0;

pinInGpio = Gpio(pinInNum, 'in', 'both');

function start(freq, duty)
{
    var pwm_run_fd = fs.openSync(pwmDir + '/run', 'w');
    fs.writeSync(pwm_run_fd, '0');
    count = 0;
    state = pinInGpio.readSync(state);
    pinInGpio.watch(onPinChange);

    // Reset the duty, then set period and duty to desired values
    var period_ns = Math.round((1/freq)*1e9)
    var duty_ns = Math.round(period_ns*duty);
    
    var pwm_duty_fd = fs.openSync(pwmDir + '/duty_ns', 'w');
    fs.writeSync(pwm_duty_fd, '0');
    var pwm_period_fd = fs.openSync(pwmDir + '/period_ns', 'w');
    fs.writeSync(pwm_period_fd, period_ns.toFixed());
    fs.closeSync(pwm_period_fd);
    fs.writeSync(pwm_duty_fd, duty_ns.toFixed());
    fs.closeSync(pwm_duty_fd);
    fs.writeSync(pwm_run_fd, '1');
    fs.closeSync(pwm_run_fd);
}

function stop()
{
    var pwm_run_fd = fs.openSync(pwmDir + '/run', 'w');
    fs.writeSync(pwm_run_fd, '0');
    pinInGpio.unwatch()
    fs.closeSync(pwm_run_fd);
}

function onCounterTimeout()
{
    stop();
    console.log(count/2/5);
}

function onPinChange(err, value)
{
    if (err !== null)
    {
        throw err;
    }
    if (value == state)
    {
        console.log('Got pin change but value is same; missed one?');
    }
    else
    {
        state = value;
        count = count + 1;
    }
}

start(process.argv[2], 0.50);
setTimeout(onCounterTimeout, 5000);
