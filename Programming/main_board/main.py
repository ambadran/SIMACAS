from machine import Pin, SPI, SoftSPI
from nrf24l01 import NRF24L01
from micropython import const
import time
from server import server

# Axis Control
_RX_POLL_DELAY = const(15)
_RESPONDER_SEND_DELAY = const(10)

spi = SPI(0, sck=Pin(18), mosi=Pin(19), miso=Pin(16))
cfg = {"spi": spi, "csn": 20, "ce": 21}


# Addresses are in little-endian format. They correspond to big-endian
pipes = (b"\xe1\xf0\xf0\xf0\xf0", b"\xd2\xf0\xf0\xf0\xf0")

csn = Pin(cfg["csn"], mode=Pin.OUT, value=1)
ce = Pin(cfg["ce"], mode=Pin.OUT, value=0)
spi = cfg["spi"]

# The most important variable holding the NRF24L01 device object
nrf = NRF24L01(spi, csn, ce, payload_size=16)

nrf.open_tx_pipe(pipes[0])
nrf.open_rx_pipe(1, pipes[1])
nrf.start_listening()

# class Axes:
#     def __init__(self, axes)

def move_abs(axe_num, distance):
    global nrf
    nrf.send_ascii_m(f"M1i{distance}");

# def move_rel(axe_num, distance):
#     global nrf
#     nrf.


# Water Distribution system
L1 = const(0)
L2 = const(1)
S1 = const(0)
S2 = const(1)
S3 = const(2)
L1S1_pin = const(3)
L1S2_pin = const(4)
L1S3_pin = const(5)
L2S1_pin = const(6)
L2S2_pin = const(7)
L2S3_pin = const(8)
solenoids = [ [Pin(L1S1_pin, Pin.OUT), Pin(L1S2_pin, Pin.OUT), Pin(L1S3_pin, Pin.OUT)],
              [Pin(L2S1_pin, Pin.OUT), Pin(L2S2_pin, Pin.OUT), Pin(L2S3_pin, Pin.OUT)] ]

def solenoid(l, s):
    return solenoids[l-1][s-1]

server(move_abs)
