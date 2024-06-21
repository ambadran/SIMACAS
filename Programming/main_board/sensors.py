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
class Sensors:
    pass
