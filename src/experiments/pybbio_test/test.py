# vim: set ts=4 sw=4 et ai :

import bbio
import sys
import time

count = 0
state = 0
PWM_DIR = '/sys/class/pwm/pwm3/'
PIN = bbio.GPIO1_28

def start(freq):
    global count
    global state

    pwm_run = open(PWM_DIR+'run', 'w')
    pwm_run.write('0')
    pwm_duty = open(PWM_DIR+'duty_ns', 'w')
    pwm_duty.write('0')
    pwm_period = open(PWM_DIR+'period_ns', 'w')
    period = round((1.0/freq)*1e9)
    duty = round(period/2.0)
    pwm_period.write('%d' % period)
    pwm_period.close()
    pwm_duty.write('%d' % duty)
    pwm_duty.close()
    count = 0
    state = bbio.digitalRead(PIN);
    bbio.attachInterrupt(PIN, onPinChange, bbio.BOTH);
    pwm_run.write('1')
    pwm_run.close()

def stop():
    bbio.detachInterrupt(PIN);
    pwm_run = open(PWM_DIR+'run', 'w')
    pwm_run.write('0')

def onPinChange():
    global count
    global state

    new_state = bbio.digitalRead(PIN)
    if new_state == state:
        print "Got pin change by value is same; missed one?"
    else:
        state = new_state
        count += 1

start(float(sys.argv[1]))
time.sleep(5)
stop()
print '%f' % (count/2.0/5.0)

