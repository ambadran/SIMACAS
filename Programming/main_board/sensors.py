'''
Abstraction to Read All Sensor Values in one Object, 
This is Done by communicating to the sensor Board through UART or CAN
    - Supported Commands
        -   '\n': test Responsiveness
        -   'I<digit>': test Reading capabilities
        -   'L': returns Light Intensity value (Lumens/m^2) from TEMT6000 Sensor
        -   'T': returns Temperature value (degree celcius) from DHT11 Sensor
        -   'H': returns Humidity value (Percentage) from DHT11 Sensor
        -   'M': returns the CO2 value (PPM) from the MQ135 Sensor
        -   'S': returns Water Tension value (Kilo-Pascals) from the Soil Moisture Sensor
'''
from can_protocol import CAN
from random import getrandbits

class Sensors:
    '''
    Abstraction object to easily read all sensors
    '''
    def __init__(self):
        pass

    @property
    def temperature(self):
        '''
        send CAN packet to 'T' and read reply to get Temperature value
        '''
        #TODO:
        return 26 + getrandbits(2)

    @property
    def humidity(self):
        '''
        send CAN packet to 'H' and read reply to get Temperature value
        '''
        #TODO:
        return 16 + getrandbits(2)

    @property
    def light_intensity(self):
        '''
        send CAN packet to 'L' and read reply to get Temperature value
        '''
        #TODO:
        return getrandbits(1)

    @property
    def CO2(self):
        '''
        send CAN packet to 'M' and read reply to get Temperature value
        '''
        #TODO:
        return 593 + getrandbits(5)

    @property
    def soil_moisture(self):
        '''
        send CAN packet to 'M' and read reply to get Temperature value
        '''
        #TODO:
        return 60 + getrandbits(5)







