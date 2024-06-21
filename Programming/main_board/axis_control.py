'''
Abstraction for controlling Axis Movements by sending Gcode through nRF24l01 Radio Transceiver
'''
from machine import Pin, SPI
from nrf24l01 import NRF24L01
from micropython import const
from time import sleep_ms


class AxisControl:
    '''
    Abstraction for controlling Axis Movements by sending Gcode through nRF24l01 Radio Transceiver
    '''
    DEFAULT_NUDGE_VALUE = 5

    SPI_HARDWARE_INDEX = const(0)
    DEFAULT_PAYLOAD_SIZE = const(16)
    AXIS_CHANNEL = {0: 40,
                    1: 46,
                    2: 58}  # Radio Channels for each Axis
    PIPES = (b"\xe1\xf0\xf0\xf0\xf0", b"\xd2\xf0\xf0\xf0\xf0")  # Addresses are in little-endian format. They correspond to big-endian
    def __init__(self, sck_pin: int, mosi_pin: int, miso_pin: int, csn_pin: int, ce_pin: int):
        # self.spi = SPI(0, sck=Pin(18), mosi=Pin(19), miso=Pin(16))  #TODO: put in actuators
        # self.cfg = {"spi": spi, "csn": 20, "ce": 21}
        self.spi = SPI(self.SPI_HARDWARE_INDEX, sck=Pin(sck_pin), mosi=Pin(mosi_pin), miso=Pin(miso_pin))
        self.csn = Pin(csn_pin, mode=Pin.OUT, value=1)
        self.ce = Pin(ce_pin, mode=Pin.OUT, value=0)

        self.nrf = NRF24L01(self.spi, self.csn, self.ce, payload_size=self.DEFAULT_PAYLOAD_SIZE)

        self.nrf.open_tx_pipe(self.PIPES[0])
        self.nrf.open_rx_pipe(1, self.PIPES[1])
        self.nrf.start_listening()

        self.positions = [0, 0, 0]

    def move(self, axe_num, distance):
        '''
        Relative Movement Command. 
        Just moves the amount given as argument.
        '''
        self.nrf.set_channel(self.AXIS_CHANNEL[axe_num])
        sleep_ms(300)
        self.nrf.send_ascii_m(f"M1i{distance}")

    def forward(self, axe_num):
        '''
        Nudge forward a certain Axe
        '''
        self.move(axe_num, self.DEFAULT_NUDGE_VALUE)
        self.positions[axe_num] += 1

    def backwards(self, axe_num):
        '''
        Nudge backwards a certain Axe
        '''
        self.move(axe_num, -self.DEFAULT_NUDGE_VALUE)
        self.positions[axe_num] -= 1

    # @property
    # def position0(self):
    #     '''
    #     Getter Method for position by asking the local STC MCU to read the Ultrasonic sensor 
    #     and send it here to the Main MCU
    #     '''
    #     pass

    # @position.setter
    # def position0(self, value):
    #     '''
    #     Setter Method to set the Absolute Position of the Axis. 
    #     Calculations and PID are implemented on the local STC MCU (not here)
    #     Here the absolute wanted position is just sent
    #     '''
    #     pass

    # @property
    # def position1(self):
    #     '''
    #     Getter Method for position by asking the local STC MCU to read the Ultrasonic sensor 
    #     and send it here to the Main MCU
    #     '''
    #     pass

    # @position.setter
    # def position1(self, value):
    #     '''
    #     Setter Method to set the Absolute Position of the Axis. 
    #     Calculations and PID are implemented on the local STC MCU (not here)
    #     Here the absolute wanted position is just sent
    #     '''
    #     pass

    # @property
    # def position2(self):
    #     '''
    #     Getter Method for position by asking the local STC MCU to read the Ultrasonic sensor 
    #     and send it here to the Main MCU
    #     '''
    #     pass

    # @position.setter
    # def position2(self, abs_value):
    #     '''
    #     Setter Method to set the Absolute Position of the Axis. 
    #     Calculations and PID are implemented on the local STC MCU (not here)
    #     Here the absolute wanted position is just sent
    #     '''
    #     pass


    # def get_position(self, axe_num):
    #     '''
    #     Return position give axe number
    #     '''
    #     pass

    # def set_position(self, axe_num, abs_value):
    #     '''
    #     sets the corresponding Absolute position
    #     '''
    #     pass


