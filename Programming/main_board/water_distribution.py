'''
Abstraction for Water Distribution System Controled by Relays ON/OFF signal
'''
from machine import Pin

class WaterDistribution:
    '''
    Abstraction for Water Distribution System Controled by Relays ON/OFF signal
    '''
    def __init__(self, r1_pin: int, r2_pin, r3_pin):
        self.r1 = Pin(r1_pin, Pin.OUT)
        self.r2 = Pin(r2_pin, Pin.OUT)
        self.r3 = Pin(r3_pin, Pin.OUT)

        self.relays = (self.r1, self.r2, self.r3)

    def off(self):
        '''
        Turns ALL Channels (Relays) OFF
        '''
        for relay in self.relays:
            relay.off()

    def on(self):
        '''
        Turns ALL Channels (Relays) ON
        '''
        for relay in self.relays:
            relay.on()

    def toggle(self):
        '''
        TOGGLES ALL Channels (Relays)
        '''
        for relay in self.relays:
            relay.toggle()



