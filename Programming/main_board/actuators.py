'''
Abstraction to control ALL Actuators together
    - Axis Control Movements
    - Water Distribution Relay States
    - LED Strip PWM signal
'''
from axis_control import AxisControl
from water_distribution import WaterDistribution
from led_strip import LedStrip

class Actuators:
    '''
    Abstraction to control ALL Actuators together
        - Axis Control Movements
        - Water Distribution Relay States
        - LED Strip PWM signal
    '''
    SCK_PIN = const(18)
    MOSI_PIN = const(19)
    MISO_PIN = const(16)
    CSN_PIN = const(20)
    CE_PIN = const(21)

    RELAY1A_PIN = const(0)
    RELAY2A_PIN = const(1)
    RELAY3A_PIN = const(2)
    RELAY1B_PIN = const(3)
    RELAY2B_PIN = const(4)
    RELAY3B_PIN = const(5)

    LED_STRIP1_PIN = const(7)
    LED_STRIP2_PIN = const(8)
    LED_STRIP3_PIN = const(9)

    def __init__(self):
        # 1- Axis Control of Movements through nRF24l01
        self.axis = AxisControl(Actuators.SCK_PIN, Actuators.MOSI_PIN, Actuators.MISO_PIN,
                                Actuators.CSN_PIN, Actuators.CE_PIN)

        # Solenoid Valve water/fertilizer channels controlled by Relays
        self.distribution0 = WaterDistribution(Actuators.RELAY1A_PIN,
                                               Actuators.RELAY2A_PIN, 
                                               Actuators.RELAY3A_PIN)
        self.distribution1 = WaterDistribution(Actuators.RELAY1B_PIN, 
                                               Actuators.RELAY2B_PIN, 
                                               Actuators.RELAY3B_PIN)
        self.distributions = (self.distribution0, self.distribution1)

        # LED Strips
        self.led_strip0 = LedStrip(self.LED_STRIP1_PIN)
        self.led_strip1 = LedStrip(self.LED_STRIP2_PIN)
        self.led_strip2 = LedStrip(self.LED_STRIP3_PIN)
        self.led_strips = (self.led_strip0, self.led_strip1, self.led_strip2)


