'''
Abstraction for LED strip control using PWM
'''
from micropython import const
from machine import Pin, PWM

class LedStrip:
    '''
    Abstraction for LED strip control using PWM
    '''
    DEFAULT_FREQ = const(20000)
    DUTY_0 = const(0)
    DUTY_25 = const(16384)
    DUTY_50 = const(32768)
    DUTY_75 = const(49151)
    DUTY_100 = const(65534)
    def __init__(self, pin_num: int):
        self.pwm = PWM(Pin(pin_num), freq=self.DEFAULT_FREQ, duty_u16=self.DUTY_50)

    def duty(self, percentage):
        '''
        set PWM duty_cycle given %
        '''
        value = int((percentage/100)*self.DUTY_100)
        self.pwm.duty_u16(value)

    def off(self):
        self.pwm.duty_u16(self.DUTY_0)

    def on(self):
        self.pwm.duty_u16(self.DUTY_100)

    #def toggle(self):
    #    #TODO

    #def ramp_up(self, time):
    #    '''
    #    turns on light from 0 to 100% in <time> argument
    #    '''
    #    #TODO:

    #def ramp_down(self, time):
    #    '''
    #    turns on light from 0 to 100% in <time> argument
    #    '''
    #    #TODO:

