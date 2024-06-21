from sensors import Sensors
from actuators import Actuators
from mpc import MPC
from server import Server
from time import sleep, sleep_ms
import _thread

# class GlobalVars:
#     '''
#     Convenience Object for inter-core communication
#     '''
#     def __init__(self):
#         # -1 backwards, 0 No movement, 1 forward
#         self.mechanism1: int = 0
#         self.mechanism2: int = 0
#         self.mechanism3: int = 0

#         self.water1: bool = False
#         self.water2: bool = False
#         self.water3: bool = False

#         self.fertilizer1: bool = False
#         self.fertilizer2: bool = False
#         self.fertilizer3: bool = False

#         self.light1: int = 50
#         self.light2: int = 50
#         self.light3: int = 50
# global_vars = GlobalVars()
global_vars = {
       'mechanism1': 0,
       'mechanism2': 0,
       'mechanism3': 0,
       'water1': 0,
       'water2': 0,
       'water3': 0,
       'fertilizer1': 0,
       'fertilizer2': 0,
       'fertilizer3': 0,
       'light1': 50,
       'light2': 50,
       'light3': 50
       }



def ControllerCore():
    '''
    Abstraction for the First Core:
        - Reads Sensor Data
        - Controls Actuator States
        - Executes MPC control Algorithm
        - Saves Sensor Data Globally and executes Global Actuators States
    '''
    global global_vars
    sensors = Sensors()
    actuators = Actuators()
    mpc = MPC(sensors, actuators)

    while True:
        print(global_vars, end=' \r')

        #### Handling Actuators ####
        # 1- Handling Axe Position
        while global_vars['mechanism1'] != actuators.axis.positions[0]:
            if global_vars['mechanism1'] > actuators.axis.positions[0]:
                actuators.axis.forward(0)
            elif global_vars['mechanism1'] < actuators.axis.positions[0]:
                actuators.axis.backwards(0)

        while global_vars['mechanism2'] != actuators.axis.positions[1]:
            if global_vars['mechanism2'] > actuators.axis.positions[1]:
                actuators.axis.forward(1)
            elif global_vars['mechanism2'] < actuators.axis.positions[1]:
                actuators.axis.backwards(1)

        while global_vars['mechanism3'] != actuators.axis.positions[2]:
            if global_vars['mechanism3'] > actuators.axis.positions[2]:
                actuators.axis.forward(2)
            elif global_vars['mechanism3'] < actuators.axis.positions[2]:
                actuators.axis.backwards(2)

        # 2- Handling Water / Fertilizer Channels
        actuators.distribution0.r1.value( global_vars['water1'] )
        actuators.distribution0.r2.value( global_vars['water2'] )
        actuators.distribution0.r3.value( global_vars['water3'] )

        actuators.distribution1.r1.value( global_vars['fertilizer1'] )
        actuators.distribution1.r2.value( global_vars['fertilizer2'] )
        actuators.distribution1.r3.value( global_vars['fertilizer3'] )

        # 3- Handling LED Strips PWM
        actuators.led_strip0.duty( global_vars['light1'] )
        actuators.led_strip1.duty( global_vars['light2'] )
        actuators.led_strip2.duty( global_vars['light3'] )

        sleep(0.5)

def ServerCore():
    '''
    Abstraction for the Second Core
        - Host Web App (HTML file including the CSS and JAVASCRIPT)
        - Serve the HTML GET and POST requests
        - Updates internal variables to control Actuators through other Core
        - Updates the real-time Sensor values displayed on the Web App
    '''
    global global_vars

    server = Server()

    while True:
        server.wait_for_client()

        server.handle_html_request(server.identify_html_request())

        global_vars.update(server.actuators_dict)

        server.led.toggle()

        print('\n')


#### Running Both Cores ####
def main():
    _thread.start_new_thread(ControllerCore, ())
    ServerCore()

_thread.start_new_thread(ControllerCore, ())
ServerCore()


