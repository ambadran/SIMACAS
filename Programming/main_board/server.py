'''
Meant to run on the second Core of Pico for Optimal Performance
    - Host Web App (HTML file including the CSS and JAVASCRIPT)
    - Serve the HTML GET and POST requests
    - Updates internal variables to control Actuators through other Core
    - Updates the real-time Sensor values displayed on the Web App
'''
from micropython import const
from machine import Pin
import network
import socket
import json
import time
# import random

class HTML_REQUEST:
    GET_SENSOR_ACTUATOR = 0
    POST_SWITCH = 1
    GET_WEB = 2

class Server:
    # Access Point Parameters
    SSID = const("SIMACAS")
    PASSWORD = const("12345678")

    ON_OFF_MAPPING = {'on': 1, 'off': 0}
    def __init__(self):
        '''
        initiate server
        '''
        self.led = Pin("LED", Pin.OUT)  # on-board LED to show state
        self.led.off()

        self.reset()
        #TODO: implement try except block to avoid redefining socket
        self.init_access_point()
        self.init_socket()

        # should've been class method but contain self
        self.HANDLE_HTML_REQUEST = {HTML_REQUEST.GET_SENSOR_ACTUATOR: self.handle_get_values,
                               HTML_REQUEST.POST_SWITCH: self.handle_post_switches,
                               HTML_REQUEST.GET_WEB: self.handle_get_web}
 
    def reset(self):
        '''
        returns station object on reset.
        just deactivate and activate again 
        '''
        self.station = network.WLAN(network.AP_IF)

        self.station.active(False)
        time.sleep(2)
        self.station.active(True)

    def init_access_point(self):
        '''
        set up the Access Point
        '''
        self.station.config(ssid=self.SSID, password=self.PASSWORD)

        while not self.station.active():
            print(f"Station Initializing.. ", end=' \r')

        self.led.on()
        print('Access Point Active!')
        print(self.station.ifconfig())

    def init_socket(self):
        '''
        initiate socket connection
        '''
        self.addr = socket.getaddrinfo('0.0.0.0', 80)[0][-1]
        self.s = socket.socket()
        self.s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.s.bind(self.addr)
        self.s.listen(1)  # Reduce the backlog to minimize memory usage
        print('Listening on', self.addr)

    def web_page(self):
        '''
        return the HTML page
        '''
        with open('index.html', 'r') as f:
            web_page = f.read()

        return web_page

    def wait_for_client(self):
        '''
        Await client to connect then return new socket object used to 
        communicate with the connected client. 
        This socket is distinct from the listening socket (s) 
        and is used for sending and receiving data with the specific client that connected.
        '''
        self.client, addr = self.s.accept()
        print('Got a connection from %s' % str(addr))


