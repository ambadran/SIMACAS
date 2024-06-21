from sensors import Sensors
from actuators import Actuators
from mpc import MPC
from server import Server
import _thread

class GlobalVariables:
    '''
    Convenience Object for inter-core communication
    '''
    def __init__(self):
        pass

class ControllerCore:
    '''
    Abstraction for the First Core:
        - Reads Sensor Data
        - Controls Actuator States
        - Executes MPC control Algorithm
        - Saves Sensor Data Globally and executes Global Actuators States
    '''
    def __init__(self):
        self.sensors = Sensors()
        self.actuators = Actuators()
        self.mpc = MPC(self.sensors, self.actuators)

class ServerCore:
    '''
    Abstraction for the Second Core
        - Host Web App (HTML file including the CSS and JAVASCRIPT)
        - Serve the HTML GET and POST requests
        - Updates internal variables to control Actuators through other Core
        - Updates the real-time Sensor values displayed on the Web App
    '''
    def __init__(self):
        server = Server()

#### Running Both Cores ####
# _thread.start_new_thread(core1, ())
# core0()
